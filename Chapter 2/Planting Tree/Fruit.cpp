#include "framework.h"
#include "Point2D.h"
#include "Fruit.h"
#include "Engine.h"

Fruit::Fruit(HWND hwnd, WCHAR *path, ID2D1HwndRenderTarget* m_pRT)
{
	wcscpy_s(FruitName, path);
	OpenImageFile(hwnd, path, m_pRT);
}

Fruit::Fruit()
{

}

Fruit::~Fruit()
{

}
/*
void Fruit::OnPaint(HWND hWnd, int xPos, int yPos, ID2D1Bitmap* m_pD2DBitmap, ID2D1HwndRenderTarget* m_pRT)
{
	m_pRT->SetTransform(D2D1::Matrix3x2F::Identity());

	// Clear the background
	//m_pRT->Clear(D2D1::ColorF(D2D1::ColorF::White));


	if (m_pD2DBitmap)
	{
		m_pRT->DrawBitmap(m_pD2DBitmap, D2D1::RectF(xPos, yPos, xPos + 20, yPos + 20));
	}

	return;
}

void Fruit::Initialize(ID2D1HwndRenderTarget* m_pRenderTarget)
{

}

void Fruit::Draw(HWND hwnd, ID2D1Bitmap* m_pD2DBitmap, ID2D1HwndRenderTarget* m_pRT)
{
	OnPaint(hwnd, position.x, position.y, m_pD2DBitmap, m_pRT);
	return;
}

*/