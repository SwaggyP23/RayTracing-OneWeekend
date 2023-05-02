#include "Logger.h"
#include "Color.h"
#include "ImGuiInternal/ImGuiLayer.h"

#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>
#include <imgui/imgui.h>
//#include <stb_image_writer/stb_image_write.h>

// TODO: For now i will follow whatever that thing does then i want to switch to opengl and imgui raytracing!

namespace rtow {

	int Main(int argc, char** argv)
	{
		// Image Dimensions...
		constexpr float ASPECT_RATIO = 16.0f / 9.0f;
		constexpr uint32_t WIDTH = 400;
		constexpr uint32_t HEIGHT = (uint32_t)(WIDTH / ASPECT_RATIO);

		// Seting up Camera...
		constexpr float ViewportHeight = 2.0f;
		constexpr float ViewportWidth = ViewportHeight * ASPECT_RATIO;
		constexpr float FocalLength = 1.0f; // Distance from eye to projection plane..

		constexpr glm::vec3 Origin = { 0.0f, 0.0f, 0.0f };
		constexpr glm::vec3 Horizantal = { ViewportWidth, 0.0f ,0.0f };
		constexpr glm::vec3 Vertical = { 0.0f, ViewportHeight, 0.0f };
		constexpr glm::vec3 LowLeftCorner = Origin - Horizantal * 0.5f - Vertical * 0.5f - glm::vec3(0.0f, 0.0f, FocalLength);

		// Rendering...
		std::cout << "P3\n" << WIDTH << ' ' << HEIGHT << "\n255\n";

		for (int i = HEIGHT - 1; i >= 0; i--)
		{
			std::cerr << "\rScanlines remaining: " << i << ' ' << std::flush;
			for (int j = 0; j < WIDTH; j++)
			{
				float u = (float)j / (WIDTH - 1);
				float v = (float)i / (HEIGHT - 1);

				Ray r(Origin, LowLeftCorner + u * Horizantal + v * Vertical - Origin);
				glm::vec3 pixelColor = RayColor(r);

				WriteColor(std::cout, pixelColor);
			}
		}
		
		std::cerr << "\nDONE!\n";

		return 0;
	}

}

//int main(int argc, char** argv)
//{
//	return rtow::Main(argc, argv);
//}

int main(int, char**)
{
	constexpr uint32_t WIDTH = 1280;
	constexpr uint32_t HEIGHT = 960;

	if (!glfwInit())
		RTOW_LOG_ERROR("Something did not work in glfwinit()!");

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "RayTracingOneWeekend", NULL, NULL);
	if (!window)
		RTOW_LOG_ERROR("Failed to create a window!");

	glfwSetWindowPos(window, ((mode->width - 1280) / 2), (mode->height - 960) / 2);

	glfwMakeContextCurrent(window);

	int gladSuccess = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	if (!gladSuccess)
		RTOW_LOG_ERROR("Could not load Glad!");

	glfwSetWindowUserPointer(window, nullptr); // TODO: For now it is null!
	glfwSwapInterval(true); // Setting V-Sync on!

	rtow::ImGuiLayer imguiLayer(WIDTH, HEIGHT);
	rtow::ImGuiLayer::Init(window);

	while (!glfwWindowShouldClose(window))
	{
		//glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		//glClear(GL_COLOR_BUFFER_BIT);
		glfwPollEvents();
		
		imguiLayer.Begin();

		ImGui::Begin("Wassup!");
		ImGui::Button("IOBEOIBOE");
		ImGui::TextColored({ 1, 0, 1, 1 }, "Ayre iza btimshe");
		ImGui::End();

		imguiLayer.End();

		glfwSwapBuffers(window);
	}

	rtow::ImGuiLayer::Shutdown();
	glfwDestroyWindow(window);
	glfwTerminate();

}