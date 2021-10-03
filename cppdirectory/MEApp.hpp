#include "MEWindows.hpp"

namespace ME{
class MEApp{
    private:
    const uint32_t width;
    const uint32_t height;
    const std::string name;

    public:
    MEApp(uint32_t width,uint32_t height,std::string name);
    MEWindow OurWindow{width,height,name};
    void run();
};
}