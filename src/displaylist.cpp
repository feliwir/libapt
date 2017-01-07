#include "displaylist.hpp"
#include <iostream>
using namespace libapt;

void DisplayList::Insert(uint32_t depth, std::shared_ptr<Character> ch,
	const glm::vec2 & translate, const glm::mat2 & rotscale, const std::string& name,
	std::shared_ptr<Container> parent)
{
	DisplayObject obj;
	obj.Create(ch,translate,rotscale,name,parent);
	m_objects[depth] = obj;
	#ifndef  NDEBUG
	std::cout << "Placed object at depth: " << depth << std::endl;
	std::cout << "Name: " << name << std::endl;
	#endif // ! NDEBUG
}

void DisplayList::AddClipLayer(uint32_t depth, uint32_t clipdepth, std::shared_ptr<Character> ch, 
	const glm::vec2 & translate, const glm::mat2 & rotscale, const 
	std::string & name, std::shared_ptr<Container> parent)
{
	DisplayObject obj;
	obj.CreateClipLayer(ch, translate, rotscale, name, parent,clipdepth);
	m_objects[depth] = obj;
#ifndef  NDEBUG
	std::cout << "Placed clipping layer at depth: " << depth << " with clipdepth: "<< clipdepth << std::endl;
	std::cout << "Name: " << name << std::endl;
#endif // ! NDEBUG
}

void DisplayList::Erase(uint32_t depth)
{
	if (m_objects.find(depth) == m_objects.end())
	{
		std::cout << "Error: cannot remove at depth: " << depth << std::endl;
		return;
	}

	m_objects.erase(depth);
	#ifndef  NDEBUG
	std::cout << "Removed object at depth: " << depth << std::endl;
	#endif // ! NDEBUG
}

void DisplayList::Move(uint32_t depth, const glm::vec2 & translate, const glm::mat2 & rotscale)
{
	auto& obj = m_objects[depth];
	obj.SetRotscale(rotscale);
	obj.SetTranslate(translate);
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
	obj.SetColor(color);
#ifndef NDEBUG
	std::cout << "Colortransform at depth: " << depth <<
		" Color: (" << color.r << "-" << color.g << "-" << color.b << "-" << color.a<< ")" << std::endl;
#endif // !NDEBUG
}

void DisplayList::Render(const Transformation & t)
{
	std::shared_ptr<ClipMask> mask;
	uint32_t clipdepth = 0;
	Transformation ct = t;

	for (auto& pair : m_objects)
	{
		auto& dispO = pair.second;
		dispO.Render(ct);

		if (dispO.IsClippingLayer())
		{
			ct.mask = dispO.GetClippingLayer();
			clipdepth = dispO.GetClipDepth();
		}
		else if(pair.first > clipdepth)
		{
			ct.mask = nullptr;
			clipdepth = 0;
		}
	}
}
