#ifndef __MY_POINTLIGHT_RENDER_SYSTEM_H__
#define __MY_POINTLIGHT_RENDER_SYSTEM_H__

#include "my_camera.h"
#include "my_device.h"
#include "my_frame_info.h"
#include "my_game_object.h"
#include "my_pipeline.h"

// std
#include <memory>
#include <vector>

class MyPointLightRenderSystem 
{
public:
    MyPointLightRenderSystem(
        MyDevice& device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);
    ~MyPointLightRenderSystem();

    MyPointLightRenderSystem(const MyPointLightRenderSystem&) = delete;
    MyPointLightRenderSystem& operator=(const MyPointLightRenderSystem&) = delete;

    void update(MyGlobalUBO& ubo, float angle, std::vector<MyGameObject>& gameObjects);
    void render(MyFrameInfo& frameInfo, std::vector<MyGameObject>& gameObjects);

private:
    void _createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
    void _createPipeline(VkRenderPass renderPass);

    MyDevice&                   m_myDevice;

    std::unique_ptr<MyPipeline> m_pMyPipeline;
    VkPipelineLayout            m_vkPipelineLayout;
};

#endif

