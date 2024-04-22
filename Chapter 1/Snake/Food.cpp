#include "framework.h"
#include "Point2D.h"
#include "Food.h"
#include "Engine.h"

Food::Food(HWND hwnd)
{
	
}

Food::~Food()
{

}

void Food::OnPaint(HWND hWnd, int xPos, int yPos, ID2D1Bitmap* m_pD2DBitmap, ID2D1HwndRenderTarget* m_pRT)
{
	m_pRT->SetTransform(D2D1::Matrix3x2F::Identity());

	// Clear the background
	//m_pRT->Clear(D2D1::ColorF(D2D1::ColorF::White));

	
	if (m_pD2DBitmap)
	{
		m_pRT->DrawBitmap(m_pD2DBitmap, D2D1::RectF(xPos, yPos, xPos + 10, yPos + 10));
	}

	return;
}

void Food::Initialize(ID2D1HwndRenderTarget* m_pRenderTarget)
{

}

void Food::Reset(Snake *snake)
{
	srand((unsigned)time(NULL));
	bool found = false;
	while (!found)
	{
		position.x = rand() % 760;
		position.y = rand() % 550;
		position.x -= position.x % 10;
		position.y -= position.y % 10;
		found = true;
		for (int i = 0; i < snake->length; i++)
		{
			if (snake->position[i].x == position.x && snake->position[i].y == position.y)
			{
				found = false;
			}
		}
	}
	return;
}

void Food::Draw(HWND hwnd, ID2D1Bitmap* m_pD2DBitmap_Food, ID2D1HwndRenderTarget* m_pRT)
{
	OnPaint(hwnd, position.x, position.y, m_pD2DBitmap_Food, m_pRT);
	return;
}