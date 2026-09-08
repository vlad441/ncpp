//https://github.com/KhronosGroup/Vulkan-Headers/blob/main/include/vulkan/vk_platform.h
//https://github.com/KhronosGroup/Vulkan-Headers/blob/main/include/vulkan/vulkan_core.h
//https://github.com/KhronosGroup/Vulkan-Headers/blob/main/include/vulkan/vulkan.h
//https://github.com/KhronosGroup/glslang
#ifndef VULKAN_LOADER_H
#define VULKAN_LOADER_H
#include "vulkan/vulkan.h"
#endif

//============================= Глобальные указатели на функции Vulkan =============================
PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr = NULL;

// Функции уровня Instance (глобальные и инстансные)
PFN_vkCreateInstance vkCreateInstance = NULL;
PFN_vkEnumerateInstanceExtensionProperties vkEnumerateInstanceExtensionProperties = NULL;
PFN_vkEnumerateInstanceLayerProperties vkEnumerateInstanceLayerProperties = NULL;
PFN_vkDestroyInstance vkDestroyInstance = NULL;
PFN_vkEnumeratePhysicalDevices vkEnumeratePhysicalDevices = NULL;
PFN_vkGetPhysicalDeviceProperties vkGetPhysicalDeviceProperties = NULL;
PFN_vkGetPhysicalDeviceFeatures vkGetPhysicalDeviceFeatures = NULL;
PFN_vkGetPhysicalDeviceQueueFamilyProperties vkGetPhysicalDeviceQueueFamilyProperties = NULL;
PFN_vkCreateDevice vkCreateDevice = NULL;
PFN_vkGetDeviceProcAddr vkGetDeviceProcAddr = NULL;

// Функции поверхности и презентации (WSI)
PFN_vkDestroySurfaceKHR vkDestroySurfaceKHR = NULL;
PFN_vkGetPhysicalDeviceSurfaceSupportKHR vkGetPhysicalDeviceSurfaceSupportKHR = NULL;
PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR vkGetPhysicalDeviceSurfaceCapabilitiesKHR = NULL;
PFN_vkGetPhysicalDeviceSurfaceFormatsKHR vkGetPhysicalDeviceSurfaceFormatsKHR = NULL;
PFN_vkGetPhysicalDeviceSurfacePresentModesKHR vkGetPhysicalDeviceSurfacePresentModesKHR = NULL;

#ifdef _WIN32
PFN_vkCreateWin32SurfaceKHR vkCreateWin32SurfaceKHR = NULL;
#elif USE_WAYLAND
PFN_vkCreateWaylandSurfaceKHR vkCreateWaylandSurfaceKHR = NULL;
#else
PFN_vkCreateXlibSurfaceKHR vkCreateXlibSurfaceKHR = NULL;
#endif

// Функции уровня Logical Device (получаются через vkGetDeviceProcAddr)
PFN_vkGetDeviceQueue vkGetDeviceQueue = NULL;
PFN_vkDestroyDevice vkDestroyDevice = NULL;
PFN_vkCreateSwapchainKHR vkCreateSwapchainKHR = NULL;
PFN_vkDestroySwapchainKHR vkDestroySwapchainKHR = NULL;
PFN_vkGetSwapchainImagesKHR vkGetSwapchainImagesKHR = NULL;
PFN_vkCreateImageView vkCreateImageView = NULL;
PFN_vkDestroyImageView vkDestroyImageView = NULL;
PFN_vkCreateShaderModule vkCreateShaderModule = NULL;
PFN_vkDestroyShaderModule vkDestroyShaderModule = NULL;
PFN_vkCreatePipelineLayout vkCreatePipelineLayout = NULL;
PFN_vkDestroyPipelineLayout vkDestroyPipelineLayout = NULL;
PFN_vkCreateRenderPass vkCreateRenderPass = NULL;
PFN_vkDestroyRenderPass vkDestroyRenderPass = NULL;
PFN_vkCreateGraphicsPipelines vkCreateGraphicsPipelines = NULL;
PFN_vkDestroyPipeline vkDestroyPipeline = NULL;
PFN_vkCreateFramebuffer vkCreateFramebuffer = NULL;
PFN_vkDestroyFramebuffer vkDestroyFramebuffer = NULL;
PFN_vkCreateCommandPool vkCreateCommandPool = NULL;
PFN_vkDestroyCommandPool vkDestroyCommandPool = NULL;
PFN_vkAllocateCommandBuffers vkAllocateCommandBuffers = NULL;
PFN_vkBeginCommandBuffer vkBeginCommandBuffer = NULL;
PFN_vkEndCommandBuffer vkEndCommandBuffer = NULL;
PFN_vkCmdBeginRenderPass vkCmdBeginRenderPass = NULL;
PFN_vkCmdEndRenderPass vkCmdEndRenderPass = NULL;
PFN_vkCmdBindPipeline vkCmdBindPipeline = NULL;
PFN_vkCmdDraw vkCmdDraw = NULL;
PFN_vkCreateSemaphore vkCreateSemaphore = NULL;
PFN_vkDestroySemaphore vkDestroySemaphore = NULL;
PFN_vkCreateFence vkCreateFence = NULL;
PFN_vkDestroyFence vkDestroyFence = NULL;
PFN_vkWaitForFences vkWaitForFences = NULL;
PFN_vkResetFences vkResetFences = NULL;
PFN_vkAcquireNextImageKHR vkAcquireNextImageKHR = NULL;
PFN_vkQueueSubmit vkQueueSubmit = NULL;
PFN_vkQueuePresentKHR vkQueuePresentKHR = NULL;
PFN_vkQueueWaitIdle vkQueueWaitIdle = NULL;
PFN_vkDeviceWaitIdle vkDeviceWaitIdle = NULL;

