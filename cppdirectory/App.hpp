#pragma once
#include "MEWindow.hpp"
#include "MEDevice.hpp"


namespace ME{

class MEApp{
private:
    static constexpr uint32_t width = 800;
    static constexpr uint32_t height = 600;
    std::string name;

    MEWindow window{width,height,name};
    MEDevice device{window};
public:
    void run();
    MEApp(const std::string name);
};

};