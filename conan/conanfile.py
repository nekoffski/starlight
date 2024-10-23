from conan import ConanFile


requirements = [
    "zlib/1.2.11"
]


class Recipe(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeToolchain", "CMakeDeps"

    def requirements(self):
        for requirement in requirements:
            self.requires(requirement)
