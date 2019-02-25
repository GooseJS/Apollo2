project "GLEW"
	architecture "x64"
    location "../../lib/glew"
    kind "SharedLib"
    language "C"

	targetdir ("%{prj.location}/build/bin/" .. outputDir .. "/%{prj.name}")
    objdir ("%{prj.location}/build/bin-obj/" .. outputDir .. "/%{prj.name}")

	files {
        "%{prj.location}/src/glew.c"
    }

    includedirs {
        "%{prj.location}/include"
    }

	filter "system:windows"
        systemversion "latest"
		defines {
			"WIN32",
			"WIN32_LEAN_AND_MEAN",
            "GLEW_NO_GLU",
            "_CRT_SECURE_NO_WARNINGS",
			"GLEW_BUILD"
		}
		links
		{
			"opengl32.lib"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../../data"),
		}
    filter { "system:windows", "configurations:Release" }
        buildoptions "/MT"