#include "framework.h"
#include "Point2D.h"
#include "Tree.h"
#include "Engine.h"

Tree::Tree(HWND hwnd, WCHAR *path, ID2D1HwndRenderTarget* m_pRT)
{
	wcscpy_s(TreeName, path);
	OpenImageFile(hwnd, path, m_pRT);
}

Tree::Tree()
{

}

Tree::~Tree()
{

}
/*
void Tree::OnPaint(HWND hWnd, int xPos, int yPos, ID2D1Bitmap* m_pD2DBitmap, ID2D1HwndRenderTarget* m_pRT)
{
	m_pRT->SetTransform(D2D1::Matrix3x2F::Identity());

	// Clear the background
	//m_pRT->Clear(D2D1::ColorF(D2D1::ColorF::White));

	if (m_pD2DBitmap)
	{
		m_pRT->DrawBitmap(m_pD2DBitmap, D2D1::RectF(xPos, yPos, xPos + 200, yPos + 200));
	}

	return;
}

void Tree::Initialize()
{

}

void Tree::Draw(HWND hwnd, ID2D1Bitmap* m_pD2DBitmap, ID2D1HwndRenderTarget* m_pRT)
{
	OnPaint(hwnd, nowPos.x, nowPos.y, m_pD2DBitmap, m_pRT);
	return;
}

void Tree::Reset()
{
	FruitPos.clear();
}
*/