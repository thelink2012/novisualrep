--[[
    NFSU2 No Visual Rep Build Script
    Use 'premake5 --help' for help
--]]

--[[
    Options and Actions
--]]

newoption {
    trigger     = "outdir",
    value       = "path",
    description = "Output directory for the build files"
}
if not _OPTIONS["outdir"] then
    _OPTIONS["outdir"] = "build"
end


--[[
    The Solution
--]]
solution "NFSU2 No Visual Rep"

    configurations { "Release", "Debug" }

    location( _OPTIONS["outdir"] )
    targetprefix "" -- no 'lib' prefix on gcc
    targetdir "bin"
    implibdir "bin"

    startproject "novisualrep"

    flags {
        "StaticRuntime",
        "NoImportLib",
        "NoRTTI",
        "NoBufferSecurityCheck"
    }

    defines {
        "_CRT_SECURE_NO_WARNINGS",
        "_SCL_SECURE_NO_WARNINGS"
    }

    configuration "Debug*"
        flags { "Symbols" }
        
    configuration "Release*"
        defines { "NDEBUG" }
        optimize "Speed"

    configuration "vs*"
        buildoptions { "/arch:IA32" }           -- disable the use of SSE/SSE2 instructions

    project "novisualrep"
        language "C++"
        kind "SharedLib"
        targetname "novisualrep"
        targetextension ".asi"
        
        flags { "NoPCH" }

        files {
            "src/**.cpp",
            "src/**.hpp",
            "src/**.h",
        }
		
		includedirs {
			"src",
			"deps/injector/include"
		}
