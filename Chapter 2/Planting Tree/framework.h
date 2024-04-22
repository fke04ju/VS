#pragma once
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "WindowsCodecs.lib")
#pragma comment(lib, "SHLWAPI.LIB")
#define _AFXDLL
// Windows Header Files
#include <afxwin.h>
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
#include <vector>
#include <fstream>
#include <comdef.h>
#include <algorithm>
#include <io.h>
#include <direct.h>
#include <sys/stat.h>


#include ".\include\json\json.h"
#include ".\include\json\value.h"
#include ".\include\json\reader.h"
#include ".\include\json\writer.h"


using namespace std;

template<typename Interface>
inline void SafeRelease(Interface*& ppInterfaceToRelease)
{
    if (ppInterfaceToRelease != NULL)
    {
        (ppInterfaceToRelease)->Release();

        (ppInterfaceToRelease) = NULL;
    }
}

