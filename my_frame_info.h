#ifndef __MY_FRAMEINFO_H__
#define __MY_FRAMEINFO_H__

#include "my_camera.h"

// lib
#include <vulkan/vulkan.h>

struct MyPointLight 
{
	glm::vec4 position{ 0.0f, 1.3f, 0.0f, 1.0f};         // ignore w
	glm::vec4 color{1.0f, 1.0f, 1.0f, .7f};              // w is intensity
};

struct MyGlobalUBO 
{
	glm::mat4    projection{ 1.f };
	glm::mat4    view{ 1.f };
	glm::mat4    inverseView{ 1.f };
	glm::vec4    ambientLightColor{ 1.f, 1.f, 1.f, .07f };  // w is intensity
	MyPointLight pointLight;
};

struct MyFrameInfo
{
	int             frameIndex;
	float           frameTime;
	VkCommandBuffer commandBuffer;
	MyCamera&       camera;
	glm::vec3       color;
	VkDescriptorSet globalDescriptorSet;
};

#endif

