#include "MetalShader.hh"

#include <dispatch/dispatch.h>

#include <ranges>

#include "starlight/core/Functional.hh"

namespace sl {

namespace {

Str parseMetalError(NS::Error* error) {
    if (not error) {
        return "Unknown Metal error";
    }

    auto* description = error->localizedDescription();

    if (not description) {
        return "Unknown Metal error";
    }

    return Str{description->utf8String()};
}

}  // namespace

Result<std::unique_ptr<MetalShader>> MetalShader::create(
    MetalContext& ctx, const ShaderDescription& description
) {
    if (description.byteCode.empty() || description.stages.empty()) {
        return Error::unexpected(
            ErrorCode::invalidArgument, "Invalid shader description"
        );
    }

    auto* pool = NS::AutoreleasePool::alloc()->init();
    DEFER { pool->release(); };

    auto* data = dispatch_data_create(
        description.byteCode.data(), description.byteCode.size(), nullptr,
        DISPATCH_DATA_DESTRUCTOR_DEFAULT
    );

    if (not data) {
        return Error::unexpected(
            ErrorCode::metalError, "Failed to create dispatch data"
        );
    }

    NAMED_DEFER(dataReleaser) { dispatch_release(data); };

    NS::Error* nativeError = nullptr;
    auto* library = ctx.device().newLibrary(data, &nativeError);

    if (not library) {
        return Error::unexpected(
            ErrorCode::metalError, "Could not create Metal library: {}",
            parseMetalError(nativeError)
        );
    }

    NAMED_DEFER(libraryReleaser) { library->release(); };

    Functions functions;

    NAMED_DEFER(functionsReleaser) {
        functions.forEach([](auto* function) { function->release(); });
    };

    for (const auto& [stage, module] : description.stages) {
        auto* name = NS::String::string(
            module.entryPoint.c_str(), NS::UTF8StringEncoding
        );

        auto* function = library->newFunction(name);

        if (not function) {
            return Error::unexpected(
                ErrorCode::metalError, "Could not create Metal function: {}",
                module.entryPoint
            );
        }

        functions.emplace(stage, function);
    }

    functionsReleaser.dismiss();
    libraryReleaser.dismiss();

    return std::make_unique<MetalShader>(
        library, std::move(functions), Guard{}
    );
}

MetalShader::~MetalShader() {
    m_functions.forEach([](auto* function) { function->release(); });
    m_library->release();
}

MetalShader::MetalShader(MTL::Library* library, Functions&& functions, Guard)
    : m_library(library), m_functions(std::move(functions)) {}

}  // namespace sl
