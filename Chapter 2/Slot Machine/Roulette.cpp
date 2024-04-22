#include "framework.h"
#include "Roulette.h"

ID2D1SolidColorBrush* r_pRedBrush;
ID2D1SolidColorBrush* r_pWhiteBrush;
ID2D1SolidColorBrush* r_pBlackBrush;
ID2D1SolidColorBrush* r_pDarkRedBrush;
ID2D1SolidColorBrush* r_pYellowBrush;

Roulette::Roulette(ID2D1Factory* m_pD2DFactory, ID2D1HwndRenderTarget* m_pRT, IWICFormatConverter* m_pConvertedSourceBitmap, IDWriteFactory* m_pWriteFactory, IDWriteTextFormat* m_pWriteTextFormat, IDWriteTextFormat* m_pScoreTextFormat)
{
	roulettePicture.resize(28);
	r_pD2DFactory = m_pD2DFactory;
	r_pRT = m_pRT;
	r_pConvertedSourceBitmap = m_pConvertedSourceBitmap;
	r_pWriteFactory = m_pWriteFactory;
	r_pWriteTextFormat = m_pWriteTextFormat;
	r_pScoreTextFormat = m_pScoreTextFormat;
	area.resize(28);
	area[0] = { 0, 0 };
	area[1] = { 0, 100 };
	area[2] = { 0, 200 };
	area[3] = { 0, 300 };
	area[4] = { 0, 400 };
	area[5] = { 0, 500 };
	area[6] = { 0, 600 };
	area[7] = { 0, 700 };
	area[8] = { 100, 700 };
	area[9] = { 200, 700 };
	area[10] = { 300, 700 };
	area[11] = { 400, 700 };
	area[12] = { 500, 700 };
	area[13] = { 600, 700 };
	area[14] = { 700, 700 };
	area[15] = { 700, 600 };
	area[16] = { 700, 500 };
	area[17] = { 700, 400 };
	area[18] = { 700, 300 };
	area[19] = { 700, 200 };
	area[20] = { 700, 100 };
	area[21] = { 700, 0 };
	area[22] = { 600, 0 };
	area[23] = { 500, 0 };
	area[24] = { 400, 0 };
	area[25] = { 300, 0 };
	area[26] = { 200, 0 };
	area[27] = { 100, 0 };
	light.resize(28);
	light[0] = { 100, 100 };
	light[1] = { 100, 150 };
	light[2] = { 100, 250 };
	light[3] = { 100, 350 };
	light[4] = { 100, 450 };
	light[5] = { 100, 550 };
	light[6] = { 100, 650 };
	light[7] = { 100, 700 };
	light[8] = { 150, 700 };
	light[9] = { 250, 700 };
	light[10] = { 350, 700 };
	light[11] = { 450, 700 };
	light[12] = { 550, 700 };
	light[13] = { 650, 700 };
	light[14] = { 700, 700 };
	light[15] = { 700, 650 };
	light[16] = { 700, 550 };
	light[17] = { 700, 450 };
	light[18] = { 700, 350 };
	light[19] = { 700, 250 };
	light[20] = { 700, 150 };
	light[21] = { 700, 100 };
	light[22] = { 650, 100 };
	light[23] = { 550, 100 };
	light[24] = { 450, 100 };
	light[25] = { 350, 100 };
	light[26] = { 250, 100 };
	light[27] = { 150, 100 };
	CurrentPoint = { 150, 200 };
	TotalPoint = { 450, 200 };
	LargeSmallPoint = { 375, 500 };
	LargeLight = { 325, 515 };
	SmallLight = { 475, 515 };
	r_pRT->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), &r_pRedBrush);
	r_pRT->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &r_pWhiteBrush);
	r_pRT->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &r_pBlackBrush);
	r_pRT->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::DarkRed), &r_pDarkRedBrush);
	r_pRT->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Yellow), &r_pYellowBrush);

	rouletteAreaLight.resize(28);
	for (int i = 0; i < 28; i++)
	{
		rouletteAreaLight[i] = 0;
	}
	largeLight = false;
	smallLight = false;
}

Roulette::~Roulette()
{

}

void Roulette::Initialize()
{

}

void Roulette::ClearLight()
{
	for (int i = 0; i < 28; i++)
	{
		rouletteAreaLight[i] = false;
	}
}

void Roulette::Draw(HWND hwnd) // draw roulette and large&small area
{
	r_pRT->FillRectangle(D2D1::RectF(CurrentPoint.x, CurrentPoint.y, CurrentPoint.x + 200, CurrentPoint.y + 35), r_pBlackBrush);
	r_pRT->FillRectangle(D2D1::RectF(TotalPoint.x, TotalPoint.y, TotalPoint.x + 200, TotalPoint.y + 35), r_pBlackBrush);
	r_pRT->FillRectangle(D2D1::RectF(LargeSmallPoint.x - 10, LargeSmallPoint.y, LargeSmallPoint.x + 60, LargeSmallPoint.y + 35), r_pBlackBrush);
	for (int i = 0; i < 28; i++)
	{
		r_pRT->DrawRectangle(D2D1::RectF(area[i].x, area[i].y, area[i].x + 100, area[i].y + 100), r_pBlackBrush, 0.5f);
		r_pRT->FillRectangle(D2D1::RectF(area[i].x, area[i].y, area[i].x + 100, area[i].y + 100), (rouletteAreaLight[i]) ? r_pYellowBrush : r_pWhiteBrush);
		if (roulettePicture[i] >= 0 && roulettePicture[i] <= 7)
		{
			componentSet[roulettePicture[i]]->Draw(hwnd, area[i].x, area[i].y, 100, 100, r_pRT);
		}
		else
		{
			specialComponentSet[roulettePicture[i] - 100]->Draw(hwnd, area[i].x, area[i].y, 100, 100, r_pRT);
		}
	}
	for (int i = 0; i < 28; i++)
	{
		r_pRT->FillEllipse(D2D1::Ellipse(D2D1::Point2F(light[i].x, light[i].y), 10, 10), (rouletteAreaLight[i]) ? r_pRedBrush : r_pDarkRedBrush);
	}
	r_pRT->FillEllipse(D2D1::Ellipse(D2D1::Point2F(LargeLight.x, LargeLight.y), 10, 10), (largeLight) ? r_pRedBrush : r_pDarkRedBrush);
	r_pRT->FillEllipse(D2D1::Ellipse(D2D1::Point2F(SmallLight.x, SmallLight.y), 10, 10), (smallLight) ? r_pRedBrush : r_pDarkRedBrush);
}

void Roulette::ChangeState(Instruction ins)
{
	rouletteAreaLight[ins.lightNumber] = ins.Lightening;
}

void Roulette::ChangeLargeSmallState(Instruction ins)
{
	if (ins.RorB == 3)
	{
		largeLight = true;
		smallLight = false;
	}
	else if (ins.RorB == 4)
	{
		largeLight = false;
		smallLight = true;
	}
}
