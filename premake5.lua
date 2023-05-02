workspace "RTWeekend"
    architecture "x86_64"

    configurations
    {
        "Debug",
        "Release"
    }

    flags
    {
        "MultiProcessorCompile"
    }

outputdir = "%{cfg.architecture}-%{cfg.buildcfg}"

group "Core"
    include "RayTracingOneWeekend"
group ""

group "Dependencies"
    include "RayTracingOneWeekend/dependencies/glfw"
    include "RayTracingOneWeekend/dependencies/Glad"
    include "RayTracingOneWeekend/dependencies/ImGui/imgui"
group ""