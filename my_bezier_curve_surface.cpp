#include "my_bezier_curve_surface.h"
#include <iostream>
#include <random>
#define M_PI        3.14159265358979323846264338327950288   /* pi */

// Add the control point coordinates to calculate bezier curve or surface
void MyBezier::addControlPoint(float x, float y, float z)
{
	m_vControlPoints.push_back(glm::vec3(x, y, z));
}

// Returns the number of control points currently stored
int MyBezier::numberOfControlPoints()
{
	return (int)m_vControlPoints.size();
}

// Gets the control point coordinates at a specific index 
void MyBezier::controlPoint(int index, glm::vec3& point)
{
	if (index >= 0 && index < numberOfControlPoints())
	{
		point = m_vControlPoints[index];
	}
}

// A function used to clear the elements in private member data m_vControlPoints
void MyBezier::clearControlPoints()
{
    m_vControlPoints.clear(); 
}

// Computes the points on the bezier curve given a resolution value
void MyBezier::createBezierCurve(int resolution)
{
    m_vCurve.clear();

    // If there are 2 or less control points, don't compute the bezier curve
    if (numberOfControlPoints() <= 2)
    {
        return;
    }

	// Identify the degree based on the number of control points m_vControlPoints
    int degree = numberOfControlPoints() - 1;

    glm::vec3 point; 

	// Use a for loop to loop through u from 0 to 1 based on the resolution
	for (int i = 0; i <= resolution; i++)
    {
        float u = 1.0f / resolution * i;

        // Use the function _pointOnBezierCurve to compute the point based on u
        _pointOnBezierCurve(degree, u, point); 

        MyModel::PointLine curve_point;
        curve_point.position.x = point.x;
        curve_point.position.y = point.y;
        curve_point.position.z = 0.0f;
        curve_point.color = glm::vec3(1.0f, 0.0f, 1.0f);

        // Add the computed point for u to m_vCurve
        m_vCurve.push_back(curve_point);
    }	
}

// Modify the coordinates of the control point at the given index in m_vControlPoints with the given (x, y, z)
void MyBezier::modifyControlPoint(int index, float x, float y, float z)
{
    if (index < 0 || index >= numberOfControlPoints()) return; // Check if given index is out of range
    m_vControlPoints[index].x = x; 
    m_vControlPoints[index].y = y; 
    m_vControlPoints[index].z = z;
}

// Removes the control point at a specific index
void MyBezier::deleteControlPoint(int index)
{
    if (index < 0 || index >= numberOfControlPoints()) return; // Check if given index is out of range
    m_vControlPoints.erase(m_vControlPoints.begin() + index);
}

