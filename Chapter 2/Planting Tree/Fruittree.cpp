#include "framework.h"
#include "Point2D.h"
#include "Fruittree.h"

Fruittree::Fruittree(HWND hwnd, WCHAR* path, ID2D1HwndRenderTarget* m_pRT) : Tree(hwnd, path, m_pRT)
{
	
}

Fruittree::Fruittree() : Tree()
{

}

Fruittree::~Fruittree()
{
	for (int i = 0; i < fruitSet.size(); i++)
	{
		delete fruitSet[i].first;
	}
}

void Fruittree::DrawTree(HWND hwnd, int size, ID2D1HwndRenderTarget* m_pRT)
{
	if (!m_pD2DBitmap)
	{
		return;
	}
	Draw(hwnd, fruittreePos.x, fruittreePos.y, size, m_pRT);
	for (int i = 0; i < fruitSet.size(); i++)
	{
		fruitSet[i].first->Draw(hwnd, fruittreePos.x+fruitSet[i].second.x - size/20, fruittreePos.y+fruitSet[i].second.y - size/20, size/10, m_pRT);
	}
}