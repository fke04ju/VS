#pragma once
#include "Point2D.h"
#include "Image.h"


class Fruit : public Image
{
public:
	Fruit(HWND hwnd, WCHAR *path, ID2D1HwndRenderTarget* m_pRT);
	Fruit();
	~Fruit();
	/*
	void OnPaint(HWND hWnd, int xPos, int yPos, ID2D1Bitmap* m_pD2DBitmap, ID2D1HwndRenderTarget* m_pRT);
	void Initialize(ID2D1HwndRenderTarget* m_pRenderTarget);
	void Draw(HWND hwnd, ID2D1Bitmap* m_pD2DBitmap, ID2D1HwndRenderTarget* m_pRT);
	*/
	//Point2D position;
	WCHAR FruitName[260];

private:

};