#include "displaylist.hpp"
#include <libapt/manager.hpp>
#include <iostream>
using namespace libapt;

void DisplayList::Insert(uint32_t depth, std::shared_ptr<Character> ch,
	const glm::vec2 & translate, const glm::mat2 & rotscale, const std::string& name,
	std::shared_ptr<DisplayObject> parent)
{
	std::shared_ptr<DisplayObject> obj = std::make_shared<DisplayObject>();
	obj->Create(ch,translate,rotscale,name,parent);

	as::Value v;
	//add to parent
	if(parent!=nullptr && name.size()>0)
	{
		v.FromObject(obj);
		parent->SetProperty(name,v);
	}
	m_objects[depth] = obj;

	#ifndef  NDEBUG
	std::cout << "Placed object at depth: " << depth << std::endl;
	std::cout << "Name: " << name << std::endl;
	#endif // ! NDEBUG
}

void DisplayList::AddClipLayer(uint32_t depth, uint32_t clipdepth, std::shared_ptr<Character> ch, 
	const glm::vec2 & translate, const glm::mat2 & rotscale, const 
	std::string & name, std::shared_ptr<DisplayObject> parent)
{
	std::shared_ptr<DisplayObject> obj = std::make_shared<DisplayObject>();
	obj->CreateClipLayer(ch, translate, rotscale, name, parent,clipdepth);
	m_objects[depth] = obj;
#ifndef  NDEBUG
	std::cout << "Placed clipping layer at depth: " << depth << " with clipdepth: "<< clipdepth << std::endl;
	std::cout << "Name: " << name << std::endl;
#endif // ! NDEBUG
}

void DisplayList::Erase(uint32_t depth)
{
	auto it = m_objects.find(depth);
	if (m_objects.find(depth) == m_objects.end())
	{
		std::cout << "Error: cannot remove at depth: " << depth << std::endl;
		return;
	}

	m_objects.erase(it);
	#ifndef  NDEBUG
	std::cout << "Removed object at depth: " << depth << std::endl;
	#endif // ! NDEBUG
}

void DisplayList::Move(uint32_t depth, const glm::vec2 & translate, const glm::mat2 & rotscale)
{
	auto& obj = m_objects[depth];
	obj->SetRotscale(rotscale);
	obj->SetTranslate(translate);
	#ifndef NDEBUG
	std::cout << "Moved object at depth: " << depth <<
		" Translate: (" << translate.x << "-" << translate.y << ")" << std::endl;
	#endif // !NDEBUG
}

void DisplayList::Colortransform(uint32_t depth, glm::u8vec4 color)
{
	if (m_objects.find(depth) == m_objects.end())
	{
		std::cout << "Error: cannot colortransform at depth: " << depth << std::endl;
		return;
	}	

	auto& obj = m_objects[depth];
	obj->SetColor(color);
#ifndef NDEBUG
	std::cout << "Colortransform at depth: " << depth <<
		" Color: (" << color.r << "-" << color.g << "-" << color.b << "-" << color.a<< ")" << std::endl;
#endif // !NDEBUG
}

void DisplayList::Render(const Transformation& t)
{
	std::shared_ptr<ClipMask> mask;
	uint32_t clipdepth = 0;
	Transformation ct = t;

	for (auto& pair : m_objects)
	{
		auto& object = pair.second;
		object->Render(ct);

		if (pair.first > clipdepth && mask!=nullptr)
		{
			ct.mask = nullptr;
			clipdepth = 0;

		}
		if (object->IsClippingLayer())
		{
			ct.mask = object->GetClippingLayer();
			clipdepth = object->GetClipDepth();
		}
		
	}
}
