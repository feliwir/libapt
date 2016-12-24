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
        class Object
        {
		public:
			Value GetProperty(const std::string& property);
			void SetProperty(const std::string& property,Value v);
		protected:
			virtual void OnPropertyChanged(const std::string& property, Value v) = 0;
		protected:
			std::map<std::string, Value> m_properties;
        };
    }
}