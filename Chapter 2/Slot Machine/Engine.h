#pragma once

#include "Point2D.h"
#include "Score.h"
#include "Roulette.h"
#include "BetArea.h"

#define RESOLUTION_X 1220
#define RESOLUTION_Y 840

class Engine
{
public:
	Engine(HWND hwnd);
	~Engine();
	HRESULT InitializeD2D(HWND hwnd);
	HRESULT CreateDeviceResources(HWND hWnd);
	void Animater(HWND hwnd);
	void Selecting(int currTime);
	void Winning(int currTime);
	void LargeAndSmall(int currTime);
	void Pending(int currTime);
	void ReadJSON(string filename, HWND hwnd);
	void updateState(int currentTime);
	void PressEvent(int xPos, int yPos);
	void Logic();
	void AutoMode();
	void GetPoint();

	const FLOAT DEFAULT_DPI = 96.f;
	IWICImagingFactory* m_pIWICFactory;
	ID2D1Factory* m_pD2DFactory;
	ID2D1HwndRenderTarget* m_pRT;
	IWICFormatConverter* m_pConvertedSourceBitmap;
	IDWriteFactory* m_pWriteFactory;
	IDWriteTextFormat* m_pWriteTextFormat;
	IDWriteTextFormat* m_pScoreTextFormat;

	vector<Instruction> timestamp;

	vector<int> roulettePicture;
	Score *score;
	Roulette *roulette;
	BetArea* betArea;

	bool isBetStart;
	bool isWinner;
	bool isLargesmallStart;
	bool getPoint;
	bool isCompareStart;
	bool isComparing;
	bool isLSWinner;
	bool guess;

private:
};