#include "my_simple_render_system.h"

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

// std
#include <array>
#include <cassert>
#include <stdexcept>
#include <iostream>
#include <iomanip>

struct MySimplePushConstantData
{
    glm::mat4 modelMatrix{ 1.0f };
    glm::mat4 normalMatrix{ 1.0f };
};

MySimpleRenderSystem::MySimpleRenderSystem(MyDevice& device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout)
    : m_myDevice{ device } 
{
    _createPipelineLayout(globalSetLayout);
    _createPipeline(renderPass);
}

MySimpleRenderSystem::~MySimpleRenderSystem()
{
    vkDestroyPipelineLayout(m_myDevice.device(), m_vkPipelineLayout, nullptr);
}

void MySimpleRenderSystem::_createPipelineLayout(VkDescriptorSetLayout globalSetLayout) 
{
    VkPushConstantRange pushConstantRange{};
    pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
    pushConstantRange.offset = 0;
    pushConstantRange.size = sizeof(MySimplePushConstantData);

    std::vector<VkDescriptorSetLayout> descriptorSetLayouts{ globalSetLayout };

    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(descriptorSetLayouts.size());
    pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();
    pipelineLayoutInfo.pushConstantRangeCount = 1;
    pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
    if (vkCreatePipelineLayout(m_myDevice.device(), &pipelineLayoutInfo, nullptr, &m_vkPipelineLayout) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create pipeline layout!");
    }
}

void MySimpleRenderSystem::_createPipeline(VkRenderPass renderPass)
{
    assert(m_vkPipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");

    PipelineConfigInfo pipelineConfig{};
    MyPipeline::defaultPipelineConfigInfo(pipelineConfig);
    pipelineConfig.renderPass = renderPass;
    pipelineConfig.pipelineLayout = m_vkPipelineLayout;

    m_pMyPipeline = std::make_unique<MyPipeline>(
        m_myDevice,
        "shaders/simple_shader.vert.spv",
        "shaders/phong_shader.frag.spv",
        pipelineConfig);
}

void MySimpleRenderSystem::renderGameObjects(std::string name, MyFrameInfo& frameInfo, std::vector<MyGameObject>& gameObjects)
{
    // Bind the descriptor set to the render pipeline
    m_pMyPipeline->bind(frameInfo.commandBuffer);

    vkCmdBindDescriptorSets(
        frameInfo.commandBuffer,
        VK_PIPELINE_BIND_POINT_GRAPHICS,
        m_vkPipelineLayout,
        0, // only bind set 0 for now
        1, // count of 1
        &frameInfo.globalDescriptorSet,
        0,
        nullptr);

    for (auto& obj : gameObjects)
    {
        if (obj.name() == name && obj.model != nullptr)
        {
            MySimplePushConstantData push{};

            auto modelMatrix = obj.transform.mat4();

            // test
            //modelMatrix = glm::mat4(1.0f);

            // Move the color setting as part of attributeDescription in MyPipeline::createGraphicsPipeline
            // push.color = obj.color;

            // Note: do this for now to perform on CPU
            // We will do it later to perform it on GPU
            push.modelMatrix = obj.transform.mat4();
            push.normalMatrix = obj.transform.normalMatrix();

            vkCmdPushConstants(
                frameInfo.commandBuffer,
                m_vkPipelineLayout,
                VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
                0,
                sizeof(MySimplePushConstantData),
                &push);

            obj.model->bind(frameInfo.commandBuffer);
            obj.model->draw(frameInfo.commandBuffer);
        }
    }
}

