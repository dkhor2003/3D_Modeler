#include "my_application.h"

// Render system
#include "my_simple_render_system.h"
#include "my_point_line_render_system.h"
#include "my_button_render_system.h"
#include "my_control_point_render_system.h"
#include "my_pointlight_render_system.h"
#include "my_texture_render_system.h"
#include "my_texture.h"
#include "my_window.h"

// use radian rather degree for angle
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

// Std
#include <stdexcept>
#include <array>
#include <chrono>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <filesystem>
#include <cassert>
#include <limits>
#include <math.h>
#include <numeric>
#include <random>

const std::string TEXTURE_PATH = "./textures/3_textures.png"; // Initialize texture image path here

MyApplication::MyApplication() :
    m_bPerspectiveProjection(true)
{
    m_pMyGlobalPool =
        MyDescriptorPool::Builder(m_myDevice)
        .setMaxSets(MySwapChain::MAX_FRAMES_IN_FLIGHT)
        .addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, MySwapChain::MAX_FRAMES_IN_FLIGHT)
        .addPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, MySwapChain::MAX_FRAMES_IN_FLIGHT)
        .build();

    _loadGameObjects();
}

MyApplication::~MyApplication()
{

}

void MyApplication::run() 
{
    // Since texture image don't change every frame, only create one
    MyTexture myTexture(m_myDevice, TEXTURE_PATH);

    // Create buffer for each frame to avoid memory accessing issue
    // Note: with the new map function, we don't need to worry about
    // offset alignment for UBO buffer
    std::vector<std::unique_ptr<MyBuffer>> uboBuffers(MySwapChain::MAX_FRAMES_IN_FLIGHT);
    for (int i = 0; i < uboBuffers.size(); i++) 
    {
        uboBuffers[i] = std::make_unique<MyBuffer>(
            m_myDevice,
            sizeof(MyGlobalUBO),
            1,
            VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, // no host coherience bit but we could do it here, so we don't need to use flush. We need to revisit here and see if we can remove the last argument
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);

        uboBuffers[i]->map();
    }

    // Create descriptor set layout object
    auto globalSetLayout =
        MyDescriptorSetLayout::Builder(m_myDevice)
        .addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS)  // can be accessed by all shader stages
        .addBinding(1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT) // Texture image can only be accessed by fragment shader stage
        .build();

    // Create one descriptor set per frame
    std::vector<VkDescriptorSet> globalDescriptorSets(MySwapChain::MAX_FRAMES_IN_FLIGHT);
    for (int i = 0; i < globalDescriptorSets.size(); i++)
	{
        auto bufferInfo = uboBuffers[i]->descriptorInfo();
        auto textureInfo = myTexture.descriptorInfo();
        MyDescriptorWriter(*globalSetLayout, *m_pMyGlobalPool)
            .writeBuffer(0, &bufferInfo)
            .writeImage(1, &textureInfo)
            .build(globalDescriptorSets[i]);
    }

    // Declare the render systems
    MySimpleRenderSystem       simpleRenderSystem{ m_myDevice, m_myRenderer.swapChainRenderPass(), globalSetLayout->descriptorSetLayout()};       // Draw surface
    MyButtonRenderSystem       buttonRenderSystem{ m_myDevice, m_myRenderer.swapChainRenderPass() };                                              // Draw Button (independent of light)
    MyControlPointRenderSystem pointRenderSystem{ m_myDevice, m_myRenderer.swapChainRenderPass(), VK_PRIMITIVE_TOPOLOGY_POINT_LIST };             // Draw points
    MyPointLineRenderSystem    lineRenderSystem{ m_myDevice, m_myRenderer.swapChainRenderPass(), VK_PRIMITIVE_TOPOLOGY_LINE_STRIP };              // Draw line strip
    MyPointLineRenderSystem    normalRenderSystem{ m_myDevice, m_myRenderer.swapChainRenderPass(), VK_PRIMITIVE_TOPOLOGY_LINE_LIST };             // Draw lines
    MyPointLightRenderSystem   pointLightSystem{ m_myDevice, m_myRenderer.swapChainRenderPass(), globalSetLayout->descriptorSetLayout() };        // Draw point light source
    MyTextureRenderSystem      textureRenderSystem{ m_myDevice, m_myRenderer.swapChainRenderPass(), globalSetLayout->descriptorSetLayout() };     // Draw Texture on Surface
    

    m_myWindow.bindMyApplication(this);

    // Switch to edit mode initially
    switchEditMode();

    auto currentTime = std::chrono::high_resolution_clock::now();

    while (!m_myWindow.shouldClose()) 
    {
        // Note: depending on the platforms (PC, Linux or Mac), this function
        // will cause the event proecssing to block during a Window move, resize or
        // menu operation. Users can use the "window refresh callback" to redraw the
        // contents of the window when necessary during such operation.
        m_myWindow.pollEvents();

        // Need to get the call after glfwPollEvants because the call above may take time
        auto newTime = std::chrono::high_resolution_clock::now();
        float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
        currentTime = newTime;

        float apsectRatio = m_myRenderer.aspectRatio();

        // Put it here because the viewport may change
		//std::cout << "Aspect ratio: " << apsectRatio << std::endl;

        if (m_bPerspectiveProjection)
            // near and far will automatically apply negative values
            m_myCamera.setPerspectiveProjection(glm::radians(50.f), apsectRatio, 0.1f, 100.f);
        else
            // because Y is down by default for Vulkan, when we set top to be minus value, we can flip the coordinate
            // such that Y is up. 
            m_myCamera.setOrthographicProjection(-apsectRatio, apsectRatio, -1.0f, 1.0f, -100.0f, 100.0f);
 
        // Please note that commandBuffer could be null pointer
        // if the swapChain needs to be recreated
        if (auto commandBuffer = m_myRenderer.beginFrame())
		{
            // In case we have multiple render passes for the current frame
            // begin offsreen shadow pass
            // render shadow casting objects
            // end offscreen shadow pass

            int frameIndex = m_myRenderer.frameIndex();
            MyFrameInfo frameInfo{ frameIndex, frameTime, commandBuffer, m_myCamera, glm::vec3(1.0f, 1.0f, 1.0f), globalDescriptorSets[frameIndex]};

            // update UBO to GPU
            MyGlobalUBO ubo{};
            ubo.projection = m_myCamera.projectionMatrix();
            ubo.view = m_myCamera.viewMatrix();
            ubo.inverseView = m_myCamera.inverseView(); 
            pointLightSystem.update(ubo, m_fAngle, m_vMyGameObjects); 
            uboBuffers[frameIndex]->writeToBuffer(&ubo);

            // becasue we don't use host coherence flag, we need to call flash
            uboBuffers[frameIndex]->flush();

            // render
            m_myRenderer.beginSwapChainRenderPass(commandBuffer);
            
            if (m_bCheckModel)
            {
                simpleRenderSystem.renderGameObjects("saved_model", frameInfo, m_vMyGameObjects);
            }
            else
            {
                // The order the render systems determines which one is rendered on top
                pointRenderSystem.renderControlPoints(frameInfo, m_vMyGameObjects); // render the control points

                if (!m_bCreateTerrain)
                {
                    frameInfo.color = glm::vec3(1.0f, 1.0f, 0.0f);
                    lineRenderSystem.renderControlPoints(frameInfo, m_vMyGameObjects); // don't render the lines between control points when modeling terrain
                }

                if (!m_bCreateTerrain)
                {
                    frameInfo.color = glm::vec3(0.0f, 0.0f, 1.0f);
                    lineRenderSystem.renderCenterLine(frameInfo, m_vMyGameObjects); // render center line only in 3D hollow object modeling mode
                }

                frameInfo.color = glm::vec3(1.0f, 1.0f, 1.0f);
                lineRenderSystem.renderBezierCurve(frameInfo, m_vMyGameObjects); // render the bezier curve

                if (m_bShowButtons) // render the buttons only if it is needed to be shown
                {
                    buttonRenderSystem.renderGameObjects("color_button", frameInfo, m_vMyGameObjects);
                    buttonRenderSystem.renderGameObjects("arrow_button", frameInfo, m_vMyGameObjects);
                    textureRenderSystem.render("texture_button", frameInfo, m_vMyGameObjects, true);
                    if (m_bCreateTerrain)
                    {
                        buttonRenderSystem.renderGameObjects("range_button", frameInfo, m_vMyGameObjects); 
                    } 
                }

                if (m_bShowNormals) // render normal vectors only if users decided to show the normals
                {
                    normalRenderSystem.renderNormals(frameInfo, m_vMyGameObjects);
                }

                if (m_bShowSurface) // render surface only if users decided to show the surface
                {
                    if (m_iCurrentTextureIndex == 3) // Render surface without texture
                    {
                        simpleRenderSystem.renderGameObjects("bezier_surface", frameInfo, m_vMyGameObjects);
                    }
                    else // Render surface with texture
                    {
                        textureRenderSystem.render("texture_surface", frameInfo, m_vMyGameObjects, false);
                    }
                }
            }

            pointLightSystem.render(frameInfo, m_vMyGameObjects);

            m_myRenderer.endSwapChainRenderPass(commandBuffer);

            m_myRenderer.endFrame();
        }
    }

    // GPU will block until all CPU is complete
    vkDeviceWaitIdle(m_myDevice.device());
}

