#pragma once
#include "Point2D.h"
#include "Snake.h"
/*
const FLOAT DEFAULT_DPI = 96.f;
static IWICImagingFactory* m_pIWICFactory;
static ID2D1Factory* m_pD2DFactory;
static ID2D1HwndRenderTarget* m_pRT;
static ID2D1Bitmap* m_pD2DBitmap;
static IWICFormatConverter* m_pConvertedSourceBitmap;
*/
class Food
{
public:
	Food(HWND hwnd);
	~Food();

	void OnPaint(HWND hWnd, int xPos, int yPos, ID2D1Bitmap* m_pD2DBitmap, ID2D1HwndRenderTarget* m_pRT);
	void Initialize(ID2D1HwndRenderTarget* m_pRenderTarget);
	void Reset(Snake* snake);
	void Draw(HWND hwnd, ID2D1Bitmap* m_pD2DBitmap_Food, ID2D1HwndRenderTarget* m_pRTs);

	Point2D position;

private:

};