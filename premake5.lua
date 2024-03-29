-- A solution contains projects, and defines the available configurations
solution "PacmanReplica"
	configurations { "Debug", "Release"}

	flags { "Unicode" , "NoPCH"}

	-- A project defines one build target
	project ("PacmanReplicaProject")	
		binaries = "./bin/"
		build = "./build/"
		os.mkdir(binaries)
		os.mkdir(build)
	
		kind "ConsoleApp"
		location (build)
		language "C++"
		targetdir (binaries)
		
		configuration { "windows" }
			buildoptions ""
			linkoptions { "/NODEFAULTLIB:msvcrt" } -- https://github.com/yuriks/robotic/blob/master/premake5.lua
		configuration { "linux" }
			buildoptions "-std=c++11" --http://industriousone.com/topic/xcode4-c11-build-option
			toolset "gcc"
		configuration {}
		
		files { "./src/**.h", "./src/**.cpp" } -- build all .h and .cpp files recursively
		excludes { "./graphics_dependencies/**" }  -- don't build files in graphics_dependencies/
		
		
		-- where are header files?
		-- tag::headers[]
		configuration "windows"
		includedirs {
						"./graphics_dependencies/SDL2/include",
						"./graphics_dependencies/SDL2/include/SDL2",
						"./graphics_dependencies/glew/include",
						"./graphics_dependencies/glm",
						"./graphics_dependencies/SDL2_image/include",
						"./graphics_dependencies/SDL2_ttf/include",
						"./graphics_dependencies/SDL2_mixer/include",
					}
		configuration { "linux" }
		includedirs {
					-- should be installed as in ./graphics_dependencies/README.asciidoc
					}
		configuration {}
		-- end::headers[]
		
		
		-- what libraries need linking to
		-- tag::libraries[]
		configuration "windows"
			links { "SDL2", "SDL2main", "opengl32", "glew32", "SDL2_image", "SDL2_ttf", "SDL2_mixer" }
		configuration "linux"
			links { "SDL2", "SDL2main", "GL", "GLEW", "SDL2_image", "SDL2_ttf", "SDL2_mixer" }
		configuration {}
		-- end::libraries[]
		
		-- where are libraries?
		-- tag::librariesDirs[]
		configuration "windows"
		libdirs {
					"./graphics_dependencies/glew/lib/Release/Win32",
					"./graphics_dependencies/SDL2/lib/win32",
					"./graphics_dependencies/SDL2_image/lib/x86/",
					"./graphics_dependencies/SDL2_ttf/lib/x86/",
					"./graphics_dependencies/SDL2_mixer/lib/x86/",
				}
		configuration "linux"
					-- should be installed as in ./graphics_dependencies/README.asciidoc
		configuration {}
		-- end::librariesDirs[]
		
		
		configuration "*Debug"
			defines { "DEBUG" }
			flags { "Symbols" }
			optimize "Off"
			targetsuffix "-debug"
		
		
		configuration "*Release"
			defines { "NDEBUG" }
			optimize "On"
			targetsuffix "-release"
		
		
		-- copy dlls on windows
		-- tag::windowsDLLCopy[]
		if os.get() == "windows" then
			os.copyfile("./graphics_dependencies/glew/bin/Release/Win32/glew32.dll", path.join(binaries, "glew32.dll"))
			os.copyfile("./graphics_dependencies/SDL2/lib/win32/SDL2.dll", path.join(binaries, "SDL2.dll"))
			os.copyfile("./graphics_dependencies/SDL2_image/lib/x86/SDL2_image.dll", path.join(binaries, "SDL2_image.dll"))
			os.copyfile("./graphics_dependencies/SDL2_image/lib/x86/libjpeg-9.dll", path.join(binaries, "libjpeg-9.dll"))
			os.copyfile("./graphics_dependencies/SDL2_image/lib/x86/libpng16-16.dll", path.join(binaries, "libpng16-16.dll"))
			os.copyfile("./graphics_dependencies/SDL2_image/lib/x86/libtiff-5.dll", path.join(binaries, "libtiff-5.dll"))
			os.copyfile("./graphics_dependencies/SDL2_image/lib/x86/libwebp-4.dll", path.join(binaries, "libwebp-4.dll"))
			os.copyfile("./graphics_dependencies/SDL2_image/lib/x86/zlib1.dll", path.join(binaries, "zlib1.dll"))
			os.copyfile("./graphics_dependencies/SDL2_ttf/lib/x86/SDL2_ttf.dll", path.join(binaries, "SDL2_ttf.dll"))
			os.copyfile("./graphics_dependencies/SDL2_ttf/lib/x86/libfreetype-6.dll", path.join(binaries, "libfreetype-6.dll"))
			os.copyfile("./graphics_dependencies/SDL2_mixer/lib/x86/SDL2_mixer.dll", path.join(binaries, "SDL2_mixer.dll"))
			os.copyfile("./graphics_dependencies/SDL2_mixer/lib/x86/libFLAC-8.dll", path.join(binaries, "libFLAC-8.dll"))
			os.copyfile("./graphics_dependencies/SDL2_mixer/lib/x86/libmodplug-1.dll", path.join(binaries, "libmodplug-1.dll"))
			os.copyfile("./graphics_dependencies/SDL2_mixer/lib/x86/libogg-0.dll", path.join(binaries, "libogg-0.dll"))
			os.copyfile("./graphics_dependencies/SDL2_mixer/lib/x86/libvorbis-0.dll", path.join(binaries, "libvorbis-0.dll"))
			os.copyfile("./graphics_dependencies/SDL2_mixer/lib/x86/libvorbisfile-3.dll", path.join(binaries, "libvorbisfile-3.dll"))
			os.copyfile("./graphics_dependencies/SDL2_mixer/lib/x86/smpeg2.dll", path.join(binaries, "smpeg2.dll"))
		end
		-- end::windowsDLLCopy[]