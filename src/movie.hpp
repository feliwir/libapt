#pragma once
#include "export.hpp"
#include "import.hpp"
#include "container.hpp"
#include <vector>

namespace libapt
{
	class Movie : public Container
	{
	public:
		virtual void Parse(uint8_t*& iter);

		inline uint32_t GetWidth() { return m_screenwidth; }
		inline uint32_t GetHeight() { return m_screenheight; }
		inline const std::vector<Import>& GetImports() { return m_imports; }
		inline const std::vector<Export>& GetExports() { return m_exports; }
		void SetCharacter(const std::shared_ptr<Character> ch, uint32_t index);

		inline const std::shared_ptr<Character> GetCharacter(uint32_t index) const
		{
			return m_dictionary[index];
		}
		inline const std::vector<std::shared_ptr<Character>>& GetCharacters()
		{
			return m_dictionary;
		}
	private:
		uint32_t m_unknown;
		uint32_t m_characterCount;
		std::vector<std::shared_ptr<Character>> m_dictionary;
		uint32_t m_screenwidth;
		uint32_t m_screenheight;
		uint32_t m_unknown2;
		uint32_t m_importcount;
		std::vector<Import> m_imports;
		uint32_t m_exportcount;
		std::vector<Export> m_exports;
	};
}