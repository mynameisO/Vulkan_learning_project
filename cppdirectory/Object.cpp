#include "Object.hpp"

namespace ME{

MEObject::MEObject(std::vector<Vertex>& vertices,MEDevice& Idevice) 
:device(Idevice),vertices(vertices)
{
    try{
        createBuffer();
        allocateBuffer();
        mapMemory();
    }catch(std::exception& e){
        std::cerr << e.what() << std::endl;
    }
}

MEObject::~MEObject(){
    vkDestroyBuffer(device.device(),vertexBuffer,nullptr);
    vkFreeMemory(device.device(),vertexBufferMemory,nullptr);
}

uint32_t MEObject::findMemorytype(uint32_t typefilter, VkMemoryPropertyFlags properties){
    VkPhysicalDeviceMemoryProperties memProperties{};
    vkGetPhysicalDeviceMemoryProperties(device.getPhysicalDevice(),&memProperties);

    for(uint32_t i = 0;i < memProperties.memoryTypeCount;i++){
        if((typefilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties)){
            return i;
        }
    }
    
    throw std::runtime_error("failed to find suitable memory type");
}

void MEObject::createBuffer(){
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = sizeof(vertices[0]) * vertices.size();
    bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if(vkCreateBuffer(device.device(),&bufferInfo,nullptr,&vertexBuffer) != VK_SUCCESS){
        throw std::runtime_error("Failed to create buffer.");
    }
}

void MEObject::allocateBuffer(){
    VkMemoryRequirements memRequiremens;
    vkGetBufferMemoryRequirements(device.device(),vertexBuffer,&memRequiremens);

    VkMemoryAllocateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    createInfo.memoryTypeIndex = findMemorytype(
                                    memRequiremens.memoryTypeBits, 
                                    VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | 
                                    VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
                                    );
    createInfo.allocationSize = memRequiremens.size;

    if(vkAllocateMemory(device.device(),&createInfo,nullptr,&vertexBufferMemory) != VK_SUCCESS){
        throw std::runtime_error("Failed to allocate memory.");
    }

    vkBindBufferMemory(device.device(),vertexBuffer,vertexBufferMemory,0);
}

void MEObject::mapMemory(){
    void* data;
    size_t memorysize = sizeof(vertices[0])*vertices.size();
    vkMapMemory(device.device(),vertexBufferMemory,0,memorysize,0,&data);
    memcpy(data,vertices.data(),memorysize);
    vkUnmapMemory(device.device(),vertexBufferMemory);
}

void MEObject::Bind(VkCommandBuffer commandBuffer){
    VkBuffer vertexBuffers[] = {vertexBuffer};
    VkDeviceSize offset[] = {0};

    vkCmdBindVertexBuffers(commandBuffer,0,1,vertexBuffers,offset);
}

void MEObject::Draw(VkCommandBuffer commandBuffer){
    vkCmdDraw(commandBuffer,vertices.size(),1,0,0);
}

}