void MyApplication::switchProjectionMatrix()
{
    // Switch between perspective and orthographic projection matrix
    m_bPerspectiveProjection = !m_bPerspectiveProjection;
}

void MyApplication::_loadGameObjects()
{
    // Initialize the memory of Game Objects
    m_pMyBezier = std::make_shared<MyBezier>();
    m_pMyColors = std::make_shared<MyColors>(); 
    m_pMyUtilities = std::make_shared<MyUtilities>(); 

    m_pMyColors->initializePalette(); 
    m_pMyUtilities->initializeArrows(); 
    m_pMyUtilities->initializeRange();
    m_pMyUtilities->initializeTexture(); 

    // Add dynamic control points (no more than 121 points)
    // Bezier surface can be created with maximum 10 u-degree and 10 v-degree, which means a maximum of 121 control points can be rendered
    std::shared_ptr<MyModel> mypointLine = std::make_shared<MyModel>(m_myDevice, 121);
    auto mygameobj1 = MyGameObject::createGameObject("control_points");
    mygameobj1.model = mypointLine;
    m_vMyGameObjects.push_back(std::move(mygameobj1));

    // Add center line (2 points)
    std::shared_ptr<MyModel> mycenterLine = std::make_shared<MyModel>(m_myDevice, 2);
    auto mygameobj2 = MyGameObject::createGameObject("centerLine");
    mygameobj2.model = mycenterLine;

    MyModel::PointLine vertex1, vertex2;
    vertex1.position = glm::vec3(-1.0f, 0.0f, 0.0f);
    vertex1.color = glm::vec3(0.0f, 0.0f, 1.0f);
    vertex2.position = glm::vec3(1.0f, 0.0f, 0.0f);
    vertex2.color = glm::vec3(0.0f, 0.0f, 1.0f);

    std::vector<MyModel::PointLine> centerLine;
    centerLine.push_back(vertex1);
    centerLine.push_back(vertex2);
    mycenterLine->updatePointLines(centerLine);
    m_vMyGameObjects.push_back(std::move(mygameobj2));

	// Note: if you increase the resolutions of the curve, you need to increase the value 2000 here
    std::shared_ptr<MyModel> mybezierCurve = std::make_shared<MyModel>(m_myDevice, 2000);
    auto mygameobj3 = MyGameObject::createGameObject("bezier_curve");
    mygameobj3.model = mybezierCurve;
    m_vMyGameObjects.push_back(std::move(mygameobj3));

    // Add revolution surface game object, but no model yet
    auto mygameobj4 = MyGameObject::createGameObject("bezier_surface"); // it will crash if don't check nullptr on model in the render system
    m_vMyGameObjects.push_back(std::move(mygameobj4));

    // Add normal vectors game object, but no model yet
    auto mygameobj5 = MyGameObject::createGameObject("surface_normals");
    m_vMyGameObjects.push_back(std::move(mygameobj5));

    // Create the color button model
    auto mygameobj6 = MyGameObject::createGameObject("color_button");
    MyModel::Builder color_buttons;
    color_buttons.vertices = m_pMyColors->m_vButtons;
    color_buttons.indices = m_pMyColors->m_vIndices;
    std::shared_ptr<MyModel> myColorButtons = std::make_shared<MyModel>(m_myDevice, color_buttons);
    mygameobj6.model = myColorButtons; 
    m_vMyGameObjects.push_back(std::move(mygameobj6)); 

    // Create the arrow button model
    auto mygameobj7 = MyGameObject::createGameObject("arrow_button");
    MyModel::Builder arrow_buttons;
    arrow_buttons.vertices = m_pMyUtilities->m_vButtons;
    arrow_buttons.indices = m_pMyUtilities->m_vIndices1;
    std::shared_ptr<MyModel> myArrowButtons = std::make_shared<MyModel>(m_myDevice, arrow_buttons);
    mygameobj7.model = myArrowButtons; 
    m_vMyGameObjects.push_back(std::move(mygameobj7)); 

    // Create the range button model
    auto mygameobj8 = MyGameObject::createGameObject("range_button");
    MyModel::Builder range_buttons;
    range_buttons.vertices = m_pMyUtilities->m_vRange;
    range_buttons.indices = m_pMyUtilities->m_vIndices2;
    std::shared_ptr<MyModel> myRangeButtons = std::make_shared<MyModel>(m_myDevice, range_buttons);
    mygameobj8.model = myRangeButtons; 
    m_vMyGameObjects.push_back(std::move(mygameobj8));  

    // Initialize a game object to hold the surface with texture model
    auto mygameobj9 = MyGameObject::createGameObject("texture_surface"); // it will crash if don't check nullptr on model in the render system
    m_vMyGameObjects.push_back(std::move(mygameobj9));

    // Create the range button model
    auto mygameobj10 = MyGameObject::createGameObject("texture_button");
    MyModel::Builder texture_buttons;
    texture_buttons.vertices = m_pMyUtilities->m_vTexture;
    texture_buttons.indices = m_pMyUtilities->m_vIndices3;
    std::shared_ptr<MyModel> myTextureButtons = std::make_shared<MyModel>(m_myDevice, texture_buttons);
    mygameobj10.textureModel = myTextureButtons; 
    m_vMyGameObjects.push_back(std::move(mygameobj10)); 

    // Create light source
    auto mygameobj11 = MyGameObject::makePointLight();  // named it "light_source" in my_game_object.cpp
    mygameobj11.transform.translation = glm::vec3(0.0f, 1.2f, 0.0f); // initialize it on top of created model at start
    m_vMyGameObjects.push_back(std::move(mygameobj11));

    // Create game object to hold saved models for viewing purposes
    auto mygameobj12 = MyGameObject::createGameObject("saved_model");
    m_vMyGameObjects.push_back(std::move(mygameobj12));
}

