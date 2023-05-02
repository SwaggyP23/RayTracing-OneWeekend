#include "ImGuiLayer.h"

#include <glfw/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

namespace rtow {

	namespace Theme {

		constexpr uint32_t Accent = IM_COL32(236, 158, 36, 150);
		constexpr uint32_t AccentDimmed = IM_COL32(236, 158, 36, 36);
		constexpr uint32_t AccentHighlighted = IM_COL32(236, 158, 36, 255);
		constexpr uint32_t Background = IM_COL32(36, 36, 36, 255);
		constexpr uint32_t BackgroundMedium = IM_COL32(20, 20, 20, 255);
		constexpr uint32_t BackgroundDark = IM_COL32(10, 10, 10, 255);
		constexpr uint32_t BackgroundPopup = IM_COL32(15, 15, 15, 255);
		constexpr uint32_t Compliment = IM_COL32(78, 131, 146, 225);
		constexpr uint32_t GroupHeader = IM_COL32(55, 55, 55, 255);
		constexpr uint32_t Highlight = IM_COL32(39, 185, 242, 255);
		constexpr uint32_t Muted = IM_COL32(77, 77, 77, 255);
		constexpr uint32_t NiceBlue = IM_COL32(83, 232, 254, 255);
		constexpr uint32_t PropertyField = IM_COL32(15, 15, 15, 255);
		constexpr uint32_t Selection = IM_COL32(237, 192, 119, 255);
		constexpr uint32_t SelectionMuted = IM_COL32(237, 201, 142, 23);
		constexpr uint32_t Titlebar = IM_COL32(21, 21, 21, 255);
		constexpr uint32_t Text = IM_COL32(192, 192, 192, 255);
		constexpr uint32_t TextBrighter = IM_COL32(240, 240, 240, 255);
		constexpr uint32_t TextDarker = IM_COL32(128, 128, 128, 255);
		constexpr uint32_t TextError = IM_COL32(230, 51, 51, 255);

	}

