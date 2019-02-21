#pragma once

#ifdef APOLLO_WIN32
#ifdef APOLLO_BUILD_DLL
#define APOLLO_API __declspec(dllexport)
#else
#define APOLLO_API __declspec(dllimport)
#endif
#else
#error Not yet supported.
#endif