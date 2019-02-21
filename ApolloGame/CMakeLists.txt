set(APOLLO_GAME_SRC		"src/Apollo/Apollo.cpp")

add_executable(ApolloGame "${APOLLO_GAME_SRC}")

target_include_directories(ApolloGame PUBLIC	"src"
												ApolloEngine)

target_link_libraries(ApolloGame PUBLIC ApolloEngine)

set_target_properties(ApolloGame PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/data")

if (MSVC)
	set_target_properties(ApolloGame PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:CONSOLE")
endif()