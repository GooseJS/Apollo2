project "ImGui"
    location "../../lib/imgui"
    kind "StaticLib"
    language "C++"

	targetdir ("%{prj.location}/build/bin/" .. outputDir .. "/%{prj.name}")
    objdir ("%{prj.location}/build/bin-obj/" .. outputDir .. "/%{prj.name}")

	files
	{
        "%{prj.location}/imconfig.h",
        "%{prj.location}/imgui.h",
        "%{prj.location}/imgui.cpp",
        "%{prj.location}/imgui_draw.cpp",
        "%{prj.location}/imgui_internal.h",
        "%{prj.location}/imgui_widgets.cpp",
        "%{prj.location}/imstb_rectpack.h",
        "%{prj.location}/imstb_textedit.h",
        "%{prj.location}/imstb_truetype.h",
        "%{prj.location}/imgui_demo.cpp"
    }

	filter "system:windows"
        systemversion "latest"
        cppdialect "C++17"
        staticruntime "On"

    filter { "system:windows", "configurations:Release" }
        buildoptions "/MT"
