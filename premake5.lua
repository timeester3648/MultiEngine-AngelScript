include "../../premake/common_premake_defines.lua"

project "AngelScript"
	kind "StaticLib"
	language "C++"
	cppdialect "C++latest"
	cdialect "C17"
	targetname "%{prj.name}"
	inlining "Auto"

	files {
		"./sdk/angelscript/include/**.h",
		"./sdk/angelscript/source/**.h",
		"./sdk/angelscript/source/**.cpp"
	}

	defines {
		"ANGELSCRIPT_EXPORT"
	}

	includedirs {
		"%{IncludeDir.angel_script}"
	}

	filter "configurations:Debug"
		defines { "AS_DEBUG" }

	filter "toolset:msc"
		files {
			"./sdk/angelscript/source/as_callfunc_x64_msvc_asm.asm"
		}

	filter { "toolset:msc", "files:**as_callfunc_x64_msvc_asm.asm" }
		buildmessage "Compiling %{file.basename}.asm"

		buildcommands {
			"ml64 /c /Cp /Cx /nologo /Zi /Fo%{cfg.objdir}/%{file.basename}.obj %{file.path}"
		}

		buildoutputs { "%{cfg.objdir}/%{file.basename}.obj" }
	
	filter "system:windows"
		disablewarnings { "4468" }