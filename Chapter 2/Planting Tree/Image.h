#pragma once

#include "Point2D.h"


class Image
{
public:
	Image();
	~Image();

	//HRESULT CreateDeviceResources(HWND hWnd, ID2D1HwndRenderTarget* m_pRT, ID2D1Bitmap* m_pD2DBitmap);
	BOOL LocateImageFile(HWND hWnd, LPWSTR pszFileName, DWORD cchFileName);
	HRESULT OpenImageFile(HWND hWnd, LPCWSTR lpszFilePath, ID2D1HwndRenderTarget* m_pRT);
	void OnPaint(HWND hWnd, int xPos, int yPos, ID2D1HwndRenderTarget* mp_RT);
	void Draw(HWND hwnd, int xPos, int yPos, int size, ID2D1HwndRenderTarget* m_pRT);
	//const FLOAT DEFAULT_DPI = 96.f;
	ID2D1Bitmap* m_pD2DBitmap;


private:

};