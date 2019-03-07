project "FastNoise"
    location "../../lib/FastNoise"
    kind "StaticLib"
    language "C++"

	targetdir ("%{prj.location}/build/bin/" .. outputDir .. "/%{prj.name}")
    objdir ("%{prj.location}/build/bin-obj/" .. outputDir .. "/%{prj.name}")

	files
	{
        "%{prj.location}/FastNoise.h",
        "%{prj.location}/FastNoise.cpp"
    }

	filter "configurations:Debug"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		runtime "Release"
		optimize "On"

	filter "system:windows"
        systemversion "latest"
        cppdialect "C++17"
        staticruntime "off"