// Function used to check whether the given coordinate overlaps with an existing control point, if yes return its index in m_vControlPointVertices
int  MyApplication::_queryControlPoints(float posx, float posy)
{
    for (int i = 0; i < m_vControlPointVertices.size(); i++)
    {
        // Tolerate offsets within 0.02 range of actual coordinate
        if (fabs(m_vControlPointVertices[i].position.x - posx) <= 0.02 && fabs(m_vControlPointVertices[i].position.y - posy) <= 0.02)
        {
            return i;
        }
    }

    return -1; 
}

void MyApplication::mouseButtonEvent(bool bMouseDown, float posx, float posy)
{
    if (m_bCreateModel && bMouseDown)
    {
		// Convert position from (0, 1) to (-1, 1) and flip Y (before = down is positive, now = up is positive)

        //            (0, 0)               (1, 0)               (-1, 1)                (1, 1)
        //             ------------------------                   ------------------------
        //             |                      |                   |                      |
        //             |                      |                   |                      |
        //             |                      |                   |                      |
        //             |        WINDOW        |      ----->       |        WINDOW        |
        //             |                      |                   |                      |
        //             |                      |                   |                      |
        //             |                      |                   |                      |
        //             ------------------------                   ------------------------
        //            (0, 1)               (1, 1)               (-1, -1)               (1, -1)       

		float x = 2.0f * posx - 1.0f;
        float y = -2.0f * posy + 1.0f;
 
        m_bMoving = true;

        // Query what is currently being clicked
        int index = _queryControlPoints(x, y); // check if a control point is being clicked
        glm::vec4 buttonChosen = m_pMyColors->queryColorButton(x, y); // check if a color button is being clicked
        glm::vec3 colorChosen = glm::vec3(buttonChosen.x, buttonChosen.y, buttonChosen.z); 
        int index_of_color_button = int(buttonChosen[3]); 
        int index_of_arrow_button = m_pMyUtilities->queryArrowButton(x, y);     // check if an arrow button is being clicked
        int index_of_range_button = m_pMyUtilities->queryRangeButton(x, y);     // check if a range button is being clicked
        int index_of_texture_button = m_pMyUtilities->queryTextureButton(x, y); // check if a texture button is being clicked

        if (m_bShowButtons)
        {
            // If utility buttons is shown and a color button is chosen, change the color of surface
            if (buttonChosen != glm::vec4(-1.0f, -1.0f, -1.0f, -1.0f))
            {
                m_v3Color = colorChosen; 
                m_iIndex_of_color_button = index_of_color_button; 
                m_pMyColors->pressButton(index_of_color_button, 0.5f, m_vMyGameObjects, m_myDevice); 
            }

            // If utility buttons is shown and an arrow button is chosen,
            // increase or decrease the resolution of the model. 
            else if (index_of_arrow_button != -1)
            {
                m_iIndex_of_arrow_button = index_of_arrow_button; 
                if (index_of_arrow_button == 0)
                {
                    m_iResolution -= 1; 
                    if (m_iResolution < MIN_RESOLUTION) 
                    {
                        m_iResolution = MIN_RESOLUTION; 
                    }
                }
                else if (index_of_arrow_button == 1)
                {
                    m_iResolution += 1; 
                    if (m_iResolution > MAX_RESOLUTION) 
                    {
                        m_iResolution = MAX_RESOLUTION; 
                    }
                }
                std::cout << "Current Resolution: " << m_iResolution << std::endl; 
                std::cout << LINEBREAK << std::endl; 
                m_pMyUtilities->pressArrowButton(index_of_arrow_button, 0.5f, m_vMyGameObjects, m_myDevice);
            }

            // If utility buttons is shown and a range button is chosen,
            // increase or decrease the degree of u or v direction, if in terrain modeling mode
            else if (index_of_range_button != -1 && m_bCreateTerrain)
            {
                m_iIndex_of_range_button = index_of_range_button; 
                m_pMyUtilities->pressRangeButton(index_of_range_button, 0.5f, m_vMyGameObjects, m_myDevice, true); 
                resetSurface(); // reset control points when u- or v-degree is changed
            }

            else if (index_of_texture_button != -1)
            {
                m_iIndex_of_texture_button = index_of_texture_button;
                m_iCurrentTextureIndex = index_of_texture_button; 
                m_pMyUtilities->pressTextureButton(index_of_texture_button, 0.5f, m_vMyGameObjects, m_myDevice); 
            }
            
        }

        // If currently clicking on an existing control point, set it such that its position can be controlled by user depending on mouse position
        if (index != -1)
        {
            std::cout << "Selected control point: " << "(" << std::fixed << std::setprecision(2) << m_vControlPointVertices[index].position.x 
                      << ", " << std::fixed << std::setprecision(2) << m_vControlPointVertices[index].position.y << ")" << std::endl; 

            m_iIndex_of_selected_point = index; 

            if (m_bRemove) // If in remove mode, any selected control point will be deleted
            {
                std::cout << "--> Removing it" <<std::endl; 
                m_vControlPointVertices.erase(m_vControlPointVertices.begin() + m_iIndex_of_selected_point);
                m_pMyBezier->deleteControlPoint(m_iIndex_of_selected_point);
            }
            std::cout << LINEBREAK << std::endl;
        }

        // Otherwise, create a new control point
        else 
        {
            // Don't add the control point below the center line when modeling 3D hollow object
            if (posy > 0.5f && !m_bCreateTerrain){}
            else
            {
                // Add a new control point only if no buttons are being clicked, otherwise we will create 
                // a control point at the location of the buttons, which is messy. 
                if (!(m_bShowButtons && (index_of_arrow_button != -1 || index_of_color_button != -1 || index_of_range_button != -1 || index_of_texture_button != -1)))
                {
                    std::cout << "Added control point: (" << std::fixed << std::setprecision(2) << x 
                              << " , " << std::fixed << std::setprecision(2) << y << ")" << std::endl;
                              
                    std::cout << LINEBREAK << std::endl; 

                    float z; 

                    // control points have randomized depth value at start when adding a control
                    // point in terrain modeling
                    if (m_bCreateTerrain) 
                    {
                        z = -1.0f + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(2.f)));
                    }

                    // depth of control points does not matter when modeling 3D hollow objects
                    else
                    {
                        z = 0.0f;
                    }

                    m_pMyBezier->addControlPoint(x, y, z);

                    MyModel::PointLine controlPoint;
                    controlPoint.position.x = x;
                    controlPoint.position.y = y;
                    controlPoint.position.z = z;
                    controlPoint.color = RED; // Initialize each control point with color red

                    m_vControlPointVertices.push_back(controlPoint);
                }
            }
        }

        // No more than 121 points
        if (m_vControlPointVertices.size() >= 121)
        {
            return;
        }

        // Create bezier curve only when modeling 3D hollow object
        if (!m_bCreateTerrain)
        {
            m_pMyBezier->createBezierCurve(m_iResolution);
        }

        // recreate surface and normals in real time if m_bShowSurface or m_bShowNormals is true
        if (m_bShowSurface || m_bShowNormals)
        {
            createSurface(m_v3Color); 
        }

        // Update the game objects for rendering
        for (auto& obj : m_vMyGameObjects)
        {
            if (obj.name() == std::string("control_points"))
            {
                obj.model->updatePointLines(m_vControlPointVertices);
            }
            else if (obj.name() == std::string("bezier_curve"))
            {
                obj.model->updatePointLines(m_pMyBezier->m_vCurve);
            }
        }
    }

    // If in edit mode and mouse button released -> end movement of selected control point if applicable
    else if (m_bCreateModel && !bMouseDown)
    {
        // std::cout << "Mouse button released" << std::endl;
        // std::cout << LINEBREAK << std::endl; 
        m_bMoving = false; 
        m_iIndex_of_selected_point = -1; // no control points are being selected now

        // revert the color of buttons after being pressed
        if (m_iIndex_of_color_button != -1)
        {
            m_pMyColors->pressButton(m_iIndex_of_color_button, 2.0f, m_vMyGameObjects, m_myDevice); 
            m_iIndex_of_color_button = -1; 
        }

        if (m_iIndex_of_arrow_button != -1)
        {
            m_pMyUtilities->pressArrowButton(m_iIndex_of_arrow_button, 2.0f, m_vMyGameObjects, m_myDevice);
            m_iIndex_of_arrow_button = -1; 
        }

        if (m_iIndex_of_range_button != -1)
        {
            m_pMyUtilities->pressRangeButton(m_iIndex_of_range_button, 2.0f, m_vMyGameObjects, m_myDevice, false);
            m_iIndex_of_range_button = -1; 
        }

        if (m_iIndex_of_texture_button != -1)
        {
            m_pMyUtilities->pressTextureButton(m_iIndex_of_texture_button, 2.0f, m_vMyGameObjects, m_myDevice); 
            m_iIndex_of_texture_button = -1; 
        }
    }
    
    // moving camera
    else 
    {
        m_bMouseButtonPress = bMouseDown;
        m_myCamera.setButton(m_bMouseButtonPress, posx, posy);
    }
}

