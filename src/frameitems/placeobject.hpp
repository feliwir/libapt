#pragma once
#include "frameitem.hpp"
#include "action.hpp"
#include <glm/glm.hpp>

namespace libapt
{
	class PlaceObject : public FrameItem
	{
	private:
		struct Flags
		{
			uint8_t PlaceFlagMove : 1;
			uint8_t PlaceFlagHasCharacter : 1;
			uint8_t PlaceFlagHasMatrix : 1;
			uint8_t PlaceFlagHasColorTransform : 1;
			uint8_t PlaceFlagHasRatio : 1;
			uint8_t PlaceFlagHasName : 1;
			uint8_t PlaceFlagHasClipDepth : 1;
			uint8_t PlaceFlagHasClipActions : 1;
			uint16_t Reserved;
		};
	public:
		PlaceObject();
		virtual void Parse(uint8_t *offset, const uint8_t *base);

		inline int32_t GetDepth() { return m_depth; }
		inline uint32_t GetCharacter() { return m_character; }
		inline glm::f32mat2 GetRotScale() { return m_rotscale; }
		inline glm::f32vec2 GetTranslate() { return m_translate; }
		inline const std::string& GetName() { return m_name; }

		inline bool HasMove()
		{
			return m_flags.PlaceFlagMove;
		}

		inline bool HasCharacter()
		{
			return m_flags.PlaceFlagHasCharacter;
		}

		inline bool HasMatrix()
		{
			return m_flags.PlaceFlagHasMatrix;
		}

		inline bool HasColortransform()
		{
			return m_flags.PlaceFlagHasColorTransform;
		}

		inline bool HasRatio()
		{
			return m_flags.PlaceFlagHasRatio;
		}

		inline bool HasName()
		{
			return m_flags.PlaceFlagHasName;
		}
	private:
		Flags m_flags;
		int32_t m_depth;
		int32_t m_character;
		glm::f32mat2 m_rotscale;
		glm::f32vec2 m_translate;
		glm::u8vec4 m_color;
		uint32_t m_unknown;
		glm::f32 m_ratio;
		std::string m_name;
		int32_t m_clipdepth;
		std::shared_ptr<Action> m_action;
	};
}