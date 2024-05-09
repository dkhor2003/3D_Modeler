#include "my_texture_render_system.h"

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

// std
#include <array>
#include <cassert>
#include <stdexcept>

struct MyTexturePushConstantData
{
    glm::mat4 modelMatrix{ 1.f };
    glm::mat4 normalMatrix{ 1.f };
};

MyTextureRenderSystem::MyTextureRenderSystem(MyDevice& device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout)
    : m_myDevice{ device }
{
    _createPipelineLayout(globalSetLayout);
    _createPipeline(renderPass);
}

MyTextureRenderSystem::~MyTextureRenderSystem()
{
    vkDestroyPipelineLayout(m_myDevice.device(), m_vkPipelineLayout, nullptr);
}

void MyTextureRenderSystem::_createPipelineLayout(VkDescriptorSetLayout globalSetLayout)
{
    VkPushConstantRange pushConstantRange{};
    pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
    pushConstantRange.offset = 0;
    pushConstantRange.size = sizeof(MyTexturePushConstantData);

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

void MyTextureRenderSystem::_createPipeline(VkRenderPass renderPass)
{
    assert(m_vkPipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");

    PipelineConfigInfo pipelineConfig{};
    MyPipeline::defaultPipelineConfigInfo(pipelineConfig, m_myDevice.msaaSamples());
    pipelineConfig.renderPass = renderPass;
    pipelineConfig.pipelineLayout = m_vkPipelineLayout;

    m_pMyPipeline1 = std::make_unique<MyPipeline>(
        m_myDevice,
        "shaders/texture_shader.vert.spv",
        "shaders/texture_shader.frag.spv",
        pipelineConfig);

    m_pMyPipeline2 = std::make_unique<MyPipeline>(
        m_myDevice,
        "shaders/texture_shader.vert.spv",
        "shaders/texture_button_shader.frag.spv",
        pipelineConfig);
}

void MyTextureRenderSystem::render(std::string name, MyFrameInfo& frameInfo, std::vector<MyGameObject>& gameObjects, bool isButton)
{
    // Bind the descriptor set to the render pipeline

    vkCmdBindDescriptorSets(
        frameInfo.commandBuffer,
        VK_PIPELINE_BIND_POINT_GRAPHICS,
        m_vkPipelineLayout,
        0, // only bind set 0 for now
        1, // count of 1
        &frameInfo.globalDescriptorSet,
        0,
        nullptr);

    if (isButton)
    {
        m_pMyPipeline2->bind(frameInfo.commandBuffer);
    }
    else
    {
        m_pMyPipeline1->bind(frameInfo.commandBuffer);
    }

    // Now loop through all key-value pair of the game objects
    for (auto& obj : gameObjects)
    {
        if (obj.name() == name && obj.textureModel != nullptr)
        {
            MyTexturePushConstantData push{};

            // Note: do this for now to perform on CPU
            // We will do it later to perform it on GPU
            push.modelMatrix = obj.transform.mat4();
            push.normalMatrix = obj.transform.normalMatrix();

            vkCmdPushConstants(
                frameInfo.commandBuffer,
                m_vkPipelineLayout,
                VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
                0,
                sizeof(MyTexturePushConstantData),
                &push);

            obj.textureModel->bind(frameInfo.commandBuffer);
            obj.textureModel->draw(frameInfo.commandBuffer);
        }
    }
}

