#pragma once
#include "const.hpp"
#include "dat.hpp"
#include "error.hpp"
#include "fileprovider.hpp"
#include <memory>
#include <string>
#include <vector>
#include <map>

namespace libapt
{
	class Manager;
	class Character;
	class DisplayObject;
	class Geometry;

	class Apt : public std::enable_shared_from_this<Apt>
	{
	public:
		Apt();
		~Apt();
		Error Load(const uint8_t* data, unsigned int size, std::shared_ptr<Manager> mngr, const std::string& name);
		void Render();

		std::shared_ptr<Character> GetExport(const std::string& name);

		std::vector<std::shared_ptr<Character>> GetCharacters();

		std::shared_ptr<Character> GetCharacter(uint32_t id);

		std::shared_ptr<Texture> GetTexture(int id);

		uint32_t GetWidth();

		uint32_t GetHeight();

		Const::Entry GetConstant(uint32_t index);

		inline const uint8_t* GetBase() 
		{ 
			return m_data; 
		}

		inline std::shared_ptr<Manager> GetManager()
		{
			return m_manager;
		}

		const bool HasResized();
	private:
		Error LoadConst(const std::string name, std::shared_ptr<IFileProvider> fp);
		Error LoadDat(const std::string name, std::shared_ptr<IFileProvider> fp);
	private:
		std::shared_ptr<DisplayObject> m_movieclip;
		std::shared_ptr<Manager> m_manager;
		std::map<int, std::shared_ptr<Texture>> m_textures;
		std::map<int, int> m_imageMap;
		std::vector<std::shared_ptr<Geometry>> m_geometries;
		Const m_const;
		Dat m_dat;
		bool m_renderable;
		const uint8_t* m_data;
	};
}