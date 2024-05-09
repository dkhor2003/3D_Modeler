#ifndef __MY_CONTROL_POINT_RENDER_SYSTEM_H__
#define __MY_CONTROL_POINT_RENDER_SYSTEM_H__

#include "my_device.h"
#include "my_game_object.h"
#include "my_pipeline.h"
#include "my_camera.h"
#include "my_frame_info.h"

// std
#include <memory>
#include <vector>


class MyControlPointRenderSystem
{
public:
	MyControlPointRenderSystem(MyDevice& device, VkRenderPass renderPass, VkPrimitiveTopology topology);
	~MyControlPointRenderSystem();

	MyControlPointRenderSystem(const MyControlPointRenderSystem&) = delete;
	MyControlPointRenderSystem& operator=(const MyControlPointRenderSystem&) = delete;

	void renderControlPoints(MyFrameInfo& frameInfo, std::vector<MyGameObject>& gameObjects);

private:
	void _createPipelineLayout();
	void _createPipeline(VkRenderPass renderPass, VkPrimitiveTopology topology);
	void _renderPointsLines(std::string name, MyFrameInfo& frameInfo, std::vector<MyGameObject>& gameObjects);

	MyDevice&                   m_myDevice;

	std::unique_ptr<MyPipeline> m_pMyPipeline;
	VkPipelineLayout            m_vkPipelineLayout;
};

#endif

