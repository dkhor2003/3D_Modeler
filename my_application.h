#ifndef __MY_APPLICATION_H__
#define __MY_APPLICATION_H__

#include "my_descriptors.h"
#include "my_window.h"
#include "my_device.h"
#include "my_renderer.h"
#include "my_game_object.h"
#include "my_camera.h"
#include "my_bezier_curve_surface.h"
#include "my_colors.h"
#include "my_utilities.h"
#include <glm/glm.hpp>

#include <memory>
#include <vector>

class MyApplication 
{
public:
	static constexpr int WIDTH = 1000;
	static constexpr int HEIGHT = 1000;
	static constexpr int MAX_RESOLUTION = 100; 
	static constexpr int MIN_RESOLUTION = 3; 
	const std::string    LINEBREAK = "--------------------------------------------------------------------"; 
	const glm::vec3      DARK_GREEN = glm::vec3(0.007f, 0.4f, 0.1f); 
	const glm::vec3      RED = glm::vec3(1.0f, 0.0f, 0.0f);

	MyApplication();
	~MyApplication();

	MyApplication(const MyApplication&) = delete;
	MyApplication& operator=(const MyApplication&) = delete;

	void run();

	// Assignment
	void switchProjectionMatrix();
	void mouseButtonEvent(bool bMouseDown, float posx, float posy);
	void mouseMotionEvent(float posx, float posy);
	void setCameraNavigationMode(MyCamera::MyCameraMode mode);

	void switchEditMode();
	void resetSurface();
	void showHideNormalVectors();
	void showHideSurface();
	void createSurface(glm::vec3 surface_color);
	void removeOrMove(); 
	void showUtilityButtons(); 
	void switchModelingMode(); 
	void depthControl(bool increase);
	void nextPoint(bool next);
	void updateLightPosition(bool clockwise);
	void saveModel(); 
	void checkModel(); 

private:
	void _loadGameObjects();
	int  _queryControlPoints(float posx, float posy); 

	MyWindow                        m_myWindow{ WIDTH, HEIGHT, "3D Modeler" };
	MyDevice                        m_myDevice{ m_myWindow };
	MyRenderer                      m_myRenderer{ m_myWindow, m_myDevice };

	// Note: the order matters, because the destructor is called in the reversed order
	// globalPool needs to delete before m_myDevice
	std::unique_ptr<MyDescriptorPool> m_pMyGlobalPool{};
	std::vector<MyGameObject>       m_vMyGameObjects;
	MyCamera                        m_myCamera{};
	bool                            m_bPerspectiveProjection;
	bool                            m_bMouseButtonPress = false;
	bool                            m_bCreateModel = false;
	bool                            m_bShowNormals = false;
	bool                            m_bShowSurface =  false;
	bool                            m_bMoving = false;
	bool                            m_bRemove = false; 
	bool                            m_bShowButtons = false; 
	bool                            m_bShowButtonsBefore = false; 
	bool                            m_bCreateTerrain = false; 
	bool                            m_bCheckModel = false; 

	std::vector<MyModel::PointLine> m_vControlPointVertices;
	std::vector<MyModel::PointLine> m_vNormalVectors;
	std::shared_ptr<MyBezier>       m_pMyBezier;
	std::shared_ptr<MyColors>       m_pMyColors; 
	std::shared_ptr<MyUtilities>    m_pMyUtilities; 

	int                             m_iIndex_of_selected_point = -1; 
	int                             m_iIndex_of_color_button = -1; 
	int                             m_iIndex_of_arrow_button = -1; 
	int                             m_iIndex_of_range_button = -1; 
	int                             m_iIndex_of_texture_button = -1; 
	int                             m_iIndex_of_control_point = -1;  
	int                             m_iResolution = 20; 
	int                             m_iCurrentTextureIndex = 3; // In my texture image, the no texture is set to index 3

	float                           m_fAngle = 0.0f; 

	glm::vec3                       m_v3Color = glm::vec3(1.0f, 1.0f, 1.0f); // default color for surface is white  
	glm::vec3                       m_v3LightOffset{ 0.0f };
};

#endif

