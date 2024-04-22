#pragma once
#include "Point2D.h"
#include "Component.h"

class Roulette
{
public:
	Roulette(ID2D1Factory* m_pD2DFactory, ID2D1HwndRenderTarget* m_pRT, IWICFormatConverter* m_pConvertedSourceBitmap, IDWriteFactory* m_pWriteFactory, IDWriteTextFormat* m_pWriteTextFormat, IDWriteTextFormat* m_pScoreTextFormat);
	~Roulette();

	void Initialize();
	void ClearLight();
	void Draw(HWND hwnd);
	void ChangeState(Instruction ins);
	void ChangeLargeSmallState(Instruction ins);

	vector<Component*> componentSet;
	vector<Component*> specialComponentSet;
	Component *Background;
	Component *RouletteBackground;
	vector<int> roulettePicture;
	vector<Point2D> area;
	vector<Point2D> light;
	Point2D CurrentPoint, TotalPoint, LargeSmallPoint;
	Point2D LargeLight, SmallLight;
	vector<bool> rouletteAreaLight;
	bool largeLight;
	bool smallLight;
	int rouletteWinner;
	int compareWinner;

	ID2D1Factory* r_pD2DFactory;
	ID2D1HwndRenderTarget* r_pRT;
	IWICFormatConverter* r_pConvertedSourceBitmap;
	IDWriteFactory* r_pWriteFactory;
	IDWriteTextFormat* r_pWriteTextFormat;
	IDWriteTextFormat* r_pScoreTextFormat;

private:
};