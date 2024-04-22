#pragma once

#include "Point2D.h"
const FLOAT DEFAULT_DPI = 96.f;
static IWICImagingFactory* m_pIWICFactory;
static ID2D1Factory* m_pD2DFactory;
static ID2D1HwndRenderTarget* m_pRT;
static ID2D1Bitmap* m_pD2DBitmap;
static IWICFormatConverter* m_pConvertedSourceBitmap;

class Image
{
public:
	Image();
	~Image();

	HRESULT CreateDeviceResources(HWND hWnd);
	BOOL LocateImageFile(HWND hWnd, LPWSTR pszFileName, DWORD cchFileName);
	HRESULT OpenImageFile(HWND hWnd, LPCWSTR lpszFilePath);
	static void OnPaint(HWND hWnd, int xPos, int yPos);
	WCHAR SnakeHead[MAX_PATH];
	WCHAR SnakeBody[MAX_PATH];
	WCHAR FoodBody[MAX_PATH];

private:

};