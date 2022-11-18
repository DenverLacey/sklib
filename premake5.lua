workspace "sklib"
	configurations { "Debug", "Release" }

project "sklib"
	kind "ConsoleApp"
	language "C++"
    cppdialect "C++17"

	files {
		"sk/**.h",
        "sk/**.cpp",

        "src/**.h",
        "src/**.cpp"
	}

	includedirs { 
        ".", 
        "src"
    }

	filter "configurations:Debug"
		defines { "DEBUG" }
		symbols "On"

	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "On"
