namespace ncpp { namespace GUI { struct GLWindow; } namespace GL { GUI::GLWindow* _glwnd=NULL; } }

namespace ncpp { namespace GUI {
	struct GLWindow : Window { VkSurfaceKHR surface; VkSwapchainKHR swapChain;
		VkCommandPool cmdPool; VkCommandBuffer cmdBuff;
		VkSemaphore imageAvailableSemaphore; VkSemaphore renderFinishedSemaphore; VkFence inFlightFence;
		uint32_t imageIndex; char bmode; float aspect;
		
		void createGLContext(bool useDoubleBuff = true){ useDoubleBuff?bmode=2:bmode=1;
		#ifdef _WIN32
			VkWin32SurfaceCreateInfoKHR surfaceInfo = {}; surfaceInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
			surfaceInfo.hwnd = wndID; surfaceInfo.hinstance = GetModuleHandle(NULL);
			if(vkCreateWin32SurfaceKHR(GL::g_instance, &surfaceInfo, NULL, &surface) != VK_SUCCESS){ Except("Failed to create Win32 Vulkan Surface!\n"); return; }
		#elif USE_WAYLAND
			VkWaylandSurfaceCreateInfoKHR surfaceInfo = {}; surfaceInfo.sType = VK_STRUCTURE_TYPE_WAYLAND_SURFACE_CREATE_INFO_KHR;
			//surfaceInfo.display = app->wl_display; // Укажите ваш wl_display*
			//surfaceInfo.surface = (wl_surface*)wndID; // Укажите ваш wl_surface*
			if(vkCreateWaylandSurfaceKHR(GL::g_instance, &surfaceInfo, NULL, &surface) != VK_SUCCESS){ Except("Failed to create Wayland Vulkan Surface!\n"); return; }
		#else
			VkXlibSurfaceCreateInfoKHR surfaceInfo = {}; surfaceInfo.sType = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR;
			surfaceInfo.dpy = app->display; surfaceInfo.window = wndID;
			if(vkCreateXlibSurfaceKHR(GL::g_instance, &surfaceInfo, NULL, &surface) != VK_SUCCESS){ Except("Failed to create Xlib Vulkan Surface!\n"); return; }
		#endif
		}

		void destroyGLContext(){ if(GL::g_instance&&surface){ vkDestroySurfaceKHR(GL::g_instance, surface, NULL); surface = VK_NULL_HANDLE; } }

		bool setContext(){ if(!GL::g_dev){ Except("setContext(): Vulkan device not created\n"); return false; } GL::_glwnd=this; return true; }
		void resetContext(){} // Заглушка для совместимости с интерфейсом OpenGL
				
		void swapBuffers(){ if (!GL::g_dev || !swapChain) return;
			VkPresentInfoKHR presentInfo = {};
			presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
			presentInfo.waitSemaphoreCount = 0;
			presentInfo.pWaitSemaphores = NULL;
			presentInfo.swapchainCount = 1;
			presentInfo.pSwapchains = &swapChain;
			presentInfo.pImageIndices = &imageIndex;
			vkQueuePresentKHR(GL::g_Queue, &presentInfo); }

		GLWindow() : Window(), surface(NULL), swapChain(NULL){}
		GLWindow(App* app1, const char* name = "", int x = DEF_HWND_X, int y = DEF_HWND_Y, unsigned int width = DEF_HWND_WIDTH, unsigned int height = DEF_HWND_HEIGHT) 
			: Window(app1, name, x, y, width, height), surface(NULL), swapChain(NULL){ createGLContext(); }
		GLWindow(const char* name, int x = DEF_HWND_X, int y = DEF_HWND_Y, unsigned int width = DEF_HWND_WIDTH, unsigned int height = DEF_HWND_HEIGHT) 
			: Window(name, x, y, width, height), surface(NULL), swapChain(NULL){ createGLContext(); }

		~GLWindow(){ destroyGLContext(); }

		bool hasContext(){ return (GL::g_dev != NULL); }
		void draw(){ if(bmode <= 1){ if(GL::g_dev) vkDeviceWaitIdle(GL::g_dev); }else{ swapBuffers(); } }
		
		//recreateSwapchain?
		void recalcWndSize(){ int w=0, h=0; getSize(w, h); aspect = (float)w/h;
			// В Vulkan вместо glViewport подменяются динамические состояния в командном буфере, 
			// но сигнатура метода сохранена полностью.
		}
#if __cplusplus >= 201103L
		GLWindow(GLWindow&& tmp) noexcept { wndID = 0; move(*this, tmp); }
		GLWindow& operator=(GLWindow&& tmp) noexcept { move(*this, tmp); return *this; }
		GLWindow(const GLWindow&) = delete; 
		GLWindow& operator=(const GLWindow&) = delete;
		GLWindow& steal(GLWindow& tmp) { move(*this, tmp); return *this; }
		GLWindow& steal(GLWindow&& tmp) { move(*this, tmp); return *this; }
		friend void move(GLWindow& dst, GLWindow&& tmp) { move(dst, (GLWindow&)tmp); }
#else
		private: GLWindow(const GLWindow&); GLWindow& operator=(const GLWindow&); public:
		GLWindow& steal(const GLWindow& victim) { move(*this, (GLWindow&)victim); return *this; }
		friend void move(GLWindow& dst, const GLWindow& victim) { move(dst, (GLWindow&)victim); }
#endif
		friend void move(GLWindow& dst, GLWindow& tmp){ if(&dst == &tmp) return; dst.destroyGLContext(); dst.destroy();
			dst.app = tmp.app;
			dst.wndID = tmp.wndID; tmp.wndID = 0;
			dst.surface = tmp.surface; tmp.surface = NULL;
			dst.swapChain = tmp.swapChain; tmp.swapChain = NULL;
			dst.bmode = tmp.bmode;
		}
	};
} }