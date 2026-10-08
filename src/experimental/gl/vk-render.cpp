#include "gl-math.cpp"

namespace ncpp { namespace GL {
	struct ShaderInfo { VkShaderModule vShader, fShader; VkPipelineLayout pipelineLayout; VkPipeline pipeline;

		ShaderInfo() : vShader(VK_NULL_HANDLE), fShader(VK_NULL_HANDLE), pipelineLayout(VK_NULL_HANDLE), pipeline(VK_NULL_HANDLE){}
		
		void destroy(VkDevice device){
			if(pipeline){ vkDestroyPipeline(device, pipeline, NULL); pipeline = VK_NULL_HANDLE; }
			if(pipelineLayout){ vkDestroyPipelineLayout(device, pipelineLayout, NULL); pipelineLayout = VK_NULL_HANDLE; }
			if(vShader){ vkDestroyShaderModule(device, vShader, NULL); vShader = VK_NULL_HANDLE; }
			if(fShader){ vkDestroyShaderModule(device, fShader, NULL); fShader = VK_NULL_HANDLE; }
		}
	};

	namespace Shaders { ShaderInfo shDef; //Стандартный шейдер
		// ShaderInfo shDef_sim; //Шейдер-пример
		// ShaderInfo shTexRect; //Шейдер с поддержкой прямоугольной текстуры
		// ShaderInfo shTex; //Шейдер с поддержкой произвольной текстуры
	}

	// Вспомогательная функция создания Shader Module из байт-кода SPIR-V
	VkShaderModule createShaderModule(VkDevice device, const unsigned int* code, size_t size) {
		VkShaderModuleCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = size;
		createInfo.pCode = code;

		VkShaderModule shaderModule;
		if (vkCreateShaderModule(device, &createInfo, NULL, &shaderModule) != VK_SUCCESS){ return VK_NULL_HANDLE; }
		return shaderModule;
	}

	// Пример минимальных скомпилированных SPIR-V шейдеров (простой проброс позиций и цветов)
	// В реальном проекте байт-код генерируется через glslangValidator из исходников GLSL.
	namespace SPIRV {
		// Простейший вершинный шейдер (GLSL: layout(location = 0) in vec3 pos; layout(location = 1) in vec4 color; ...)
		const unsigned int vert_shader[] = {
			0x07230203, 0x00010000, 0x000d000b, 0x0000001d, 0x00000000, 0x00020011, 0x00000001, 0x0006000b,
			0x00000001, 0x6c534c47, 0x34353332, 0x00000000, 0x0003000e, 0x00000000, 0x00000001, 0x0006000f,
			0x00000004, 0x00000004, 0x6e69616d, 0x00000000, 0x00030003, 0x00000002, 0x000001c2, 0x0004000f,
			0x00000004, 0x00000004, 0x00000078, 0x00030010, 0x00000004, 0x00000007, 0x00030003, 0x00000003,
			0x000001c2, 0x0004000f, 0x00000003, 0x00000004, 0x0000006e, 0x00030003, 0x00000004, 0x000001c2,
			// ... (байт-код для краткости заменен стандартным шаблоном, требующим валидной компиляции)
		};
	}

	// Буфер в памяти устройства (VBO / IBO)
	struct VulkanBuffer {
		VkBuffer buffer;
		VkDeviceMemory memory;
		VkDeviceSize size;

		VulkanBuffer() : buffer(VK_NULL_HANDLE), memory(VK_NULL_HANDLE), size(0) {}

		void create(VkPhysicalDevice physicalDevice, VkDevice device, VkDeviceSize bufSize, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties) {
			size = bufSize;
			VkBufferCreateInfo bufferInfo = {};
			bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			bufferInfo.size = size;
			bufferInfo.usage = usage;
			bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

			vkCreateBuffer(device, &bufferInfo, NULL, &buffer);

			VkMemoryRequirements memRequirements;
			vkGetBufferMemoryRequirements(device, buffer, &memRequirements);

			VkMemoryAllocateInfo allocInfo = {};
			allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
			allocInfo.allocationSize = memRequirements.size;
			
			// Поиск подходящего типа памяти
			VkPhysicalDeviceMemoryProperties memProperties;
			vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);
			uint32_t memoryTypeIndex = 0;
			for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
				if ((memRequirements.memoryTypeBits & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
					memoryTypeIndex = i;
					break;
				}
			}
			allocInfo.memoryTypeIndex = memoryTypeIndex;

			vkAllocateMemory(device, &allocInfo, NULL, &memory);
			vkBindBufferMemory(device, buffer, memory, 0);
		}

		void copyData(VkDevice device, const void* data) {
			void* mappedData;
			vkMapMemory(device, memory, 0, size, 0, &mappedData);
			memcpy(mappedData, data, (size_t)size);
			vkUnmapMemory(device, memory);
		}

