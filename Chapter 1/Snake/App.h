#pragma once

#include "Engine.h"

#ifndef HINST_THISCOMPONENT
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)
#endif

#define fps30 0.034f
#define fps60 0.016f

class MainApp
{
public:
	MainApp();
	~MainApp();
	HRESULT Initialize();
	void RunMessageLoop();
	double static_secs_logic;
	double static_secs_draw;
	int speed;
private:
	HWND m_hwnd;
	Engine* engine;
	static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
};