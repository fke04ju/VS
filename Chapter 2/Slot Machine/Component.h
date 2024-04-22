#pragma once
#include "Point2D.h"
#include "Image.h"

class Component : public Image
{
public:
	Component(HWND hwnd, WCHAR* path, ID2D1HwndRenderTarget* m_pRT);
	Component();
	~Component();

	WCHAR componentPath[MAX_PATH];
private:
};
