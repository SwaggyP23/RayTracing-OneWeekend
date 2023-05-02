project "RTWeekend"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    staticruntime "off"

    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin/Intermediates/" .. outputdir .. "/%{prj.name}")

    files
    {
        "src/**.h",
        "src/**.cpp",
        "dependencies/stb/stb_image_writer/stb_image_write.cpp"
    }

    includedirs
    {
        "src",
        "dependencies/Glad/include",
        "dependencies/glfw/include",
        "dependencies/glm",
        "dependencies/ImGui",
        "dependencies/stb"
    }

    links
    {
        "ImGui",
        "Glad",
        "GLFW",
        "opengl32.lib"
    }

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Debug"
        defines "RTOW_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "RTOW_RELEASE"
        runtime "Release"
        optimize "Speed"
        inlining "Auto"