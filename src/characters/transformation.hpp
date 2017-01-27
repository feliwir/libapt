#pragma once
#include "../graphics/clipmask.hpp"
#include <memory>
#include <glm/glm.hpp>


namespace libapt
{
        struct Transformation
		{
			glm::vec2 translate;
			glm::mat2 rotscale;
			glm::vec4 color;
			std::shared_ptr<ClipMask> mask;
			bool visible;
			Transformation();
		};
}