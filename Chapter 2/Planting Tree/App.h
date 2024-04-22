#pragma once

#include "Engine.h"

#ifndef HINST_THISCOMPONENT
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)
#endif
class dialog;

LRESULT CALLBACK EditProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
Engine* m_engine;
Engine* e_engine;

class MainApp
{
public:
	MainApp();
	~MainApp();
	HRESULT Initialize();
	void RunMessageLoop();

private:
};