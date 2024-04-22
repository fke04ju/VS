#pragma once

#include "Point2D.h"
#include "framework.h"
#include "Tree.h"
#include "Fruit.h"


class Fruittree : public Tree
{
public:
	Fruittree(HWND hwnd, WCHAR* path, ID2D1HwndRenderTarget* m_pRT);
	Fruittree();
	~Fruittree();
	void DrawTree(HWND hwnd, int size, ID2D1HwndRenderTarget* m_pRT);
	Point2D fruittreePos;
	vector<pair<Fruit*, Point2D>> fruitSet;
	WCHAR fruittreeName[MAX_PATH];
};