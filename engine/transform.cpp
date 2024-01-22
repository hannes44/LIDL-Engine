#include "Transform.hpp"
#include <glm/gtc/matrix_transform.hpp>

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

glm::vec3 engine::Transform::getScale()
{
	return glm::vec3(transformMatrix[0][0], transformMatrix[1][1], transformMatrix[2][2]);
}

void engine::Transform::setScale(glm::vec3 scale)
{
	transformMatrix = glm::scale(transformMatrix, scale);
}
