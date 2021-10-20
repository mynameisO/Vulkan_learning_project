#pragma once
#define GLFW_INCLUDE_VULKAN
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>
#include <array>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
namespace ME{

struct Vertex{
    glm::vec2 pos;
    glm::vec3 color;
    static VkVertexInputBindingDescription getBindingDescription(){
        VkVertexInputBindingDescription bindingInfo{};
        bindingInfo.binding = 0;
        bindingInfo.stride = sizeof(Vertex);
        bindingInfo.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
        return bindingInfo;
    }
    static std::array<VkVertexInputAttributeDescription,2> getAttributeDescription(){
        std::array<VkVertexInputAttributeDescription,2> attributeInfo{};

        attributeInfo[0].binding = 0;
        attributeInfo[0].location = 0;
        attributeInfo[0].format = VK_FORMAT_R32G32_SFLOAT;
        attributeInfo[0].offset = offsetof(Vertex,pos);

        attributeInfo[1].binding = 0;
        attributeInfo[1].location = 1;
        attributeInfo[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeInfo[1].offset = offsetof(Vertex,color);

        return attributeInfo;
    }
};

}
