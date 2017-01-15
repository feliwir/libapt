#pragma once
#include "character.hpp"
#include <libapt/texture.hpp>
#include <vector>

namespace libapt
{
	class Button : public Character
	{
	private:
		enum State
		{
			NONE	= 0,
			HOVER	= 1,
			CLICKED	= 2,
			RELEASE = 3,
		};

		union ActionFlags
		{
			struct 
			{
				bool CondOverDownToIdle : 1;
				char KeyPress : 7;
				bool CondIdleToOverUp : 1;
				bool CondOverUpToIdle : 1;
				bool CondOverUpToOverDown : 1;
				bool CondOverDownToOverUp : 1;
				bool CondOverDownToOutDown : 1;
				bool CondOutDownToOverDown : 1;
				bool CondOutDownToIdle : 1;
				bool CondIdleToOverDown : 1;
			};
			uint32_t value;
		};

		struct Action {
			ActionFlags flags; //matches with ButtonActionFlags;
			uint8_t *actiondata;
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
	private:
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
	};
}