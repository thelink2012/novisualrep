workspace "novisualrep"
    platforms { "Win32" }

project "novisualrep"
    language "C++"
    kind "SharedLib"
    targetname "novisualrep"
    targetextension ".asi"
    includedirs "deps/injector/include"
    
workspace "*"
    configurations { "Release", "Debug" }
    location "build"
    targetdir "bin"
    implibdir "bin"
    targetprefix "" -- no 'lib' prefix on gcc
    
    files { "src/**.cpp" }
    
    flags {
        "NoPCH",
        "NoImportLib",
        "NoBufferSecurityCheck"
    }
    staticruntime "On"

filter "configurations:Debug"
    symbols "On"
    
filter "configurations:Release"
    defines { "NDEBUG" }
    symbols "Off"
    
    optimize "Speed"
    functionlevellinking "On"
    flags { "LinkTimeOptimization" }
    
filter { "toolset:*_xp"}
    defines { "WINVER=0x0501", "_WIN32_WINNT=0x0501" }
    buildoptions { "/Zc:threadSafeInit-" }
    
filter { "action:vs*" }
    defines { "_CRT_SECURE_NO_WARNINGS", "_SCL_SECURE_NO_WARNINGS" }

filter { "action:vs*", "platforms:Win32" }
    buildoptions { "/arch:IA32" } -- disable SSE/SSE2