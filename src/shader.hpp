#pragma
#include <string>
#include <map>
#include <libapt/error.hpp>

namespace libapt
{
    class Shader
    {
	public:
		~Shader();
		Shader();
		Error Load(const std::string& vert, const std::string& frag);
		void Use();
		bool Valid();

		void addUniform(const std::string& uniform);
		void addAttribute(const std::string& attribute);

		int32_t uniform(const std::string& uniform);
		int32_t attribute(const std::string& attribute);
	private:
		void Cleanup();
	private:
		uint32_t m_vert;
		uint32_t m_frag;
		uint32_t m_program;

		std::map<std::string, int32_t> m_attributeList;
		std::map<std::string, int32_t> m_uniformLocationList;
    };
}