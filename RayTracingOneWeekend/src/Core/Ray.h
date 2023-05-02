#pragma once

#include <glm/glm.hpp>

namespace rtow {

	struct Ray
	{
		constexpr Ray() noexcept = default;
		Ray(const glm::vec3& origin, const glm::vec3 direction) : Origin(origin), Direction(direction) {}

		glm::vec3 At(float t) const
		{
			return Origin + t * Direction;
		}

		glm::vec3 Origin = {};
		glm::vec3 Direction = {};
	};

}