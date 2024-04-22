#pragma once
#include "framework.h"
#include "Point2D.h"
class Snake
{
public:
	Snake(HWND hwnd);
	~Snake();

	void OnPaint(HWND hWnd, int xPos, int yPos, ID2D1Bitmap* m_pD2DBitmap, ID2D1HwndRenderTarget* m_pRT, bool head);
	void Initialize();
	void GoUp();
	void GoDown();
	void GoLeft();
	void GoRight();
	void Reset();
	void Advance();
	bool CheckSelfCollision();
	bool CheckFoodCollision(int x, int y);
	void Grow();
	void Draw(HWND hwnd, ID2D1Bitmap* m_pD2DBitmap_Body, ID2D1Bitmap* m_pD2DBitmap_Head, ID2D1HwndRenderTarget* m_pRT);
	
	Point2D position[5000];
	Point2D direction;
	int length;
	int rot;

private:

};