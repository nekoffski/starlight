// #pragma once

// #include "starlight/core/log/Logger.h"
// #include "starlight/core/path/PathManager.hpp"
// #include "starlight/platform/model/ModelLoader.h"
// #include "starlight/platform/shader/Shader.h"
// #include "starlight/platform/shader/ShaderCompiler.hpp"
// #include "starlight/platform/texture/Cubemap.h"
// #include "starlight/platform/texture/Texture.h"

// namespace sl::asset {

// template <typename T>
// struct AssetLoader {
// };

// template <>
// struct AssetLoader<platform::texture::Cubemap> {
// };

// template <>
// struct AssetLoader<platform::shader::Shader> {
//     std::shared_ptr<platform::shader::Shader> load(AssetLoaderArgs<Shader> args) {
//         SL_DEBUG("loading shader: \n {}/{}/{}", args.vertexPath, args.fragmentPath, args.geometryPath);
//         auto shader = platform::shader::Shader::create(core::path::PathManager::createGlobalPath<Shader>(args.vertexPath),
//             core::path::PathManager::createGlobalPath<Shader>(args.fragmentPath), core::path::PathManager::createGlobalPath<Shader>(args.geometryPath));
//         platform::shader::ShaderCompiler::compile(shader);
//         return shader;
//     }
// };

// template <>
// struct AssetLoader<platform::texture::Texture> {
// };

// template <>
// struct AssetLoader<geometry::Model> {
// };
// }
