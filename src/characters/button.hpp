#pragma once
#include "character.hpp"
#include "../input.hpp"
#include <libapt/texture.hpp>
#include <vector>
#include "../graphics/buffer.hpp"
namespace libapt
{
	class Button : public Character
	{
	private:
		enum State
		{
			IDLE	= 0,
			OUTUP	= 1,
			OVERUP	= 2,
			OUTDOWN	= 3,
			OVERDOWN= 4,
		};

		union ActionFlags
		{
			struct 
			{
				bool IdleToOverUp		: 1;
				bool OverUpToIdle		: 1;
				bool OverUpToOverDown	: 1;
				bool OverDownToOverUp	: 1;
				bool OverDownToOutDown  : 1;
				bool OutDownToOverDown	: 1;
				bool IdleToOverDown		: 1;
				bool OverDownToIdle		: 1;
				Input ConditionFlag_KeyPress;
			};
			uint32_t value;
		};

		struct Action {
			ActionFlags flags; //matches with ButtonActionFlags;
			const uint8_t* bytecode;
		};

		union RecordFlags
		{
			struct
			{
				bool ButtonStateUp : 1;
				bool ButtonStateOver : 1;
				bool ButtonStateDown : 1;
				bool ButtonStateHitTest : 1;
				char Reserved : 4;
			};
			uint32_t value;
		};

		struct Record
		{
			RecordFlags	flags; //matches with ButtonFlags
			uint32_t	character;
			int32_t		depth;
			glm::mat2	rotateandscale;
			glm::vec2	translate;
			glm::vec4	color; ///always 1.0,1.0,1.0,1.0 as far as I can see
			glm::vec4	unknown; ///always zero as far as I can see
		};
	public:
		Button();
		virtual void Parse(uint8_t*& iter) override;
		virtual void Update(const Transformation& t, std::shared_ptr<DisplayObject> instance) override;

		void OnFocus(std::shared_ptr<DisplayObject> instance);
	private:
		void CreateDebugBuffer();
	private:
		glm::vec4 m_color;
		Buffer m_buffer;
		uint32_t m_unknown;
		glm::vec4 m_bounds;
		uint32_t m_trianglecount;
		uint32_t m_vertexcount;
		std::vector<glm::vec2> m_vertices;
		std::vector<glm::u16vec3> m_triangles;
		uint32_t m_recordcount;
		std::vector<Record> m_records;
		uint32_t m_actioncount;
		std::vector<Action> m_actions;
		uint32_t m_unknown2;
		State m_state;
		State m_last;
	};
}