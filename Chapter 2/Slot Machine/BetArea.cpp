#include "framework.h"
#include "BetArea.h"

ID2D1SolidColorBrush* b_pRedBrush;
ID2D1SolidColorBrush* b_pWhiteBrush;
ID2D1SolidColorBrush* b_pBlackBrush;
ID2D1SolidColorBrush* b_pDarkRedBrush;
ID2D1SolidColorBrush* b_pYellowBrush;

BetArea::BetArea(ID2D1Factory* m_pD2DFactory, ID2D1HwndRenderTarget* m_pRT, IWICFormatConverter* m_pConvertedSourceBitmap, IDWriteFactory* m_pWriteFactory, IDWriteTextFormat* m_pWriteTextFormat, IDWriteTextFormat* m_pScoreTextFormat)
{
	b_pD2DFactory = m_pD2DFactory;
	b_pRT = m_pRT;
	b_pConvertedSourceBitmap = m_pConvertedSourceBitmap;
	b_pWriteFactory = m_pWriteFactory;
	b_pWriteTextFormat = m_pWriteTextFormat;
	b_pScoreTextFormat = m_pScoreTextFormat;
	area.resize(28);
	area[0] = { 810, 200 };
	area[1] = { 910, 200 };
	area[2] = { 1010, 200 };
	area[3] = { 1110, 200 };
	area[4] = { 810, 500 };
	area[5] = { 910, 500 };
	area[6] = { 1010, 500 };
	area[7] = { 1110, 500 };
	light.resize(8);
	light[0] = { 860, 200 };
	light[1] = { 960, 200 };
	light[2] = { 1060, 200 };
	light[3] = { 1160, 200 };
	light[4] = { 860, 500 };
	light[5] = { 960, 500 };
	light[6] = { 1060, 500 };
	light[7] = { 1160, 500 };

	b_pRT->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), &b_pRedBrush);
	b_pRT->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &b_pWhiteBrush);
	b_pRT->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &b_pBlackBrush);
	b_pRT->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::DarkRed), &b_pDarkRedBrush);
	b_pRT->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Yellow), &b_pYellowBrush);

	betAreaLight.resize(8);
	pressLight.resize(8);
	for (int i = 0; i < 8; i++)
	{
		betAreaLight[i] = false;
		pressLight[i] = -1;
	}
}

BetArea::~BetArea()
{

}

