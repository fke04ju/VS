#pragma once

#include "Point2D.h"
#include "Snake.h"
#include "Food.h"

#define RESOLUTION_X 800
#define RESOLUTION_Y 600
#define CELL_SIZE 5
#define FRAME_SLEEP 100

class Engine 
{
public:
	Engine(HWND hwnd);
	~Engine();

	HRESULT CreateDeviceResources(HWND hWnd);
	BOOL LocateImageFile(HWND hWnd, LPWSTR pszFileName, DWORD cchFileName);
	HRESULT OpenImageFile(HWND hWnd, LPCWSTR lpszFilePath, ID2D1Bitmap** m_pD2DBitmap);
	HRESULT InitializeD2D(HWND m_hwnd);
	void KeyUP(WPARAM wParam);
	void Reset();
	void Logic(HWND m_hwnd, int *state, int speed, HWND start_button, HWND restart_button);
	void Draw(HWND m_hwnd, double elapsed_secs, HWND start_button, HWND restart_button);
	ID2D1HwndRenderTarget* m_pRT;
	const FLOAT DEFAULT_DPI = 96.f;
	IWICImagingFactory* m_pIWICFactory;
	ID2D1Factory* m_pD2DFactory;
	ID2D1Bitmap* m_pD2DBitmap_Body;
	ID2D1Bitmap* m_pD2DBitmap_Head;
	ID2D1Bitmap* m_pD2DBitmap_Food;
	IWICFormatConverter* m_pConvertedSourceBitmap;
	WCHAR SnakeHead[MAX_PATH];
	WCHAR SnakeBody[MAX_PATH];
	WCHAR FoodBody[MAX_PATH];

private:
	Snake* snake;
	Food* food;

	int score;
	int highscore;
	bool playing;
	bool keyPressed;
	int frame;
	double timethrough;
	int fps;
	int engine_state;
};