void MyApplication::mouseMotionEvent(float posx, float posy)
{
    // If in viewing mode, mouse movements are used to control camera position
    if (!m_bCreateModel)
	{
        m_myCamera.setMotion(m_bMouseButtonPress, posx, posy);
	}

    // Otherwise, in editting mode, mouse movements can be used to edit control point position
    else
    {
        if (!m_bMoving) return; 
        if (posy >= 0.5f && !m_bCreateTerrain) return; // cannot move points over the center line when modeling hollow objects

        float x = 2.0f * posx - 1.0f;
        float y = -2.0f * posy + 1.0f;

        
        if (m_iIndex_of_selected_point != -1)
        {
            if (!m_bRemove) // If a point is currently selected and we are in move mode, move it to desired coordinate
            {
                // Update position of selected control point
                m_vControlPointVertices[m_iIndex_of_selected_point].position.x = x; 
                m_vControlPointVertices[m_iIndex_of_selected_point].position.y = y; 
                m_pMyBezier->modifyControlPoint(m_iIndex_of_selected_point, x, y, m_vControlPointVertices[m_iIndex_of_selected_point].position.z); 

                // Recreate bezier curve only when modeling 3D hollow objects
                if (!m_bCreateTerrain)
                {
                    m_pMyBezier->createBezierCurve(m_iResolution);
                }   

                if (m_bShowSurface || m_bShowNormals)
                {
                    createSurface(m_v3Color); // recreate surface and normals in real time if m_bShowSurface or m_bShowNormals is true
                }

                // Update the corresponding models for rendering
                for (auto& obj : m_vMyGameObjects)
                {
                    if (obj.name() == std::string("control_points"))
                    {
                        obj.model->updatePointLines(m_vControlPointVertices);
                    }
                    else if (obj.name() == std::string("bezier_curve"))
                    {
                        obj.model->updatePointLines(m_pMyBezier->m_vCurve);
                    }
                }
            }
        }
    }
}

