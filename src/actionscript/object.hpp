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
			inline uint32_t GetCurrentFrame()
			{
				return m_cf;
			}

			void NextFrame();
		protected:
			virtual void OnPropertyChanged(const std::string& property);
		protected:
			std::map<std::string, Value> m_properties;
			std::vector<Value> m_constants;
			PlayState m_ps;
			uint32_t m_cf;
        };
    }
}