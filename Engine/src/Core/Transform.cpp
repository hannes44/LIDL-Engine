#include "Transform.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

engine::Transform::Transform()
{
}

glm::vec3 engine::Transform::getPosition()
{
	return glm::vec3(transformMatrix[3]);
}

void engine::Transform::setPosition(glm::vec3 position)
{
	transformMatrix[3] = glm::vec4(position, 1.0f);
}

glm::quat engine::Transform::getRotation() 
{
	return glm::quat(transformMatrix);
}

// TODO: Fix this, I do not trust it at all
void engine::Transform::setRotation(glm::quat rotation) 
{
	//transformMatrix = glm::rotate(transformMatrix, glm::angle(rotation) - glm::angle(getRotation()), glm::vec3(0, 1, 0));
}

// TODO: Fix this, I do not trust it at all
void engine::Transform::setRotationFromDirection(glm::vec3 direction, glm::vec3 normal) 
{
	glm::vec3 up = normal;

	glm::vec3 xaxis = glm::normalize(glm::cross(up, direction));

	glm::vec3 yaxis = glm::normalize(glm::cross(direction, xaxis));

	transformMatrix[0][0] = xaxis.x;
	transformMatrix[0][1] = yaxis.x;
	transformMatrix[0][2] = direction.x;

	transformMatrix[1][0] = xaxis.y;
	transformMatrix[1][1] = yaxis.y;
	transformMatrix[1][2] = direction.y;

	transformMatrix[2][0] = xaxis.z;
	transformMatrix[2][1] = yaxis.z;
	transformMatrix[2][2] = direction.z;

}

void engine::Transform::shiftPosition(glm::vec3 offset)
{
	engine::Transform::setPosition(engine::Transform::getPosition() + offset);
}

glm::vec3 engine::Transform::getScale()
{
	return glm::vec3(transformMatrix[0][0], transformMatrix[1][1], transformMatrix[2][2]);
}

void engine::Transform::setScale(glm::vec3 scale)
{
	transformMatrix[0][0] = scale.x;
	transformMatrix[1][1] = scale.y;
	transformMatrix[2][2] = scale.z;
}
