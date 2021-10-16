#include "MEWindow.hpp"

namespace ME{

void MEWindow::createWindow(){
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API,GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE,GL_FALSE);

    window = glfwCreateWindow(width,height,name.c_str(),nullptr,nullptr);
}

bool MEWindow::windowShouldClose(){
    return glfwWindowShouldClose(window);
}

MEWindow::MEWindow(const uint32_t width,const uint32_t height,const std::string& name)
    :width(width),height(height),name(name)
{
    createWindow();
}

MEWindow::~MEWindow(){
    glfwDestroyWindow(window);
    glfwTerminate();
}

void MEWindow::createWindowSurface(VkInstance instance,VkSurfaceKHR* surface){
    if(glfwCreateWindowSurface(instance,window,nullptr,surface) != VK_SUCCESS){
        throw std::runtime_error("fail to create surface!");
    }
}

void MEWindow::getFrameBufferSize(int* width,int* height){
    glfwGetFramebufferSize(window,width,height);
}

};