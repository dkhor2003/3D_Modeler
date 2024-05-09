#ifndef __MY_UTILITIES_H__
#define __MY_UTILITIES_H__

#include <vector>
#include <glm/glm.hpp>
#include "my_model.h"
#include "my_device.h"
#include "my_game_object.h"

class MyUtilities
{
public:
    // Initialize the part of the window that the arrow buttons will appear
    static constexpr float A_RIGHT = 0.9f;
	static constexpr float A_LEFT = 0.7f;
    static constexpr float A_TOP = -0.5f;
	static constexpr float A_BOTTOM = -0.95f;
    static constexpr float HEIGHT = 0.2f;
    static constexpr float ARROW_GAP = 0.025f; 
    static constexpr float ARROW_WIDTH = 0.08f; 

    // Initialize the part of the window that the range buttons will appear
    static constexpr float R_RIGHT = 0.0f;
	static constexpr float R_LEFT = -0.95f;
    static constexpr float R_TOP = -0.35f;
	static constexpr float R_BOTTOM = -0.7f;
    static constexpr float GAP = 0.02f; 
    static constexpr float ALPHA_HEIGHT = 0.08f; 
    static constexpr float ALPHA_BOX_WIDTH = 0.1f; 
    static constexpr float STROKE_WIDTH = 0.02f; 
    static constexpr float BUTTON_START = -0.8f;
    static constexpr float BUTTON_BOX_WIDTH = 0.14f;
    static constexpr float RANGE_GAP = 0.01f; 
    static constexpr int   MAX_DEGREE = 10; 

    // Initialize some variables to determine the position of texture buttons
    static constexpr float T_TOP = -0.3f; 
    static constexpr float BOX_WIDTH = 0.2f; 

    // Methods for initializing the buttons position and color
    void      initializeArrows(); 
    void      initializeRange();  
    void      initializeTexture(); 

    // Methods for simulating the effect of pressing the buttons
    void      pressArrowButton(int index, float intensity, std::vector<MyGameObject>& gameObjects, MyDevice &device);
    void      pressRangeButton(int index, float intensity, std::vector<MyGameObject>& gameObjects, MyDevice &device, bool pressed); 
    void      pressTextureButton(int index, float intensity, std::vector<MyGameObject>& gameObjects, MyDevice &device);

    // Methods for getting the index of button being pressed
    int       queryArrowButton(float x, float y); 
    int       queryRangeButton(float x, float y); 
    int       queryTextureButton(float x, float y); 

    // Methods for getting information about the user's selected u- and v-degree for bezier surface rendering
    int       u_Deg() { return u_deg; };
    int       v_Deg() { return v_deg; }; 

    // Initialize all color options here
    const glm::vec3 WHITE = glm::vec3(1.0f, 1.0f, 1.0f);
    const glm::vec3 GRAY  = glm::vec3(0.3f, 0.3f, 0.3f);
    const glm::vec3 BLACK = glm::vec3(0.0f, 0.0f, 0.0f);

	std::vector<MyModel::Vertex>    m_vButtons;
	std::vector<uint32_t>           m_vIndices1;

    std::vector<MyModel::Vertex>    m_vRange;
	std::vector<uint32_t>           m_vIndices2;

    std::vector<MyModel::Vertex>    m_vTexture;
	std::vector<uint32_t>           m_vIndices3;

private:
    // Initialize u- and v-degree for range buttons
    int u_deg = 2; 
    int v_deg = 2; 
};

#endif