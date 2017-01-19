#include <libapt/apt.hpp>
#include <libapt/manager.hpp>
#include "displayobject.hpp"
#include "characters/character.hpp"
#include "characters/movie.hpp"
#include "characters/image.hpp"
#include "characters/shape.hpp"
#include "graphics/buffer.hpp"
#include "util.hpp"
#include "geometry.hpp"
#include <iostream>
using namespace libapt;

Apt::Apt()
{
	m_geomBuf = std::make_shared<Buffer>();
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
	m_manager = mngr;

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

	auto ch = Character::Create(iter ,shared_from_this());
	if (ch->GetType() != Character::MOVIE)
	{
		std::cout << "An apt MUST contain a movie as first character!" << std::endl;
		return MISSING_MOVIE;
	}
	auto m = std::dynamic_pointer_cast<Movie>(ch);

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
			int imgId = im->GetImage();
			Dat::Entry e = m_dat.GetEntry(imgId);
			

			int texId = 0;
			if (e.type == Dat::NO_BOUNDS)
			{
				texId = e.p1;
			}
			else
			{
				texId = imgId;
			}

			m_imageMap[imgId] = texId;

			if (m_textures[texId] != nullptr)
				continue;

			std::string texPath = "art/Textures/apt_" + name + "_" + std::to_string(texId) + ".tga";
			auto tex = std::make_shared<Texture>();
			uint32_t fSize = 0;
			const uint8_t* buffer = mngr->GetFileprovider()->LoadBinary(texPath,fSize);
			tex->Load(buffer, fSize);
			delete[] buffer;		
			m_textures[texId] = tex;
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
	//set character of our display object
	m_movieclip = std::make_shared<DisplayObject>();
	m_movieclip->SetCharacter(ch);
	m_movieclip->SetName("_root");

	//compile all geometries
	for (const auto& g : m_geometries)
	{
		g->Compile(shared_from_this());
	}
	m_geomBuf->Finalize();
	return NO_ERROR;
}

std::shared_ptr<Character> Apt::GetExport(const std::string& name)
{
	auto m = std::dynamic_pointer_cast<Movie>(m_movieclip->GetCharacter());
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
	m_movieclip->SetFocus(nullptr);
	m_movieclip->Render(Transformation());
}

std::vector<std::shared_ptr<Character>> Apt::GetCharacters()
{
	auto m = std::dynamic_pointer_cast<Movie>(m_movieclip->GetCharacter());
	return m->GetCharacters();
}

Apt::~Apt()
{
	delete[] m_data;
}

std::shared_ptr<Texture> Apt::GetTexture(int id)
{
	int texId = m_imageMap[id];
	return m_textures[texId];
}

uint32_t Apt::GetWidth()
{
	auto m = std::dynamic_pointer_cast<Movie>(m_movieclip->GetCharacter());
	return m->GetWidth();
}

uint32_t Apt::GetHeight()
{
	auto m = std::dynamic_pointer_cast<Movie>(m_movieclip->GetCharacter());
	return m->GetHeight();
}

Const::Entry Apt::GetConstant(uint32_t index)
{
	return m_const.GetItem(index);
}

std::shared_ptr<Character> Apt::GetCharacter(uint32_t id)
{
	auto m = std::dynamic_pointer_cast<Movie>(m_movieclip->GetCharacter());
	return m->GetCharacters()[id];
}

const bool Apt::HasResized()
{
	return m_manager->HasResized();
}
