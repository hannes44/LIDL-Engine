#pragma once

#include "uuid_v4.h"
#include "endianness.h"

namespace engine
{
	class Uuid
	{
	public:
		UUIDv4::UUID id;
		Uuid() {
			UUIDv4::UUIDGenerator<std::mt19937_64> uuidGenerator;
			id = uuidGenerator.getUUID();
		};

		UUIDv4::UUID getId();

	};
}