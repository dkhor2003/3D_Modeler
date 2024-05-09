#ifndef __MY_TEXTURE_RENDER_SYSTEM_H__
#define __MY_TEXTURE_RENDER_SYSTEM_H__

#include "my_camera.h"
#include "my_device.h"
#include "my_frame_info.h"
#include "my_game_object.h"
#include "my_pipeline.h"

// std
#include <memory>
#include <vector>

class MyTextureRenderSystem
{
public:
	MyTextureRenderSystem(MyDevice& device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);
	~MyTextureRenderSystem();

	MyTextureRenderSystem(const MyTextureRenderSystem&) = delete;
	MyTextureRenderSystem& operator=(const MyTextureRenderSystem&) = delete;

	void render(std::string name, MyFrameInfo& frameInfo, std::vector<MyGameObject>& gameObjects, bool isButton);

private:
	void _createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
	void _createPipeline(VkRenderPass renderPass);

	MyDevice& m_myDevice;

	std::unique_ptr<MyPipeline> m_pMyPipeline1;
	std::unique_ptr<MyPipeline> m_pMyPipeline2;
	VkPipelineLayout            m_vkPipelineLayout;
};

#endif

