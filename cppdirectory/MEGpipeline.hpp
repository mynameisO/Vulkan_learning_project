#pragma once 
#include "MEDevice.hpp"

namespace ME{

struct GpipelineConfig{
    VkViewport viewport{};
    VkRect2D scissor{};
    VkPipelineViewportStateCreateInfo viewportstate{};
    VkPipelineRasterizationStateCreateInfo rasterize{};
    VkPipelineMultisampleStateCreateInfo multisampling{};
    VkPipelineDepthStencilStateCreateInfo depthStensilTesting;
    VkPipelineColorBlendAttachmentState colorAttachment{};
    VkPipelineColorBlendStateCreateInfo colorBlending{};

    static GpipelineConfig defaultConfig(VkViewport viewport,VkRect2D scissor);
};


class MEGpipeline{
private:
    MEDevice& device_;
    GpipelineConfig pipelineConfig;
    VkRenderPass renderpass;
    VkPipelineLayout pipelineLayout;
    VkPipeline pipeline;
    void createPipeline(const std::string& vertFilename,const std::string& fragFilename);
    VkShaderModule createShaderModule(const std::vector<char>& code);
public:
    VkRenderPass renderPass(){return renderpass;}
    VkPipeline graphicPipeline(){return pipeline;}
    MEGpipeline(MEDevice& device,std::string vertFilename,std::string fragFilename);
    ~MEGpipeline();
    static std::vector<char> readFile(const std::string& filename);
};

}