	void ImGuiLayer::Init(GLFWwindow* window)
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		ImGui::StyleColorsDark();

		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 6.0f;
			style.PopupRounding = 3.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		SetDarkThemeColors();

		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 410");
	}

	ImGuiLayer::ImGuiLayer(uint32_t width, uint32_t height) noexcept
		: m_Width(width), m_Height(height)
	{
	}

	void ImGuiLayer::Shutdown()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::Begin()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// Enabling Docking By default
		static bool dockSpaceOpen = true;
		static bool opt_fullscreen = true;
		static bool opt_padding = false;
		constexpr static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
		// and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0.0f, 0.0f });
		ImGui::Begin("DockSpace Demo", &dockSpaceOpen, window_flags);
		ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// Submit the DockSpace
		ImGuiIO& io = ImGui::GetIO();
		ImGuiStyle& style = ImGui::GetStyle();
		float minWindowSizeX = style.WindowMinSize.x;
		style.WindowMinSize.x = 370.0f; // Minimum Window Size!
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}
		style.WindowMinSize.x = minWindowSizeX;
	}

	void ImGuiLayer::End()
	{
		ImGui::End(); // Docking window...

		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2(1280, 960);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}

	void ImGuiLayer::SetDarkThemeColors()
	{
		ImGuiStyle& style = ImGui::GetStyle();
		auto& colors = ImGui::GetStyle().Colors;

		// Headers
		colors[ImGuiCol_Header] = ImGui::ColorConvertU32ToFloat4(Theme::GroupHeader);
		colors[ImGuiCol_HeaderHovered] = ImGui::ColorConvertU32ToFloat4(Theme::GroupHeader);
		colors[ImGuiCol_HeaderActive] = ImGui::ColorConvertU32ToFloat4(Theme::GroupHeader);

		// Buttons
		colors[ImGuiCol_Button] = ImColor(56, 56, 56, 200);
		colors[ImGuiCol_ButtonHovered] = ImColor(70, 70, 70, 255);
		colors[ImGuiCol_ButtonActive] = ImColor(56, 56, 56, 150);

		// Frame BG
		colors[ImGuiCol_FrameBg] = ImGui::ColorConvertU32ToFloat4(Theme::PropertyField);
		colors[ImGuiCol_FrameBgHovered] = ImGui::ColorConvertU32ToFloat4(Theme::PropertyField);
		colors[ImGuiCol_FrameBgActive] = ImGui::ColorConvertU32ToFloat4(Theme::PropertyField);

		// Scroll Bar
		colors[ImGuiCol_ScrollbarBg] = ImVec4{ 0.02f, 0.02f, 0.02f, 0.53f };
		colors[ImGuiCol_ScrollbarGrab] = ImVec4{ 0.31f, 0.31f, 0.31f, 1.0f };
		colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4{ 0.41f, 0.41f, 0.41f, 1.0f };
		colors[ImGuiCol_ScrollbarGrabActive] = ImVec4{ 0.51f, 0.51f, 0.51f, 1.0f };

		// Check Mark
		colors[ImGuiCol_CheckMark] = ImColor(200, 200, 200, 255);

		// Slider Grab
		colors[ImGuiCol_SliderGrab] = ImVec4(0.51f, 0.51f, 0.51f, 0.7f);
		colors[ImGuiCol_SliderGrabActive] = ImVec4(0.66f, 0.66f, 0.66f, 1.0f);

		// Text
		colors[ImGuiCol_Text] = ImGui::ColorConvertU32ToFloat4(Theme::Text);

		// Checkbox
		colors[ImGuiCol_CheckMark] = ImGui::ColorConvertU32ToFloat4(Theme::Text);

		// Separator
		colors[ImGuiCol_Separator] = ImGui::ColorConvertU32ToFloat4(Theme::BackgroundDark);
		colors[ImGuiCol_SeparatorActive] = ImGui::ColorConvertU32ToFloat4(Theme::Highlight);
		colors[ImGuiCol_SeparatorHovered] = ImColor(39, 185, 242, 150);

		// Resize Grip
		colors[ImGuiCol_ResizeGrip] = ImVec4(0.91f, 0.91f, 0.91f, 0.25f);
		colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.81f, 0.81f, 0.81f, 0.67f);
		colors[ImGuiCol_ResizeGripActive] = ImVec4(0.46f, 0.46f, 0.46f, 0.95f);

		// Window
		colors[ImGuiCol_WindowBg] = ImGui::ColorConvertU32ToFloat4(Theme::Titlebar);
		colors[ImGuiCol_ChildBg] = ImGui::ColorConvertU32ToFloat4(Theme::Background);
		colors[ImGuiCol_PopupBg] = ImGui::ColorConvertU32ToFloat4(Theme::BackgroundPopup);
		colors[ImGuiCol_Border] = ImGui::ColorConvertU32ToFloat4(Theme::BackgroundDark);

		// Popup
		colors[ImGuiCol_PopupBg] = ImGui::ColorConvertU32ToFloat4(Theme::Background);

		// Tables
		colors[ImGuiCol_TableHeaderBg] = ImGui::ColorConvertU32ToFloat4(Theme::GroupHeader);
		colors[ImGuiCol_TableBorderLight] = ImGui::ColorConvertU32ToFloat4(Theme::BackgroundDark);
		colors[ImGuiCol_TableRowBg] = ImGui::ColorConvertU32ToFloat4(Theme::BackgroundMedium);
		colors[ImGuiCol_TableRowBgAlt] = ImGui::ColorConvertU32ToFloat4(Theme::BackgroundDark);

		// MenuBar
		colors[ImGuiCol_MenuBarBg] = ImColor(0, 0, 0, 0);

		// Tabs
		colors[ImGuiCol_Tab] = ImGui::ColorConvertU32ToFloat4(Theme::Titlebar);
		colors[ImGuiCol_TabHovered] = ImColor(255, 255, 135, 30);
		colors[ImGuiCol_TabActive] = ImGui::ColorConvertU32ToFloat4(Theme::Accent);
		colors[ImGuiCol_TabUnfocused] = ImGui::ColorConvertU32ToFloat4(Theme::Titlebar);
		colors[ImGuiCol_TabUnfocusedActive] = ImGui::ColorConvertU32ToFloat4(Theme::AccentDimmed);

		// Title
		colors[ImGuiCol_TitleBg] = ImGui::ColorConvertU32ToFloat4(Theme::Titlebar);
		colors[ImGuiCol_TitleBgActive] = ImGui::ColorConvertU32ToFloat4(Theme::Titlebar);
		colors[ImGuiCol_TitleBgCollapsed] = ImColor(38.25f, 38.3775f, 38.505f, 255.0f);

		/// Style
		style.FrameRounding = 2.5f;
		style.FrameBorderSize = 1.0f;
		style.IndentSpacing = 11.0f;
	}

}