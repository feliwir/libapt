#pragma once
#include <glm/glm.hpp>

namespace libapt
{
        struct Transformation
		{
			glm::vec2 translate;
			glm::mat2 rotscale;

			Transformation();
		};
}