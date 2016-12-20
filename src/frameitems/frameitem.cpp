#include "frameitem.hpp"
#include "action.hpp"
#include "background.hpp"
#include "framelabel.hpp"
#include "initaction.hpp"
#include "placeobject.hpp"
#include "removeobject.hpp"
#include "../util.hpp"
#include <iostream>
using namespace libapt;

std::shared_ptr<FrameItem> FrameItem::Create(uint8_t *& iter, const uint8_t * base)
{
	Type type = static_cast<Type>(read<uint32_t>(iter));
	std::shared_ptr<FrameItem> fi = nullptr;

	switch (type)
	{
	case ACTION:
		fi = std::make_shared<Action>();
		break;
	case FRAMELABEL:
		fi = std::make_shared<Framelabel>();
		break;
	case PLACEOBJECT:
		fi = std::make_shared<PlaceObject>();
		break;
	case REMOVEOBJECT:
		fi = std::make_shared<RemoveObject>();
		break;
	case BACKGROUNDCOLOR:
		fi = std::make_shared<Background>();
		break;
	case INITACTION:
		fi = std::make_shared<InitAction>();
		break;
	default:
		std::cout << "Unimplemented frameitem: " << type << std::endl;
	}

	if (fi)
	{
		fi->SetType(type);
		fi->Parse(iter, base);
	}

	return fi;
}