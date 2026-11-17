#include "Renderer.hh"

#include <kcore/Functional.hh>
#include <kcore/Pipeline.hh>

#include "RenderGraph.hh"
#include "starlight/renderer/core/RenderResource.hh"

namespace sl {

Renderer::Renderer(const Config& config, RenderDevice& device)
    : m_config(config), m_device(device), m_resourcePool(config, device) {}

bool Renderer::tick() {
    if (m_pendingRequests.empty()) {
        return false;
    }
    tryToSubmitFrame();
    return true;
}

void Renderer::flush() {
    for (;;) {
        if (not tick()) {
            break;
        }
    }
}

Result<void> Renderer::submit(const RenderRequest& request) {
    const auto r = validateRequest(request);
    if (not r) {
        return r;
    }
    m_pendingRequests.push(request);
    return {};
}

Result<void> Renderer::validateRequest(const RenderRequest& request) {
    if (m_pendingRequests.size() >= m_config.renderer.maxFramesInFlight) {
        return Error::unexpected(
            ErrorCode::tooManyFrameRequests, "Too many frame requests"
        );
    }
    const auto r = m_device.validateResources(request.resources);
    if (not r) {
        return r;
    }
    for (const auto& item : request.scene.items) {
        const auto* shader = m_device.getShaderDescription(item.shader);
        if (not shader) {
            return Error::unexpected(
                kc::ErrorCode::invalidArgument, "Invalid shader dependency"
            );
        }
        for (const auto& draw : item.primitives) {
            const auto* geometry = m_resourcePool.getPrimitive(draw.primitive);
            if (not geometry) {
                return Error::unexpected(
                    kc::ErrorCode::invalidArgument,
                    "Invalid primitive dependency"
                );
            }
            const auto indices = geometry->resolveIndexRange(draw.indexRange);
            if (not indices) {
                return Error::unexpected(indices.error());
            }
            if (not shader->vertexInputs) {
                return Error::unexpected(
                    kc::ErrorCode::invalidArgument,
                    "Shader does not declare vertex inputs"
                );
            }
            const auto r = validateVertexCompatibility(
                geometry->vertexLayout, *shader->vertexInputs
            );
            if (not r) {
                return r;
            }
        }
    }
    return {};
}

Result<RenderTarget> Renderer::createTarget(
    std::shared_ptr<RenderSurfaceProvider> surfaceProvider
) {
    auto surface = m_device.attachSurface(std::move(surfaceProvider));

    if (not surface) {
        return Error::unexpected(
            ErrorCode::surfaceCreationFailed,
            "Failed to create render surface: {}", surface.error()
        );
    }
    return RenderTarget{*surface};
}

void Renderer::destroyTarget(RenderTarget target) {
    std::visit(
        kc::Overloader{
            [&](SurfaceHandle surface) { m_device.destroySurface(surface); },
            [&](TextureHandle) {
                log::panic("Destroying texture render output is not supported");
            }
        },
        target
    );
}

// shortcut: GPU-idle release stalls; defer retirement when resources churn.
void Renderer::destroyTexture(TextureHandle handle) {
    flush();
    m_device.waitIdle();
    m_device.destroyTexture(handle);
}

void Renderer::destroySampler(SamplerHandle handle) {
    flush();
    m_device.waitIdle();
    m_device.destroySampler(handle);
}

void Renderer::destroyPrimitive(PrimitiveHandle handle) {
    if (not m_resourcePool.getPrimitive(handle)) {
        return;
    }
    flush();
    m_device.waitIdle();
    m_resourcePool.destroyPrimitive(handle);
}

void Renderer::destroyShader(ShaderHandle handle) {
    flush();
    m_device.waitIdle();
    std::vector<GraphicsPipelineDescription> expired;
    m_graphicsPipelines.forEach([&](const auto& description, auto pipeline) {
        if (description.shader == handle) {
            m_device.destroyGraphicsPipeline(pipeline);
            expired.push_back(description);
        }
    });
    m_graphicsPipelines.remove(expired);
    m_device.destroyShader(handle);
}

void Renderer::tryToSubmitFrame() {
    const auto& request = m_pendingRequests.front();
    KCORE_NAMED_DEFER(requestDropper) { m_pendingRequests.pop(); };

    const auto r = m_device.trySubmitFrame([&](RenderFrameRecorder& context) {
        return recordFrame(context, request);
    });

    if (not r) [[unlikely]] {
        if (r.error().code() != ErrorCode::tooManyFramesInFlight) [[likely]] {
            log::error("Failed to record frame: {}", r.error());
        } else {
            requestDropper.dismiss();
        }
        return;
    }
}

Result<GraphicsPipelineHandle> Renderer::resolveGraphicsPipeline(
    const GraphicsPipelineDescription& description
) {
    if (auto pipeline = m_graphicsPipelines.find(description); pipeline) {
        return *pipeline;
    }

    auto maybePipeline = m_device.createGraphicsPipeline(description);

    if (not maybePipeline) {
        return Error::unexpected(
            ErrorCode::deviceOperationFailed,
            "Failed to create graphics pipeline: {}", maybePipeline.error()
        );
    }

    return m_graphicsPipelines.emplace(description, *maybePipeline);
}

Result<ShaderInterface> Renderer::getShaderInterface(
    ShaderHandle handle
) const {
    const auto* description = m_device.getShaderDescription(handle);
    if (not description) {
        return Error::unexpected(
            kc::ErrorCode::invalidArgument, "Invalid shader handle"
        );
    }

    ShaderInterface interface{.handle = handle};
    interface.parameters.reserve(description->uniforms.size());
    for (const auto& uniform : description->uniforms) {
        interface.parameters.push_back({uniform.name, uniform.size});
    }
    return interface;
}

Result<void> Renderer::recordFrame(
    RenderFrameRecorder& recorder, const RenderRequest& request
) {
    RenderGraph graph;

    for (const auto& view : request.views) {
        ColorAttachment attachment{
            .target = view.target,
            .loadOp = LoadOp::clear,
            .storeOp = StoreOp::store,
            .clearColor = request.clearColor
        };

        RenderPassDescription pass{
            .label = "Main Pass", .colorAttachments = std::span(&attachment, 1)
        };

        auto recordPass = [this, &request, view](RenderPassEncoder& encoder) {
            return recordView(encoder, request, view.parameters);
        };
        graph.addRenderPass(pass, std::move(recordPass));
    }

    return graph.record(recorder);
}

Result<void> Renderer::recordView(
    RenderPassEncoder& encoder, const RenderRequest& request,
    std::span<const ShaderParameterBlock> viewParameters
) {
    for (const auto& item : request.scene.items) {
        const auto r =
            recordItem(encoder, item, viewParameters, request.resources);
        if (not r) {
            return r;
        }
    }
    return {};
}

Result<void> Renderer::recordItem(
    RenderPassEncoder& encoder, const RenderItem& item,
    std::span<const ShaderParameterBlock> viewParameters,
    const ShaderResourceDependencies& resources
) {
    const auto* shader = m_device.getShaderDescription(item.shader);
    if (not shader) {
        return Error::unexpected(
            kc::ErrorCode::invalidArgument, "Invalid shader handle"
        );
    }
    const auto parameters =
        combineShaderParameterBlocks(item.parameters, viewParameters);
    const auto r = encoder.setShaderParameters(*shader, parameters, resources);
    if (not r) {
        return r;
    }
    for (const auto& draw : item.primitives) {
        const auto r = recordPrimitive(encoder, item.shader, draw);
        if (not r) {
            return r;
        }
    }
    return {};
}

Result<void> Renderer::recordPrimitive(
    RenderPassEncoder& encoder, ShaderHandle shader, const PrimitiveDraw& draw
) {
    const auto* primitive = m_resourcePool.getPrimitive(draw.primitive);
    if (not primitive) {
        return Error::unexpected(
            kc::ErrorCode::invalidArgument, "Invalid primitive handle"
        );
    }
    GraphicsPipelineDescription description{
        .format = TextureFormat::bgra8unorm,
        .shader = shader,
        .vertexLayout = primitive->vertexLayout,
    };
    const auto resolvePipeline = [&] {
        return resolveGraphicsPipeline(description);
    };
    const auto bindPipeline = [&](GraphicsPipelineHandle pipeline) {
        return encoder.setPipeline(pipeline);
    };
    const auto bindVertexBuffer = [&] {
        return encoder.setVertexBuffer({
            .handle = primitive->vertexBuffer,
            .offset = primitive->vertexBufferOffset,
        });
    };
    const auto bindIndexBuffer = [&] {
        return encoder.setIndexBuffer({
            .handle = primitive->indexBuffer,
            .offset = primitive->indexBufferOffset,
        });
    };
    const auto resolveIndices = [&] {
        return primitive->resolveIndexRange(draw.indexRange);
    };
    const auto drawIndices = [&](const IndexRange& indices) {
        encoder.drawIndexed({
            .indexCount = indices.indexCount,
            .firstIndex = indices.firstIndex,
        });
    };
    return kc::startPipeline(resolvePipeline)
        .then(bindPipeline)
        .then(bindVertexBuffer)
        .then(bindIndexBuffer)
        .then(resolveIndices)
        .transform(drawIndices)
        .result();
}

}  // namespace sl
