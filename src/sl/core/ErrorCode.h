#pragma once

namespace sl::core {

enum class ErrorCode : int {
    InvalidJsonString = 30,

    CorruptedConfig = 50,

    UnknownTextureFormat = 100,

    CouldNotInitializeWindowLibrary = 200,
    CouldNotCreateWindowInstance = 201,

    CouldNotLoadGraphicsApi = 300,

    CouldNotLoadImage = 400,

    FileDoesNotExist = 450,
    CouldNotReadFile = 451,

    ProjectJsonIsInvalid = 475,
    CouldNotDeserializeScene = 476,

    CouldNotLinkShaderProgram = 500,
    CouldNotReadVertexShader = 501,
    CouldNotReadFragmentShader = 502,
    CouldNotCompileFragmentShader = 503,
    CouldNotCompileVertexShader = 504,

    CouldNotLoadModel = 600,

    CouldNotFindAssetLoader = 700,

    PathAlreadyRegistered = 800,
    PathNotRegistered = 801,

    AssetError = 900,

    GuiUserError = 1000
};
}
