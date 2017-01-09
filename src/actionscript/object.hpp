#pragma once
#include "value.hpp"
#include <string>
#include <map>
#include <vector>

namespace libapt
{
    namespace as
    {
        //An object in Actionscript
		//PROPERTIES:
		//bool	Initialized
		//Func	InitFunc
        class Object : public std::enable_shared_from_this<Object>
        {
		public:
			enum PlayState
			{
				PLAYING = 0,
				STOPPED = 1
			};
		public:
			Value GetProperty(const std::string& property);
			void SetProperty(const std::string& property,Value v);

			Value GetVariable(const std::string& name);
			void SetVariable(const std::string& name, Value v);

			void SetPlaystate(const PlayState ps);
			void SetCurrentFrame(const uint32_t cf);
			void NextFrame();
		protected:
			virtual void OnPropertyChanged(const std::string& property) = 0;
			virtual void OnPlayStateChanged() = 0;
			virtual void OnFrameChanged() = 0;
		protected:
			std::map<std::string, Value> m_properties;
			std::vector<Value> m_constants;
			std::map<std::string, Value> m_variables;
			PlayState m_ps;
			uint32_t m_cf;
        };
    }
}