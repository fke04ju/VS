#pragma once
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "WindowsCodecs.lib")
#pragma comment(lib, "SHLWAPI.LIB")
// Windows Header Files
#include <windows.h>
#include <windowsx.h>
// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <wchar.h>
#include <math.h>

#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <wincodec.h>
#include <ctime>
#include <string>
#include <sstream>
#include <chrono>
#include <shobjidl.h>
#include <shlwapi.h>

template<typename Interface>
inline void SafeRelease(Interface*& ppInterfaceToRelease)
{
    if (ppInterfaceToRelease != NULL)
    {
        (ppInterfaceToRelease)->Release();

        (ppInterfaceToRelease) = NULL;
    }
}