void BetArea::Draw(HWND hwnd) // draw bet area
{
	WCHAR wscore[10] = L"";
	for (int i = 0; i < 7; i++)
	{
		b_pRT->DrawRectangle(D2D1::RectF(area[i].x, area[i].y - 100, area[i].x + 100, area[i].y), b_pBlackBrush, 0.5f);
		b_pRT->FillRectangle(D2D1::RectF(area[i].x, area[i].y - 100, area[i].x + 100, area[i].y), b_pWhiteBrush);
		b_pRT->DrawRectangle(D2D1::RectF(area[i].x, area[i].y, area[i].x + 100, area[i].y + 100), b_pBlackBrush, 0.5f);
		b_pRT->FillRectangle(D2D1::RectF(area[i].x, area[i].y, area[i].x + 100, area[i].y + 100), (betAreaLight[i]) ? b_pYellowBrush : b_pWhiteBrush);
		componentSet[i]->Draw(hwnd, area[i].x, area[i].y, 100, 100, b_pRT);
		b_pRT->FillEllipse(D2D1::Ellipse(D2D1::Point2F(area[i].x + 50, area[i].y), 10, 10), b_pDarkRedBrush);
		b_pRT->FillEllipse(D2D1::Ellipse(D2D1::Point2F(light[i].x, light[i].y), 10, 10), (betAreaLight[i]) ? b_pRedBrush : b_pDarkRedBrush);

		if (pressLight[i] == 1)
		{
			b_pRT->FillRectangle(D2D1::RectF(area[i].x, area[i].y + 100, area[i].x + 50, area[i].y + 150), b_pYellowBrush);
			b_pRT->FillRectangle(D2D1::RectF(area[i].x + 50, area[i].y + 100, area[i].x + 100, area[i].y + 150), b_pWhiteBrush);
			b_pRT->FillRectangle(D2D1::RectF(area[i].x, area[i].y + 150, area[i].x + 100, area[i].y + 200), b_pWhiteBrush);
		}
		else if(pressLight[i] == 10)
		{
			b_pRT->FillRectangle(D2D1::RectF(area[i].x, area[i].y + 100, area[i].x + 50, area[i].y + 150), b_pWhiteBrush);
			b_pRT->FillRectangle(D2D1::RectF(area[i].x + 50, area[i].y + 100, area[i].x + 100, area[i].y + 150), b_pYellowBrush);
			b_pRT->FillRectangle(D2D1::RectF(area[i].x, area[i].y + 150, area[i].x + 100, area[i].y + 200), b_pWhiteBrush);
		}
		else if (pressLight[i] == 0)
		{
			b_pRT->FillRectangle(D2D1::RectF(area[i].x, area[i].y + 100, area[i].x + 50, area[i].y + 150), b_pWhiteBrush);
			b_pRT->FillRectangle(D2D1::RectF(area[i].x + 50, area[i].y + 100, area[i].x + 100, area[i].y + 150), b_pWhiteBrush);
			b_pRT->FillRectangle(D2D1::RectF(area[i].x, area[i].y + 150, area[i].x + 100, area[i].y + 200), b_pYellowBrush);
		}
		else
		{
			b_pRT->FillRectangle(D2D1::RectF(area[i].x, area[i].y + 100, area[i].x + 50, area[i].y + 150), b_pWhiteBrush);
			b_pRT->FillRectangle(D2D1::RectF(area[i].x + 50, area[i].y + 100, area[i].x + 100, area[i].y + 150), b_pWhiteBrush);
			b_pRT->FillRectangle(D2D1::RectF(area[i].x, area[i].y + 150, area[i].x + 100, area[i].y + 200), b_pWhiteBrush);
		}
		b_pRT->DrawRectangle(D2D1::RectF(area[i].x, area[i].y + 100, area[i].x + 50, area[i].y + 150), b_pBlackBrush, 0.5f);
		b_pRT->DrawRectangle(D2D1::RectF(area[i].x + 50, area[i].y + 100, area[i].x + 100, area[i].y + 150), b_pBlackBrush, 0.5f);
		b_pRT->DrawRectangle(D2D1::RectF(area[i].x, area[i].y + 150, area[i].x + 100, area[i].y + 200), b_pBlackBrush, 0.5f);

		swprintf_s(wscore, L"%d", 1);
		b_pRT->DrawTextW(wscore, wcslen(wscore), b_pWriteTextFormat, D2D1::RectF(area[i].x, area[i].y + 100, area[i].x + 50, area[i].y + 150), b_pBlackBrush);
		swprintf_s(wscore, L"%d", 10);
		b_pRT->DrawTextW(wscore, wcslen(wscore), b_pWriteTextFormat, D2D1::RectF(area[i].x + 50, area[i].y + 100, area[i].x + 100, area[i].y + 150), b_pBlackBrush);
		swprintf_s(wscore, L"%s", L"clear");
		b_pRT->DrawTextW(wscore, wcslen(wscore), b_pWriteTextFormat, D2D1::RectF(area[i].x, area[i].y + 150, area[i].x + 100, area[i].y + 200), b_pBlackBrush);
	}
}

void BetArea::ClearLight()
{
	for (int i = 0; i < 7; i++)
	{
		betAreaLight[i] = false;
	}
}

void BetArea::Release()
{
	num = -1;
	isOne = false;
	isTen = false;
	isClear = false;
	for (int i = 0; i < 7; i++)
	{
		pressLight[i] = -1;
	}
}

void BetArea::Press(int xPos, int yPos)
{
	for (int i = 0; i < 7; i++)
	{
		if (xPos > area[i].x + 2 && xPos < area[i].x + 98 && yPos > area[i].y + 102 && yPos < area[i].y + 198)
		{
			num = i;
			if (xPos > area[i].x + 2 && xPos < area[i].x + 48 && yPos > area[i].y + 102 && yPos < area[i].y + 148)
			{
				isOne = true;
			}
			else if (xPos > area[i].x + 52 && xPos < area[i].x + 98 && yPos > area[i].y + 102 && yPos < area[i].y + 148)
			{
				isTen = true;
			}
			else if(xPos > area[i].x + 2 && xPos < area[i].x + 98 && yPos > area[i].y + 152 && yPos < area[i].y + 198)
			{
				isClear = true;
			}
		}
	}
	if (num >= 0)
	{
		if (isOne)
		{
			pressLight[num] = 1;
		}
		if (isTen)
		{
			pressLight[num] = 10;
		}
		if (isClear)
		{
			pressLight[num] = 0;
		}
	}
}

void BetArea::ChangeState(Instruction ins)
{
	if (ins.lightNumber >= 0 && ins.lightNumber < 7)
	{
		betAreaLight[ins.lightNumber] = ins.Lightening;
	}
}