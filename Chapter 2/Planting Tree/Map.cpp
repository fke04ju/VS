#include "framework.h"
#include "Point2D.h"
#include "Map.h"
#include "Engine.h"

Map::Map()
{

}

Map::~Map()
{

}
/*
void Map::OnPaint(HWND hWnd, int xPos, int yPos, ID2D1Bitmap* m_pD2DBitmap, ID2D1HwndRenderTarget* m_pRT)
{
	m_pRT->SetTransform(D2D1::Matrix3x2F::Identity());

	// Clear the background
	//m_pRT->Clear(D2D1::ColorF(D2D1::ColorF::White));


	if (m_pD2DBitmap)
	{
		m_pRT->DrawBitmap(m_pD2DBitmap, D2D1::RectF(xPos, yPos, xPos + 784, yPos + 540));
	}

	return;
}
void Map::Draw(HWND hwnd, ID2D1Bitmap* m_pD2DBitmap, ID2D1HwndRenderTarget* m_pRT)
{
	OnPaint(hwnd, 0, 50, m_pD2DBitmap, m_pRT);
	return;
}
*/
void Map::Initialize()
{

}

void Map::Reset()
{

}
struct sortByY2
{

	inline bool operator() (const pair<int, Point2D> struct1, const pair<int, Point2D> struct2)
	{
		return (struct1.second.y < struct2.second.y);
	}
};

void Map::DrawMap(HWND hwnd, ID2D1HwndRenderTarget* m_pRT)
{
	vector<pair<int, Point2D>> tmp;
	for (int i = 0; i < fruittreeSet.size(); i++)
	{
		for (int j = 0; j < fruittreeSet[i].fruittreePos.size(); j++)
		{
			//fruittreeSet[i].fruittree->fruittreePos = fruittreeSet[i].fruittreePos[j];
			tmp.push_back(make_pair( i, fruittreeSet[i].fruittreePos[j] ));
		}
	}
	std::sort(tmp.begin(), tmp.end(), sortByY2());
	for (int i = 0; i < tmp.size(); i++)
	{
		fruittreeSet[tmp[i].first].fruittree->fruittreePos = tmp[i].second;
		fruittreeSet[tmp[i].first].fruittree->DrawTree(hwnd, 200, m_pRT);
	}

	/*
	for (int i = 0; i < fruittreeSet.size(); i++)
	{
		for (int j = 0; j < fruittreeSet[i].fruittreePos.size(); j++)
		{
			
			fruittreeSet[i].fruittree->fruittreePos = fruittreeSet[i].fruittreePos[j];
			fruittreeSet[i].fruittree->DrawTree(hwnd, 200, m_pRT);
			
			
			WCHAR aaa[MAX_PATH];
			_stprintf_s(aaa, L"X = %d\n", fruittreeSet[i].fruittreePos[j].x);
			OutputDebugString(aaa);
			_stprintf_s(aaa, L"Y = %d\n", fruittreeSet[i].fruittreePos[j].y);
			OutputDebugString(aaa);
			OutputDebugString(fruittreeSet[i].fruittree->TreeName);
			
		}
	}
	*/
}

struct sortByY 
{

	inline bool operator() (const Point2D struct1, const struct Point2D struct2)
	{
		return (struct1.y < struct2.y);
	}
};

void Map::SortTree()
{
	for (auto t : fruittreeSet)
	{
		std::sort(t.fruittreePos.begin(), t.fruittreePos.end(), sortByY());
	}
}
