#include "window.hpp"

#include <iostream>

static void glfw_resize_callback(GLFWwindow*, int w, int h)
{
    glViewport(0, 0, w, h);
}

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

window::window(
    uint32_t sz_x, 
    uint32_t sz_y, 
    const std::string& title,
    window_opengl_hints hints)
{
    if (!glfwInit())
        std::exit(-1);

    //glfwWindowHint(GLFW_SAMPLES, hints.aa_samples);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, hints.ver_major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, hints.ver_minor);
    _wndptr = glfwCreateWindow(sz_x, sz_y, title.c_str(), NULL, NULL);

    if (_wndptr == NULL) 
    {
        std::cout << "GLFW Window creation failure!" << std::endl;
        glfwTerminate();
        std::exit(-1);
    }

    glfwMakeContextCurrent(_wndptr);
    glfwSwapInterval(1);
    glfwSetFramebufferSizeCallback(_wndptr, &glfw_resize_callback);
    glfwSetErrorCallback(glfw_error_callback);

    int glad_ld_res = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    if(!glad_ld_res)
    {
        std::cout << "GLAD loading failure!" << std::endl;
        glfwTerminate();
        std::exit(-1);
    }
}

bool window::should_close()
{
    return glfwWindowShouldClose(_wndptr);
}

GLFWwindow* window::wnd_ptr()
{
    return _wndptr;
}