set(APOLLO_ENGINE_SRC	"src/Apollo/logger/Logger.h"
						"src/Apollo/logger/Logger.cpp"
						"src/Apollo/Apollo.h"
						"src/Apollo/Core.h"
						"src/Apollo/GL.h"
						"src/Apollo/Window.h"
						"src/Apollo/Window.cpp"
						"src/Apollo/render/Texture.h"
						"src/Apollo/render/Shader.h"
						"src/Apollo/render/Renderable.h"
						"src/Apollo/render/2d/SpriteBatch.h"
						"src/Apollo/render/2d/SpriteBatch.cpp")

add_library(ApolloEngine SHARED "${APOLLO_ENGINE_SRC}")

target_include_directories(ApolloEngine PUBLIC	"src"
												"../lib/spdlog/include"
												"../lib/stb"
												glfw
												libglew_shared)

target_link_libraries(ApolloEngine PUBLIC glfw libglew_shared)

#add_definitions(-DAPOLLO_BUILD_DLL)
target_compile_definitions(ApolloEngine PRIVATE -DAPOLLO_BUILD_DLL)

if (MSVC)
	target_compile_definitions(ApolloEngine PUBLIC -DAPOLLO_WIN32)

	add_custom_command(TARGET ApolloEngine POST_BUILD        # Adds a post-build event to MyTest
		COMMAND ${CMAKE_COMMAND} -E copy_if_different  # which executes "cmake - E copy_if_different..."
        "${CMAKE_CURRENT_BINARY_DIR}/ApolloEngine.dll"      # <--this is in-file
        "${CMAKE_CURRENT_SOURCE_DIR}/../data/ApolloEngine.dll")
	add_custom_command(TARGET ApolloEngine POST_BUILD        # Adds a post-build event to MyTest
		COMMAND ${CMAKE_COMMAND} -E copy_if_different  # which executes "cmake - E copy_if_different..."
        "${CMAKE_CURRENT_BINARY_DIR}/../bin/glewd.dll"      # <--this is in-file
        "${CMAKE_CURRENT_SOURCE_DIR}/../data/glewd.dll") 
endif()