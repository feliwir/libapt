#pragma once
#include "apt.hpp"
#include "error.hpp"
#include "fileprovider.hpp"
#include "rendertarget.hpp"
#include <memory>
#include <string>
#include <map>
#include <stdint.h>

namespace libapt
{
	class Manager : public std::enable_shared_from_this<Manager>
	{
	public:
		Manager();
		Manager(std::shared_ptr<IFileProvider> fp);
		Error AddApt(const std::string& name);
		Error SetActive(const std::string& name);
		uint32_t GetFramebuffer();
		void UseFramebuffer(bool use);

		void Update();
		void Render();

		inline std::shared_ptr<IFileProvider> GetFileprovider() { return m_fileprovider; }

		std::shared_ptr<Character> ImportCharacter(const std::string& movie, const std::string& name);

		std::shared_ptr<Apt> GetApt(const std::string& name);

		void SetFps(uint32_t fps);
	private:
		std::map<std::string, std::shared_ptr<Apt>> m_apts;
		std::map<uint32_t, Texture> m_textures;
		std::shared_ptr<IFileProvider> m_fileprovider;
		std::map<uint32_t,Texture> m_textureList;
		std::shared_ptr<Apt> m_active;
		std::unique_ptr<RenderTarget> m_target;
		bool m_useFb;
		uint32_t m_fps;
	};
}