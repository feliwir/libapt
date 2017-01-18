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
			Object();

			virtual Value GetProperty(const std::string& property);
			virtual void SetProperty(const std::string& property,Value v);
			std::map<std::string, Value>& GetProperties();

			virtual Value GetVariable(const std::string& name);
			virtual void SetVariable(const std::string& name, Value v);
			std::map<std::string, Value>& GetVariables();

			inline std::vector<Value>& GetConstants()
			{
				return m_constants;
			}

			inline PlayState GetPlaystate()
			{
				return m_ps;
			}
			void SetPlaystate(const PlayState ps);
			void SetCurrentFrame(const uint32_t cf);
			void NextFrame();
		protected:
			virtual void OnPropertyChanged(const std::string& property);
			virtual void OnPlayStateChanged();
		protected:
			std::map<std::string, Value> m_properties;
			std::map<std::string, Value> m_variables;
			std::vector<Value> m_constants;
			PlayState m_ps;
			uint32_t m_cf;
        };
    }
}