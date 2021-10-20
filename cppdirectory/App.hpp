#pragma once
#include "MEWindow.hpp"
#include "MEDevice.hpp"
#include "MEGpipeline.hpp"
#include "Object.hpp"

namespace ME{

class MEApp{
private:
    static constexpr uint32_t width = 800;
    static constexpr uint32_t height = 600;
    static constexpr int MAX_FRAMES_IN_FLIGHT = 2;
    size_t currentFrame = 0;
    std::string name;

    std::vector<Vertex> vertices = {
        {{0.0f,-0.5f},{1.0f,0.0f,0.0f}},
        {{0.5f,0.5f},{0.0f,1.0f,0.0f}},
        {{-0.5f,0.5f},{0.0f,0.0f,1.0f}}
    };
    MEWindow window{width,height,name};
    MEDevice device{window};
    MEObject triangle{vertices,device};
    MEGpipeline graphicPipeline{device,"Shader/Shader32.vert.spv","Shader/Shader32.frag.spv"};
    std::vector<VkFramebuffer> frameBuffer;
    std::vector<VkCommandBuffer> commandBuffer;
    std::vector<VkSemaphore> imageAvailableSemephore;
    std::vector<VkSemaphore> renderFinishedSemephore;
    std::vector<VkFence> inFlightFences;
    std::vector<VkFence> imageInFlight;

    void createFrameBuffer();
    void createCommandBuffer();
    void drawFrame();
    void createSyncObject();

    void destroyFrame();
    void destroyCommandBuffer();
public:
    void run();
    MEApp(const std::string name);
    ~MEApp();
};

};