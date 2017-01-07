#include "character.hpp"
#include <libapt/apt.hpp>
#include "image.hpp"
#include "movie.hpp"
#include "shape.hpp"
#include "sprite.hpp"
#include "edittext.hpp"
#include "font.hpp"
#include "button.hpp"
#include "../util.hpp"
#include "../displayobject.hpp"
#include <iostream>
using namespace libapt;

std::shared_ptr<Character> Character::Create(uint8_t*& iter,
	std::shared_ptr<Apt> owner)
{
	const uint8_t* thisCharacter = iter;

	Type type = static_cast<Type>(read<uint32_t>(iter));
	uint32_t signature = read<uint32_t>(iter);
	if (signature != Character::SIGNATURE)
	{
		std::cout << "Character with wrong signature!" << std::endl;
		return nullptr;
	}


	std::shared_ptr<Character> ch = nullptr;

	switch (type)
	{
	case Character::SHAPE:
		ch = std::make_shared<Shape>();
		break;
	case Character::EDITTEXT:
		ch = std::make_shared<Edittext>();
		break;
	case Character::FONT:
		ch = std::make_shared<Font>();
		break;
	case Character::BUTTON:
		ch = std::make_shared<Button>();
		break;
	case Character::SPRITE:
		ch = std::make_shared<Sprite>();
		break;
	case Character::IMAGE:
		ch = std::make_shared<Image>();
		break;
	case Character::MOVIE:
		ch = std::make_shared<Movie>();
		break;
	default:
		std::cout << "Unimplemented character: " << type << std::endl;
	}

	if (ch)
	{
		ch->SetOwner(owner);
		ch->SetType(type);
		ch->SetOffset(thisCharacter - owner->GetBase());
		ch->Parse(iter);
	}
	return ch;
}

std::shared_ptr<Character> Character::MakeInstance()
{
	std::shared_ptr<Character> result = nullptr;
	auto ch = shared_from_this();
	switch (ch->GetType())
	{
	case Character::SHAPE:
		result = Copy<Shape>();
		break;
	case Character::EDITTEXT:
		result = Copy<Edittext>();
		break;
	case Character::BUTTON:
		result = Copy<Button>();
		break;
	case Character::SPRITE:
		result = Copy<Sprite>();
		break;
	case Character::MOVIE:
		result = Copy<Movie>();
		break;
	}

	return result;
}
