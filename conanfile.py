import os

from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMake, cmake_layout, CMakeDeps


requirements = [
    "vulkan-loader/1.3.290.0",
    "vulkan-headers/1.3.290.0",
    "glfw/3.4",
    "fmt/10.2.1",
    "spdlog/1.14.1",
    "gtest/1.15.0",
    "benchmark/1.9.0",
    "boost/1.86.0",
    "nlohmann_json/3.11.3",
    "glm/cci.20230113",
    "spirv-cross/cci.20211113",
    "kstd/1.0"
]

env_variables_names = [
    'SL_ENABLE_COVERAGE',
    'SL_BUILD_TESTS',
    'SL_BUILD_BENCHMARKS',
    'SL_BUILD_SANDBOX',
    'SL_BUILD_EDITOR',
    'SL_RUN_TESTS',
    'SL_RUN_BENCHMARKS',
    'SL_WERR',
    'SL_ENABLE_SANITIZERS'
]

variables = {k: int(os.getenv(k, 0)) for k in env_variables_names}


class Recipe(ConanFile):
    settings = "os", "compiler", "build_type", "arch"

    def requirements(self):
        for requirement in requirements:
            self.requires(requirement)

    def layout(self):
        cmake_layout(self)

    def generate(self):
        deps = CMakeDeps(self)
        deps.generate()
        tc = CMakeToolchain(self)
        for k, v in variables.items():
            tc.variables[k] = v
        tc.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

        if variables['SL_RUN_TESTS']:
            self.test()

        if variables['SL_RUN_BENCHMARKS']:
            self.benchmark()

    def test(self):
        self.output.info("Running tests...")
        self.run('ctest -VV --output-on-failure --test-dir ./engine')

    def benchmark(self):
        self.output.info("Running benchmarks...")
        self.run("engine/bench/starlight_benchmark")
