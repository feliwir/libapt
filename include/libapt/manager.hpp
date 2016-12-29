#pragma once
#include "apt.hpp"
#include "error.hpp"
#include "fileprovider.hpp"
#include "rendertarget.hpp"
#include <memory>
#include <string>
#include <map>
#include <chrono>
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
		void SetDimension(uint32_t width, uint32_t height);
		void SetFps(const uint32_t fps);
		void Update();
		void Render(const bool window = true);

		inline std::shared_ptr<IFileProvider> GetFileprovider() { return m_fileprovider; }

		std::shared_ptr<Character> ImportCharacter(const std::string& movie, const std::string& name);

		std::shared_ptr<Apt> GetApt(const std::string& name);
	private:
		std::map<std::string, std::shared_ptr<Apt>> m_apts;
		std::map<uint32_t, Texture> m_textures;
		std::shared_ptr<IFileProvider> m_fileprovider;
		std::map<uint32_t,Texture> m_textureList;
		std::shared_ptr<Apt> m_active;
		std::unique_ptr<RenderTarget> m_target;
		std::chrono::system_clock::time_point m_last;
		uint32_t m_fps;
	};
}