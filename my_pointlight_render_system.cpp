#include "my_pointlight_render_system.h"

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

// std
#include <array>
#include <cassert>
#include <stdexcept>

struct PointLightPushConstants 
{
    glm::vec4 position{};
    float radius;
};

MyPointLightRenderSystem::MyPointLightRenderSystem(
    MyDevice& device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout)
    : m_myDevice{ device }
{
    _createPipelineLayout(globalSetLayout);
    _createPipeline(renderPass);
}

MyPointLightRenderSystem::~MyPointLightRenderSystem()
{
    vkDestroyPipelineLayout(m_myDevice.device(), m_vkPipelineLayout, nullptr);
}

void MyPointLightRenderSystem::_createPipelineLayout(VkDescriptorSetLayout globalSetLayout)
{
    VkPushConstantRange pushConstantRange{};
    pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
    pushConstantRange.offset = 0;
    pushConstantRange.size = sizeof(PointLightPushConstants);

    std::vector<VkDescriptorSetLayout> descriptorSetLayouts{ globalSetLayout };

    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(descriptorSetLayouts.size());
    pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();
    pipelineLayoutInfo.pushConstantRangeCount = 1;
    pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
    if (vkCreatePipelineLayout(m_myDevice.device(), &pipelineLayoutInfo, nullptr, &m_vkPipelineLayout) !=
        VK_SUCCESS) {
        throw std::runtime_error("failed to create pipeline layout!");
    }
}

void MyPointLightRenderSystem::_createPipeline(VkRenderPass renderPass) 
{
    assert(m_vkPipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");

    PipelineConfigInfo pipelineConfig{};
    MyPipeline::defaultPipelineConfigInfo(pipelineConfig);
    pipelineConfig.pointLightRendering = true;

    // We only need for simple render system because
    // we create the vertex in the vertex shader for point light
    // pipelineConfig.attributeDescriptions.clear(); 
    // pipelineConfig.bindingDescriptions.clear();

    pipelineConfig.renderPass = renderPass;
    pipelineConfig.pipelineLayout = m_vkPipelineLayout;

    m_pMyPipeline = std::make_unique<MyPipeline>(
        m_myDevice,
        "shaders/point_light.vert.spv",
        "shaders/point_light.frag.spv",
        pipelineConfig);
}

void MyPointLightRenderSystem::update(MyGlobalUBO& ubo, float angle, std::vector<MyGameObject>& gameObjects) 
{
    auto rotateLight = glm::rotate(glm::mat4(1.f), angle, { -1.f, 0.f, 0.f });
    for (auto& obj : gameObjects) 
    {
        if (obj.name() != std::string("light_source")) continue;

        // update light position
        obj.transform.translation = glm::vec3(rotateLight * glm::vec4(0.0f, 1.2f, 0.0f, 1.f));

        // copy light to ubo
        ubo.pointLight.position = glm::vec4(obj.transform.translation, 1.f);
    }
}

void MyPointLightRenderSystem::render(MyFrameInfo& frameInfo, std::vector<MyGameObject>& gameObjects) 
{
    m_pMyPipeline->bind(frameInfo.commandBuffer);

    vkCmdBindDescriptorSets(
        frameInfo.commandBuffer,
        VK_PIPELINE_BIND_POINT_GRAPHICS,
        m_vkPipelineLayout,
        0,
        1,
        &frameInfo.globalDescriptorSet,
        0,
        nullptr);


    for (auto& obj : gameObjects) 
    {
        if (obj.name() != std::string("light_source")) continue;

        PointLightPushConstants push{};
        push.position = glm::vec4(obj.transform.translation, 1.f);
        push.radius = obj.transform.scale.x;

        vkCmdPushConstants(
            frameInfo.commandBuffer,
            m_vkPipelineLayout,
            VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
            0,
            sizeof(PointLightPushConstants),
            &push);
        vkCmdDraw(frameInfo.commandBuffer, 6, 1, 0, 0);
    }
}