PFN_vkCreateBuffer vkCreateBuffer = NULL;
PFN_vkDestroyBuffer vkDestroyBuffer = NULL;
PFN_vkGetBufferMemoryRequirements vkGetBufferMemoryRequirements = NULL;
PFN_vkAllocateMemory vkAllocateMemory = NULL;
PFN_vkFreeMemory vkFreeMemory = NULL;
PFN_vkBindBufferMemory vkBindBufferMemory = NULL;
PFN_vkCmdBindVertexBuffers vkCmdBindVertexBuffers = NULL;

namespace ncpp { namespace GL { void* h_vulkanLib = NULL;

bool loadVKLib(){
#ifdef _WIN32
    h_vulkanLib = DLIB_LOAD("vulkan-1.dll");
#else
    h_vulkanLib = DLIB_LOAD("libvulkan.so.1"); if(!h_vulkanLib) h_vulkanLib = DLIB_LOAD("libvulkan.so");
#endif
    if(!h_vulkanLib){ Except("Vulkan: loadVKLib() failed.\n"); return false; }
    
	// Единственная гарантированная экспортируемая функция из динамической библиотеки
    vkGetInstanceProcAddr = (PFN_vkGetInstanceProcAddr)DGET_ADDR(h_vulkanLib, "vkGetInstanceProcAddr"); 
    if(!vkGetInstanceProcAddr){ Except("Vulkan: Load function vkGetInstanceProcAddr() failed.\n"); freeVKLib(); return false; }
    return true; //loadVKCore()
}
    
void freeVKLib(){ if(h_vulkanLib){ DLIB_FREE(h_vulkanLib); h_vulkanLib=NULL; } }

#define VK_LOAD_INSTANCE_FUNC(instance, name) \
    name = (PFN_##name)vkGetInstanceProcAddr(instance, #name); \
    if(!name) { Except("Vulkan: Failed to load instance function: " #name "\n"); return false; }

#define VK_LOAD_DEVICE_FUNC(device, name) \
    name = (PFN_##name)vkGetDeviceProcAddr(device, #name); \
    if(!name) { Except("Vulkan: Failed to load device function: " #name "\n"); return false; }

// 1. Загрузка глобальных функций (до создания инстанса)
bool loadVKCore(){ if(!loadVKLib()) return false;
    VK_LOAD_INSTANCE_FUNC(NULL, vkCreateInstance);
    VK_LOAD_INSTANCE_FUNC(NULL, vkEnumerateInstanceExtensionProperties);
    VK_LOAD_INSTANCE_FUNC(NULL, vkEnumerateInstanceLayerProperties); return true; }

// 2. Загрузка функций инстанса (вызывается СРАЗУ ПОСЛЕ создания VkInstance)
bool loadVKFnInstance(VkInstance instance){ if(!instance) return false;
    
    VK_LOAD_INSTANCE_FUNC(instance, vkDestroyInstance);
    VK_LOAD_INSTANCE_FUNC(instance, vkEnumeratePhysicalDevices);
    VK_LOAD_INSTANCE_FUNC(instance, vkGetPhysicalDeviceProperties);
    VK_LOAD_INSTANCE_FUNC(instance, vkGetPhysicalDeviceFeatures);
    VK_LOAD_INSTANCE_FUNC(instance, vkGetPhysicalDeviceQueueFamilyProperties);
    VK_LOAD_INSTANCE_FUNC(instance, vkCreateDevice);
    VK_LOAD_INSTANCE_FUNC(instance, vkGetDeviceProcAddr);

    // WSI / Оконные и презентационные функции
    VK_LOAD_INSTANCE_FUNC(instance, vkDestroySurfaceKHR);
    VK_LOAD_INSTANCE_FUNC(instance, vkGetPhysicalDeviceSurfaceSupportKHR);
    VK_LOAD_INSTANCE_FUNC(instance, vkGetPhysicalDeviceSurfaceCapabilitiesKHR);
    VK_LOAD_INSTANCE_FUNC(instance, vkGetPhysicalDeviceSurfaceFormatsKHR);
    VK_LOAD_INSTANCE_FUNC(instance, vkGetPhysicalDeviceSurfacePresentModesKHR);

#ifdef _WIN32
    VK_LOAD_INSTANCE_FUNC(instance, vkCreateWin32SurfaceKHR);
#elif USE_WAYLAND
	VK_LOAD_INSTANCE_FUNC(instance, vkCreateWaylandSurfaceKHR);
#else
    VK_LOAD_INSTANCE_FUNC(instance, vkCreateXlibSurfaceKHR);
#endif
    return true; }

// 3. Загрузка функций логического устройства (вызывается после создания VkDevice)
bool loadVKFnDevice(VkDevice device){ if(!device) return false;

    VK_LOAD_DEVICE_FUNC(device, vkGetDeviceQueue);
    VK_LOAD_DEVICE_FUNC(device, vkDestroyDevice);
    //VK_Load_Swapchain: // метки нет, просто код:
    VK_LOAD_DEVICE_FUNC(device, vkCreateSwapchainKHR);
    VK_LOAD_DEVICE_FUNC(device, vkDestroySwapchainKHR);
    VK_LOAD_DEVICE_FUNC(device, vkGetSwapchainImagesKHR);
    VK_LOAD_DEVICE_FUNC(device, vkCreateImageView);
    VK_LOAD_DEVICE_FUNC(device, vkDestroyImageView);
    VK_LOAD_DEVICE_FUNC(device, vkCreateShaderModule);
    VK_LOAD_DEVICE_FUNC(device, vkDestroyShaderModule);
    VK_LOAD_DEVICE_FUNC(device, vkCreatePipelineLayout);
    VK_LOAD_DEVICE_FUNC(device, vkDestroyPipelineLayout);
    VK_LOAD_DEVICE_FUNC(device, vkCreateRenderPass);
    VK_LOAD_DEVICE_FUNC(device, vkDestroyRenderPass);
    VK_LOAD_DEVICE_FUNC(device, vkCreateGraphicsPipelines);
    VK_LOAD_DEVICE_FUNC(device, vkDestroyPipeline);
    VK_LOAD_DEVICE_FUNC(device, vkCreateFramebuffer);
    VK_LOAD_DEVICE_FUNC(device, vkDestroyFramebuffer);
    VK_LOAD_DEVICE_FUNC(device, vkCreateCommandPool);
    VK_LOAD_DEVICE_FUNC(device, vkDestroyCommandPool);
    VK_LOAD_DEVICE_FUNC(device, vkAllocateCommandBuffers);
    VK_LOAD_DEVICE_FUNC(device, vkBeginCommandBuffer);
    VK_LOAD_DEVICE_FUNC(device, vkEndCommandBuffer);
    VK_LOAD_DEVICE_FUNC(device, vkCmdBeginRenderPass);
    VK_LOAD_DEVICE_FUNC(device, vkCmdEndRenderPass);
    VK_LOAD_DEVICE_FUNC(device, vkCmdBindPipeline);
    VK_LOAD_DEVICE_FUNC(device, vkCmdDraw);
    VK_LOAD_DEVICE_FUNC(device, vkCreateSemaphore);
    VK_LOAD_DEVICE_FUNC(device, vkDestroySemaphore);
    VK_LOAD_DEVICE_FUNC(device, vkCreateFence);
    VK_LOAD_DEVICE_FUNC(device, vkDestroyFence);
    VK_LOAD_DEVICE_FUNC(device, vkWaitForFences);
    VK_LOAD_DEVICE_FUNC(device, vkResetFences);
    VK_LOAD_DEVICE_FUNC(device, vkAcquireNextImageKHR);
    VK_LOAD_DEVICE_FUNC(device, vkQueueSubmit);
    VK_LOAD_DEVICE_FUNC(device, vkQueuePresentKHR);
    VK_LOAD_DEVICE_FUNC(device, vkQueueWaitIdle);
    VK_LOAD_DEVICE_FUNC(device, vkDeviceWaitIdle);
	
	VK_LOAD_DEVICE_FUNC(device, vkCreateBuffer);
    VK_LOAD_DEVICE_FUNC(device, vkDestroyBuffer);
    VK_LOAD_DEVICE_FUNC(device, vkGetBufferMemoryRequirements);
    VK_LOAD_DEVICE_FUNC(device, vkAllocateMemory);
    VK_LOAD_DEVICE_FUNC(device, vkFreeMemory);
    VK_LOAD_DEVICE_FUNC(device, vkBindBufferMemory);
    VK_LOAD_DEVICE_FUNC(device, vkCmdBindVertexBuffers);
    return true; }
} }
//========================================= Vulkan 1.0 API =========================================
// namespace ncpp { namespace GL {
// bool initVK_10(){  }
// } }
//========================================= Vulkan 1.1 API =========================================
// namespace ncpp { namespace GL {
// bool initVK_11(){  } 
// } }
#undef VK_LOAD_INSTANCE_FUNC
#undef VK_LOAD_DEVICE_FUNC
//========================================= Vulkan Init =========================================
namespace ncpp { namespace GL { VkInstance g_instance; VkPhysicalDevice g_physDev; VkDevice g_dev; VkQueue g_Queue; 
	unsigned char _VKVer = 10; const unsigned char& VKVer=_VKVer;

VkInstance createVKInstance(const char* appName, char VkVer=10){ 
	VkInstance instance; VkApplicationInfo appInfo = {};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = appName;
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "ncpp GL Engine";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_0;

	const char* extensions[] = { VK_KHR_SURFACE_EXTENSION_NAME,
#ifdef _WIN32
		VK_KHR_WIN32_SURFACE_EXTENSION_NAME
#elif USE_WAYLAND
		VK_KHR_WAYLAND_SURFACE_EXTENSION_NAME
#else
		VK_KHR_XLIB_SURFACE_EXTENSION_NAME
#endif
	};
	VkInstanceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;
	createInfo.enabledExtensionCount = 2;
	createInfo.ppEnabledExtensionNames = extensions;
	createInfo.enabledLayerCount = 0;
	if(vkCreateInstance(&createInfo, NULL, &instance) != VK_SUCCESS){ Except("Failed to create Vulkan Instance!\n"); return NULL; } return instance; }
	
bool createVKDevice(){ // Physical Device
	int devCnt = 0; vkEnumeratePhysicalDevices(g_instance, &devCnt, NULL);
	if(devCnt==0){ Except("createVKDevice(): Failed to find GPUs with Vulkan support!\n"); return false; }
	VkPhysicalDevice* devices = new VkPhysicalDevice[devCnt];
	vkEnumeratePhysicalDevices(g_instance, &devCnt, devices); g_physDev = devices[0]; delete[] devices; // Берем первое доступное

	// Logical Device и очереди
	float queuePriority = 1.0f; VkDeviceQueueCreateInfo queueCreateInfo = {};
	queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queueCreateInfo.queueFamilyIndex = 0; // Упрощенно
	queueCreateInfo.queueCount = 1;
	queueCreateInfo.pQueuePriorities = &queuePriority;

	const char* deviceExtensions[] = { VK_KHR_SWAPCHAIN_EXTENSION_NAME }; VkDeviceCreateInfo devCreateInfo = {};
	devCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	devCreateInfo.queueCreateInfoCount = 1;
	devCreateInfo.pQueueCreateInfos = &queueCreateInfo;
	devCreateInfo.enabledExtensionCount = 1;
	devCreateInfo.ppEnabledExtensionNames = deviceExtensions;

	if(vkCreateDevice(g_physDev, &devCreateInfo, NULL, &g_dev) != VK_SUCCESS){ Except("createVKDevice(): Failed to create Vulkan logical device!\n"); return false; }
	vkGetDeviceQueue(g_dev, 0, 0, &g_Queue); print("(#) Vulkan context & surface successfully initialized.\n"); return true;
}

void destroyVK(){
	if(g_dev){ vkDeviceWaitIdle(g_dev); vkDestroyDevice(g_dev, NULL); g_dev = NULL; }
	if(g_instance){ vkDestroyInstance(g_instance, NULL); g_instance = NULL; } }

bool initVK(const char* name="ncpp Vulkan App"){ if(!loadVKLib()) return false;
	loadVKCore(); g_instance = createVKInstance(name); loadVKFnInstance(g_instance);
	createVKDevice(); loadVKFnDevice(g_dev);
}

const char* GLVersion(){ return "Vulkan -.-"; }
bool initGL(){ return initVK(); }

} }