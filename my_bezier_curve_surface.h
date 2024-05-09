#ifndef __MY_BEZIER_CURVE_SURFACE_H__
#define __MY_BEZIER_CURVE_SURFACE_H__

#include <vector>
#include <glm/glm.hpp>
#include "my_model.h"

class MyBezier
{
public:
	void addControlPoint(float x, float y, float z);
	int  numberOfControlPoints();
	void clearControlPoints(); 
	void controlPoint(int index, glm::vec3& point);
	void createBezierCurve(int resolution);
	void createRevolutionSurface(int xResolution, int rResolution, glm::vec3 color, int textureIndex);
	void createBezierSurface(int u_deg, int v_deg, int xResolution, int rResolution, glm::vec3 color, int textureIndex);
	void modifyControlPoint(int index, float x, float y, float z);
	void deleteControlPoint(int index);

	std::vector<MyModel::PointLine> m_vCurve;
	std::vector<MyModel::Vertex>    m_vSurface;
	std::vector<uint32_t>           m_vIndices;

protected:

	void _allBernstein(int n, float u, float* B);
	void _pointOnBezierCurve(int n, float u, glm::vec3 &point);
	void _pointOnBezierSurface(int u_deg, int v_deg, float u, float v, glm::vec3 &point);
	void _derivative(int degree, float u, glm::vec3& der);
	void _uTangent(int u_deg, int v_deg, float u, float v, glm::vec3& der);
	void _vTangent(int u_deg, int v_deg, float u, float v, glm::vec3& der);

	std::vector<glm::vec3>          m_vControlPoints;
};


#endif

