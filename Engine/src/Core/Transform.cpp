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

void engine::Transform::setRotationFromQuaternion(const glm::quat& orientation) 
{
    glm::mat4 rotationMatrix = glm::mat4_cast(orientation);

    glm::vec3 position = glm::vec3(transformMatrix[3]);

    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            transformMatrix[i][j] = rotationMatrix[i][j];
        }
    }

    transformMatrix[3] = glm::vec4(position, 1.0f);
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

glm::vec3 engine::Transform::getLocalForward()
{
	glm::vec3 localForwardVector = glm::vec3(-transformMatrix[2][0], -transformMatrix[2][1], -transformMatrix[2][2]);
	localForwardVector = glm::normalize(localForwardVector);
	return localForwardVector;
}
