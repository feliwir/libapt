#pragma once
#include <libapt/scriptcontext.hpp>
#include "character.hpp"
#include "../frame.hpp"
#include <glm/glm.hpp>
#include <map>
#include <memory>
#include <vector>
#include <stdint.h>


namespace libapt
{
    //the base class for movie & sprite
    class Container : public Character
    {
	private:
		struct DisplayObject
		{
			std::shared_ptr<Character> ch;
			glm::vec2 translate;
			glm::mat2 rotscale;
		};
	public:
		Container();
		void Update(const Transformation& t);
		virtual void Parse(uint8_t *&iter) = 0;
		inline int GetCurrentFrame() { return m_currentFrame; }
	protected:
		uint32_t m_framecount;
		std::vector<Frame> m_frames;
	private:
		void HandleAction(std::shared_ptr<FrameItem> fi);
		void HandleBackground(std::shared_ptr<FrameItem> fi);
		void HandlePlaceObject(std::shared_ptr<FrameItem> fi);
		void HandleRemoveObject(std::shared_ptr<FrameItem> fi);
		void HandleInitAction(std::shared_ptr<FrameItem> fi);
    private:
        std::map<uint32_t, DisplayObject> m_displayObjects;
		uint32_t m_currentFrame;
		glm::vec4 m_bgColor;
		ScriptContext m_sc;
    };
}