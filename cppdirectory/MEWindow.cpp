#include "MEWindows.hpp"

namespace ME{

    //constructor and destructor

    MEWindow::MEWindow(uint32_t width,uint32_t height,std::string name)
    :width(width),height(height),name(name)
    {
        initWindow();
    }

    MEWindow::~MEWindow(){
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    //window function

    bool MEWindow::windowSouldClose(){
        return glfwWindowShouldClose(window);
    }

    void MEWindow::initWindow(){
        glfwInit();

        glfwWindowHint(GLFW_CLIENT_API,GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE,GLFW_FALSE);

        window = glfwCreateWindow(width,height,name.c_str(),nullptr,nullptr);
    }
}