// Set camera's mode
void MyApplication::setCameraNavigationMode(MyCamera::MyCameraMode mode)
{
    if (!m_bCreateModel)
    {
        m_myCamera.setMode(mode);   
    }
}

void MyApplication::switchEditMode()
{
    // Switch between edit mode and camera mode
    m_bCreateModel = !m_bCreateModel;

    // If we are in edit mode, set the camera mode to orthographics mode
    m_bPerspectiveProjection = !m_bCreateModel;

    if (m_bPerspectiveProjection)
    {
	    std::cout << "Switch to Navigation Mode" << std::endl;
        std::cout << LINEBREAK << std::endl; 
        setCameraNavigationMode(MyCamera::MYCAMERA_FITALL);
        if (m_bShowButtons)
        {
            // If the buttons are on when transitioning to viewing mode, enable m_bShowButtonsBefore, 
            // so that when we transition back into edit mode, buttons will automatically be on
            m_bShowButtonsBefore = true; 
        }
        m_bShowButtons = false; 
        
        // In viewing mode of terrain modeling, points' depth can be tuned
        if (m_bShowSurface && m_bCreateTerrain)
        {
            // The current selected control point is set to the first control point in the list
            // This effect is achieved by coloring that point dark green, so users can differentiate
            // between other control points and the currently selected one. 
            m_iIndex_of_control_point = 0;  
            m_vControlPointVertices[m_iIndex_of_control_point].color = DARK_GREEN;

            // Update the corresponding models for rendering
            for (auto& obj : m_vMyGameObjects)
            {
                if (obj.name() == std::string("control_points"))
                {
                    obj.model->updatePointLines(m_vControlPointVertices);
                }
            }
        }
    }
    else
    {
        if (m_bCheckModel) // If we are checking an imported model before, hitting SPACE to go into create mode will disable model checking mode
        {
            m_bCheckModel = false; 
        }

        if (m_bShowButtonsBefore)
        {
            m_bShowButtons = true; 
            m_bShowButtonsBefore = false; 
        }

        m_fAngle = 0.0f; // Reset light to original position so that the light source object won't get in way of view when editing model. 

	    std::cout << "Switch to Edit Mode" << std::endl;
        std::cout << LINEBREAK << std::endl; 
        m_myCamera.setViewYXZ(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 0.0f, 0.0f));

        // switch the current control point selected for tuning back to red color when entering editing mode
        // from viewing mode
        if(m_iIndex_of_control_point != -1)
        {
            m_vControlPointVertices[m_iIndex_of_control_point].color = RED;
            m_iIndex_of_control_point = -1; 

            // Update the corresponding models for rendering
            for (auto& obj : m_vMyGameObjects)
            {
                if (obj.name() == std::string("control_points"))
                {
                    obj.model->updatePointLines(m_vControlPointVertices);
                }
            }
        }
    }
}

