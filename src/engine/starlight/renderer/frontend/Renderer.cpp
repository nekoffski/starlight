#include "Renderer.hh"

#include "RenderGraph.hh"
#include "starlight/core/Functional.hh"
#include "starlight/renderer/rhi/RenderResource.hh"

namespace sl {

namespace {

Bytes toBytes(std::span<const Vertex3> vertices) {
    Bytes bytes;
    bytes.resize(vertices.size() * sizeof(Vertex3));
    std::memcpy(bytes.data(), vertices.data(), bytes.size());
    return bytes;
}

Bytes toBytes(std::span<const u32> indices) {
    Bytes bytes;
    bytes.resize(indices.size() * sizeof(u32));
    std::memcpy(bytes.data(), indices.data(), bytes.size());
    return bytes;
}

}  // namespace

Renderer::Renderer(const Config& config, RenderDevice& device)
    : m_config(config), m_device(device) {}

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

Result<Primitive> Renderer::uploadPrimitive(
    const PrimitiveUploadData& description
) {
    DeviceBufferDescription vertexBufferDescription{
        .usage = DeviceBufferUsage::vertexBuffer,
        .bytes = toBytes(description.vertices)
    };

    auto vertexBuffer = m_device.createBuffer(vertexBufferDescription);

    if (not vertexBuffer) {
        return Error::unexpected(
            ErrorCode::deviceOperationFailed,
            "Failed to create vertex buffer: {}", vertexBuffer.error()
        );
    }

    DeviceBufferDescription indexBufferDescription{
        .usage = DeviceBufferUsage::indexBuffer,
        .bytes = toBytes(description.indices)
    };

    auto indexBuffer = m_device.createBuffer(indexBufferDescription);

    if (not indexBuffer) {
        m_device.destroyBuffer(*vertexBuffer);

        return Error::unexpected(
            ErrorCode::deviceOperationFailed,
            "Failed to create index buffer: {}", indexBuffer.error()
        );
    }

    return Primitive{
        .vertexBuffer = *vertexBuffer,
        .indexBuffer = *indexBuffer,
        .vertexBufferOffset = 0,
        .indexBufferOffset = 0,
        .indexCount = static_cast<u32>(description.indices.size())
    };
}

void Renderer::freePrimitive(Primitive primitive) {
    m_device.destroyBuffer(primitive.indexBuffer);
    m_device.destroyBuffer(primitive.vertexBuffer);
}

Result<void> Renderer::submit(const RenderRequest& request) {
    if (m_pendingRequests.size() >= m_config.renderer.maxFramesInFlight) {
        return Error::unexpected(
            ErrorCode::tooManyFrameRequests, "Too many frame requests"
        );
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

Result<ShaderHandle> Renderer::createShader(
    const ShaderDescription& description
) {
    return m_device.createShader(description);
}

void Renderer::destroyShader(ShaderHandle handle) {
    m_device.destroyShader(handle);
}

u64 Renderer::frameIndex() const {
    return m_frameNumber % m_config.renderer.maxFramesInFlight;
}

void Renderer::tryToSubmitFrame() {
    const auto& request = m_pendingRequests.front();
    NAMED_DEFER(requestDropper) { m_pendingRequests.pop(); };

    auto res = m_device.trySubmitFrame([&](RenderFrameRecorder& context) {
        return recordFrame(context, request);
    });

    if (not res) [[unlikely]] {
        if (res.error().code() != ErrorCode::tooManyFramesInFlight) [[likely]] {
            log::error("Failed to record frame: {}", res.error());
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
                    GraphicsPipelineDescription description{
                        .format = TextureFormat::bgra8unorm,
                        .shader = renderable.shader
                    };

                    auto pipeline = resolveGraphicsPipeline(description);

                    if (not pipeline) [[unlikely]] {
                        return Error::unexpected(pipeline.error());
                    }

                    if (auto res = encoder.setPipeline(*pipeline); not res) {
                        return Error::unexpected(res.error());
                    }

                    for (const auto& primitive : renderable.primitives) {
                        encoder.setVertexBuffer({
                            .handle = primitive.vertexBuffer,
                            .offset = primitive.vertexBufferOffset,
                        });

                        encoder.setIndexBuffer({
                            .handle = primitive.indexBuffer,
                            .offset = primitive.indexBufferOffset,
                        });
                        encoder.drawIndexed(primitive.indexCount);
                    }
                }
                return {};
            }
        );
    }

    return graph.record(recorder);
}

}  // namespace sl
