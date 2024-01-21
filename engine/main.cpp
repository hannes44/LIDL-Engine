#include <iostream>
#include "transform.hpp"

int main(int argc, char* argv[])
{
	std::cout << "Hello, World!" << std::endl;
	engine::Transform transform{};
	transform.position = glm::vec3(1.0f, 2.0f, 3.0f);
	std::cout << "Position: " << transform.position.x << ", " << transform.position.y << ", " << transform.position.z << std::endl;
	return 0;
}

