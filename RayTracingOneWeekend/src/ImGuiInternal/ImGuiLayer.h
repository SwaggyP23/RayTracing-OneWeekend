#pragma once

#include <stdint.h>

struct GLFWwindow;

namespace rtow {

	class ImGuiLayer
	{
	public:
		ImGuiLayer(uint32_t width, uint32_t height) noexcept;
		
		static void Init(GLFWwindow* window);
		static void Shutdown();
		static void SetDarkThemeColors();

		void Begin();
		void End();

	private:
		uint32_t m_Width{}, m_Height{};

	};

}