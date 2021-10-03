#include "glfwHeader.hpp"

namespace ME{
class MEWindow{
    private:
    const uint32_t width;
    const uint32_t height;
    const std::string name;
    GLFWwindow* window;
    
    void initWindow();
    void desWindow();
    public:
    MEWindow& operator=(MEWindow&) = delete;
    MEWindow(MEWindow&) = delete;
    MEWindow(uint32_t width,uint32_t height,std::string name);
    bool windowSouldClose();
    ~MEWindow();
};
}