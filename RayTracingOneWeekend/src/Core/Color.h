#pragma once

#include "Ray.h"

#include <glm/glm.hpp>
#include <ostream>

namespace rtow {

	void WriteColor(std::ostream& out, const glm::vec3& color)
	{
		out << (int)(255.999 * color.r) << ' ' << (int)(255.999 * color.g) << ' ' << (int)(255.999 * color.b) << '\n';
	}

	glm::vec3 RayColor(const Ray& ray)
	{
		glm::vec3 unitDir = glm::normalize(glm::vec3{ ray.Direction.x, ray.Direction.y, ray.Direction.z });
		float t = 0.5f * (unitDir.y + 1.0f); // Scaling between 0.0f -> 1.0f

		return (1.0f - t) * glm::vec3(1.0f) + t * glm::vec3(0.5f, 0.7f, 1.0f);
	}

}