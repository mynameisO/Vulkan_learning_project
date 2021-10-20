/*
Credit : Brandon Galea
Youtube : https://www.youtube.com/c/BrendanGalea/videos
*/
#pragma once

#include "MEWindow.hpp"

// std lib headers
#include <string>
#include <vector>

namespace ME {

struct SwapChainSupportDetails {
  VkSurfaceCapabilitiesKHR capabilities;
  std::vector<VkSurfaceFormatKHR> formats;
  std::vector<VkPresentModeKHR> presentModes;
};

struct QueueFamilyIndices {
  uint32_t graphicsFamily;
  uint32_t presentFamily;
  bool graphicsFamilyHasValue = false;
  bool presentFamilyHasValue = false;
  bool isComplete() { return graphicsFamilyHasValue && presentFamilyHasValue; }
};

class MEDevice {
 public:
#ifdef NDEBUG
  const bool enableValidationLayers = false;
#else
  const bool enableValidationLayers = true;
#endif

  MEDevice(MEWindow &window);
  ~MEDevice();

  // Not copyable or movable
  MEDevice(const MEDevice &) = delete;
  void operator=(const MEDevice &) = delete;
  MEDevice(MEDevice &&) = delete;
  MEDevice &operator=(MEDevice &&) = delete;

  VkCommandPool getCommandPool() { return commandPool; }
  VkDevice device() { return device_; }
  VkSurfaceKHR surface() { return surface_; }
  VkQueue graphicsQueue() { return graphicsQueue_; }
  VkQueue presentQueue() { return presentQueue_; }
  VkSwapchainKHR swapchain() {return swapChain_;}
  VkPhysicalDevice getPhysicalDevice() {return physicalDevice;}
  std::vector<VkImageView> swapChainImageView(){return swapChainImageView_;}

  VkFormat swapChainImageFormat(){return swapChainImageFormat_;}
  VkExtent2D swapChainExtent(){return swapChainExtent_;}

  bool createRenderPass(VkRenderPass& renderpass);
  void createViewportAndScissor(VkViewport& viewport,VkRect2D& scissor);

  SwapChainSupportDetails getSwapChainSupport() { return querySwapChainSupport(physicalDevice); }
  QueueFamilyIndices findPhysicalQueueFamilies() { return findQueueFamilies(physicalDevice); }

  VkPhysicalDeviceProperties properties;

 private:
  void createInstance();
  void setupDebugMessenger();
  void createSurface();
  void pickPhysicalDevice();
  void createLogicalDevice();
  void createCommandPool();
  void createSwapChain();
  void createImageView();

  // helper functions
  bool isDeviceSuitable(VkPhysicalDevice device);
  std::vector<const char *> getRequiredExtensions();
  bool checkValidationLayerSupport();
  QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
  void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo);
  void hasGflwRequiredInstanceExtensions();
  bool checkDeviceExtensionSupport(VkPhysicalDevice device);
  SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
  VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availabbleFormat);
  VkPresentModeKHR choosePresentMode(const std::vector<VkPresentModeKHR>& availableModes);
  VkExtent2D chooseExtent(const VkSurfaceCapabilitiesKHR& capabilities);
  
  VkInstance instance;
  VkDebugUtilsMessengerEXT debugMessenger;
  VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
  MEWindow &window;
  VkCommandPool commandPool;

  VkDevice device_;
  VkSurfaceKHR surface_;
  VkQueue graphicsQueue_;
  VkQueue presentQueue_;
  VkSwapchainKHR swapChain_;
  std::vector<VkImage> swapChainImage_;
  VkFormat swapChainImageFormat_;
  VkExtent2D swapChainExtent_;
  std::vector<VkImageView> swapChainImageView_;

  const std::vector<const char *> validationLayers = {"VK_LAYER_KHRONOS_validation"};
  const std::vector<const char *> deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
};

}  // namespace lve