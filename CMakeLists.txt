project(Apollo)
cmake_minimum_required(VERSION 3.11)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

set(GLM_QUIET OFF CACHE INTERNAL "")
set(GLM_TEST_ENABLE OFF CACHE INTERNAL "")

set(JSON_BuildTests OFF CACHE INTERNAL "")

add_definitions(-DGLEW_NO_GLU)

add_subdirectory(lib/glfw)
add_subdirectory(lib/glew)

#add_subdirectory(lib/glm)
add_subdirectory(lib/spdlog)

#add_subdirectory(lib/json)

add_subdirectory(ApolloEngine)
add_subdirectory(ApolloGame)