// Clear everything (control points, lines, curves, surfaces, normal vectors) 
void MyApplication::resetSurface()
{
    std::cout << "Reset" << std::endl;
    std::cout << LINEBREAK << std::endl; 

    // Deletes the current swap chain to unbind the connection between the buffer and the command buffer
    // Then recreate the swap chain.
    m_myRenderer.reset();  

    // Clear the control points
    m_vControlPointVertices.clear();
    m_pMyBezier->clearControlPoints();

    // Clear the normal vectors and disable its show flag
    m_vNormalVectors.clear();
    m_bShowNormals = false; 

    // Clear the bezier curve
    m_pMyBezier->m_vCurve.clear(); 

    // Clear the surface and disable its show flag
    m_pMyBezier->m_vSurface.clear(); 
    m_pMyBezier->m_vIndices.clear(); 
    m_bShowSurface = false; 

    // Now we can go ahead and delete the models for each game object (except center line) since the model's buffer does not bind to the command buffer anymore
    // After deleting it, we need to reset each object's model to their initial condition as when they are first loaded
    for (auto& obj : m_vMyGameObjects)
    {
        if (obj.name() != std::string("centerLine"))
        {
            obj.model = nullptr; 
        }
        if (obj.name() == std::string("control_points"))
        {
            std::shared_ptr<MyModel> mypointLine = std::make_shared<MyModel>(m_myDevice, 100);
            obj.model = mypointLine; 
        }
        else if (obj.name() == std::string("bezier_curve"))
        {
            std::shared_ptr<MyModel> mybezierCurve = std::make_shared<MyModel>(m_myDevice, 1000);
            obj.model = mybezierCurve; 
        }
        else if (obj.name() == std::string("color_button"))
        {
            MyModel::Builder color_buttons;
            color_buttons.vertices = m_pMyColors->m_vButtons;
            color_buttons.indices = m_pMyColors->m_vIndices;
            std::shared_ptr<MyModel> myColorButtons = std::make_shared<MyModel>(m_myDevice, color_buttons);
            obj.model = myColorButtons;
        }
        else if (obj.name() == std::string("arrow_button"))
        {
            MyModel::Builder arrow_buttons;
            arrow_buttons.vertices = m_pMyUtilities->m_vButtons;
            arrow_buttons.indices = m_pMyUtilities->m_vIndices1;
            std::shared_ptr<MyModel> myArrowButtons = std::make_shared<MyModel>(m_myDevice, arrow_buttons);
            obj.model = myArrowButtons;
        }
        else if (obj.name() == std::string("range_button"))
        {
            MyModel::Builder range_buttons;
            range_buttons.vertices = m_pMyUtilities->m_vRange;
            range_buttons.indices = m_pMyUtilities->m_vIndices2;
            std::shared_ptr<MyModel> myRangeButtons = std::make_shared<MyModel>(m_myDevice, range_buttons);
            obj.model = myRangeButtons;
        }
    }

    // switch back to edit mode
    m_bCreateModel = true; 
    m_bPerspectiveProjection = false; 
    m_myCamera.setViewYXZ(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 0.0f, 0.0f));
}

// Show or hide normal vectors 
void MyApplication::showHideNormalVectors()
{
    m_bShowNormals = !m_bShowNormals;

	if (m_bShowNormals)
	{
	    std::cout << "Show Normal Vectors" << std::endl;
        std::cout << LINEBREAK << std::endl; 
	}
	else
	{
	    std::cout << "Hide Normal Vectors" << std::endl;
        std::cout << LINEBREAK << std::endl; 
	}
}

// Show or hide surface to make editting control points easier
void MyApplication::showHideSurface()
{
    m_bShowSurface = !m_bShowSurface; 

    if (m_bShowSurface)
	{
	    std::cout << "Show Surface" << std::endl;
        std::cout << LINEBREAK << std::endl; 
        createSurface(m_v3Color); 
	}
	else
	{
	    std::cout << "Hide Surface" << std::endl;
        std::cout << LINEBREAK << std::endl; 
	}
}

