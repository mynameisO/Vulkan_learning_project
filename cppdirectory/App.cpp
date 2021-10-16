#include "App.hpp"

namespace ME{

MEApp::MEApp(const std::string name):name(name){}

void MEApp::run(){
    while(!window.windowShouldClose()){
        glfwPollEvents();
    }
}
};