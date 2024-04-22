#include "framework.h"
#include "Point2D.h"
#include "App.h"

HWND m_hwnd; // main window
HWND EditButton, SaveButton, LoadButton, QuitButton;
HWND ListSquare, DrawPlant;
HWND e_hwnd, s_hwnd; // edit window and save window
HWND e_CloseButton, e_LoadTreeButton, e_LoadFruitButton, e_ImportPictureButton, e_SavePictureButton, e_DrawWindow, e_DeletePictureButton, cbbox;
bool editCalled = false;
HINSTANCE appInstance;
HINSTANCE editInstance;
bool isChosen = false;
bool isDeleted = false;

string savedJSONFile = "./saved.json";

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

}

void MainApp::RunMessageLoop()
{
	MSG msg;

	boolean running = true;

	int frame = 0;
	while (running)
	{
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			if (msg.message == WM_QUIT)
			{
				running = false;
			}
		}
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

    WNDCLASSEX wcex2 = { sizeof(WNDCLASSEX) };
    wcex2.style = CS_HREDRAW | CS_VREDRAW;
    wcex2.lpfnWndProc = EditProc;
    wcex2.cbClsExtra = 0;
    wcex2.cbWndExtra = 0;
    wcex2.hInstance = editInstance;
    wcex2.hbrBackground = NULL;
    wcex2.lpszMenuName = NULL;
    wcex2.hCursor = LoadCursor(NULL, IDI_APPLICATION);
    wcex2.lpszClassName = L"D2DEditApp";

    RegisterClassEx(&wcex);
    RegisterClassEx(&wcex2);

    // Create the window.
    m_hwnd = CreateWindowEx(
        NULL,
        L"D2DMainApp",
        L"Planting Tree",
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

    e_hwnd = CreateWindowEx(
        NULL,
        L"D2DEditApp",
        L"Edit Menu",
        WS_OVERLAPPED | WS_CAPTION | WS_MINIMIZEBOX,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        RESOLUTION_X / 2,
        RESOLUTION_Y / 2,
        NULL,
        NULL,
        editInstance,
        this
    );

    m_engine = new Engine(m_hwnd, true);
    e_engine = new Engine(e_hwnd, false);

    hr = m_hwnd ? S_OK : E_FAIL;
    if (SUCCEEDED(hr))
    {
        m_engine->InitializeD2D(m_hwnd);
        e_engine->InitializeD2D(e_hwnd);

        ShowWindow(m_hwnd, SW_SHOWNORMAL);
        ShowWindow(e_hwnd, SW_HIDE);
        UpdateWindow(m_hwnd);
    }

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

        ListSquare = CreateWindow(
            L"Static",
            NULL,
            WS_VISIBLE | WS_CHILD,
            0, 0, RESOLUTION_X, 50,
            hwnd,
            NULL,
            GetModuleHandle(NULL),
            NULL
        );

        DrawPlant = CreateWindow(
            L"Static",
            NULL,
            WS_DISABLED | WS_CHILD,
            0, 50, RESOLUTION_X-14, RESOLUTION_Y-80,
            hwnd,
            NULL,
            GetModuleHandle(NULL),
            NULL
        );

        EditButton = CreateWindow(
            L"Button",
            L"編輯",
            WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
            10, 10, 100, 30,
            hwnd,
            (HMENU)1,
            GetModuleHandle(NULL),
            NULL
        );

        LoadButton = CreateWindow(
            L"Button",
            L"載入地圖",
            WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
            150, 10, 100, 30,
            hwnd,
            (HMENU)2,
            GetModuleHandle(NULL),
            NULL
        );

        SaveButton = CreateWindow(
            L"Button",
            L"儲存地圖",
            WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
            300, 10, 100, 30,
            hwnd,
            (HMENU)3,
            GetModuleHandle(NULL),
            NULL
        );

        QuitButton = CreateWindow(
            L"Button",
            L"離開",
            WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
            450, 10, 100, 30,
            hwnd,
            (HMENU)4,
            GetModuleHandle(NULL),
            NULL
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
                    if (LOWORD(wParam) == 1) // Open Edit menu
                    {
                        if (!editCalled)
                        {
                            editCalled = true;
                            //EnableWindow(EditButton, FALSE);
                            ShowWindow(e_hwnd, SW_HIDE);
                            ShowWindow(e_hwnd, SW_SHOW);


                        }
                        else
                        {
                            ShowWindow(e_hwnd, SW_HIDE);
                            ShowWindow(e_hwnd, SW_SHOW);
                            if (!isDeleted)
                            {
                                ComboBox_SetCueBannerText(cbbox, e_engine->fruittree->TreeName);
                                wstring ws(e_engine->fruittree->TreeName);
                                string str1(ws.begin(), ws.end());
                                e_engine->ReadComponentJSON(str1, e_hwnd);
                                e_engine->LoadPicture(e_hwnd, 5);
                                e_engine->SelectPicture(e_engine->fruittree->TreeName);
                                isChosen = true;
                            }
                        }
                    }
                    else if (LOWORD(wParam) == 2) // Load map file
                    {
                        m_engine->LoadPicture(m_hwnd, 4);
                    }
                    else if (LOWORD(wParam) == 3) // Save map file
                    {
                        m_engine->SaveDialog(hwnd, true);
                        //m_engine->SavePicture(DrawPlant);
                    }
                    else if (LOWORD(wParam) == 4) // Quit app
                    {
                        PostQuitMessage(0);
                    }
                }
            }
            result = 0;
            wasHandled = true;
            break;

            case WM_LBUTTONUP:
            {
                if (isChosen && !isDeleted)
                {
                    int xPos = GET_X_LPARAM(lParam);
                    int yPos = GET_Y_LPARAM(lParam);
                    m_engine->LoadPicture(m_hwnd, 3);
                    m_engine->Draw(m_hwnd, xPos - 100, yPos - 200, 3);
                }
            }
            result = 0;
            wasHandled = true;
            break;

            case WM_RBUTTONUP:
            {
                int xPos = GET_X_LPARAM(lParam);
                int yPos = GET_Y_LPARAM(lParam);
                m_engine->ClickDelete(m_hwnd, xPos - 100, yPos - 100, true);
            }
            result = 0;
            wasHandled = true;
            break;

            case WM_DESTROY:
            {
                //SendMessage(e_hwnd, WM_QUIT, 0, 0);
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


void getDirFile() {
    WCHAR path[MAX_PATH];
    std::fill(path, path + MAX_PATH, 0);
    GetModuleFileName(NULL, path, MAX_PATH);
    PathRemoveFileSpec(path);
    const WCHAR* comp = L"\\Component\\";
    wcscat_s(path, comp);
    wstring ws(path);
    string str(ws.begin(), ws.end());
    const char* extenName = "*.json";
    const char* definePath = str.c_str();
    char buff[1024];
    _chdir(definePath);
    unsigned long long hFile;
    struct _finddata_t fileName;
    memset(&fileName, 0, sizeof(_finddata_t));
    hFile = _findfirst(extenName, &fileName);
    WCHAR A[MAX_PATH];
    memset(&A, 0, sizeof(A));
    if (fileName.name[0] == 0)
    {
        SendMessage(cbbox, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)A);
        return;
    }
    do {
        size_t outSize;
        mbstowcs_s(&outSize, A, fileName.name, strlen(fileName.name));
        SendMessage(cbbox, (UINT) CB_ADDSTRING, (WPARAM) 0, (LPARAM) A);
    } while (hFile != -1 && _findnext(hFile, &fileName) == 0);
    //SendMessage(cbbox, CB_SETCURSEL, (WPARAM) 0, (LPARAM) 0);
}

LRESULT CALLBACK EditProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    LRESULT result = 0;

    if (message == WM_CREATE)
    {
        LPCREATESTRUCT pcs = (LPCREATESTRUCT)lParam;
        MainApp* pEditApp = (MainApp*)pcs->lpCreateParams;

        ::SetWindowLongPtrW(
            hwnd,
            GWLP_USERDATA,
            reinterpret_cast<LONG_PTR>(pEditApp)
        );

        e_LoadTreeButton = CreateWindow(
            L"Button",
            L"載入樹木圖片",
            WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
            10, 10, 100, 30,
            hwnd,
            (HMENU)2,
            GetModuleHandle(NULL),
            NULL
        );

        e_LoadFruitButton = CreateWindow(
            L"Button",
            L"載入水果圖片",
            WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
            10, 45, 100, 30,
            hwnd,
            (HMENU)3,
            GetModuleHandle(NULL),
            NULL
        );
        /*
        e_ImportPictureButton = CreateWindow(
            L"Button",
            L"使用元件",
            WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
            10, 80, 100, 30,
            hwnd,
            (HMENU)4,
            GetModuleHandle(NULL),
            NULL
        );
        */
       

        cbbox = CreateWindow(
            WC_COMBOBOX,
            TEXT(""),
            CBS_DROPDOWNLIST | CBS_HASSTRINGS | CBS_SORT | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,
            10, 115, 100, 1000,
            hwnd,
            NULL,
            GetModuleHandle(NULL),
            NULL
        );
        /*
        CComboBox pComboBox;
        pComboBox.Attach(cbbox);
        pComboBox.AddString(L"aaa");

        pComboBox.AddString(L"aaab");
        pComboBox.AddString(L"aaabb");
        pComboBox.Detach();
        */
        getDirFile();

        e_SavePictureButton = CreateWindow(
            L"Button",
            L"儲存元件",
            WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
            10, 150, 100, 30,
            hwnd,
            (HMENU)5,
            GetModuleHandle(NULL),
            NULL
        );

        e_DeletePictureButton = CreateWindow(
            L"Button",
            L"刪除元件",
            WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
            10, 185, 100, 30,
            hwnd,
            (HMENU)6,
            GetModuleHandle(NULL),
            NULL
        );

        e_CloseButton = CreateWindow(
            L"Button",
            L"關閉",
            WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
            10, 220, 60, 30,
            hwnd,
            (HMENU)1,
            GetModuleHandle(NULL),
            NULL
        );

        e_DrawWindow = CreateWindow(
            L"Static",
            NULL,
            WS_DISABLED | WS_CHILD,
            150, 25, 200, 200,
            hwnd,
            NULL,
            GetModuleHandle(NULL),
            NULL
        );



        result = 1;
    }
    else
    {
        MainApp* pEditApp = reinterpret_cast<MainApp*>(static_cast<LONG_PTR>(
            ::GetWindowLongPtrW(
                hwnd,
                GWLP_USERDATA
            )));

        bool wasHandled = false;

        if (pEditApp)
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
                    if (LOWORD(wParam) == 1) // close menu
                    {
                        //editCalled = false;
                        EnableWindow(EditButton, TRUE);
                        ShowWindow(e_hwnd, SW_HIDE);
                        //SendMessage(m_hwnd, WM_COMMAND, 1, 0);
                    }
                    else if (LOWORD(wParam) == 2) // Load tree file
                    {
                        e_engine->LoadPicture(e_hwnd, 1);
                    }
                    else if (LOWORD(wParam) == 3) // Load fruit file
                    {
                        e_engine->LoadPicture(e_hwnd, 2);
                    }
                    else if (LOWORD(wParam) == 4) // Select component
                    {
                        //e_engine->SelectPicture();
                        //ComboBox_ResetContent(cbbox);
                        //getDirFile();
                        //SendMessage(cbbox, (UINT)CB_DELETESTRING, (WPARAM)0, (LPARAM)L"");
                    }
                    else if (LOWORD(wParam) == 5) // Save component
                    {
                        WCHAR* str = e_engine->SaveDialog(e_hwnd, false);
                        WCHAR str2[MAX_PATH] = L"";
                        for (int i = 0; i < wcslen(str); i++)
                        {
                            str2[i] = str[i];
                        }
                        if (wcslen(str)!=0) {
                            //e_engine->SavePicture(e_DrawWindow);
                            //getDirFile();
                            int idx = wcslen(str) - 1;
                            while (str[idx] != '\\')
                            {
                                idx--;
                            }
                            idx++;
                            WCHAR p[MAX_PATH] = L"";
                            for (int i = idx; i < wcslen(str); i++)
                            {
                                p[i - idx] = str[i];
                            }
                            if (SendMessage(cbbox, (UINT)CB_FINDSTRING, (WPARAM)(-1), (LPARAM)p) == CB_ERR)
                            {
                                SendMessage(cbbox, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)p);
                            }
                            ComboBox_SelectString(cbbox, -1, p);
                            wstring ws(str2);
                            string str1(ws.begin(), ws.end());
                            e_engine->ReadComponentJSON(str1, e_hwnd);
                            m_engine->fruittree = e_engine->fruittree;
                            m_engine->CopyComponentJSON(str1, m_hwnd);
                            m_engine->Draw(m_hwnd, 0, 0, 4);
                            e_engine->LoadPicture(e_hwnd, 5);
                            e_engine->SelectPicture(str2);
                            m_engine->SelectPicture(str2);
                            isChosen = true;
                            isDeleted = false;
                        }

                    }
                    else if (LOWORD(wParam) == 6) // Delete component
                    {
                        int ItemIndex = SendMessage((HWND)lParam, (UINT)CB_GETCURSEL, (WPARAM)0, (LPARAM)0);
                        WCHAR listItem[MAX_PATH];
                        ComboBox_GetText(cbbox, listItem, MAX_PATH);
                        //(WCHAR)SendMessage((HWND)lParam, (UINT)CB_GETLBTEXT, (WPARAM)ItemIndex, (LPARAM)listItem);
                        if (listItem[0] != 0) {
                            WCHAR path[MAX_PATH];
                            std::fill(path, path + MAX_PATH, 0);
                            GetModuleFileName(NULL, path, MAX_PATH);
                            PathRemoveFileSpec(path);
                            const WCHAR* comp = L"\\Component\\";
                            wcscat_s(path, comp);
                            wcscat_s(path, listItem);
                            wstring ws(path);
                            string str(ws.begin(), ws.end());
                            e_engine->DeleteComponent(e_hwnd, str, 1);
                            m_engine->DeleteComponent(m_hwnd, str, 2);
                            m_engine->DeleteComponentJSON(m_hwnd, str);
                            m_engine->Draw(m_hwnd, 0, 0, 4);
                            SendMessage(cbbox, (UINT)CB_DELETESTRING, (WPARAM)0, (LPARAM)listItem);
                            ComboBox_ResetContent(cbbox);
                            getDirFile();
                            isDeleted = true;
                        }
                    }
                }
                else if (HIWORD(wParam) == CBN_SELCHANGE)
                {
                    int ItemIndex = SendMessage((HWND)lParam, (UINT)CB_GETCURSEL, (WPARAM)0, (LPARAM)0);
                    WCHAR listItem[MAX_PATH];
                    (WCHAR)SendMessage((HWND)lParam, (UINT)CB_GETLBTEXT, (WPARAM)ItemIndex, (LPARAM)listItem);
                    //MessageBox(hwnd, (LPCWSTR)listItem, TEXT("Item Selected"), MB_OK);.
                    if (listItem[0] != 0 ) {
                        WCHAR path[MAX_PATH];
                        std::fill(path, path + MAX_PATH, 0);
                        GetModuleFileName(NULL, path, MAX_PATH);
                        PathRemoveFileSpec(path);
                        const WCHAR* comp = L"\\Component\\";
                        wcscat_s(path, comp);
                        wcscat_s(path, listItem);
                        wstring ws(path);
                        string str(ws.begin(), ws.end());
                        e_engine->ReadComponentJSON(str, e_hwnd);
                        m_engine->fruittree = e_engine->fruittree;
                        e_engine->LoadPicture(e_hwnd, 5);
                        e_engine->SelectPicture(path);
                        m_engine->SelectPicture(path);
                        isChosen = true;
                        isDeleted = false;
                    }
                }
            }
            result = 0;
            wasHandled = true;
            break;

            case WM_LBUTTONUP:
            {
                if (!isDeleted)
                {
                    int xPos = GET_X_LPARAM(lParam);
                    int yPos = GET_Y_LPARAM(lParam);
                    e_engine->Draw(e_hwnd, xPos, yPos, 2);
                }
            }
            result = 0;
            wasHandled = true;
            break;

            case WM_RBUTTONUP:
            {
                int xPos = GET_X_LPARAM(lParam);
                int yPos = GET_Y_LPARAM(lParam);
                e_engine->ClickDelete(e_hwnd, xPos-150, yPos-25, false);
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