// Compute the points of the surface by revolving the points on the bezier curve along the center line
// given the resolution of the bezier curve, the resolution of the circumference of the model, and a color
// for the model
void MyBezier::createRevolutionSurface(int xResolution, int rResolution, glm::vec3 color, int textureIndex)
{
    m_vSurface.clear();
    m_vIndices.clear();

	// Deteremine the degree
	int degree = numberOfControlPoints() - 1;
	
	// Use two for loops
	// The first (outer) for loop is used to loop through xResolution (u), and the 
	// second for loop (inner) for loop is used to loop through rResolution (r)
	for (int i = 0; i <= xResolution; i++)
    {
        float u = 1.0f / xResolution * i;

        // In the outer for loop, compute the point on the Bezier curve using _pointOnBezierCurve
        glm::vec3 point, tangent;
        _pointOnBezierCurve(degree, u, point);

        // In the outer for loop, compute the tangent vector using _derivative
	    // use perpendicular formula to change the tangent vector into normal vector
        _derivative(degree, u, tangent);
        tangent = glm::normalize(tangent);

        // Vector (x, y) -> Perp Vector (-y, x);

        float deltaTheta = 2 * M_PI / rResolution;
        for (int j = 0; j <= rResolution; j++)
        {
            // float v = 1.0f / (rResolution - 1) * j;
            float v = 1.0f / rResolution * j;
		    // In the inner for loop, compute the vertex by rotate rangle
	        float cosTheta, sinTheta;

            // Compute cosTheta and sinTheta based on j
            cosTheta = glm::cos(deltaTheta * j);
            sinTheta = glm::sin(deltaTheta * j);

            // Compute the vertex based on the cosTheta and sinTheta
            // Two vertices that are separated by a very very small distance so that it looks like
            // there is one vertex with two normal vectors that are in opposite directions.
            MyModel::Vertex vertex1;
            vertex1.position = glm::vec3(point.x, point.y * cosTheta, point.y * sinTheta);
            vertex1.normal = glm::vec3(-tangent.y, tangent.x * cosTheta, tangent.x * sinTheta);
            vertex1.color = color; 
            vertex1.uv = glm::vec2(u / 4.0f + (1.0f / 4.0f * textureIndex), v);
            m_vSurface.push_back(vertex1);

            MyModel::Vertex vertex2;
            vertex2.position = glm::vec3(vertex1.position.x, vertex1.position.y * 0.999f, vertex1.position.z * 0.999f);
            vertex2.normal = vertex1.normal * -1.0f;
            vertex2.color = color; 
            vertex2.uv = glm::vec2(u / 4.0f + (1.0f / 4.0f * textureIndex), v);
            m_vSurface.push_back(vertex2);

            // In the inner for loop, compute the indices
            // Build indices and push the indices to m_vIndices, which will be passed to the index buffer for rendering
            if (i < xResolution)
            {
                uint32_t index = ((rResolution + 1) * i + j) * 2;
                if (j == rResolution)
                {
                    m_vIndices.push_back(index);
                    m_vIndices.push_back(index + 2 * (rResolution + 1));
                    m_vIndices.push_back((i + 1) * (rResolution + 1) * 2);

                    m_vIndices.push_back(index);
                    m_vIndices.push_back(i * (rResolution + 1) * 2);
                    m_vIndices.push_back((i + 1) * (rResolution + 1) * 2);

                    m_vIndices.push_back(index + 1);
                    m_vIndices.push_back(index + 2 * (rResolution + 1) + 1);
                    m_vIndices.push_back((i + 1) * (rResolution + 1) * 2 + 1);

                    m_vIndices.push_back(index + 1);
                    m_vIndices.push_back(i * (rResolution + 1) * 2 + 1);
                    m_vIndices.push_back((i + 1) * (rResolution + 1) * 2 + 1);

                    // For thickness 
                    // m_vIndices.push_back(index);
                    // m_vIndices.push_back(i * (rResolution + 1) * 2);
                    // m_vIndices.push_back(i * (rResolution + 1) * 2 + 1);

                    // m_vIndices.push_back(index);
                    // m_vIndices.push_back(index + 1);
                    // m_vIndices.push_back(i * (rResolution + 1) * 2 + 1);

                    // if (i < xResolution)
                    // {
                    //     m_vIndices.push_back(index);
                    //     m_vIndices.push_back(index + 2 * (rResolution + 1));
                    //     m_vIndices.push_back(index + 2 * (rResolution + 1) + 1);

                    //     m_vIndices.push_back(index);
                    //     m_vIndices.push_back(index + 1);
                    //     m_vIndices.push_back(index + 2 * (rResolution + 1) + 1);
                    // }
                }
                else
                {
                    m_vIndices.push_back(index);
                    m_vIndices.push_back(index + 2 * (rResolution + 1));
                    m_vIndices.push_back(index + 2 * (rResolution + 1) + 2);

                    m_vIndices.push_back(index);
                    m_vIndices.push_back(index + 2);
                    m_vIndices.push_back(index + 2 * (rResolution + 1) + 2);

                    m_vIndices.push_back(index + 1);
                    m_vIndices.push_back(index + 2 * (rResolution + 1) + 1);
                    m_vIndices.push_back(index + 2 * (rResolution + 1) + 3);

                    m_vIndices.push_back(index + 1);
                    m_vIndices.push_back(index + 3);
                    m_vIndices.push_back(index + 2 * (rResolution + 1) + 3);

                    // For thickness 
                    // m_vIndices.push_back(index);
                    // m_vIndices.push_back(index + 2);
                    // m_vIndices.push_back(index + 3);

                    // m_vIndices.push_back(index);
                    // m_vIndices.push_back(index + 1);
                    // m_vIndices.push_back(index + 3);

                    // if (i < xResolution)
                    // {
                    //     m_vIndices.push_back(index);
                    //     m_vIndices.push_back(index + 2 * (rResolution + 1));
                    //     m_vIndices.push_back(index + 2 * (rResolution + 1) + 1);

                    //     m_vIndices.push_back(index);
                    //     m_vIndices.push_back(index + 1);
                    //     m_vIndices.push_back(index + 2 * (rResolution + 1) + 1);
                    // }
                }
            }
        }
    }
}

