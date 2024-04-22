#include "framework.h"
#include "Point2D.h"
#include "Engine.h"

#pragma comment(lib, "d2d1")
#pragma comment(lib, "dwrite")

Engine::Engine(HWND hwnd)
{
	wcscpy_s(SnakeBody, L"snakebody.png");
	wcscpy_s(SnakeHead, L"snakehead.png");
	wcscpy_s(FoodBody, L"foodbody.png");
	HRESULT hr = S_OK;
	// Create WIC factory
	hr = CoCreateInstance(
		CLSID_WICImagingFactory,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(&m_pIWICFactory)
	);

	if (SUCCEEDED(hr))
	{
		// Create D2D factory
		hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pD2DFactory);
	}

	if (SUCCEEDED(hr))
	{
		hr = CreateDeviceResources(hwnd);
	}

	OpenImageFile(hwnd, FoodBody, &m_pD2DBitmap_Food);
	OpenImageFile(hwnd, SnakeBody, &m_pD2DBitmap_Body);
	OpenImageFile(hwnd, SnakeHead, &m_pD2DBitmap_Head);


	snake = new Snake(hwnd);
	food = new Food(hwnd);
	food->Reset(snake);

	playing = true;
	keyPressed = false;
	score = 5;
	highscore = 5;
	frame = 0;
	timethrough = 0.0f;
	fps = 0;
	engine_state = 0;
	return;
}

Engine::~Engine()
{
	SafeRelease(m_pD2DBitmap_Head);
	SafeRelease(m_pD2DBitmap_Body);
	SafeRelease(m_pD2DBitmap_Food);
	SafeRelease(m_pConvertedSourceBitmap);
	SafeRelease(m_pIWICFactory);
	SafeRelease(m_pD2DFactory);
	SafeRelease(m_pRT);
}

HRESULT Engine::CreateDeviceResources(HWND hWnd)
{
	HRESULT hr = S_OK;

	if (!m_pRT)
	{
		RECT rc;
		hr = GetClientRect(hWnd, &rc) ? S_OK : E_FAIL;

		if (SUCCEEDED(hr))
		{
			// Create a D2D render target properties
			D2D1_RENDER_TARGET_PROPERTIES renderTargetProperties = D2D1::RenderTargetProperties();

			// Set the DPI to be the default system DPI to allow direct mapping
			// between image pixels and desktop pixels in different system DPI settings
			renderTargetProperties.dpiX = DEFAULT_DPI;
			renderTargetProperties.dpiY = DEFAULT_DPI;

			// Create a D2D render target
			D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);

			hr = m_pD2DFactory->CreateHwndRenderTarget(
				renderTargetProperties,
				D2D1::HwndRenderTargetProperties(hWnd, size),
				&m_pRT
			);
		}
	}

	return hr;
}

BOOL Engine::LocateImageFile(HWND hWnd, LPWSTR pszFileName, DWORD cchFileName)
{
	pszFileName[0] = L'\0';

	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(ofn));

	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFilter = L"All Image Files\0"
		L"*.bmp;*.dib;*.wdp;*.mdp;*.hdp;*.gif;*.png;*.jpg;*.jpeg;*.tif;*.ico;*.webp\0"
		L"Windows Bitmap\0"               L"*.bmp;*.dib\0"
		L"High Definition Photo\0"        L"*.wdp;*.mdp;*.hdp\0"
		L"Graphics Interchange Format\0"  L"*.gif\0"
		L"Portable Network Graphics\0"    L"*.png\0"
		L"JPEG File Interchange Format\0" L"*.jpg;*.jpeg\0"
		L"Tiff File\0"                    L"*.tif\0"
		L"Icon\0"                         L"*.ico\0"
		L"All Files\0"                    L"*.*\0"
		L"\0";
	ofn.lpstrFile = pszFileName;
	ofn.nMaxFile = cchFileName;
	ofn.lpstrTitle = L"Open Image";
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

	// Display the Open dialog box. 
	return GetOpenFileName(&ofn);
}

