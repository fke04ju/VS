#pragma once

#include "Point2D.h"
#include "Map.h"
#include "Fruittree.h"

#define RESOLUTION_X 800
#define RESOLUTION_Y 600

class Engine
{
public:
	Engine(HWND hwnd, bool isMain);
	~Engine();

	HRESULT InitializeD2D(HWND m_hwnd);
	void Refresh(HWND hwnd, bool isMap);
	HRESULT Draw(HWND hwnd, int xPos, int yPos, int flag);
	HRESULT CreateDeviceResources(HWND hWnd, bool isMain);
	BOOL LocateImageFile(HWND hWnd, LPWSTR pszFileName, DWORD cchFileName);
	//HRESULT OpenImageFile(HWND hWnd, LPCWSTR lpszFilePath, ID2D1Bitmap** m_pD2DBitmap, bool isMain);
	void LoadPicture(HWND hwnd, int flag);
	void SavePicture(HWND hwnd);
	void SelectPicture(WCHAR* filePath);
	void ReadFromJSON(string filename, HWND hwnd);
	void ReadMapJSON(string filename, HWND hwnd);
	void ReadComponentJSON(string filename, HWND hwnd);
	void CopyComponentJSON(string filename, HWND hwnd);
	void DeleteComponentJSON(HWND hwnd, string filename);
	void WriteToJSON(string filename);
	void WriteMapJSON(string filename);
	void OpenDialog(HWND hwnd, int openF);
	WCHAR* SaveDialog(HWND hwnd, bool isMap);
	void ClickDelete(HWND hwnd, int xPos, int yPos, bool isMap);
	void DeleteComponent(HWND hwnd, string str, int flag);

	const FLOAT DEFAULT_DPI = 96.f;
	IWICImagingFactory* m_pIWICFactory;
	ID2D1Factory* m_pD2DFactory;
	ID2D1HwndRenderTarget* m_pRT;
	ID2D1HwndRenderTarget* m_pRT_main;
	IWICFormatConverter* m_pConvertedSourceBitmap;
	Map* map;
	Fruittree *fruittree;
	Fruit* fruit;
	bool isCleared;

private:
};