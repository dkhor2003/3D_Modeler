#ifndef __MY_COLORS_H__
#define __MY_COLORS_H__

#include <vector>
#include <glm/glm.hpp>
#include "my_model.h"
#include "my_device.h"
#include "my_game_object.h"

// Class responsible for the color palette buttons for the
class MyColors
{
public:
    // Initialize the part of the window that the buttons will appear
    static constexpr float RIGHT = 0.6f;
	static constexpr float LEFT = -0.95f;
    static constexpr float TOP = -0.75f;
	static constexpr float BOTTOM = -0.95f;

    void      initializePalette(); 
    glm::vec4 queryColorButton(float x, float y); 
    void      pressButton(int index, float intensity, std::vector<MyGameObject>& gameObjects, MyDevice &device); 

    // Initialize all color options here
    std::vector<glm::vec3>  m_vColors = { 
        {1.0f, 0.0f, 0.0f},
        {1.0f, 0.65f, 0.0f},
        {1.0f, 1.0f, 0.0f},
        {0.0f, 1.0f, 0.0f},
        {0.0f, 1.0f, 1.0f},
        {0.0f, 0.0f, 1.0f},
        {1.0f, 0.0f, 1.0f},
        {1.0f, 1.0f, 1.0f}
    }; 

	std::vector<MyModel::Vertex>    m_vButtons;
	std::vector<uint32_t>           m_vIndices;

private:

};

#endif