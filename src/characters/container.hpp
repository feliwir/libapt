#pragma once
#include "character.hpp"
#include "../frame.hpp"
#include "../frameitems/action.hpp"
#include "../displaylist.hpp"
#include "../actionscript/engine.hpp"
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
	public:
		Container();
		void Update(const Transformation& trans, std::shared_ptr<DisplayObject> instance) override;
		virtual void Parse(uint8_t *&iter) = 0;

		inline DisplayList& GetDisplaylist()
		{
			return m_dl;
		}

		inline void SetPlaying(const bool state)
		{
			m_playing = state;
		}

	protected:
		uint32_t m_framecount;
		std::vector<Frame> m_frames;
	private:
		void HandleAction(std::shared_ptr<FrameItem> fi);
		void HandleFramelabel(std::shared_ptr<FrameItem> fi, std::shared_ptr<DisplayObject> instance);
		void HandleBackground(std::shared_ptr<FrameItem> fi);
		void HandlePlaceObject(std::shared_ptr<FrameItem> fi, std::shared_ptr<DisplayObject> instance);
		void HandleRemoveObject(std::shared_ptr<FrameItem> fi);
		void HandleInitAction(std::shared_ptr<FrameItem> fi, std::shared_ptr<DisplayObject> instance);
    private:
		DisplayList m_dl;
		glm::vec4 m_bgColor;
		bool m_playing;
		//performed before displaying each frame
		std::vector<std::shared_ptr<Action>> m_actionList;
    };
}