#include "MEApp.hpp"

namespace ME{

MEApp::MEApp(uint32_t width,uint32_t height,std::string name):width(width),height(height),name(name){}

void MEApp::run(){
    while(!OurWindow.windowSouldClose()){
        glfwPollEvents();
    }
}
}