HRESULT Engine::OpenImageFile(HWND hWnd, LPCWSTR lpszFilePath, ID2D1Bitmap** m_pD2DBitmap)
{
	HRESULT hr = S_OK;

	// Step 1: Create the open dialog box and locate the image file
	if (PathFileExists(lpszFilePath))
	{
		// Step 2: Decode the source image

		// Create a decoder
		IWICBitmapDecoder* pDecoder = NULL;

		hr = m_pIWICFactory->CreateDecoderFromFilename(
			lpszFilePath,                      // Image to be decoded
			NULL,                            // Do not prefer a particular vendor
			GENERIC_READ,                    // Desired read access to the file
			WICDecodeMetadataCacheOnDemand,  // Cache metadata when needed
			&pDecoder                        // Pointer to the decoder
		);

		// Retrieve the first frame of the image from the decoder
		IWICBitmapFrameDecode* pFrame = NULL;

		if (SUCCEEDED(hr))
		{
			hr = pDecoder->GetFrame(0, &pFrame);
		}

		//Step 3: Format convert the frame to 32bppPBGRA
		if (SUCCEEDED(hr))
		{
			SafeRelease(m_pConvertedSourceBitmap);
			hr = m_pIWICFactory->CreateFormatConverter(&m_pConvertedSourceBitmap);
		}

		if (SUCCEEDED(hr))
		{
			hr = m_pConvertedSourceBitmap->Initialize(
				pFrame,                          // Input bitmap to convert
				GUID_WICPixelFormat32bppPBGRA,   // Destination pixel format
				WICBitmapDitherTypeNone,         // Specified dither pattern
				NULL,                            // Specify a particular palette 
				0.f,                             // Alpha threshold
				WICBitmapPaletteTypeCustom       // Palette translation type
			);
		}

		//Step 4: Create render target and D2D bitmap from IWICBitmapSource
		if (SUCCEEDED(hr))
		{
			//hr = CreateDeviceResources(hWnd);
		}


		if (SUCCEEDED(hr))
		{
			// Need to release the previous D2DBitmap if there is one
			SafeRelease(*m_pD2DBitmap);
			hr = m_pRT->CreateBitmapFromWicBitmap(m_pConvertedSourceBitmap, NULL, m_pD2DBitmap);
		}

		SafeRelease(pDecoder);
		SafeRelease(pFrame);

		//InvalidateRect(hWnd, 0, TRUE);
	}

	return hr;
}

HRESULT Engine::InitializeD2D(HWND m_hwnd)
{
	return S_OK;
}

void Engine::KeyUP(WPARAM wParam)
{
	if (!keyPressed)
	{
		if (wParam == VK_UP) 
		{
			snake->GoUp();
		}
		if (wParam == VK_DOWN)
		{
			snake->GoDown();
		}
		if (wParam == VK_LEFT)
		{
			snake->GoLeft();
		}
		if (wParam == VK_RIGHT)
		{
			snake->GoRight();
		}
		keyPressed = true;
	}
}

void Engine::Reset()
{
	if (!playing)
	{
		snake->Reset();
		food->Reset(snake);
		playing = true;
		score = 5;
	}
	return;
}

void Engine::Logic(HWND m_hwnd, int *state, int speed, HWND start_button, HWND restart_button)
{
	engine_state = *state;
	if (engine_state == 1)
	{
		if (playing)
		{
			frame++;
			if (frame == speed)
			{
				snake->Advance();

				if (snake->CheckFoodCollision(food->position.x, food->position.y))
				{
					food->Reset(snake);
					snake->Grow();
					score++;
					if (score > highscore)
					{
						highscore = score;
					}
				}
				if (snake->CheckSelfCollision())
				{
					playing = false;
					engine_state = 2;
					*state = 2;
				}
				keyPressed = false;
				frame = 0;
			}
		}
	}
	
	return;
	//Sleep(FRAME_SLEEP);
}

void Engine::Draw(HWND m_hwnd, double elapsed_secs, HWND start_button, HWND restart_button)
{
	timethrough += elapsed_secs;
	fps++;
	if (timethrough >= 1.0f)
	{
		// display fps
		WCHAR fpsText[32];
		swprintf(fpsText, 32, L"Game: %d FPS", fps);
		SetWindowText(m_hwnd, fpsText);
		timethrough = 0.0f;
		fps = 0;
	}

	if (engine_state == 0)
	{
		ShowWindow(start_button, SW_SHOW);
		return;
	}
	if (engine_state == 1)
	{
		ShowWindow(start_button, SW_HIDE);
		ShowWindow(restart_button, SW_HIDE);
	}
	if (engine_state == 2)
	{
		ShowWindow(restart_button, SW_SHOW);
		return;
	}

	HRESULT hr = S_OK;
	PAINTSTRUCT ps;
	// Create render target if not yet created


	if (BeginPaint(m_hwnd, &ps)) {
		hr = CreateDeviceResources(m_hwnd);

		if (SUCCEEDED(hr) && !(m_pRT->CheckWindowState() & D2D1_WINDOW_STATE_OCCLUDED))
		{
			m_pRT->BeginDraw();
			m_pRT->Clear(D2D1::ColorF(D2D1::ColorF::White));
			if (engine_state == 1)
			{
				snake->Draw(m_hwnd, m_pD2DBitmap_Body, m_pD2DBitmap_Head, m_pRT);
				food->Draw(m_hwnd, m_pD2DBitmap_Food, m_pRT);
			}
		}

		hr = m_pRT->EndDraw();

		// In case of device loss, discard D2D render target and D2DBitmap
		// They will be re-create in the next rendering pass
		if (hr == D2DERR_RECREATE_TARGET)
		{
			SafeRelease(m_pD2DBitmap_Head);
			SafeRelease(m_pD2DBitmap_Body);
			SafeRelease(m_pD2DBitmap_Food);
			SafeRelease(m_pRT);
			// Force a re-render
		}
		hr = InvalidateRect(m_hwnd, NULL, TRUE) ? S_OK : E_FAIL;
	}
	
	EndPaint(m_hwnd, &ps);
	return;
}