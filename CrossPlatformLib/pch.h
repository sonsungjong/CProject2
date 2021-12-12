// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

// add headers that you want to pre-compile here
#include <cstdio>
#include <tchar.h>
#include <string>
#include <time.h>
#include <errno.h>

#ifdef _MSC_VER
#include <Windows.h>
	//#ifdef _WIN64
	//
	//#elif defined(_WIN32)
	//
	//#else
	//
	//#endif
#elif defined(__Linux__)
#include <unistd.h>
#elif defined(__APPLE__)
#include <mach/clock.h>
#include <mach/mach.h>
#endif

typedef std::basic_string<TCHAR> TSTRING;
//#ifdef _UNICODE
//typedef std::wstring TSTRING;
//#else
//typedef std::string TSTRING;
//#endif

#endif //PCH_H
