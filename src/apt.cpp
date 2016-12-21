#include <libapt/apt.hpp>
#include <libapt/manager.hpp>
#include "characters/character.hpp"
#include "characters/movie.hpp"
#include "characters/image.hpp"
#include "characters/shape.hpp"
#include "util.hpp"
#include "geometry.hpp"

#include <iostream>
using namespace libapt;

Apt::Apt() 
{
}

Error Apt::LoadConst(const std::string name, std::shared_ptr<IFileProvider> fp)
{
	unsigned int size = 0;
	const uint8_t* buffer = fp->LoadBinary(name + ".const", size);
	if (buffer == nullptr)
		return CONST_NF;

	Error err  = m_const.Load(buffer, size);
	delete[] buffer;
	return err;
}

Error Apt::LoadDat(const std::string name, std::shared_ptr<IFileProvider> fp)
{
	unsigned int size = 0;
	bool fail = false;
	const std::string text = fp->LoadText(name + ".dat",fail);
	if (fail)
		return CONST_NF;

	Error err = m_dat.Load(text);
	return err;
}

Error Apt::Load(const uint8_t *buffer, unsigned int size, std::shared_ptr<Manager> mngr, const std::string &name)
{
	m_data = buffer;

	//get the const file
	Error err = LoadConst(name,mngr->GetFileprovider());
	if (err != NO_ERROR)
		return err;

	uint8_t *iter = const_cast<uint8_t *>(buffer);
	//Read the first 8 bytes
	std::string magic = readString(iter, 8);
	//Check if this is a valid Apt file
	if (magic != "Apt Data")
		return INVALID_APT;

	//Go back to start
	iter -= 0x08;
	iter += m_const.GetAptOffset();

	m_movie = Character::Create(iter ,shared_from_this());
	if (m_movie->GetType() != Character::MOVIE)
	{
		std::cout << "An apt MUST contain a movie as first character!" << std::endl;
		return MISSING_MOVIE;
	}
	auto m = std::dynamic_pointer_cast<Movie>(m_movie);

	uint32_t width = m->GetWidth();
	uint32_t height = m->GetHeight();
	
	//Import characters
	for (const auto& im : m->GetImports())
	{
		auto& movieName = im.GetMovie();
		auto ch = mngr->ImportCharacter(movieName, im.GetName());
		m->SetCharacter(ch,im.GetCharacter());
	}

	//Load data file
	LoadDat(name, mngr->GetFileprovider());

	//Check all image and shape characters
	int id = 0;
	for (const auto& ch : m->GetCharacters())
	{
		if (ch == nullptr)
			continue;

		if (ch->GetType() == Character::IMAGE)
		{
			auto im = std::dynamic_pointer_cast<Image>(ch);
			int texId = im->GetTexture();
			Dat::Entry e = m_dat.GetEntry(im->GetTexture());
			if (m_textures[texId] != nullptr)
				continue;

			int imgId = 0;
			if (e.type == Dat::NO_BOUNDS)
			{
				imgId = e.p1;
			}
			else
			{
				imgId = texId;
			}
			std::string texPath = "art/Textures/apt_" + name + "_" + std::to_string(imgId) + ".tga";
			m_textures[im->GetTexture()]=mngr->GetFileprovider()->LoadTexture(texPath);
		}
		else if (ch->GetType() == Character::SHAPE)
		{
			auto sh = std::dynamic_pointer_cast<Shape>(ch);
			int geoId = sh->GetGeometryId();
			auto geom = std::make_shared<Geometry>(width,height);
			bool fail = false;
			err = NO_ERROR;
			std::string geomPath = name + "_geometry/" + std::to_string(geoId) + ".ru";
			std::string data = mngr->GetFileprovider()->LoadText(geomPath,fail);
			if (!fail)
			{
				err = geom->Load(data);
			}			
			else
			{
				std::cout << "Unable to load geometry file: " << geomPath << std::endl;
			}
				
			if (err != NO_ERROR)
			{
				return err;
			}
			else
			{			
				sh->SetGeometry(geom);
			}
			m_geometries.push_back(geom);
		}
	}

	//compile all geometries
	for (const auto& g : m_geometries)
	{
		g->Compile(shared_from_this());
	}
	return NO_ERROR;
}

std::shared_ptr<Character> Apt::GetExport(const std::string& name)
{
	auto m = std::dynamic_pointer_cast<Movie>(m_movie);
	const auto& exports = m->GetExports();
	for (const auto& ex : exports)
	{
		if (ex.GetName() == name)
		{
			return m->GetCharacter(ex.GetCharacter());
		}
	}

	return nullptr;
}

void Apt::Render()
{
	auto m = std::dynamic_pointer_cast<Movie>(m_movie);
	m_movie->Update(Transformation());
}

std::vector<std::shared_ptr<Character>> Apt::GetCharacters()
{
	auto m = std::dynamic_pointer_cast<Movie>(m_movie);
	return m->GetCharacters();
}


Apt::~Apt()
{
	delete[] m_data;
}

std::shared_ptr<Texture> Apt::GetTexture(int id)
{
	return m_textures[id];
}

uint32_t Apt::GetWidth()
{
	auto m = std::dynamic_pointer_cast<Movie>(m_movie);
	return m->GetWidth();
}

uint32_t Apt::GetHeight()
{
	auto m = std::dynamic_pointer_cast<Movie>(m_movie);
	return m->GetHeight();
}

Value Apt::GetConstant(uint32_t index)
{
	return m_const.GetItem(index);
}

std::shared_ptr<Character> Apt::GetCharacter(uint32_t id)
{
	auto m = std::dynamic_pointer_cast<Movie>(m_movie);
	return m->GetCharacters()[id];
}