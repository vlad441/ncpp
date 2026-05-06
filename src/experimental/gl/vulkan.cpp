//https://github.com/KhronosGroup/Vulkan-Headers/blob/main/include/vulkan/vk_platform.h
//https://github.com/KhronosGroup/Vulkan-Headers/blob/main/include/vulkan/vulkan_core.h
//https://github.com/KhronosGroup/Vulkan-Headers/blob/main/include/vulkan/vulkan.h
#ifndef VULKAN_LOADER_H
#define VULKAN_LOADER_H
#include "vulkan/vulkan.h"
#endif

//#include "vulkan/vulkan_win32.h" - for Windows
//#include "vulkan/vulkan_xlib.h" - for X11
//#include "vulkan/vulkan_xcb.h" - for xcb (X11)
//#include "vulkan/vulkan_wayland.h" - for Wayland
//#include "vulkan/vulkan_android.h" - for Vedroid

//#include "vulkan/vulkan_loader.h"

typedef PFN_vkVoidFunction (VKAPI_PTR *PFN_vkGetInstanceProcAddr)(VkInstance instance, const char* pName); // Тип функции vkGetInstanceProcAddr

void* h_vulkanLib = NULL;
//extern PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr; //Наша глобальная точка входа (уже определен в vulkan_core.h)
PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr = NULL;

namespace ncpp { namespace VK {
bool loadVKLib(){ 
#ifdef _WIN32
	h_vulkanLib = DLIB_LOAD("vulkan-1.dll");
#else
    h_vulkanLib = DLIB_LOAD("libvulkan.so.1"); if(!h_vulkanLib) h_vulkanLib = DLIB_LOAD("libvulkan.so");
#endif
    if(!h_vulkanLib){ print("(!) Failed to load Vulkan lib\n"); return false; } return true; }

void freeVKLib(){ if(h_vulkanLib){ DLIB_FREE(h_vulkanLib); h_vulkanLib=NULL; } }

bool initVulkan(){ if(!loadVKLib()) return false;
    vkGetInstanceProcAddr = (PFN_vkGetInstanceProcAddr)DGET_ADDR(h_vulkanLib, "vkGetInstanceProcAddr"); if(!vkGetInstanceProcAddr){ freeVKLib(); return false; } 
	return true; }
} }

namespace ncpp { namespace GUI {
    struct VKWindow : Window { VkInstance instance; VkSurfaceKHR surface; VkDevice device;
        // ... другие объекты Vulkan (Swapchain, Queues)

        VKWindow(const char* name) : Window(name){ createVulkanSurface(); }

        void createVulkanSurface(){
            VkInstanceCreateInfo createInfo; // 1. Создание Instance (нужно включить расширения для Surface)
            vkCreateInstance(&createInfo, nullptr, &instance); // Настройка слоев и расширений...
#ifdef _WIN32
            // 2. Создание поверхности для Windows
            VkWin32SurfaceCreateInfoKHR surfaceInfo = {};
            surfaceInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
            surfaceInfo.hwnd = wndID; // Ваш ID окна из базового класса
            surfaceInfo.hinstance = GetModuleHandle(NULL);
            
            if(vkCreateWin32SurfaceKHR(instance, &surfaceInfo, nullptr, &surface) != VK_SUCCESS){
                print("(!) Failed to create Vulkan Surface (Win32)\n"); }
#else
            // 2. Создание поверхности для X11 (Linux)
            VkXlibSurfaceCreateInfoKHR surfaceInfo;
            surfaceInfo.sType = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR;
            surfaceInfo.dpy = app->display; // Из вашего объекта App
            surfaceInfo.window = wndID;

            if(vkCreateXlibSurfaceKHR(instance, &surfaceInfo, nullptr, &surface) != VK_SUCCESS){
                print("(!) Failed to create Vulkan Surface (Xlib)\n"); }
#endif
            print("(#) Vulkan Surface created successfully.\n"); }
			
		createGLContext(bool useDoubleBuff=false){ createVulkanSurface(); }
		void destroyGLContext();
		bool setContext();
		void resetContext();
	
        ~VKWindow(){
            if(surface) vkDestroySurfaceKHR(instance, surface, nullptr);
            if(device) vkDestroyDevice(device, nullptr);
            if(instance) vkDestroyInstance(instance, nullptr); }

        // Вместо swapBuffers в Vulkan используется vkQueuePresentKHR
        void present(){} // Логика вывода кадра на экран (Present Index, Semaphores)
    };
} }