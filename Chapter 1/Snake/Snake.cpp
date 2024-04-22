#include "framework.h"
#include "Point2D.h"
#include "Snake.h"
#include "Engine.h"

Snake::Snake(HWND hwnd)
{
	Reset();
}

Snake::~Snake() 
{
	
}

void Snake::OnPaint(HWND hWnd, int xPos, int yPos, ID2D1Bitmap* m_pD2DBitmap, ID2D1HwndRenderTarget* m_pRT, bool head)
{
	m_pRT->SetTransform(D2D1::Matrix3x2F::Identity());
	// Clear the background
	//m_pRT->Clear(D2D1::ColorF(D2D1::ColorF::White));

	if (m_pD2DBitmap)
	{
		D2D1_POINT_2F center = D2D1::Point2F(xPos + 5, yPos + 5);
		if (!head) {
			m_pRT->SetTransform(D2D1::Matrix3x2F::Rotation(0.0f, center));
		}
		else {
			if (rot == 1)
			{
				m_pRT->SetTransform(D2D1::Matrix3x2F::Rotation(0.0f, center));
			}
			else if (rot == 2)
			{
				m_pRT->SetTransform(D2D1::Matrix3x2F::Rotation(180.0f, center));
			}
			else if (rot == 3)
			{
				m_pRT->SetTransform(D2D1::Matrix3x2F::Rotation(270.0f, center));
			}
			else if (rot == 4)
			{
				m_pRT->SetTransform(D2D1::Matrix3x2F::Rotation(90.0f, center));
			}
		}
		m_pRT->DrawBitmap(m_pD2DBitmap, D2D1::RectF(xPos, yPos, xPos+10, yPos+10));
		//D2D1::RectF(xPos, yPos, rtBitmapSize.width + xPos, rtBitmapSize.height + yPos)
	}

	return;
}

void Snake::Initialize()
{

}

void Snake::Reset()
{
	length = 5;
	direction.x = 10;
	direction.y = 0;
	rot = 4;
	
	for (int i = 0; i < length; i++)
	{
		position[i].x = 200 - i*10;
		position[i].y = 100;
	}
	return;
}

void Snake::GoUp()
{
	if(direction.y == 0)
	{
		direction.x = 0;
		direction.y = -10;
		rot = 1;
	}
}

void Snake::GoDown()
{
	if (direction.y == 0)
	{
		direction.x = 0;
		direction.y = 10;
		rot = 2;
	}
}

void Snake::GoLeft()
{
	if (direction.x == 0)
	{
		direction.x = -10;
		direction.y = 0;
		rot = 3;
	}
}

void Snake::GoRight()
{
	if (direction.x == 0)
	{
		direction.x = 10;
		direction.y = 0;
		rot = 4;
	}
}

void Snake::Advance()
{
	for (int i = length - 1; i > 0; i--) 
	{
		position[i].x = position[i - 1].x;
		position[i].y = position[i - 1].y;
	}
	position[0].x = position[0].x + direction.x; 
	position[0].y = position[0].y + direction.y;
}

bool Snake::CheckSelfCollision()
{
	for (int i = 1; i < length; i++) 
	{
		if (position[0].x == position[i].x && position[0].y == position[i].y)
		{
			// collision to self
			return true;
		}
	}
	// collision to wall
	if (position[0].x+5 <= 0) return true;
	if (position[0].y+5 <= 0) return true;
	if (position[0].x+5 >= 770)
		return true;
	if (position[0].y+5 >= 560)
		return true;
	return false;
}

bool Snake::CheckFoodCollision(int x, int y)
{
	if (position[0].x == x && position[0].y == y)
	{
		return true;
	}
	return false;
}

void Snake::Grow()
{
	position[length].x = position[length - 1].x;
	position[length].y = position[length - 1].y;
	length++;
	return;
}

void Snake::Draw(HWND hwnd, ID2D1Bitmap* m_pD2DBitmap_Body, ID2D1Bitmap* m_pD2DBitmap_Head, ID2D1HwndRenderTarget* m_pRT)
{
	for (int i = 1; i < length; i++)
	{
		OnPaint(hwnd, position[i].x, position[i].y, m_pD2DBitmap_Body, m_pRT, false);
	}
	OnPaint(hwnd, position[0].x, position[0].y, m_pD2DBitmap_Head, m_pRT, true);
	
	return;
}