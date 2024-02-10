#include "ColliderChecks.hpp"

#include "Components/BoxColliderComponent.hpp"
#include "Components/SphereColliderComponent.hpp"

namespace engine {
	// TODO: Implement this correctly, taking rotations into account. 
	// Currently we know that the lazy check already performs a box-box check, so we can just return true.
	bool ColliderChecks::boxBoxCollisionCheck(BoxColliderComponent* first, BoxColliderComponent* second) {
		return true;
	}

	// TODO: Implement this correctly, taking rotations and the sphere into account.
	// Currently we know that the lazy check already performs a box-box check, so we can just return true.
	bool ColliderChecks::boxSphereCollisionCheck(BoxColliderComponent* first, SphereColliderComponent* second) {
		return true;
	}
}