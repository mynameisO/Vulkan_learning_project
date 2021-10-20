#include "App.hpp"

namespace ME{

MEApp::MEApp(const std::string name):name(name){
    try{
        createFrameBuffer();
        createCommandBuffer();
        createSyncObject();
    }catch(std::exception& e){
        std::cerr << e.what() << std::endl;
    }
}

MEApp::~MEApp(){
    for(size_t i = 0; i < MAX_FRAMES_IN_FLIGHT;i++){
        vkDestroySemaphore(device.device(),imageAvailableSemephore[i],nullptr);
        vkDestroySemaphore(device.device(),renderFinishedSemephore[i],nullptr);
        vkDestroyFence(device.device(),inFlightFences[i],nullptr);
    }
    for(auto framebuff : frameBuffer){
        vkDestroyFramebuffer(device.device(),framebuff,nullptr);
    }
}

void MEApp::run(){
    while(!window.windowShouldClose()){
        drawFrame();
        glfwPollEvents();
    }
    vkDeviceWaitIdle(device.device());
}

void MEApp::createFrameBuffer(){
    auto scImageView = device.swapChainImageView();
    auto scExtent = device.swapChainExtent();

    frameBuffer.resize(scImageView.size());
    for(size_t i = 0;i < scImageView.size();i++){
        VkImageView attachments[] = {scImageView[i]};

        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = graphicPipeline.renderPass();
        framebufferInfo.attachmentCount = 1;
        framebufferInfo.pAttachments = attachments;
        framebufferInfo.width = scExtent.width;
        framebufferInfo.height = scExtent.height;
        framebufferInfo.layers = 1;

        if(vkCreateFramebuffer(device.device(),&framebufferInfo,nullptr,&frameBuffer[i]) != VK_SUCCESS){
            throw std::runtime_error("Failed to create framebuffer.");
        }
    }
}

void MEApp::createCommandBuffer(){
    auto scImageView = device.swapChainImageView();
    auto scExtent = device.swapChainExtent();

    commandBuffer.resize(frameBuffer.size());

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandBufferCount =(uint32_t) commandBuffer.size();
    allocInfo.commandPool = device.getCommandPool();
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    if(vkAllocateCommandBuffers(device.device(),&allocInfo,commandBuffer.data())){
        throw std::runtime_error("Failed to allocate commandbuffer.");
    }

    for(size_t i = 0;i < commandBuffer.size();i++){
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.pInheritanceInfo = nullptr;
        beginInfo.flags = 0;

        if(vkBeginCommandBuffer(commandBuffer[i],&beginInfo) != VK_SUCCESS){
            throw std::runtime_error("Failed to begin recording command buffer.");
        }
        //starting a render pass
        VkRenderPassBeginInfo renderInfo{};
        renderInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderInfo.renderPass = graphicPipeline.renderPass();
        renderInfo.framebuffer = frameBuffer[i];
        renderInfo.renderArea.offset = {0,0};
        renderInfo.renderArea.extent = scExtent;

        VkClearValue clearColor = {{{0.0f,0.0f,0.0f,0.7f}}};
        renderInfo.pClearValues = &clearColor;
        renderInfo.clearValueCount = 1;

        vkCmdBeginRenderPass(commandBuffer[i],&renderInfo,VK_SUBPASS_CONTENTS_INLINE);
        //draw
        vkCmdBindPipeline(commandBuffer[i],VK_PIPELINE_BIND_POINT_GRAPHICS,graphicPipeline.graphicPipeline());
        
        triangle.Bind(commandBuffer[i]);
        triangle.Draw(commandBuffer[i]);

        vkCmdEndRenderPass(commandBuffer[i]);

        if(vkEndCommandBuffer(commandBuffer[i]) != VK_SUCCESS){
            throw std::runtime_error("Failed to record command buffer");
        }
    }
}

void MEApp::createSyncObject(){
    auto scImage = device.swapChainImageView();
    imageAvailableSemephore.resize(MAX_FRAMES_IN_FLIGHT);
    renderFinishedSemephore.resize(MAX_FRAMES_IN_FLIGHT);
    inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
    imageInFlight.resize(scImage.size(),VK_NULL_HANDLE);

    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    
    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for(size_t i = 0;i < MAX_FRAMES_IN_FLIGHT;i++){
        if( vkCreateSemaphore(device.device(),&semaphoreInfo,nullptr,&imageAvailableSemephore[i]) != VK_SUCCESS ||
            vkCreateSemaphore(device.device(),&semaphoreInfo,nullptr,&renderFinishedSemephore[i]) != VK_SUCCESS ||
            vkCreateFence(device.device(),&fenceInfo,nullptr,&inFlightFences[i]) != VK_SUCCESS
        ){
            throw std::runtime_error("Failed to create synchroniztion object for a frame.");
        }
    }
}

void MEApp::drawFrame(){
    vkWaitForFences(device.device(),1,&inFlightFences[currentFrame],VK_TRUE,UINT64_MAX);

    uint32_t imageIndex;
    vkAcquireNextImageKHR(device.device(),device.swapchain(),UINT64_MAX,imageAvailableSemephore[currentFrame],VK_NULL_HANDLE,&imageIndex);

    if(imageInFlight[imageIndex] != VK_NULL_HANDLE){
        vkWaitForFences(device.device(),1,&imageInFlight[imageIndex],VK_TRUE,UINT64_MAX);
    }

    imageInFlight[imageIndex] = inFlightFences[currentFrame];

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore wait[] = {imageAvailableSemephore[currentFrame]};
    VkPipelineStageFlags waitStage[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = wait;
    submitInfo.pWaitDstStageMask = waitStage;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer[imageIndex];

    VkSemaphore signalSemaphore[] = {renderFinishedSemephore[currentFrame]};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphore; 

    vkResetFences(device.device(),1,&inFlightFences[currentFrame]);

    if(vkQueueSubmit(device.graphicsQueue(),1,&submitInfo,inFlightFences[currentFrame]) != VK_SUCCESS){
        throw std::runtime_error("Failed to submit draw command buffer!");
    }

    VkSwapchainKHR swapChains[] = {device.swapchain()};

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphore;
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &imageIndex;
    presentInfo.pResults = nullptr;

    vkQueuePresentKHR(device.presentQueue(),&presentInfo);

    currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

};