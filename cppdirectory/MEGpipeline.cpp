#include "MEGpipeline.hpp"

namespace ME{

MEGpipeline::MEGpipeline(MEDevice& device,std::string vertFilename,std::string fragFilename):device_(device){
    try{
        createPipeline(vertFilename,fragFilename);
    }catch(std::exception& e){
        std::cerr << e.what() << std::endl;
    }
}

MEGpipeline::~MEGpipeline(){
    vkDestroyPipeline(device_.device(),pipeline,nullptr);
    vkDestroyPipelineLayout(device_.device(),pipelineLayout,nullptr);
    vkDestroyRenderPass(device_.device(),renderpass,nullptr);
}
    
void MEGpipeline::createPipeline(const std::string& vertFilename,const std::string& fragFilename){
    auto vertCode = readFile(vertFilename);
    auto fragCode = readFile(fragFilename);
    std::cout << "vertex size : " << vertCode.size() << std::endl;
    std::cout << "fragment size : " << fragCode.size() << std::endl;

    VkShaderModule vertModule = createShaderModule(vertCode);
    VkShaderModule fragModule = createShaderModule(fragCode);

    VkPipelineShaderStageCreateInfo vertStageInfo{};
    vertStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vertStageInfo.pName = "main";
    vertStageInfo.module = vertModule;

    VkPipelineShaderStageCreateInfo fragStageInfo{};
    fragStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragStageInfo.pName = "main";
    fragStageInfo.module = fragModule;

    std::vector<VkPipelineShaderStageCreateInfo> ShaderStage = {vertStageInfo,fragStageInfo};

    //vertex input
    auto bindingDesciption = Vertex::getBindingDescription();
    auto attributeDescription = Vertex::getAttributeDescription();
    VkPipelineVertexInputStateCreateInfo vertInfo{};
    vertInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertInfo.vertexBindingDescriptionCount = 1;
    vertInfo.vertexAttributeDescriptionCount = static_cast<uint32_t> (attributeDescription.size());
    vertInfo.pVertexBindingDescriptions = &bindingDesciption;
    vertInfo.pVertexAttributeDescriptions = attributeDescription.data();

    //Input assambly
    VkPipelineInputAssemblyStateCreateInfo inputInfo{};
    inputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputInfo.primitiveRestartEnable = VK_FALSE;

    VkViewport viewport{};
    VkRect2D scissor{};

    device_.createViewportAndScissor(viewport,scissor);

    pipelineConfig = GpipelineConfig::defaultConfig(viewport,scissor);

    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.pushConstantRangeCount = 0;
    pipelineLayoutInfo.setLayoutCount = 0;
    pipelineLayoutInfo.pPushConstantRanges = nullptr;
    pipelineLayoutInfo.pSetLayouts = nullptr;
    
    if(vkCreatePipelineLayout(device_.device(),&pipelineLayoutInfo,nullptr,&pipelineLayout) != VK_SUCCESS){
        throw std::runtime_error("Failed to create pipelinelayout");
    }

    if(!device_.createRenderPass(renderpass)){
        throw std::runtime_error("Failed to create renderpass");
    }

    VkGraphicsPipelineCreateInfo pipelineInfo{};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = (uint32_t) (ShaderStage.size());
    pipelineInfo.pStages = ShaderStage.data();
    pipelineInfo.pVertexInputState = &vertInfo;
    pipelineInfo.pInputAssemblyState = &inputInfo;
    pipelineInfo.pViewportState = &pipelineConfig.viewportstate;
    pipelineInfo.pRasterizationState = &pipelineConfig.rasterize;
    pipelineInfo.pMultisampleState = &pipelineConfig.multisampling;
    pipelineInfo.pDepthStencilState = &pipelineConfig.depthStensilTesting;
    pipelineInfo.pColorBlendState = &pipelineConfig.colorBlending;
    pipelineInfo.pDynamicState = nullptr;
    pipelineInfo.layout = pipelineLayout;
    pipelineInfo.renderPass = renderpass;
    pipelineInfo.subpass = 0;
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
    pipelineInfo.basePipelineIndex = -1;

    if(vkCreateGraphicsPipelines(device_.device(),VK_NULL_HANDLE,1,&pipelineInfo,nullptr,&pipeline)){
        throw std::runtime_error("Failed to create pipeline");
    }

    vkDestroyShaderModule(device_.device(),vertModule,nullptr);
    vkDestroyShaderModule(device_.device(),fragModule,nullptr);
}

std::vector<char> MEGpipeline::readFile(const std::string& filename){
    std::ifstream file(filename,std::ios::ate | std::ios::binary);

    if(!file.is_open()){
        throw std::runtime_error("failed to open file.");
    }

    size_t fileSize = file.tellg();

    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(),fileSize);

