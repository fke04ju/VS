#pragma once

#include "Engine.h"

#ifndef HINST_THISCOMPONENT
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)
#endif
class dialog;

LRESULT CALLBACK EditProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
Engine* engine;

class MainApp
{
public:
	MainApp();
	~MainApp();
	HRESULT Initialize();
	void RunMessageLoop();

	HWND hwnd;
	HWND ExitButton, StartButton, LargeButton, SmallButton, AutoButton, GetPointButton;
	HINSTANCE appInstance;

	double frameTime = 0.0;
	double logicTime = 0.0;


private:
};