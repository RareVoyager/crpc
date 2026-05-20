from conan import ConanFile
from conan.tools.cmake import CMakeDeps, CMakeToolchain, cmake_layout

# conan install . --output-folder=conan --build=missing -s build_type=Debug
# conan install . --output-folder=conan --build=missing -s build_type=Release

class CrpcRecipe(ConanFile):
    name = "crpc"
    version = "0.1.0"

    settings = "os", "compiler", "build_type", "arch"

    generators = "CMakeDeps", "CMakeToolchain"

    requires = (
        "protobuf/6.33.5",
        "zookeeper-client-c/3.9.5",
    )

    def layout(self):
        cmake_layout(self)

    def configure(self):
        self.settings.compiler.cppstd = "17"