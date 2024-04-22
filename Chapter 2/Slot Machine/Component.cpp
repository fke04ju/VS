#include "framework.h"
#include "Component.h"

Component::Component(HWND hwnd, WCHAR* path, ID2D1HwndRenderTarget* m_pRT)
{
	wcscpy_s(componentPath, path);
	OpenImageFile(hwnd, path, m_pRT);
}

Component::Component()
{

}

Component::~Component()
{

}
