#pragma once
#include "transformation.hpp"
#include <stdint.h>
#include <memory>
#include <vector>
#include <glm/glm.hpp>

namespace libapt
{
	class Apt;
	class DisplayObject;

	class Character : public std::enable_shared_from_this<Character>
	{
	private:
		static const uint32_t SIGNATURE = 0x09876543;
	public:
		enum Type
		{
			SHAPE		= 1,
			TEXT		= 2,
			FONT		= 3,
			BUTTON		= 4,
			SPRITE		= 5,
			SOUND		= 6,
			IMAGE		= 7,
			MORPH		= 8,
			MOVIE		= 9,
			STATICTEXT	= 10,
			NONE		= 11,
			VIDEO		= 12
		};

		virtual void Parse(uint8_t *&iter) = 0;
		//Update all placed objects 
		virtual void Update(const Transformation& trans, std::shared_ptr<DisplayObject> instance) = 0;
		
		virtual void Prepare(std::shared_ptr<DisplayObject> instance);

		inline void SetOffset(const uint32_t offset)
		{
			m_thisOffset = offset;
		}

		inline Type GetType() { return m_type; }

		inline void SetType(Type t) { m_type = t; }

		inline void SetOwner(std::shared_ptr<Apt> owner) { m_owner = owner; }

		inline std::shared_ptr<Apt> GetOwner()
		{
			return m_owner;
		}

		static std::shared_ptr<Character> Create(uint8_t*& iter, std::shared_ptr<Apt> owner);

		std::shared_ptr<Character> MakeInstance();
		template<class T>
		std::shared_ptr<Character> Copy()
		{
			return std::make_shared<T>(*std::dynamic_pointer_cast<T>(shared_from_this()));
		}
	protected:
		Type m_type;
		uint32_t m_thisOffset;
		std::shared_ptr<Apt> m_owner;
	};
}