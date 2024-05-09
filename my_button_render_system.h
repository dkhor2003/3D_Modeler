#ifndef __MY_BUTTON_RENDER_SYSTEM_H__
#define __MY_BUTTON_RENDER_SYSTEM_H__

#include "my_device.h"
#include "my_game_object.h"
#include "my_pipeline.h"
#include "my_camera.h"
#include "my_frame_info.h"

// std
#include <memory>
#include <vector>


class MyButtonRenderSystem
{
public:
	MyButtonRenderSystem(MyDevice& device, VkRenderPass renderPass); 
	~MyButtonRenderSystem();

	MyButtonRenderSystem(const MyButtonRenderSystem&) = delete;
	MyButtonRenderSystem& operator=(const MyButtonRenderSystem&) = delete;

	void renderGameObjects(std::string name, MyFrameInfo& frameInfo, std::vector<MyGameObject>& gameObjects);

private:
	void _createPipelineLayout();
	void _createPipeline(VkRenderPass renderPass);

	MyDevice&                   m_myDevice;

	std::unique_ptr<MyPipeline> m_pMyPipeline;
	VkPipelineLayout            m_vkPipelineLayout;
};

#endif

