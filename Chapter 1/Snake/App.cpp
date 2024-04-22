#include "framework.h"
#include "Point2D.h"
#include "App.h"

#pragma comment(lib, "d2d1")
HWND start_button;
HWND restart_button;
int menuswitch;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0);
	HRESULT hr = CoInitialize(NULL);
	if (SUCCEEDED(hr)) 
	{
		MainApp app;
		if (SUCCEEDED(app.Initialize())) 
		{
			app.RunMessageLoop();
		}
		CoUninitialize();
	}
	return 0;
}

MainApp::MainApp() : m_hwnd(NULL)
{
	
}

MainApp::~MainApp()
{
	
}

void MainApp::RunMessageLoop()
{
	MSG msg;

	std::chrono::steady_clock::time_point begin_logic = std::chrono::steady_clock::now();
	std::chrono::steady_clock::time_point begin_draw = std::chrono::steady_clock::now();
	boolean running = true;

	int frame = 0;
	while (running)
	{
		std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
		double elapsed_secs_logic = std::chrono::duration_cast<std::chrono::microseconds>(now - begin_logic).count() / 1000000.0;
		double elapsed_secs_draw = std::chrono::duration_cast<std::chrono::microseconds>(now - begin_draw).count() / 1000000.0;


		if (elapsed_secs_logic >= static_secs_logic) {
			//frames = 0;
			//framestime = 0;
			// game logic
			// messages and user input
			while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
				if (msg.message == WM_QUIT)
				{
					running = false;
				}
			}
			engine->Logic(m_hwnd, &menuswitch, speed, start_button, restart_button);
			begin_logic = now;
		}
		if (elapsed_secs_draw >= static_secs_draw)
		{
			engine->Draw(m_hwnd, elapsed_secs_draw, start_button, restart_button);
			begin_draw = now;
		}
	}
}

HRESULT MainApp::Initialize()
{
	HRESULT hr = S_OK;
	static_secs_logic = 0.01f;
	static_secs_draw = fps60;
	speed = 5;
	menuswitch = 0;
	// register window class
	WNDCLASSEX wcex = { sizeof(WNDCLASSEX) };
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = MainApp::WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = HINST_THISCOMPONENT;
	wcex.hbrBackground = NULL;
	wcex.lpszMenuName = NULL;
	wcex.hCursor = LoadCursor(NULL, IDI_APPLICATION);
	wcex.lpszClassName = L"D2DMainApp";

	ATOM x = RegisterClassEx(&wcex);

	// create window
	m_hwnd = CreateWindowEx(
		NULL,
		L"D2DMainApp",
		L"Game",
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		RESOLUTION_X-14,
		RESOLUTION_Y,
		NULL,
		NULL,
		HINST_THISCOMPONENT,
		this
	);

	start_button = CreateWindow(
		L"Button",
		L"Start",
		WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
		318, 225, 100, 50,
		m_hwnd,
		(HMENU)1,
		GetModuleHandle(NULL),
		NULL
	);

	restart_button = CreateWindow(
		L"Button",
		L"Restart",
		WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
		318, 225, 100, 50,
		m_hwnd,
		(HMENU)2,
		GetModuleHandle(NULL),
		NULL
	);

	ShowWindow(restart_button, SW_HIDE);

	hr = m_hwnd ? S_OK : E_FAIL;
	engine = new Engine(m_hwnd);
	if ((SUCCEEDED(hr))) 
	{
		engine->InitializeD2D(m_hwnd);
		ShowWindow(m_hwnd, SW_SHOWNORMAL);
		UpdateWindow(m_hwnd);
	}
	return hr;
}

LRESULT CALLBACK MainApp::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT lr = 0;
	if (message == WM_CREATE)
	{
		
		LPCREATESTRUCT pcs = (LPCREATESTRUCT)lParam;
		MainApp* pMainApp = (MainApp*)pcs->lpCreateParams;
		::SetWindowLongPtrW(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pMainApp));
		
		lr = 1;
	}
	else
	{
		MainApp* pMainApp = reinterpret_cast<MainApp*>(static_cast<LONG_PTR>(::GetWindowLongPtrW(hwnd, GWLP_USERDATA)));

		bool wasHandled = false;

		if (pMainApp)
		{
			switch (message)
			{
			case WM_DISPLAYCHANGE:
			{
				InvalidateRect(hwnd, NULL, FALSE);
			}
			lr = 0;
			wasHandled = true;
			break;

			case WM_COMMAND:
			{
				if (HIWORD(wParam) == BN_CLICKED)
				{
					if (LOWORD(wParam) == 1)
					{
						menuswitch = 1;
					}
					else if (LOWORD(wParam) == 2)
					{
						pMainApp->engine->Reset();
						menuswitch = 1;
					}
				}
			}

			case WM_LBUTTONUP:
			{
				//pMainApp->engine->Reset();
			}
			lr = 0;
			wasHandled = true;
			break;

			case WM_KEYDOWN:
			{
				pMainApp->engine->KeyUP(wParam);
			}
			lr = 0;
			wasHandled = true;
			break;

			case WM_DESTROY:
			{
				PostQuitMessage(0);
			}
			lr = 1;
			wasHandled = true;
			break;
			}
		}
		if (!wasHandled)
		{
			lr = DefWindowProc(hwnd, message, wParam, lParam);
		}
	}
	return lr;
}