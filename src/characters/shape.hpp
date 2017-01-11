#pragma once
#include "character.hpp"
#include "../geometry.hpp"
#include <glm/glm.hpp>

namespace libapt
{
    class Shape : public Character
    {
	public:
		virtual void Parse(uint8_t *&iter) override;
		virtual void Update(const Transformation& t, std::shared_ptr<DisplayObject> instance) override;

		inline uint32_t GetGeometryId() { return m_geometryId; }
		inline void SetGeometry(std::shared_ptr<Geometry> geom) { m_geometry = geom; }
	private:
		glm::vec4 m_bounds;
		uint32_t m_geometryId;
		std::shared_ptr<Geometry> m_geometry;
    };
}