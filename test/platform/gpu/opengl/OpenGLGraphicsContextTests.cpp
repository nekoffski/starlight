#include <starlight/platform/gpu/opengl/OpenGLGraphicsContext.h>

#include <gtest/gtest.h>

using namespace sl::platform::gpu::opengl;

TEST(OpenGLContextTests, givenOpenGLGraphicsContext_whenCreating_expectNoError) {
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(100, 100, "N/A", nullptr, nullptr);

    EXPECT_NO_THROW(OpenGLGraphicsContext((void*)window).init());

    glfwDestroyWindow(window);
}
