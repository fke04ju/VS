#pragma once
#include "Point2D.h"

class Image
{
public:
	Image();
	~Image();

	HRESULT OpenImageFile(HWND hWnd, LPCWSTR lpszFilePath, ID2D1HwndRenderTarget* m_pRT);
	void OnPaint(HWND hWnd, int xPos, int yPos, ID2D1HwndRenderTarget* mp_RT);
	void Draw(HWND hwnd, int xPos, int yPos, int xSize, int ySize, ID2D1HwndRenderTarget* m_pRT);
	ID2D1Bitmap* m_pD2DBitmap;
private:

};