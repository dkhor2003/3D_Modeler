#include "my_colors.h"
#include <glm/glm.hpp>

// Initialize each buttons position based on the number of colors needed
void MyColors::initializePalette()
{
    int numButtons = m_vColors.size();
    int numGaps = numButtons - 1; 
    float gapSize = 0.025f; 
    float buttonSize = ((RIGHT - LEFT) - (gapSize * numGaps)) / numButtons; 
    float start = LEFT; 

    for (int i = 0; i < numButtons; i++)
    {
        MyModel::Vertex topLeft;
        topLeft.position = glm::vec3(start, TOP, 10.0f);
        topLeft.color = m_vColors[i];  
        m_vButtons.push_back(topLeft);

        MyModel::Vertex topRight;
        topRight.position = glm::vec3(start + buttonSize, TOP, 10.0f);
        topRight.color = m_vColors[i];  
        m_vButtons.push_back(topRight);

        MyModel::Vertex bottomLeft;
        bottomLeft.position = glm::vec3(start, BOTTOM, 10.0f);
        bottomLeft.color = m_vColors[i];  
        m_vButtons.push_back(bottomLeft);

        MyModel::Vertex bottomRight;
        bottomRight.position = glm::vec3(start + buttonSize, BOTTOM, 10.0f);
        bottomRight.color = m_vColors[i];  
        m_vButtons.push_back(bottomRight);

        m_vIndices.push_back(i * 4 + 0); 
        m_vIndices.push_back(i * 4 + 2); 
        m_vIndices.push_back(i * 4 + 3); 

        m_vIndices.push_back(i * 4 + 0); 
        m_vIndices.push_back(i * 4 + 1); 
        m_vIndices.push_back(i * 4 + 3); 

        start = start + (buttonSize + (gapSize));
    }
};

// Method to return the color vector and index of chosen color based on the clicked position on window
glm::vec4 MyColors::queryColorButton(float x, float y)
{
    if (y <= TOP && y >= BOTTOM)
    {
        for (int i = 0; i < m_vColors.size(); i++)
        {
            float xMin = m_vButtons[i * 4 + 0].position.x;
            float xMax = m_vButtons[i * 4 + 1].position.x;

            if (x <= xMax && x >= xMin)
            {
                return glm::vec4(m_vColors[i], float(i));
            }
        }
    }

    // First three are the RGB colors and the last one is the index of color in m_vColors
    return glm::vec4(-1.0f, -1.0f, -1.0f, -1.0f); 
};

// Method to simulate the effect of button being pressed by changing the color intensity
void MyColors::pressButton(int index, float intensity, std::vector<MyGameObject>& gameObjects, MyDevice &device)
{
    if (index < 0 || index >= m_vColors.size()) return; 
    m_vButtons[index * 4 + 0].color *= intensity; 
    m_vButtons[index * 4 + 1].color *= intensity; 
    m_vButtons[index * 4 + 2].color *= intensity; 
    m_vButtons[index * 4 + 3].color *= intensity; 

    for (auto& obj : gameObjects)
    {
        if (obj.name() == std::string("color_button"))
        {
            MyModel::Builder builder;
            builder.vertices = m_vButtons;
            builder.indices = m_vIndices;
            std::shared_ptr<MyModel> mybutton = std::make_shared<MyModel>(device, builder);
            obj.model = mybutton; 
        }
    }
};