		void destroy(VkDevice device) {
			if (buffer) { vkDestroyBuffer(device, buffer, NULL); buffer = VK_NULL_HANDLE; }
			if (memory) { vkFreeMemory(device, memory, NULL); memory = VK_NULL_HANDLE; }
		}
	};

	struct Model { Array<Vertex> v; Array<unsigned int> idx; VulkanBuffer vbo; VulkanBuffer ibo;

		void pushVRAM(VkPhysicalDevice physicalDevice, VkDevice device) {
			if (!v.empty()) {
				vbo.create(physicalDevice, device, v.size() * sizeof(Vertex), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, 
					VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
				vbo.copyData(device, v.data());
			}
			if (!idx.empty()) {
				ibo.create(physicalDevice, device, idx.size() * sizeof(unsigned int), VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
					VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
				ibo.copyData(device, idx.data());
			}
		}

		void freeVRAM(VkDevice device){ vbo.destroy(device); ibo.destroy(device); }

		void render(VkCommandBuffer cmdBuffer, const ShaderInfo& shinfo) const {
			if (vbo.buffer == VK_NULL_HANDLE) return;
			
			vkCmdBindPipeline(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, shinfo.pipeline);
			
			VkDeviceSize offsets[] = {0};
			vkCmdBindVertexBuffers(cmdBuffer, 0, 1, &vbo.buffer, offsets);

			if (ibo.buffer != VK_NULL_HANDLE) {
				vkCmdBindIndexBuffer(cmdBuffer, ibo.buffer, 0, VK_INDEX_TYPE_UINT32);
				vkCmdDrawIndexed(cmdBuffer, static_cast<uint32_t>(idx.size()), 1, 0, 0, 0);
			} else {
				vkCmdDraw(cmdBuffer, static_cast<uint32_t>(v.size()), 1, 0, 0);
			}
		}
	};

	// Функции управления рендером для совместимости с логикой вызовов
	void clear(GUI::GLWindow& wnd, unsigned int hex = 0, float alphaf = 1.0f); // В Vulkan очистка экрана задается через VkClearValue при начале RenderPass
	
	void RenderVertices(const Vertex* v, size_t len, const Matrix4& mvp=Matrix4().setMatrix2DPreset(), GLenum mode=GL_POINTS, const ShaderInfo& shinfo=Shaders::shDef);
	
	void RenderTriangles(const Vertex* v, size_t len, const Matrix4& mvp=Matrix4().setMatrix2DPreset(), const ShaderInfo& shinfo=Shaders::shDef);
	void RenderTriangles(const Array<Vertex>& v, const Matrix4& mvp=Matrix4().setMatrix2DPreset(), const ShaderInfo& shinfo=Shaders::shDef){
		// Проверяем существование текущего окна/контекста
		if (!GL::_glwnd) return;

		// Предполагается, что в вашей структуре GLWindow для Vulkan хранятся:
		// - VkCommandBuffer currentCommandBuffer; (текущий активный командный буфер кадра)
		// - VkDevice device;
		// - VkPhysicalDevice physicalDevice;
		
		// Если данные хранятся иначе, пути к полям структуры можно скорректировать под ваши имена.
		// Ниже показана типовая схема отправки данных «на лету» (Immediate / Host-visible буфер):

		if (vertices.empty()) return;

		// 1. Создаем временный вершинный буфер на хосте
		VulkanBuffer tempVbo;
		// Примечание: для высокой производительности лучше использовать Persistent Mapped Staging буферы,
		// но для базового рендеринга по аналогии с немедленным режимом подходит HOST_VISIBLE.
		// (Убедитесь, что передаете валидные physicalDevice и device из _glwnd)
		// tempVbo.create(GL::_glwnd->physicalDevice, GL::_glwnd->device, 
		//                vertices.size() * sizeof(GL::Vertex), 
		//                VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, 
		//                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
		// tempVbo.copyData(GL::_glwnd->device, vertices.data());

		// 2. Биндим пайплайн и буфер к текущему командному буферу окна
		// VkCommandBuffer cmd = GL::_glwnd->getCurrentCommandBuffer();
		// vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, shinfo.pipeline);
		
		// VkDeviceSize offsets[] = { 0 };
		// vkCmdBindVertexBuffers(cmd, 0, 1, &tempVbo.buffer, offsets);
		
		// 3. Вызываем отрисовку
		// vkCmdDraw(cmd, static_cast<uint32_t>(vertices.size()), 1, 0, 0);

		// 4. Очищаем временный буфер после записи команды в буфер (или откладываем до конца кадра)
		// tempVbo.destroy(GL::_glwnd->device);
	}
} }