// Revolve the computed bezier curve around the center line to form a hollow 3D model
// Or compute the points on the bezier surface and form a terrain
void MyApplication::createSurface(glm::vec3 surface_color)
{
    std::cout << "Attempting to Generate Surface ..." << std::endl;
	
    m_vNormalVectors.clear();
    
	if (m_pMyBezier)
    {
        if (!m_bCreateTerrain) // create the surface of revolution
        {
            std::cout << "--> Attempt SUCCESS" << std::endl; 
            m_pMyBezier->createRevolutionSurface(m_iResolution, m_iResolution, surface_color, m_iCurrentTextureIndex);
        }
        else  // create the bezier surface
        {
            // Number of control points must equal (u + 1) * (v + 1), or else we cannot create the bezier surface
            // u = number of degrees in u-direction, v = number of degrees in v-direction
            if (m_vControlPointVertices.size() != ((m_pMyUtilities->u_Deg() + 1) * (m_pMyUtilities->v_Deg() + 1)))
            {
                m_bShowSurface = false; 
                if (m_vControlPointVertices.size() < ((m_pMyUtilities->u_Deg() + 1) * (m_pMyUtilities->v_Deg() + 1)))
                {
                    std::cout << "--> Attempt FAILED: Need " << ((m_pMyUtilities->u_Deg() + 1) * (m_pMyUtilities->v_Deg() + 1)) - m_vControlPointVertices.size() << 
                    " more control points to proceed creating the surface" << std::endl; 
                }
                else
                {
                    std::cout << "--> Attempt FAILED: Remove " << m_vControlPointVertices.size() - ((m_pMyUtilities->u_Deg() + 1) * (m_pMyUtilities->v_Deg() + 1)) << 
                    " control points to proceed creating the surface" << std::endl;
                }
                return; 
            }
            std::cout << "--> Attempt SUCCESS" << std::endl; 
            m_pMyBezier->createBezierSurface(m_pMyUtilities->u_Deg(), m_pMyUtilities->v_Deg(), m_iResolution, m_iResolution, surface_color, m_iCurrentTextureIndex); 
        }
        std::cout << LINEBREAK << std::endl; 
    }

    // Create the surface model
    MyModel::Builder builder;
    builder.vertices = m_pMyBezier->m_vSurface;
    builder.indices = m_pMyBezier->m_vIndices;
    std::shared_ptr<MyModel> mysurface = std::make_shared<MyModel>(m_myDevice, builder);

    // Create the normal vectors model
	int nvertices = (int)m_pMyBezier->m_vSurface.size();
    m_vNormalVectors.resize(nvertices * 2);

    int index = 0;

    // create the normal vectors for both sides of the vertex
    for (int ii = 0; ii < nvertices / 2; ii++)
    {
        m_vNormalVectors[index].color = glm::vec3(1.0f, 1.0f, 0.0f);
        m_vNormalVectors[index++].position = m_pMyBezier->m_vSurface[2 * ii].position;
        m_vNormalVectors[index].color = glm::vec3(1.0f, 1.0f, 0.0f);
        m_vNormalVectors[index++].position = m_pMyBezier->m_vSurface[2 * ii].position + m_pMyBezier->m_vSurface[2 * ii].normal * 0.1f;

        m_vNormalVectors[index].color = glm::vec3(1.0f, 0.0f, 0.9f);
        m_vNormalVectors[index++].position = m_pMyBezier->m_vSurface[2 * ii + 1].position;
        m_vNormalVectors[index].color = glm::vec3(1.0f, 0.0f, 0.9f);
        m_vNormalVectors[index++].position = m_pMyBezier->m_vSurface[2 * ii + 1].position + m_pMyBezier->m_vSurface[2 * ii + 1].normal * 0.1f;
    }

    std::shared_ptr<MyModel> mynormals = std::make_shared<MyModel>(m_myDevice, nvertices * 2);
    mynormals->updatePointLines(m_vNormalVectors);

    for (auto& obj : m_vMyGameObjects)
    {
        if (obj.name() == std::string("bezier_surface"))
        {
            obj.model = mysurface;
        }
        else if (obj.name() == std::string("texture_surface"))
        {
            obj.textureModel = mysurface; 
        }
        else if (obj.name() == std::string("surface_normals"))
        {
            obj.model = mynormals;
        }
    }

	// Update min and max for MyCamera from the surface and the normals
    float min_x, min_y, min_z;
    min_x = min_y = min_z = std::numeric_limits<float>::max();

    float max_x, max_y, max_z;
    max_x = max_y = max_z = std::numeric_limits<float>::min();

    for (auto & vertex : m_pMyBezier->m_vSurface)
    {
        min_x = std::min(vertex.position.x, min_x);
        min_y = std::min(vertex.position.y, min_y);
        min_z = std::min(vertex.position.z, min_z);

        max_x = std::max(vertex.position.x, max_x);
        max_y = std::max(vertex.position.y, max_y);
        max_z = std::max(vertex.position.z, max_z);

        if (m_bShowNormals) // Only consider normals if normals are currently being shown
        {
            min_x = std::min(vertex.normal.x, min_x);
            min_y = std::min(vertex.normal.y, min_y);
            min_z = std::min(vertex.normal.z, min_z);

            max_x = std::max(vertex.normal.x, max_x);
            max_y = std::max(vertex.normal.y, max_y);
            max_z = std::max(vertex.normal.z, max_z);
        }
    }
    glm::vec3 min(min_x, min_y, min_z); 
    glm::vec3 max(max_x, max_y, max_z); 

    m_myCamera.setSceneMinMax(min, max); 
}

// Function to switch between remove or move mode for modifying control points
void MyApplication::removeOrMove()
{
    m_bRemove = !m_bRemove; 
    if (m_bRemove)
    {
        std::cout << "Remove Mode: Click on an existing control point to remove it" << std::endl; 
    }
    else
    {
        std::cout << "Move Mode: Click and Drag an existing control point to move its position" << std::endl; 
    }
    std::cout << LINEBREAK << std::endl;
}

// Function to show or hide utility buttons for manipulating the model
void MyApplication::showUtilityButtons()
{
    if (!m_bCreateModel && !m_bShowButtons)
    {
        // switch back to edit mode if we try to show utility buttons in viewing mode
        switchEditMode(); 
    }

    m_bShowButtons = !m_bShowButtons; 

    if (m_bShowButtons)
    {
        std::cout << "Showing Utility Buttons" << std::endl; 
    }
    else
    {
        std::cout << "Hiding Utility Buttons" << std::endl; 
    }
    std::cout << LINEBREAK << std::endl;
}

// Function for switching between modeling 3D hollow object or terrain surface
void MyApplication::switchModelingMode()
{
    m_bCreateTerrain = !m_bCreateTerrain; 
    resetSurface(); 

    if (m_bCreateTerrain)
    {
        std::cout << "Terrain Modeling Mode" << std::endl; 
    }
    else
    {
        std::cout << "Hollow Object Modeling Mode" << std::endl; 
    }
    std::cout << LINEBREAK << std::endl;
}

// Used to modify the depth of the currently selected control point (dark green)
// to achieve the desired surface. 
void MyApplication::depthControl(bool increase)
{
    if (increase)
    {
        m_vControlPointVertices[m_iIndex_of_control_point].position.z += 0.01f; // increase depth of selected control point
    }
    else
    {
        m_vControlPointVertices[m_iIndex_of_control_point].position.z -= 0.01f; // decrease depth of selected control point
    }
    m_pMyBezier->modifyControlPoint(m_iIndex_of_control_point, 
                                    m_vControlPointVertices[m_iIndex_of_control_point].position.x, 
                                    m_vControlPointVertices[m_iIndex_of_control_point].position.y, 
                                    m_vControlPointVertices[m_iIndex_of_control_point].position.z); 

    if (m_bShowSurface || m_bShowNormals)
    {
        createSurface(m_v3Color); // recreate surface and normals in real time if m_bShowSurface or m_bShowNormals is true
    }

    // Update the corresponding models for rendering
    for (auto& obj : m_vMyGameObjects)
    {
        if (obj.name() == std::string("control_points"))
        {
            obj.model->updatePointLines(m_vControlPointVertices);
        }
    }

}