// Computes the points on the bezier surface based on the control points added based on the number of degrees
// in u and v direction, as well as the resolution in u and v direction, and a color for the surface. 
void MyBezier::createBezierSurface(int u_deg, int v_deg, int uResolution, int vResolution, glm::vec3 color, int textureIndex)
{
    m_vSurface.clear();
    m_vIndices.clear();

    // std::cout << "U resolution: " << uResolution << std::endl;
    // std::cout << "V resolution: " << vResolution << std::endl;

	for (int i = 0; i <= uResolution; i++)
    {
        float u = 1.0f / uResolution * i;

        for (int j = 0; j <= vResolution; j++)
        {
            float v = 1.0f / vResolution * j;
            glm::vec3 point, u_tangent, v_tangent; 

            _pointOnBezierSurface(u_deg, v_deg, u, v, point); 

            // Get the tangent of the computed point in u- and v-direction
            _uTangent(u_deg, v_deg, u, v, u_tangent);
            _vTangent(u_deg, v_deg, u, v, v_tangent);

            // The cross product of the u- and v-tangent will be the normal vector of the point
            glm::vec3 normal = glm::cross(v_tangent, u_tangent);
            normal = glm::normalize(normal);

            // Construct the vertex based on the computed coordinates and normal vector
            // Two vertices that are separated by a very very small distance so that it looks like
            // there is one vertex with two normal vectors that are in opposite directions.
            MyModel::Vertex vertex1;
            vertex1.position = point;
            vertex1.normal = normal;
            vertex1.color = color; 

            // u = u / how many textures + (1 / how many textures * index)
            vertex1.uv = glm::vec2(u / 4.0f + (1.0f / 4.0f * textureIndex), v);
            m_vSurface.push_back(vertex1);

            MyModel::Vertex vertex2;
            vertex2.position = glm::vec3(point.x, point.y, point.z - 0.0001f); 
            vertex2.normal = normal * -1.0f;
            vertex2.color = color; 

            // u = u / how many textures + (1 / how many textures * index)
            vertex2.uv = glm::vec2(u / 4.0f + (1.0f / 4.0f * textureIndex), v);
            m_vSurface.push_back(vertex2);

            // Build the indices to be passed to the index buffer
            uint32_t index = j * 2 * (uResolution + 1) + 2 * i;
            if (i < uResolution && j < vResolution)
            {
                m_vIndices.push_back(index);
                m_vIndices.push_back(index + 2 * (uResolution + 1)); 
                m_vIndices.push_back(index + 2 * (uResolution + 2));
                
                m_vIndices.push_back(index);
                m_vIndices.push_back(index + 2); 
                m_vIndices.push_back(index + 2 * (uResolution + 2));

                m_vIndices.push_back(index + 1);
                m_vIndices.push_back(index + 2 * (uResolution + 1) + 1); 
                m_vIndices.push_back(index + 2 * (uResolution + 2) + 1);
                
                m_vIndices.push_back(index + 1);
                m_vIndices.push_back(index + 3); 
                m_vIndices.push_back(index + 2 * (uResolution + 2) + 1);
            }

            // For thickness 
            // if (j == 0)
            // {
            //     if (i == 0)
            //     {
            //         m_vIndices.push_back(index); 
            //         m_vIndices.push_back(index + 2);
            //         m_vIndices.push_back(index + 3); 

            //         m_vIndices.push_back(index); 
            //         m_vIndices.push_back(index + 1);
            //         m_vIndices.push_back(index + 3); 

            //         m_vIndices.push_back(index); 
            //         m_vIndices.push_back(index + 2 * (uResolution + 1)); 
            //         m_vIndices.push_back(index + 2 * (uResolution + 1) + 1); 

            //         m_vIndices.push_back(index); 
            //         m_vIndices.push_back(index + 1); 
            //         m_vIndices.push_back(index + 2 * (uResolution + 1) + 1); 
            //     }
            //     else if (i == uResolution)
            //     {
            //         m_vIndices.push_back(index); 
            //         m_vIndices.push_back(index + 2 * (uResolution + 1)); 
            //         m_vIndices.push_back(index + 2 * (uResolution + 1) + 1); 

            //         m_vIndices.push_back(index); 
            //         m_vIndices.push_back(index + 1); 
            //         m_vIndices.push_back(index + 2 * (uResolution + 1) + 1); 
            //     }
            //     else
            //     {
            //         m_vIndices.push_back(index); 
            //         m_vIndices.push_back(index + 2);
            //         m_vIndices.push_back(index + 3); 

            //         m_vIndices.push_back(index); 
            //         m_vIndices.push_back(index + 1);
            //         m_vIndices.push_back(index + 3); 
            //     }
            // }
            // else if (j == vResolution && i != uResolution)
            // {
            //     m_vIndices.push_back(index); 
            //     m_vIndices.push_back(index + 2);
            //     m_vIndices.push_back(index + 3); 

            //     m_vIndices.push_back(index); 
            //     m_vIndices.push_back(index + 1);
            //     m_vIndices.push_back(index + 3); 
            // }
            // else
            // {
            //     if (i == 0 || i == uResolution)
            //     {
            //         m_vIndices.push_back(index); 
            //         m_vIndices.push_back(index + 2 * (uResolution + 1)); 
            //         m_vIndices.push_back(index + 2 * (uResolution + 1) + 1); 

            //         m_vIndices.push_back(index); 
            //         m_vIndices.push_back(index + 1); 
            //         m_vIndices.push_back(index + 2 * (uResolution + 1) + 1); 
            //     }
            // }
        }
    }
    // std::cout << "Size of surface points: " << m_vSurface.size() << std::endl; 
    // std::cout << "Size of indices: " << m_vIndices.size() << std::endl; 
    // for (int i = 0; i <= m_vIndices.size() / 3; i++)
    // {
    //     std::cout << m_vIndices[i * 3 + 0] << ", " << m_vIndices[i * 3 + 1] << ", " << m_vIndices[i * 3 + 2] << ", " << std::endl; 
    // }
}

