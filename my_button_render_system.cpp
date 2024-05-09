#include "my_button_render_system.h"

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

struct MyButtonPushConstantData
{
    glm::mat4 transform{ 1.0f };
    glm::mat4 modelMatrix{ 1.0f };
};

MyButtonRenderSystem::MyButtonRenderSystem(MyDevice& device, VkRenderPass renderPass)
    : m_myDevice{ device } 
{
    _createPipelineLayout();
    _createPipeline(renderPass);
}

MyButtonRenderSystem::~MyButtonRenderSystem()
{
    vkDestroyPipelineLayout(m_myDevice.device(), m_vkPipelineLayout, nullptr);
}

void MyButtonRenderSystem::_createPipelineLayout()
{
    VkPushConstantRange pushConstantRange{};
    pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
    pushConstantRange.offset = 0;
    pushConstantRange.size = sizeof(MyButtonPushConstantData);

    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 0;
    pipelineLayoutInfo.pSetLayouts = nullptr;
    pipelineLayoutInfo.pushConstantRangeCount = 1;
    pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
    if (vkCreatePipelineLayout(m_myDevice.device(), &pipelineLayoutInfo, nullptr, &m_vkPipelineLayout) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create pipeline layout!");
    }
}

void MyButtonRenderSystem::_createPipeline(VkRenderPass renderPass)
{
    assert(m_vkPipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");

    PipelineConfigInfo pipelineConfig{};
    MyPipeline::defaultPipelineConfigInfo(pipelineConfig);
    pipelineConfig.renderPass = renderPass;
    pipelineConfig.pipelineLayout = m_vkPipelineLayout;

    m_pMyPipeline = std::make_unique<MyPipeline>(
        m_myDevice,
        "shaders/button_shader.vert.spv",
        "shaders/simple_shader.frag.spv",
        pipelineConfig);
}

void MyButtonRenderSystem::renderGameObjects(std::string name, MyFrameInfo& frameInfo, std::vector<MyGameObject>& gameObjects)
{
    m_pMyPipeline->bind(frameInfo.commandBuffer);
    auto projectionView = frameInfo.camera.projectionMatrix() * frameInfo.camera.viewMatrix();

    for (auto& obj : gameObjects)
    {
        if (obj.name() == name && obj.model != nullptr)
        {
            MyButtonPushConstantData push{};

            auto modelMatrix = obj.transform.mat4();

            // test
            //modelMatrix = glm::mat4(1.0f);

            // Move the color setting as part of attributeDescription in MyPipeline::createGraphicsPipeline
            // push.color = obj.color;

            // Note: do this for now to perform on CPU
            // We will do it later to perform it on GPU
            push.transform = projectionView * modelMatrix;
            push.modelMatrix = modelMatrix;

            vkCmdPushConstants(
                frameInfo.commandBuffer,
                m_vkPipelineLayout,
                VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
                0,
                sizeof(MyButtonPushConstantData),
                &push);

            obj.model->bind(frameInfo.commandBuffer);
            obj.model->draw(frameInfo.commandBuffer);
        }
    }
}


