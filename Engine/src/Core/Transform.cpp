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

glm::quat engine::Transform::getRotation() {
	return glm::quat(transformMatrix);
}

// TODO: Fix this, I do not trust it at all
void engine::Transform::setRotation(glm::quat rotation) {
	transformMatrix = glm::rotate(transformMatrix, glm::angle(rotation) - glm::angle(getRotation()), glm::vec3(0, 1, 0));
}

// TODO: Fix this, I do not trust it at all
void engine::Transform::setRotationFromDirection(glm::vec3 direction) {
	setRotation(glm::quatLookAt(glm::normalize(direction), glm::vec3(0, 1, 0)));
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
