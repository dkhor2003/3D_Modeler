#ifndef __MY_GAMEOBJECT_H__
#define __MY_GAMEOBJECT_H__

#include "my_model.h"
#include <glm/gtc/matrix_transform.hpp>

// std
#include <memory>

struct TransformComponent
{
	glm::vec3 translation{ 0.0f, 0.0f, 0.0f };
	glm::vec3 scale{ 1.0f, 1.0f, 1.0f };
	glm::vec3 rotation{ 0.0f, 0.0f, 0.0f };

	// Matrix corresponds to the overall transformation - scale * Rz * Rx * Ry * transform (row, pitch, yaw)
	// Rotation conventation uses Tait-Bryan angles with axis order Y(1), X(2), Z(3)
	glm::mat4 mat4(); 
	glm::mat3 normalMatrix(); 
};

class MyGameObject
{
public:
	using id_t = unsigned int;

	static MyGameObject createGameObject(std::string name)
	{
		static id_t currentID = 0;
		return MyGameObject{ currentID++, name };
	}

	static MyGameObject makePointLight(float radius = 0.1f, glm::vec3 color = glm::vec3(1.f));

	MyGameObject(const MyGameObject&) = delete;
	MyGameObject& operator=(const MyGameObject&) = delete;
	MyGameObject(MyGameObject&&) = default;
	MyGameObject& operator=(MyGameObject&&) = default;

	id_t                     getID() const { return m_iID; }
	std::shared_ptr<MyModel> model{};
	std::shared_ptr<MyModel> textureModel{};
	glm::vec3                color{};
	TransformComponent       transform{};
	std::string              name() const { return m_sName; }

private:
	MyGameObject(id_t objID, std::string name) : m_iID{ objID }, m_sName{ name } {}
	id_t        m_iID;
	std::string m_sName;
};

#endif

