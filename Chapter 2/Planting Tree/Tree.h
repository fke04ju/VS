#pragma once
#include "framework.h"
#include "Point2D.h"
#include "Fruit.h"
#include "Image.h"

class Tree : public Image
{
public:
	Tree(HWND hwnd, WCHAR* path, ID2D1HwndRenderTarget* m_pRT);
	Tree();
	~Tree();
	/*
	void OnPaint(HWND hWnd, int xPos, int yPos, ID2D1Bitmap* m_pD2DBitmap, ID2D1HwndRenderTarget* m_pRT);
	void Initialize();
	void Reset();
	void Draw(HWND hwnd, ID2D1Bitmap* m_pD2DBitmap, ID2D1HwndRenderTarget* m_pRT);
	*/
	//Point2D nowPos;
	WCHAR TreeName[260];
	//vector<Fruit> FruitPos;

private:

};