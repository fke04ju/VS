#pragma once
#include "Point2D.h"
#include "Tree.h"
#include "Fruittree.h"

struct treePos {
	Fruittree* fruittree;
	vector<Point2D> fruittreePos;
};

class Map
{
public:
	Map();
	~Map();

	//void OnPaint(HWND hWnd, int xPos, int yPos, ID2D1Bitmap* m_pD2DBitmap, ID2D1HwndRenderTarget* m_pRT);
	void DrawMap(HWND hwnd, ID2D1HwndRenderTarget* m_pRT);
	void Initialize();
	void Reset();
	void SortTree();
	//void Draw(HWND hwnd, ID2D1Bitmap* m_pD2DBitmap, ID2D1HwndRenderTarget* m_pRTs);
	
	//vector<Tree> treePos;
	WCHAR MapName[260];
	WCHAR BackGround[260];
	vector<struct treePos> fruittreeSet;

private:

};