#pragma once
#include "frameitem.hpp"
#include "action.hpp"
#include <glm/glm.hpp>

namespace libapt
{
	class PlaceObject : public FrameItem
	{
	private:
		union Flags
		{
			struct
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

			uint32_t Data;
		};

		union ClipEventFlags
		{
			struct
			{
				uint8_t ClipEventKeyUp			: 1;
				uint8_t ClipEventKeyDown		: 1;
				uint8_t ClipEventMouseUp		: 1;
				uint8_t ClipEventMouseDown		: 1;
				uint8_t ClipEventMouseMove		: 1;
				uint8_t ClipEventUnload			: 1;
				uint8_t ClipEventEnterFrame		: 1;
				uint8_t ClipEventLoad			: 1;
				uint8_t ClipEventDragOver		: 1;
				uint8_t ClipEventRollOut		: 1;
				uint8_t ClipEventRollOver		: 1;
				uint8_t ClipEventReleaseOutside : 1;
				uint8_t ClipEventRelease		: 1;
				uint8_t ClipEventPress			: 1;
				uint8_t ClipEventInitialize		: 1;
				uint8_t ClipEventData			: 1;
				uint8_t Reserved				: 5;
				uint8_t ClipEventConstruct		: 1;
				uint8_t ClipEventKeyPress		: 1;
				uint8_t ClipEventDragOut		: 1;
				uint8_t Reserved2				: 8;
			};
			uint32_t Data;
		};

		struct ClipAction
		{
			ClipEventFlags flags;
			uint8_t* bytes;
		};

		struct ClipActions
		{
			uint32_t count;
			std::vector<ClipAction> actions;
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

		inline bool HasClipActions()
		{
			return m_flags.PlaceFlagHasClipActions;
		}

		const ClipActions& GetClipActions()
		{
			return m_clipactions;
		}
	private:
		void ParseClipActions(uint8_t*& offset, const uint8_t *base);
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
		ClipActions m_clipactions;

	};
}