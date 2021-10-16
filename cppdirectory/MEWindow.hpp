#pragma once
#include"glfwheader.hpp"

namespace ME{

class MEWindow{
private: 
    GLFWwindow* window;
    const uint32_t width;
    const uint32_t height;
    std::string name;

    MEWindow(const MEWindow&) = delete;
    MEWindow& operator=(const MEWindow&) = delete;

    void createWindow();
public:
    MEWindow(const uint32_t width,const uint32_t height,const std::string& name);
    ~MEWindow();
    bool windowShouldClose();
    void createWindowSurface(VkInstance instance,VkSurfaceKHR* surface);
    void getFrameBufferSize(int* width,int* height);
};
};