void MyBezier::_allBernstein(int n, float u, float* B)
{
    /* Compute all n-th degree Bernstein Polynomials */
    /* Input: n, u */
    /* Output: B (an array, B[0], �K, B[n]) */
    int k = 0;
    float u1 = 1.0f - u;
    float saved = 0.0f;
    float temp = 0.0f;
    B[0] = 1.0;

    for (int j = 1; j <= n; j++)
    {
        saved = 0.0;
        for (k = 0; k < j; k++)
        {
            temp = B[k];
            B[k] = saved + u1 * temp;
            saved = u * temp;
        }

        B[j] = saved;
    }
}

void MyBezier::_pointOnBezierCurve(int degree, float u, glm::vec3 &point)
{
    /* Compute point on Bezier curve */
    std::vector<float> vfB(degree + 1, 0.0f);

    _allBernstein(degree, u, vfB.data()); /* B is a local array */
    point = glm::vec3{ 0.0f };
    for (int k = 0; k <= degree; k++)
    {
        point = point + m_vControlPoints[k] * vfB[k];
    }
}

void MyBezier::_pointOnBezierSurface(int u_deg, int v_deg, float u, float v, glm::vec3 &point)
{
    /* Compute point on Bezier surface */
    std::vector<float> vfBi(u_deg + 1, 0.0f);
    std::vector<float> vfBj(v_deg + 1, 0.0f);

    _allBernstein(u_deg, u, vfBi.data()); 
    _allBernstein(v_deg, v, vfBj.data()); 

    point = glm::vec3{ 0.0f };
    for (int k = 0; k <= u_deg; k++)
    {
        for (int q = 0; q <= v_deg; q++)
        {
            point = point + m_vControlPoints[k + (u_deg + 1) * q] * vfBi[k] * vfBj[q];
        }
    }
}

void MyBezier::_derivative(int degree, float u, glm::vec3& der)
{
    std::vector<float> vfB(degree, 0.0f);

    _allBernstein(degree - 1, u, vfB.data());
    der = glm::vec3{ 0.0f };
    for (int k = 0; k <= degree - 1; k++)
    {
        der = der +  (m_vControlPoints[k + 1] - m_vControlPoints[k]) * vfB[k];
    }

    der = der * (float)degree;
}

// Compute tangent of point in u-direction
void MyBezier::_uTangent(int u_deg, int v_deg, float u, float v, glm::vec3& der)
{
    std::vector<float> Bi(u_deg, 0.0f);
    std::vector<float> Bj(v_deg + 1, 0.0f);

    _allBernstein(u_deg - 1, u, Bi.data()); 
    _allBernstein(v_deg, v, Bj.data()); 

    der = glm::vec3{ 0.0f };

    for (int i = 0; i <= u_deg - 1; i++)
    {
        for (int j = 0; j <= v_deg; j++)
        {
            der = der + Bi[i] * Bj[j] * (m_vControlPoints[(i + (u_deg + 1) * j) + 1] - m_vControlPoints[i + (u_deg + 1) * j]); 
        }
    }

    der = der * float(u_deg); 
}

// Compute tangent of point in v-direction
void MyBezier::_vTangent(int u_deg, int v_deg, float u, float v, glm::vec3& der)
{
    std::vector<float> Bi(u_deg + 1, 0.0f);
    std::vector<float> Bj(v_deg, 0.0f);

    _allBernstein(u_deg, u, Bi.data()); 
    _allBernstein(v_deg - 1, v, Bj.data()); 

    der = glm::vec3{ 0.0f };

    for (int i = 0; i <= u_deg; i++)
    {
        for (int j = 0; j <= v_deg - 1; j++)
        {
            der = der + Bi[i] * Bj[j] * (m_vControlPoints[(i + (u_deg + 1) * (j + 1))] - m_vControlPoints[i + (u_deg + 1) * j]); 
        }
    }

    der = der * float(v_deg); 
}