    file.close();
    return buffer;
}

VkShaderModule MEGpipeline::createShaderModule(const std::vector<char>& code){
    VkShaderModuleCreateInfo createInfo{};
    createInfo.codeSize =  static_cast<uint32_t>(code.size());
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

    VkShaderModule shaderModule;

    if(vkCreateShaderModule(device_.device(),&createInfo,nullptr,&shaderModule) != VK_SUCCESS){
        throw std::runtime_error("Fail to create shader module.");
    }

    return shaderModule;
}

GpipelineConfig GpipelineConfig::defaultConfig(VkViewport viewport,VkRect2D scissor){
    GpipelineConfig config;

    //viewport
    config.viewport = viewport;
    config.scissor = scissor;
    config.viewportstate.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    config.viewportstate.viewportCount = 1;
    config.viewportstate.pViewports = &config.viewport;
    config.viewportstate.scissorCount = 1;
    config.viewportstate.pScissors = &config.scissor;

    //rasterizer
    config.rasterize.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    config.rasterize.depthClampEnable = VK_FALSE;
    config.rasterize.rasterizerDiscardEnable = VK_FALSE;
    config.rasterize.polygonMode = VK_POLYGON_MODE_FILL;
    config.rasterize.lineWidth = 1.0f;
    config.rasterize.cullMode = VK_CULL_MODE_BACK_BIT;
    config.rasterize.frontFace = VK_FRONT_FACE_CLOCKWISE;
    config.rasterize.depthBiasEnable = VK_FALSE;
    config.rasterize.depthBiasClamp = 0.0f;
    config.rasterize.depthBiasConstantFactor = 0.0f;
    config.rasterize.depthBiasSlopeFactor = 0.0f;

    //multisampling
    VkPipelineMultisampleStateCreateInfo multisampling{};
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_FALSE;
    multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    multisampling.minSampleShading = 1.0f; // Optional
    multisampling.pSampleMask = nullptr; // Optional
    multisampling.alphaToCoverageEnable = VK_FALSE; // Optional
    multisampling.alphaToOneEnable = VK_FALSE; // Optional
    config.multisampling = multisampling;

    //depthStencil
    VkPipelineDepthStencilStateCreateInfo depthStencilInfo{};
    depthStencilInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    depthStencilInfo.depthTestEnable = VK_TRUE;
    depthStencilInfo.depthWriteEnable = VK_TRUE;
    depthStencilInfo.depthCompareOp = VK_COMPARE_OP_LESS;
    depthStencilInfo.depthBoundsTestEnable = VK_FALSE;
    depthStencilInfo.minDepthBounds = 0.0f;  // Optional
    depthStencilInfo.maxDepthBounds = 1.0f;  // Optional
    depthStencilInfo.stencilTestEnable = VK_FALSE;
    depthStencilInfo.front = {};  // Optional
    depthStencilInfo.back = {};   // Optional
    config.depthStensilTesting = depthStencilInfo;
    
    //color blending
    config.colorAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    config.colorAttachment.blendEnable = VK_FALSE;
    config.colorAttachment.colorBlendOp = VK_BLEND_OP_ADD;
    config.colorAttachment.alphaBlendOp = VK_BLEND_OP_ADD;
    config.colorAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
    config.colorAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    config.colorAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    config.colorAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;

    config.colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    config.colorBlending.logicOpEnable = VK_FALSE;
    config.colorBlending.logicOp = VK_LOGIC_OP_COPY;
    config.colorBlending.attachmentCount = 1;
    config.colorBlending.pAttachments = &config.colorAttachment;
    config.colorBlending.blendConstants[0] = 0.0f;
    config.colorBlending.blendConstants[1] = 0.0f;
    config.colorBlending.blendConstants[2] = 0.0f;
    config.colorBlending.blendConstants[3] = 0.0f;

    return config;
}

}