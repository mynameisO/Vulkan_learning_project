#pragma once
#include "MEGpipeline.hpp"

namespace ME{

class MEObject{
private:
    std::vector<Vertex> vertices;
    VkBuffer vertexBuffer;
    VkDeviceMemory vertexBufferMemory;
    MEDevice& device;
    uint32_t findMemorytype(uint32_t typefilter, VkMemoryPropertyFlags properties);
    void createBuffer();
    void allocateBuffer();
    void mapMemory();

public:
    MEObject(std::vector<Vertex>& vertices,MEDevice& device);
//    MEObject(const std::string objectModel);
    void Bind(VkCommandBuffer commandBuffer);
    void Draw(VkCommandBuffer commandBuffer);
    ~MEObject();
};

}