#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <Windows.h>
// C RunTime Header Files
#include <cstdlib>
#include <malloc.h>
#include <memory>
#include <tchar.h>
#include <string>
#define _WINSOCK_DEPRECATED_NO_WARNINGS
typedef std::basic_string<TCHAR> TSTRING;