// While viewing bezier surface, use left or right key to move to previous or next control point
// respectively, this is important for tuning the depth of that control point for a desired
// surface. Current point to be tuned will be colored dark green.
void MyApplication::nextPoint(bool next)
{
    m_vControlPointVertices[m_iIndex_of_control_point].color = RED;

    if (next) // select the next control point
    {
        m_iIndex_of_control_point += 1;
        if (m_iIndex_of_control_point >= m_vControlPointVertices.size())
        {
            m_iIndex_of_control_point = 0; 
        }
    }
    else // select the previous control point
    {
        m_iIndex_of_control_point -= 1;
        if (m_iIndex_of_control_point < 0)
        {
            m_iIndex_of_control_point = m_vControlPointVertices.size() - 1; 
        }
    }

    m_vControlPointVertices[m_iIndex_of_control_point].color = DARK_GREEN;

    for (auto& obj : m_vMyGameObjects)
    {
        if (obj.name() == std::string("control_points"))
        {
            obj.model->updatePointLines(m_vControlPointVertices);
        }
    }
}

// Function for updating the position of light based on user inputs
void MyApplication::updateLightPosition(bool clockwise)
{
    if (clockwise)
    {
        m_fAngle += 0.05f; 
    }
    else
    {
        m_fAngle -= 0.05f;
    }
}

// Method for saving the model created. Might not be the best way to code the obj file, but it is good for now
void MyApplication::saveModel()
{
    if (m_pMyBezier->m_vSurface.size() == 0) // Don't create obj file if no model has been created yet
    {
        std::cout << "No model detected, create one first" << std::endl; 
        std::cout << LINEBREAK << std::endl;
        return; 
    }

    // Ask for model name, which will also be the model's obj file name
    std::string fileName; 
    std::cout << "What is the name for your model (separate with _ if multiple words): "; 
    std::cin >> fileName; 
    std::cout << LINEBREAK << std::endl;

    // Prevents having models with duplicated names
    std::error_code ec;
    while (std::filesystem::exists("models/" + fileName + ".obj", ec) || ec)
    {
        std::cout << "A model with that name already exists, please enter another name for your model (separate with _ if multiple words): ";
        std::cin >> fileName; 
        std::cout << LINEBREAK << std::endl;
    }

    std::ofstream outfile ("models/" + fileName + ".obj");

    outfile << "o " + fileName << std::endl;

    std::string x, y, z, u, v; 

    // Load model vertices positions
    for (int i = 0; i < m_pMyBezier->m_vSurface.size(); i++)
    {
        x = std::to_string(m_pMyBezier->m_vSurface[i].position.x);
        y = std::to_string(m_pMyBezier->m_vSurface[i].position.y);
        z = std::to_string(m_pMyBezier->m_vSurface[i].position.z);

        outfile << "v " + x + " " + y + " " + z << std::endl; 
    }

    // Load model vertices UV coordinates for texture mapping
    for (int i = 0; i < m_pMyBezier->m_vSurface.size(); i++)
    {
        u = std::to_string((m_pMyBezier->m_vSurface[i].uv.x - (m_iCurrentTextureIndex * 0.25f)) * 4.0f);
        v = std::to_string(m_pMyBezier->m_vSurface[i].uv.y);

        outfile << "vt " + u + " " + v << std::endl; 
    }

    // Load model vertices normals
    for (int i = 0; i < m_pMyBezier->m_vSurface.size(); i++)
    {
        x = std::to_string(m_pMyBezier->m_vSurface[i].normal.x);
        y = std::to_string(m_pMyBezier->m_vSurface[i].normal.y);
        z = std::to_string(m_pMyBezier->m_vSurface[i].normal.z);

        outfile << "vn " + x + " " + y + " " + z << std::endl; 
    }

    // Load model faces (in triangles in this case) 
    for (int i = 0; i < m_pMyBezier->m_vIndices.size() / 3; i++)
    {
        // Plus 1 because obj file is index 1 based. 
        x = std::to_string(m_pMyBezier->m_vIndices[i * 3 + 0] + 1);
        y = std::to_string(m_pMyBezier->m_vIndices[i * 3 + 1] + 1);
        z = std::to_string(m_pMyBezier->m_vIndices[i * 3 + 2] + 1);

        outfile << "f " + x + "/" + x + "/" + x + " " + y + "/" + y + "/" + y + " " + z + "/" + z + "/" + z << std::endl; 
    }

    outfile.close();
}

// Method for users to check the model that they saved
void MyApplication::checkModel()
{
    std::string fileName; 
    std::cout << "Which model to load (enter its name): "; 
    std::cin >> fileName; 
    std::cout << LINEBREAK << std::endl;

    // Check if given model name exists, if not just quit
    std::error_code ec;
    if (!std::filesystem::exists("models/" + fileName + ".obj", ec) || ec)
    {
        std::cout << "No obj file with name " + fileName + ".obj exists under the models directory" << std::endl;
        std::cout << LINEBREAK << std::endl;
        return;  
    }
    
    glm::vec3 min, max;

    // Import the saved model for viewing and saved its min and max for camera
    std::shared_ptr<MyModel> mymodel = MyModel::createModelFromFile(m_myDevice, "models/" + fileName + ".obj", min, max);
    for (auto& obj : m_vMyGameObjects)
    {
        if (obj.name() == std::string("saved_model"))
        {
            obj.model = mymodel;
        }
    }

    m_myCamera.setSceneMinMax(min, max); // reset camera's min and max based on the imported model

    if (m_bCreateModel) // If in creating model mode, switch to view mode
    {
        switchEditMode(); 
    }
    else // If already in view mode, set camera to fit all mode to ensure object is in middle of screen
    {
        setCameraNavigationMode(MyCamera::MYCAMERA_FITALL);
    }

    m_bCheckModel = true; // set m_bCheckModel to true, so that only the imported model and light source is rendered
}
