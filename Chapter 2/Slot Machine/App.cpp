#include "framework.h"
#include "Point2D.h"
#include "App.h"

int time_start;
int time_end;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0);
	HRESULT hr = CoInitialize(NULL);
	if (SUCCEEDED(hr))
	{
		MainApp mainApp;
		if (SUCCEEDED(mainApp.Initialize()))
		{
			mainApp.RunMessageLoop();
		}
		CoUninitialize();
	}
	return 0;
}

MainApp::MainApp()
{
    
}

MainApp::~MainApp()
{
    delete engine;
}

void MainApp::RunMessageLoop()
{
	MSG msg;

	boolean running = true;

    time_start = GetTickCount64();
    
	while (running)
	{
        time_end = GetTickCount64();
        int elapsed = time_end - time_start;
        logicTime += elapsed;
        frameTime += elapsed;
        if (logicTime >= LOGICTIME) // logic operation
        {
            engine->Logic();
            logicTime -= LOGICTIME;
        }
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
            //engine->Animater(hwnd, modeList[frame]);
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			if (msg.message == WM_QUIT)
			{
				running = false;
			}
        }
        else
        {
            if (frameTime >= FRAMETIME) // draw frame
            {
                engine->Animater(hwnd);
                frameTime -= FRAMETIME;
            }
        }
        time_start = time_end;
	}
}

HRESULT MainApp::Initialize()
{
    HRESULT hr = S_OK;

    // Register the window class.
    WNDCLASSEX wcex = { sizeof(WNDCLASSEX) };
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = appInstance;
    wcex.hbrBackground = NULL;
    wcex.lpszMenuName = NULL;
    wcex.hCursor = LoadCursor(NULL, IDI_APPLICATION);
    wcex.lpszClassName = L"D2DMainApp";

    RegisterClassEx(&wcex);

    // Create the window.
    hwnd = CreateWindowEx(
        NULL,
        L"D2DMainApp",
        L"小瑪莉",
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_CLIPCHILDREN,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        RESOLUTION_X,
        RESOLUTION_Y,
        NULL,
        NULL,
        appInstance,
        this
    );

    ExitButton = CreateWindow(
        L"Button",
        L"離開",
        WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
        1010, 0, 200, 100,
        hwnd,
        (HMENU)1,
        GetModuleHandle(NULL),
        NULL
    );

    StartButton = CreateWindow(
        L"Button",
        L"開始",
        WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
        810, 700, 100, 100,
        hwnd,
        (HMENU)2,
        GetModuleHandle(NULL),
        NULL
    );

    LargeButton = CreateWindow(
        L"Button",
        L"大",
        WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
        910, 700, 100, 100,
        hwnd,
        (HMENU)3,
        GetModuleHandle(NULL),
        NULL
    );

    SmallButton = CreateWindow(
        L"Button",
        L"小",
        WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
        1010, 700, 100, 100,
        hwnd,
        (HMENU)4,
        GetModuleHandle(NULL),
        NULL
    );

    AutoButton = CreateWindow(
        L"Button",
        L"自動",
        WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
        810, 0, 200, 100,
        hwnd,
        (HMENU)5,
        GetModuleHandle(NULL),
        NULL
    );

    GetPointButton = CreateWindow(
        L"Button",
        L"取分",
        WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
        1110, 700, 100, 100,
        hwnd,
        (HMENU)6,
        GetModuleHandle(NULL),
        NULL
    );

    engine = new Engine(hwnd);

    hr = hwnd ? S_OK : E_FAIL;
    if (SUCCEEDED(hr))
    {
        engine->InitializeD2D(hwnd);

        ShowWindow(hwnd, SW_SHOWNORMAL);
        UpdateWindow(hwnd);
    }
    engine->ReadJSON("./source/config.json", hwnd);


    return hr;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    LRESULT result = 0;

    if (message == WM_CREATE)
    {
        LPCREATESTRUCT pcs = (LPCREATESTRUCT)lParam;
        MainApp* pMainApp = (MainApp*)pcs->lpCreateParams;

        ::SetWindowLongPtrW(
            hwnd,
            GWLP_USERDATA,
            reinterpret_cast<LONG_PTR>(pMainApp)
        );

        result = 1;
    }
    else
    {
        MainApp* pMainApp = reinterpret_cast<MainApp*>(static_cast<LONG_PTR>(
            ::GetWindowLongPtrW(
                hwnd,
                GWLP_USERDATA
            )));

        bool wasHandled = false;

        if (pMainApp)
        {
            switch (message)
            {

            case WM_DISPLAYCHANGE:
            {
                InvalidateRect(hwnd, NULL, FALSE);
            }
            result = 0;
            wasHandled = true;
            break;

            case WM_COMMAND:
            {
                if (HIWORD(wParam) == BN_CLICKED)
                {
                    if (LOWORD(wParam) == 1) // Exit
                    {
                        PostQuitMessage(0);
                    }
                    else if (LOWORD(wParam) == 2) // Start
                    {
                        engine->isBetStart = true;
                    }
                    else if (LOWORD(wParam) == 3) // Large
                    {
                        engine->isLargesmallStart = true;
                        engine->guess = true;
                    }
                    else if (LOWORD(wParam) == 4) // Small
                    {
                        engine->isLargesmallStart = true;
                        engine->guess = false;
                    }
                    else if (LOWORD(wParam) == 5) // Auto
                    {
                        engine->AutoMode();
                        // TODO:Add animation
                    }
                    else if (LOWORD(wParam) == 6) // Get Point
                    {
                        engine->getPoint = true;
                    }
                }
            }
            result = 0;
            wasHandled = true;
            break;

            case WM_LBUTTONDOWN:
            {
                int xPos = GET_X_LPARAM(lParam);
                int yPos = GET_Y_LPARAM(lParam);
                // TODO : add click event in BetArea
                engine->PressEvent(xPos, yPos);
            }
            result = 0;
            wasHandled = true;
            break;

            case WM_LBUTTONUP:
            {
                engine->betArea->Release();
            }
            result = 0;
            wasHandled = true;
            break;

            case WM_DESTROY:
            {
                PostQuitMessage(0);
            }
            result = 1;
            wasHandled = true;
            break;
            }
        }

        if (!wasHandled)
        {
            result = DefWindowProc(hwnd, message, wParam, lParam);
        }
    }

    return result;
}