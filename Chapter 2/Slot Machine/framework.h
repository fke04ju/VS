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


#include ".\include\json\json.h"
#include ".\include\json\value.h"
#include ".\include\json\reader.h"
#include ".\include\json\writer.h"

#define ST_PENDING              1001
#define ST_ENOUGHMONEYTOBET     1002
#define ST_SELECTING            1003
#define ST_WINNING              1004
#define ST_ENOUGHMONEYTOCOMPARE 1005
#define ST_COMPARE_SELECTING    1006
#define ST_COMPARE_WINNING      1007

#define LOGICTIME 500
#define FRAMETIME 10

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

typedef struct Instruction
{
    int timeToDo;       // timestamp
    int lightNumber;    // which light should light or not
    bool Lightening;    // light or not
    int RorB;           // Roulette or BetArea
}Instruction;

