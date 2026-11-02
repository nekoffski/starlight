#include "Renderer.hh"

#include "RenderGraph.hh"
#include "starlight/core/Functional.hh"
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
    if (m_pendingRequests.size() >= m_config.renderer.maxFramesInFlight) {
        return Error::unexpected(
            ErrorCode::tooManyFrameRequests, "Too many frame requests"
        );
    }
    const auto r = m_device.validateResources(request.resources);
    if (not r) {
        return r;
    }
    m_pendingRequests.push(request);
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
        Overloader{
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

u64 Renderer::frameIndex() const {
    return m_frameNumber % m_config.renderer.maxFramesInFlight;
}

void Renderer::tryToSubmitFrame() {
    const auto& request = m_pendingRequests.front();
    NAMED_DEFER(requestDropper) { m_pendingRequests.pop(); };

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
    m_frameNumber++;
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
            ErrorCode::invalidArgument, "Invalid shader handle"
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

        graph.addRenderPass(
            pass, [&](RenderPassEncoder& encoder) -> Result<void> {
                for (const auto& renderable : request.scene.items) {
                    const auto* shaderDescription =
                        m_device.getShaderDescription(renderable.shader);

                    // TODO: refactor
                    if (not shaderDescription) {
                        return Error::unexpected(
                            ErrorCode::invalidArgument, "Invalid shader handle"
                        );
                    }

                    const auto& vertexLayout = shaderDescription->vertexLayout;

                    if (not vertexLayout.has_value()) {
                        return Error::unexpected(
                            ErrorCode::invalidArgument,
                            "Shader does not have a vertex layout"
                        );
                    }

                    GraphicsPipelineDescription description{
                        .format = TextureFormat::bgra8unorm,
                        .shader = renderable.shader,
                    };

                    auto pipeline = resolveGraphicsPipeline(description);

                    if (not pipeline) [[unlikely]] {
                        return Error::unexpected(pipeline.error());
                    }

                    if (auto r = encoder.setPipeline(*pipeline); not r) {
                        return Error::unexpected(r.error());
                    }

                    auto r = encoder.setShaderParameters(
                        *shaderDescription, renderable.parameters,
                        request.resources
                    );
                    if (not r) {
                        return r;
                    }

                    for (const auto& primitiveHandle : renderable.primitives) {
                        const auto& primitive =
                            m_resourcePool.getPrimitive(primitiveHandle);

                        if (not primitive) {
                            return Error::unexpected(
                                ErrorCode::invalidArgument,
                                "Invalid primitive handle"
                            );
                        }

                        if (primitive->vertexLayout != *vertexLayout) {
                            return Error::unexpected(
                                ErrorCode::invalidArgument,
                                "Primitive vertex layout does not match shader "
                                "vertex layout"
                            );
                        }

                        r = encoder.setVertexBuffer({
                            .handle = primitive->vertexBuffer,
                            .offset = primitive->vertexBufferOffset,
                        });
                        if (not r) {
                            return r;
                        }

                        r = encoder.setIndexBuffer({
                            .handle = primitive->indexBuffer,
                            .offset = primitive->indexBufferOffset,
                        });
                        if (not r) {
                            return r;
                        }
                        encoder.drawIndexed(primitive->indexCount);
                    }
                }
                return {};
            }
        );
    }

    return graph.record(recorder);
}

}  // namespace sl
