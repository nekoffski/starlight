from conan import ConanFile
from conan.errors import ConanInvalidConfiguration
from conan.tools.cmake import cmake_layout


class StarlightConan(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeDeps", "CMakeToolchain"

    default_options = {
        "*:fPIC": True,
        "*:shared": False,
    }

    def requirements(self):
        self.requires("fmt/10.2.1")
        self.requires("spdlog/1.14.1")
        self.requires("stduuid/1.2.3")
        self.requires("stb/cci.20240531")

        if self.settings.os == "Macos":
            self.requires("metal-cpp/26")
        elif self.settings.os == "Linux":
            self.requires("vulkan-headers/1.4.350.0")
            self.requires("vulkan-loader/1.4.350.0")

    def build_requirements(self):
        self.test_requires("gtest/1.15.0")

    def validate(self):
        if self.settings.os not in ("Macos", "Linux"):
            raise ConanInvalidConfiguration(
                "Starlight currently supports only macOS and Linux."
            )

    def layout(self):
        cmake_layout(self)

