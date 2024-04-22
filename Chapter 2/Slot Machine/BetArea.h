#pragma once
#include "Point2D.h"
#include "Component.h"

class BetArea
{
public:
	BetArea(ID2D1Factory* m_pD2DFactory, ID2D1HwndRenderTarget* m_pRT, IWICFormatConverter* m_pConvertedSourceBitmap, IDWriteFactory* m_pWriteFactory, IDWriteTextFormat* m_pWriteTextFormat, IDWriteTextFormat* m_pScoreTextFormat);
	~BetArea();

	void Draw(HWND hwnd);
	void ClearLight();
	void Release();
	void Press(int xPos, int yPos);
	void ChangeState(Instruction ins);

	vector<Component*> componentSet;
	vector<bool> betAreaLight;
	vector<int> pressLight;

	vector<Point2D> area;
	vector<Point2D> light;

	int num = -1;
	bool isOne = false;
	bool isTen = false;
	bool isClear = false;

	ID2D1Factory* b_pD2DFactory;
	ID2D1HwndRenderTarget* b_pRT;
	IWICFormatConverter* b_pConvertedSourceBitmap;
	IDWriteFactory* b_pWriteFactory;
	IDWriteTextFormat* b_pWriteTextFormat;
	IDWriteTextFormat* b_pScoreTextFormat;

private:
};