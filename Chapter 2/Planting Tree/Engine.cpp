#include "framework.h"
#include "Point2D.h"
#include "Engine.h"

#pragma comment(lib, "d2d1")
#pragma comment(lib, "dwrite")

WCHAR szFilePath[MAX_PATH];
WCHAR szFolderPath[MAX_PATH];
bool isLoadFruit = false;

Engine::Engine(HWND hwnd, bool isMain)
{
	HRESULT hr = S_OK;
	// Create WIC factory
	hr = CoCreateInstance(
		CLSID_WICImagingFactory,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(&m_pIWICFactory)
	);

	if (SUCCEEDED(hr))
	{
		// Create D2D factory
		hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pD2DFactory);
	}

	if (SUCCEEDED(hr))
	{
		hr = CreateDeviceResources(hwnd, isMain);
	}
	//tree = new Tree();
	//fruit = new Fruit();
	map = new Map();
	//fruittree = new Fruittree();
	isCleared = false;
}

Engine::~Engine()
{
	delete map;
	delete fruittree;
	delete fruit;
	SafeRelease(m_pConvertedSourceBitmap);
	SafeRelease(m_pIWICFactory);
	SafeRelease(m_pD2DFactory);
	SafeRelease(m_pRT);
	SafeRelease(m_pRT_main);
}

HRESULT Engine::CreateDeviceResources(HWND hWnd, bool isMain)
{
	HRESULT hr = S_OK;

	if (!isMain)
	{
		if (!m_pRT)
		{
			RECT rc;
			hr = GetClientRect(hWnd, &rc) ? S_OK : E_FAIL;

			if (SUCCEEDED(hr))
			{
				// Create a D2D render target properties
				D2D1_RENDER_TARGET_PROPERTIES renderTargetProperties = D2D1::RenderTargetProperties();

				// Set the DPI to be the default system DPI to allow direct mapping
				// between image pixels and desktop pixels in different system DPI settings
				renderTargetProperties.dpiX = DEFAULT_DPI;
				renderTargetProperties.dpiY = DEFAULT_DPI;

				// Create a D2D render target
				D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);

				hr = m_pD2DFactory->CreateHwndRenderTarget(
					renderTargetProperties,
					D2D1::HwndRenderTargetProperties(hWnd, size),
					&m_pRT
				);
			}
		}
	}
	else
	{
		if (!m_pRT_main)
		{
			RECT rc;
			hr = GetClientRect(hWnd, &rc) ? S_OK : E_FAIL;

			if (SUCCEEDED(hr))
			{
				// Create a D2D render target properties
				D2D1_RENDER_TARGET_PROPERTIES renderTargetProperties = D2D1::RenderTargetProperties();

				// Set the DPI to be the default system DPI to allow direct mapping
				// between image pixels and desktop pixels in different system DPI settings
				renderTargetProperties.dpiX = DEFAULT_DPI;
				renderTargetProperties.dpiY = DEFAULT_DPI;

				// Create a D2D render target
				D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);

				hr = m_pD2DFactory->CreateHwndRenderTarget(
					renderTargetProperties,
					D2D1::HwndRenderTargetProperties(hWnd, size),
					&m_pRT_main
				);
			}
		}
	}
	

	return hr;
}

BOOL Engine::LocateImageFile(HWND hWnd, LPWSTR pszFileName, DWORD cchFileName)
{
	pszFileName[0] = L'\0';

	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(ofn));

	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFilter = L"All Image Files\0"
		L"*.bmp;*.dib;*.wdp;*.mdp;*.hdp;*.gif;*.png;*.jpg;*.jpeg;*.tif;*.ico;*.webp\0"
		L"Windows Bitmap\0"               L"*.bmp;*.dib\0"
		L"High Definition Photo\0"        L"*.wdp;*.mdp;*.hdp\0"
		L"Graphics Interchange Format\0"  L"*.gif\0"
		L"Portable Network Graphics\0"    L"*.png\0"
		L"JPEG File Interchange Format\0" L"*.jpg;*.jpeg\0"
		L"Tiff File\0"                    L"*.tif\0"
		L"Icon\0"                         L"*.ico\0"
		L"All Files\0"                    L"*.*\0"
		L"\0";
	ofn.lpstrFile = pszFileName;
	ofn.nMaxFile = cchFileName;
	ofn.lpstrTitle = L"Open Image";
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

	// Display the Open dialog box. 
	return GetOpenFileName(&ofn);
}
/*
HRESULT Engine::OpenImageFile(HWND hWnd, LPCWSTR lpszFilePath, ID2D1Bitmap** m_pD2DBitmap, bool isMain)
{
	HRESULT hr = S_OK;

	// Step 1: Create the open dialog box and locate the image file
	if (PathFileExists(lpszFilePath))
	{
		// Step 2: Decode the source image

		// Create a decoder
		IWICBitmapDecoder* pDecoder = NULL;

		hr = m_pIWICFactory->CreateDecoderFromFilename(
			lpszFilePath,                      // Image to be decoded
			NULL,                            // Do not prefer a particular vendor
			GENERIC_READ,                    // Desired read access to the file
			WICDecodeMetadataCacheOnDemand,  // Cache metadata when needed
			&pDecoder                        // Pointer to the decoder
		);

		// Retrieve the first frame of the image from the decoder
		IWICBitmapFrameDecode* pFrame = NULL;

		if (SUCCEEDED(hr))
		{
			hr = pDecoder->GetFrame(0, &pFrame);
		}

		//Step 3: Format convert the frame to 32bppPBGRA
		if (SUCCEEDED(hr))
		{
			SafeRelease(m_pConvertedSourceBitmap);
			hr = m_pIWICFactory->CreateFormatConverter(&m_pConvertedSourceBitmap);
		}

		if (SUCCEEDED(hr))
		{
			hr = m_pConvertedSourceBitmap->Initialize(
				pFrame,                          // Input bitmap to convert
				GUID_WICPixelFormat32bppPBGRA,   // Destination pixel format
				WICBitmapDitherTypeNone,         // Specified dither pattern
				NULL,                            // Specify a particular palette 
				0.f,                             // Alpha threshold
				WICBitmapPaletteTypeCustom       // Palette translation type
			);
		}

		//Step 4: Create render target and D2D bitmap from IWICBitmapSource
		if (SUCCEEDED(hr))
		{
			//hr = CreateDeviceResources(hWnd);
		}


		if (SUCCEEDED(hr))
		{
			// Need to release the previous D2DBitmap if there is one
			SafeRelease(*m_pD2DBitmap);
			if (isMain)
			{
				hr = m_pRT_main->CreateBitmapFromWicBitmap(m_pConvertedSourceBitmap, NULL, m_pD2DBitmap);
			}
			else 
			{
				hr = m_pRT->CreateBitmapFromWicBitmap(m_pConvertedSourceBitmap, NULL, m_pD2DBitmap);
			}
		}

		SafeRelease(pDecoder);
		SafeRelease(pFrame);

		//InvalidateRect(hWnd, 0, TRUE);
	}

	return hr;
}
*/
HRESULT Engine::InitializeD2D(HWND m_hwnd)
{
	return S_OK;
}

void Engine::Refresh(HWND hwnd, bool isMap)
{
	if (isMap)
	{
		map->DrawMap(hwnd, m_pRT_main);
	}
	else
	{
		fruittree->Draw(hwnd, 150, 25, 200, m_pRT);
	}
}

HRESULT Engine::Draw(HWND hwnd, int xPos, int yPos, int flag)
{ 
	if (szFilePath == 0)
	{
		return S_OK;
	}


	HRESULT hr = S_OK;
	PAINTSTRUCT ps;
	// Create render target if not yet created


	if (BeginPaint(hwnd, &ps)) {
		hr = CreateDeviceResources(hwnd, true);
		if (flag == 3) // Paste tree to map
		{
			if (SUCCEEDED(hr) && !(m_pRT_main->CheckWindowState() & D2D1_WINDOW_STATE_OCCLUDED) && fruittree)
			{
				m_pRT_main->BeginDraw();
				if (!isCleared)
				{
					m_pRT_main->Clear(D2D1::ColorF(D2D1::ColorF::White));
					isCleared = true;
				}
				bool found = false;
				for (int i = 0; i < map->fruittreeSet.size(); i++)
				{
					if (!wcscmp(map->fruittreeSet[i].fruittree->fruittreeName, fruittree->fruittreeName))
					{
						map->fruittreeSet[i].fruittreePos.push_back({ xPos , yPos });
						found = true;
					}
				}
				if (!found)
				{
					map->fruittreeSet.push_back({ fruittree });
					map->fruittreeSet[map->fruittreeSet.size()-1].fruittreePos.push_back({ xPos , yPos });
				}
				map->SortTree();
				map->DrawMap(hwnd, m_pRT_main);
			}
			hr = m_pRT_main->EndDraw();
			//tree->Reset();
		}
		else if (flag == 4) // Load map
		{
			if (SUCCEEDED(hr) && !(m_pRT_main->CheckWindowState() & D2D1_WINDOW_STATE_OCCLUDED))
			{
				m_pRT_main->BeginDraw();
				m_pRT_main->Clear(D2D1::ColorF(D2D1::ColorF::White));
				isCleared = true;
				map->DrawMap(hwnd, m_pRT_main);
			}
			hr = m_pRT_main->EndDraw();
		}
		else
		{
			if (SUCCEEDED(hr) && !(m_pRT->CheckWindowState() & D2D1_WINDOW_STATE_OCCLUDED))
			{
				m_pRT->BeginDraw();
				if (flag == 1) // Load tree
				{
					m_pRT->Clear(D2D1::ColorF(D2D1::ColorF::White));

					//wcscpy_s(fruittree->TreeName, sizeof(fruittree->TreeName) / sizeof(WCHAR), szFilePath);
					fruittree->fruittreePos = { 150, 25 };
					fruittree->DrawTree(hwnd, 200, m_pRT);
				}
				else if (flag == 2) // Load fruit
				{
					if (fruittree)
					{
						if (isLoadFruit)
						{
							//Point2D b = { xPos, yPos };
							//Fruit a = Fruit(hwnd, szFilePath, m_pRT);
							//fill(begin(szFilePath), end(szFilePath), 0);
							fruit = new Fruit(hwnd, szFilePath, m_pRT);
							//fill(begin(szFilePath), end(szFilePath), 0);
							Point2D aa = { xPos - 150, yPos - 25 };
							fruittree->fruitSet.push_back(make_pair(fruit, aa));
						}
						//fruittree->fruitSet[fruittree->fruitSet.size() - 1].second = { xPos, yPos };
						fruittree->DrawTree(hwnd, 200, m_pRT);
					}
					else 
					{
						m_pRT->Clear(D2D1::ColorF(D2D1::ColorF::White));
					}
				}
				else if (flag == 5)
				{
					m_pRT->Clear(D2D1::ColorF(D2D1::ColorF::White));
					isCleared = true;
					fruittree->fruittreePos.x = 150;
					fruittree->fruittreePos.y = 25;
					fruittree->DrawTree(hwnd, 200, m_pRT);
				}
			}
			hr = m_pRT->EndDraw();
		}


		// In case of device loss, discard D2D render target and D2DBitmap
		// They will be re-create in the next rendering pass
		if (hr == D2DERR_RECREATE_TARGET)
		{
			SafeRelease(m_pRT);
			SafeRelease(m_pRT_main);
			// Force a re-render
		}
		hr = InvalidateRect(hwnd, NULL, TRUE) ? S_OK : E_FAIL;
	}

	EndPaint(hwnd, &ps);
	return S_OK;
}

void Engine::LoadPicture(HWND hwnd, int flag)
{
	if (flag == 1) // Load tree
	{
		OpenDialog(hwnd, 1);
		fruittree = new Fruittree(hwnd, szFilePath, m_pRT);
		fill(begin(szFilePath), end(szFilePath), 0);
		//OpenImageFile(hwnd, szFilePath, &m_pD2DBitmap_Tree, false);
		//wcscpy_s(fruittree.TreeName, 260, szFilePath);
		Draw(hwnd, 150, 25, 1);
	}
	else if(flag == 2) // Load fruit
	{
		fill(begin(szFilePath), end(szFilePath), 0);
		OpenDialog(hwnd, 2);
		if (szFilePath != L"")
		{
			isLoadFruit = true;
		}
		//Point2D aa = { 0, 0 };
		//fruittree->fruitSet.push_back(make_pair(a, aa));

		//a.Draw(hwnd, 0, 300, 20, m_pRT);
		//OpenImageFile(hwnd, szFilePath, &m_pD2DBitmap_Fruit, false);
		//auto a = Fruit(hwnd, szFilePath);
		//a.Draw(hwnd, 0, 350, 10);
		//Draw(hwnd, 0, 350, 10);
		//wcscpy_s(fruittree.fruitSet.FruitName, sizeof(fruit->FruitName) / sizeof(WCHAR), szFilePath);

	}
	else if (flag == 3) // Paste tree to map
	{
		if (szFolderPath[0] != 0)
		{
			wstring ws(szFolderPath);
			string str(ws.begin(), ws.end());
			ReadFromJSON(str, hwnd);
			//Fruittree tmptree = Fruittree(hwnd, fruittree->TreeName, m_pRT_main);
			//tree->Reset();
			//OpenImageFile(hwnd, tree->TreeName, &m_pD2DBitmap_Tree, true);
			//OpenImageFile(hwnd, fruit->FruitName, &m_pD2DBitmap_Fruit, true);
		}
	}
	else if (flag == 4) // Load map
	{
		OpenDialog(hwnd, 4);
		//OpenImageFile(hwnd, szFilePath, &m_pD2DBitmap_Map, true);
		wstring s(szFilePath);
		string ss(s.begin(), s.end());
		ReadMapJSON(ss, hwnd);
		Draw(hwnd, 0, 50, 4);
	}
	else if (flag == 5) // Load Component
	{
		//m_pRT->Clear(D2D1::ColorF(D2D1::ColorF::White));
		Draw(hwnd, 0, 0, 5);
	}
}

/*
void Engine::SavePicture(HWND hwnd)
{
	//SaveDialog();

	HDC hdcWindow;
	HDC hdcMemory;
	HBITMAP hBitmap;
	BYTE* lpvBits;
	BITMAPINFO bi;
	BITMAPINFOHEADER bih;
	BITMAPFILEHEADER bifh;
	HANDLE hFile;
	DWORD dwBytesWritten = 0;
	DWORD dwSize;
	int xSize, ySize;
	RECT rect;
	hdcWindow = GetDC(NULL);

	GetWindowRect(hwnd, &rect);

	xSize = rect.right - rect.left;
	//xSize = 200; ySize = 200;
	ySize = rect.bottom - rect.top;
	hBitmap = CreateCompatibleBitmap(hdcWindow, xSize, ySize);
	hdcMemory = CreateCompatibleDC(hdcWindow);
	SelectObject(hdcMemory, hBitmap);
	BitBlt(hdcMemory, 0, 0, xSize, ySize,
		hdcWindow, rect.left, rect.top, SRCCOPY);

	bih.biSize = sizeof(BITMAPINFOHEADER);
	bih.biWidth = xSize;
	bih.biHeight = ySize;
	bih.biPlanes = 1;
	bih.biBitCount = 32;
	bih.biCompression = BI_RGB;
	bih.biSizeImage = 0;
	bih.biXPelsPerMeter = 0;
	bih.biYPelsPerMeter = 0;
	bih.biClrUsed = 0;
	bih.biClrImportant = 0;

	dwSize = (((xSize * 32 + 31) / 32) * 4 * ySize);
	lpvBits = (BYTE*)malloc(dwSize);

	bifh.bfType = 0x4D42;
	bifh.bfSize = sizeof(BITMAPFILEHEADER) +
		sizeof(BITMAPINFOHEADER) + dwSize;
	bifh.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) +
		(DWORD)sizeof(BITMAPINFOHEADER);
	bifh.bfReserved1 = 0;
	bifh.bfReserved2 = 0;

	bi.bmiHeader = bih;

	GetDIBits(hdcMemory, hBitmap, 0, (UINT)ySize,
		lpvBits, &bi, DIB_RGB_COLORS);
	hFile = CreateFile(L"./saved_pic.jpg", GENERIC_WRITE, 0,
		NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	WriteFile(hFile, (LPSTR)&bifh,
		sizeof(BITMAPFILEHEADER), &dwBytesWritten, NULL);
	WriteFile(hFile, (LPSTR)&bih,
		sizeof(BITMAPINFOHEADER), &dwBytesWritten, NULL);
	WriteFile(hFile, (LPSTR)lpvBits,
		dwSize, &dwBytesWritten, NULL);

	DeleteDC(hdcMemory);
	DeleteObject(hBitmap);
	CloseHandle(hFile);
	free(lpvBits);
	ReleaseDC(hwnd, hdcWindow);
}
*/

void Engine::SelectPicture(WCHAR *filePath)
{
	std::fill(szFolderPath, szFolderPath + MAX_PATH, 0);
	wcscpy_s(szFolderPath, filePath);
	//wstring ws(szFolderPath);
	//string str(ws.begin(), ws.end());
	//WriteToJSON(str);
}

bool isFileExists(string& name)
{
	struct stat buffer;
	return (stat(name.c_str(), &buffer) == 0);
}

void Engine::ReadMapJSON(string filename, HWND hwnd)
{
	Json::Reader reader;
	Json::Value data;
	std::ifstream is;
	is.open(filename, std::ios::binary);
	if (!reader.parse(is, data, false))
	{
		return;
	}
	vector<int> notexistcomponent;
	for (int i = 0; i < data["Type"].size(); i++)
	{
		std::string tstr = data["Type"][i]["Key"].asString();
		if (!isFileExists(tstr))
		{
			wstring warning = L"此地圖包含不存在之元件，是否仍要開啟?\n缺少元件名稱:" + wstring(tstr.begin(), tstr.end());

			if(MessageBox(hwnd, warning.c_str(), L"警告", MB_YESNO) == 7)
			{
				is.close();
				return;
			}
			notexistcomponent.push_back(i);
		}
	}

	for (int i = 0; i < map->fruittreeSet.size(); i++)
	{
		delete map->fruittreeSet[i].fruittree;
	}
	map->fruittreeSet.clear();
	std::wstring ws;
	std::string str = data["Map"].asString();
	for (int i = 0; i < str.size(); i++)
	{
		ws += WCHAR(str[i]);
	}
	wcscpy_s(map->MapName, sizeof(map->MapName) / sizeof(WCHAR), ws.c_str());
	ws.clear();
	for (int i = 0; i < data["Type"].size(); i++)
	{
		if (std::count(notexistcomponent.begin(), notexistcomponent.end(), i))
		{
			continue;
		}
		str = data["Type"][i]["TreeName"].asString();
		for (int j = 0; j < str.size(); j++)
		{
			ws += WCHAR(str[j]);
		}
		Fruittree* tmp = new Fruittree(hwnd, _wcsdup(ws.c_str()), m_pRT_main);
		wcscpy_s(tmp->TreeName, sizeof(tmp->TreeName) / sizeof(WCHAR), ws.c_str());
		ws.clear();
		str = data["Type"][i]["Key"].asString();
		for (int j = 0; j < str.size(); j++)
		{
			ws += WCHAR(str[j]);
		}
		wcscpy_s(tmp->fruittreeName, sizeof(tmp->fruittreeName) / sizeof(WCHAR), ws.c_str());
		ws.clear();
		vector<Point2D> tmpPos;
		for (int j = 0; j < (data["Type"][i]["TreePos"].size()); j++)
		{
			tmpPos.push_back({ stoi(data["Type"][i]["TreePos"][j]["x"].asString()) , stoi(data["Type"][i]["TreePos"][j]["y"].asString()) });
		}
		for (int j = 0; j < (data["Type"][i]["FruitPos"].size()); j++)
		{
			int a = data["Type"][i]["FruitPos"].size();
			str = data["Type"][i]["FruitPos"][j]["Fruit"].asString();
			for (int k = 0; k < str.size(); k++)
			{
				ws += WCHAR(str[k]);
			}
			WCHAR stmp[260];
			wcscpy_s(stmp, sizeof(stmp) / sizeof(WCHAR), ws.c_str());
			Fruit *ftmp = new Fruit(hwnd, stmp, m_pRT_main);
			ws.clear();
			int X = stoi(data["Type"][i]["FruitPos"][j]["x"].asString());
			int Y = stoi(data["Type"][i]["FruitPos"][j]["y"].asString());
			Point2D fruitPos = { X, Y };
			tmp->fruitSet.push_back(make_pair(ftmp, fruitPos));
		}
		map->fruittreeSet.push_back({ tmp , tmpPos });
	}
	is.close();
}

void Engine::WriteMapJSON(string filename)
{
	std::ifstream ifs(map->MapName, std::ios::binary);
	std::ofstream ofs;
	char buf[1024];
	ofs.open(map->MapName, std::ios::binary);
	while (ifs.read(buf, sizeof(buf)))
	{
		ofs.write(buf, ifs.gcount());
	}
	ofs.write(buf, ifs.gcount());
	ofs.close();
	Json::StreamWriterBuilder builder;
	builder["commentStyle"] = "None";
	builder["indentation"] = "   ";
	Json::Value data;
	std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
	wstring ws(map->MapName);
	string str(ws.begin(), ws.end());
	data["Map"] = str;
	for (int i = 0; i < (map->fruittreeSet.size()); i++)
	{
		struct treePos tmp = map->fruittreeSet[i];
		char tmpTreeName[260] = "";
		sprintf_s(tmpTreeName, "%ws", tmp.fruittree->fruittreeName);
		data["Type"][i]["Key"] = tmpTreeName;
		sprintf_s(tmpTreeName, "%ws", tmp.fruittree->TreeName);
		data["Type"][i]["TreeName"] = tmpTreeName;
		for (int j = 0; j < (tmp.fruittree->fruitSet.size()); j++)
		{
			sprintf_s(tmpTreeName, "%ws", tmp.fruittree->fruitSet[j].first->FruitName);
			data["Type"][i]["FruitPos"][j]["Fruit"] = tmpTreeName;
			data["Type"][i]["FruitPos"][j]["x"] = tmp.fruittree->fruitSet[j].second.x;
			data["Type"][i]["FruitPos"][j]["y"] = tmp.fruittree->fruitSet[j].second.y;
		}
		for (int j = 0; j < (tmp.fruittreePos.size()); j++)
		{
			data["Type"][i]["TreePos"][j]["x"] = tmp.fruittreePos[j].x;
			data["Type"][i]["TreePos"][j]["y"] = tmp.fruittreePos[j].y;
		}
	}

	std::ofstream dataStream(filename);
	writer->write(data, &dataStream);
}

void Engine::ReadFromJSON(string filename, HWND hwnd)
{
	Json::Reader reader;
	Json::Value data;
	std::ifstream is;
	is.open(filename, std::ios::binary);
	if (!reader.parse(is, data, false))
	{
		return;
	}
	std::wstring ws;
	std::string str = data["Tree"].asString();
	for (int i = 0; i < str.size(); i++)
	{
		ws += WCHAR(str[i]);
	}
	
	fruittree = new Fruittree(hwnd, _wcsdup(ws.c_str()), m_pRT_main);
	//wcscpy_s(fruittree->TreeName, sizeof(fruittree->TreeName) / sizeof(WCHAR), ws.c_str());
	ws.clear();
	str = data["Type"].asString();
	for (int i = 0; i < str.size(); i++)
	{
		ws += WCHAR(str[i]);
	}
	wcscpy_s(fruittree->fruittreeName, sizeof(fruittree->fruittreeName) / sizeof(WCHAR), ws.c_str());
	ws.clear();
	int nFruit = data["Position"].size();
	for (int i = 0; i < nFruit; i++)
	{
		str = data["Position"][i]["Fruit"].asString();
		for (int i = 0; i < str.size(); i++)
		{
			ws += WCHAR(str[i]);
		}
		Fruit *tmp = new Fruit(hwnd, _wcsdup(ws.c_str()), m_pRT_main);
		wcscpy_s(tmp->FruitName, sizeof(tmp->FruitName) / sizeof(WCHAR), ws.c_str());
		int X = stoi(data["Position"][i]["x"].asString());
		int Y = stoi(data["Position"][i]["y"].asString());
		Point2D fruitPos = { X, Y };
		fruittree->fruitSet.push_back(make_pair(tmp , fruitPos));
		ws.clear();
	}
	is.close();
}

void Engine::ReadComponentJSON(string filename, HWND hwnd)
{
	Json::Reader reader;
	Json::Value data;
	std::ifstream is;
	is.open(filename, std::ios::binary);
	if (!reader.parse(is, data, false))
	{
		return;
	}
	std::wstring ws;
	std::string str = data["Tree"].asString();
	for (int i = 0; i < str.size(); i++)
	{
		ws += WCHAR(str[i]);
	}

	fruittree = new Fruittree(hwnd, _wcsdup(ws.c_str()), m_pRT);
	//wcscpy_s(fruittree->TreeName, sizeof(fruittree->TreeName) / sizeof(WCHAR), ws.c_str());
	ws.clear();
	str = data["Type"].asString();
	for (int i = 0; i < str.size(); i++)
	{
		ws += WCHAR(str[i]);
	}
	wcscpy_s(fruittree->fruittreeName, sizeof(fruittree->fruittreeName) / sizeof(WCHAR), ws.c_str());
	ws.clear();
	int nFruit = data["Position"].size();
	for (int i = 0; i < nFruit; i++)
	{
		str = data["Position"][i]["Fruit"].asString();
		for (int i = 0; i < str.size(); i++)
		{
			ws += WCHAR(str[i]);
		}
		Fruit* tmp = new Fruit(hwnd, _wcsdup(ws.c_str()), m_pRT);
		wcscpy_s(tmp->FruitName, sizeof(tmp->FruitName) / sizeof(WCHAR), ws.c_str());
		int X = stoi(data["Position"][i]["x"].asString());
		int Y = stoi(data["Position"][i]["y"].asString());
		Point2D fruitPos = { X, Y };
		fruittree->fruitSet.push_back(make_pair(tmp, fruitPos));
		ws.clear();
	}
	is.close();
}

void Engine::CopyComponentJSON(string filename, HWND hwnd)
{
	Json::Reader reader;
	Json::Value data;
	std::ifstream is;
	is.open(filename, std::ios::binary);
	if (!reader.parse(is, data, false))
	{
		return;
	}
	std::wstring ws;
	std::string str = data["Tree"].asString();
	for (int i = 0; i < str.size(); i++)
	{
		ws += WCHAR(str[i]);
	}

	Fruittree *tmpfruittree = new Fruittree(hwnd, _wcsdup(ws.c_str()), m_pRT_main);
	//wcscpy_s(fruittree->TreeName, sizeof(fruittree->TreeName) / sizeof(WCHAR), ws.c_str());
	ws.clear();
	str = data["Type"].asString();
	for (int i = 0; i < str.size(); i++)
	{
		ws += WCHAR(str[i]);
	}
	wcscpy_s(tmpfruittree->fruittreeName, sizeof(tmpfruittree->fruittreeName) / sizeof(WCHAR), ws.c_str());
	ws.clear();
	int nFruit = data["Position"].size();
	for (int i = 0; i < nFruit; i++)
	{
		str = data["Position"][i]["Fruit"].asString();
		for (int i = 0; i < str.size(); i++)
		{
			ws += WCHAR(str[i]);
		}
		Fruit* tmp = new Fruit(hwnd, _wcsdup(ws.c_str()), m_pRT_main);
		wcscpy_s(tmp->FruitName, sizeof(tmp->FruitName) / sizeof(WCHAR), ws.c_str());
		int X = stoi(data["Position"][i]["x"].asString());
		int Y = stoi(data["Position"][i]["y"].asString());
		Point2D fruitPos = { X, Y };
		tmpfruittree->fruitSet.push_back(make_pair(tmp, fruitPos));
		ws.clear();
	}
	is.close();

	for (int i = 0; i < map->fruittreeSet.size(); i++)
	{
		if (!wcscmp(tmpfruittree->fruittreeName, map->fruittreeSet[i].fruittree->fruittreeName))
		{

			map->fruittreeSet[i].fruittree = tmpfruittree;
		}
	}
}

void Engine::DeleteComponentJSON(HWND hwnd, string filename)
{
	std::wstring wstr = std::wstring(filename.begin(), filename.end());
	int idx = -1;
	for (int i = 0; i < map->fruittreeSet.size(); i++)
	{
		if (!wcscmp(wstr.c_str(), map->fruittreeSet[i].fruittree->fruittreeName))
		{
			idx = i;
		}
	}
	if (idx != -1)
	{
		delete map->fruittreeSet[idx].fruittree;
		map->fruittreeSet[idx].fruittreePos.clear();
		map->fruittreeSet.erase(map->fruittreeSet.begin() + idx);
	}
}


void Engine::WriteToJSON(string filename)
{
	if (fruittree)
	{
		std::ifstream ifs1(fruittree->TreeName, std::ios::binary);
		std::ofstream ofs1, ofs2;
		char buf[1024];
		
		ofs1.open(fruittree->TreeName, std::ios::binary);
		while (ifs1.read(buf, sizeof(buf)))
		{
			ofs1.write(buf, ifs1.gcount());
		}
		ofs1.write(buf, ifs1.gcount());
		ofs1.close();
		Json::StreamWriterBuilder builder;
		builder["commentStyle"] = "None";
		builder["indentation"] = "   ";
		Json::Value data;
		std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
		wstring ws(fruittree->TreeName);
		string str(ws.begin(), ws.end());
		data["Tree"] = str;
		data["Type"] = filename;
		for (int i = 0; i < (fruittree->fruitSet.size()); i++)
		{
			std::ifstream ifs2(fruittree->fruitSet[i].first->FruitName, std::ios::binary);
			Fruit *tmp = fruittree->fruitSet[i].first;
			ofs2.open(tmp->FruitName, std::ios::binary);
			while (ifs2.read(buf, sizeof(buf)))
			{
				ofs2.write(buf, ifs2.gcount());
			}
			ofs2.write(buf, ifs2.gcount());
			ofs2.close();
			wstring ws(tmp->FruitName);
			string str(ws.begin(), ws.end());
			data["Position"][i]["Fruit"] = str;
			data["Position"][i]["x"] = fruittree->fruitSet[i].second.x;
			data["Position"][i]["y"] = fruittree->fruitSet[i].second.y;
		}
		std::ofstream dataStream(filename);
		writer->write(data, &dataStream);
	}
	else
	{
		Json::StreamWriterBuilder builder;
		builder["commentStyle"] = "None";
		builder["indentation"] = "   ";
		Json::Value data;
		std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
		std::ofstream dataStream(filename);
		writer->write(data, &dataStream);
	}
}


void Engine::OpenDialog(HWND hwnd, int openF)
{
	HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED |
		COINIT_DISABLE_OLE1DDE);
	if ((SUCCEEDED(hr))) {
		WCHAR getFileName[MAX_PATH] = L"";
		OPENFILENAME ofn;
		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = hwnd;
		ofn.lpstrFile = getFileName;
		ofn.nMaxFile = MAX_PATH;
		ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
		std::fill(szFolderPath, szFolderPath + MAX_PATH, 0);
		GetModuleFileName(NULL, szFolderPath, MAX_PATH);
		PathRemoveFileSpec(szFolderPath);

		if (openF == 1) // tree
		{
			const WCHAR* comp = L"\\Source\\Tree";
			wcscat_s(szFolderPath, comp);
		}
		else if (openF == 2)
		{
			const WCHAR* comp = L"\\Source\\Fruit";
			wcscat_s(szFolderPath, comp);
		}
		else
		{
			const WCHAR* comp = L"\\Map";
			wcscat_s(szFolderPath, comp);
		}
		ofn.lpstrInitialDir = szFolderPath;
		GetOpenFileName(&ofn);
		wcscpy_s(szFilePath, getFileName);

		CoUninitialize();
	}
	return;
}

WCHAR* Engine::SaveDialog(HWND hwnd, bool isMap)
{
	WCHAR svDlgPath[MAX_PATH] = L"";
	WCHAR getFileName[MAX_PATH] = L"";
	HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED |
		COINIT_DISABLE_OLE1DDE);
	if ((SUCCEEDED(hr))) {

		OPENFILENAME ofn;
		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = hwnd;
		ofn.nMaxFile = MAX_PATH;
		ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
		ofn.lpstrFilter = L"JSON Files (*.json)\0*.json";
		ofn.lpstrDefExt = L"json";
		ofn.lpstrFile = getFileName;
		GetModuleFileName(NULL, svDlgPath, MAX_PATH);
		PathRemoveFileSpec(svDlgPath);

		if (isMap)
		{
			const WCHAR* comp = L"\\Map\\";
			wcscat_s(svDlgPath, comp);
			if (CreateDirectory(svDlgPath, NULL) || ERROR_ALREADY_EXISTS == GetLastError())
			{
				ofn.lpstrInitialDir = svDlgPath;
				if (GetSaveFileName(&ofn))
				{
					wcscpy_s(map->MapName, getFileName);
				}
				wstring ws(getFileName);
				string str(ws.begin(), ws.end());
				WriteMapJSON(str);
			}
		}
		else
		{
			const WCHAR* comp = L"\\Component\\";
			wcscat_s(svDlgPath, comp);
			if (CreateDirectory(svDlgPath, NULL) || ERROR_ALREADY_EXISTS == GetLastError())
			{
				ofn.lpstrInitialDir = svDlgPath;

				if (GetSaveFileName(&ofn))
				{
					wstring ws(getFileName);
					string str(ws.begin(), ws.end());
					WriteToJSON(str);
				}
			}
		}

		CoUninitialize();
	}
	return getFileName;
}

void Engine::ClickDelete(HWND hwnd, int xPos, int yPos, bool isMap)
{
	HRESULT hr = S_OK;
	PAINTSTRUCT ps;
	// Create render target if not yet created


	if (BeginPaint(hwnd, &ps)) {
		hr = CreateDeviceResources(hwnd, true);
		if (isMap)
		{
			if (SUCCEEDED(hr) && !(m_pRT_main->CheckWindowState() & D2D1_WINDOW_STATE_OCCLUDED))
			{
				m_pRT_main->BeginDraw();
				m_pRT_main->Clear(D2D1::ColorF(D2D1::ColorF::White));
				int fidxOfDelete = -1;
				int yidxOfDelete = -1;
				int largestY = -1000;
				for (int i = 0; i < map->fruittreeSet.size(); i++)
				{
					for (int j = 0; j < map->fruittreeSet[i].fruittreePos.size(); j++)
					{
						if (abs(xPos - map->fruittreeSet[i].fruittreePos[j].x) < 100 && abs(yPos - map->fruittreeSet[i].fruittreePos[j].y) < 100)
						{
							if (map->fruittreeSet[i].fruittreePos[j].y > largestY)
							{
								largestY = map->fruittreeSet[i].fruittreePos[j].y;
								fidxOfDelete = i;
								yidxOfDelete = j;
							}
						}
					}
				}
				if (fidxOfDelete != -1 && yidxOfDelete != -1)
				{
					auto deleteUnit = map->fruittreeSet[fidxOfDelete].fruittreePos[yidxOfDelete];
					map->fruittreeSet[fidxOfDelete].fruittreePos.erase(map->fruittreeSet[fidxOfDelete].fruittreePos.begin() + yidxOfDelete);
				}

				map->DrawMap(hwnd, m_pRT_main);
			}
			m_pRT_main->EndDraw();
		}
		else
		{
			if (SUCCEEDED(hr) && !(m_pRT->CheckWindowState() & D2D1_WINDOW_STATE_OCCLUDED))
			{
				m_pRT->BeginDraw();
				m_pRT->Clear(D2D1::ColorF(D2D1::ColorF::White));
				int idxOfDelete = -1;
				int largestY = 0;
				for (int i = 0; i < fruittree->fruitSet.size(); i++)
				{
					if (abs(xPos - fruittree->fruitSet[i].second.x) < 20 && abs(yPos - fruittree->fruitSet[i].second.y) < 20)
					{
						if (fruittree->fruitSet[i].second.y > largestY)
						{
							largestY = fruittree->fruitSet[i].second.y;
							idxOfDelete = i;
						}
					}
				}
				if (idxOfDelete != -1)
				{
					auto deleteUnit = fruittree->fruitSet[idxOfDelete].first;
					fruittree->fruitSet.erase(fruittree->fruitSet.begin() + idxOfDelete);
					delete deleteUnit;
				}

				fruittree->DrawTree(hwnd, 200, m_pRT);
			}
			m_pRT->EndDraw();
		}
		if (hr == D2DERR_RECREATE_TARGET)
		{
			SafeRelease(m_pRT);
			SafeRelease(m_pRT_main);
			// Force a re-render
		}
		hr = InvalidateRect(hwnd, NULL, TRUE) ? S_OK : E_FAIL;
	}
	EndPaint(hwnd, &ps);
}

void Engine::DeleteComponent(HWND hwnd, string str, int flag)
{

	HRESULT hr = S_OK;
	PAINTSTRUCT ps;
	if (flag == 1)
	{
		if (BeginPaint(hwnd, &ps)) {
			hr = CreateDeviceResources(hwnd, true);
			if (SUCCEEDED(hr) && !(m_pRT->CheckWindowState() & D2D1_WINDOW_STATE_OCCLUDED)) {
				m_pRT->BeginDraw();
				m_pRT->Clear(D2D1::ColorF(D2D1::ColorF::White));
				m_pRT->EndDraw();
			}
			hr = InvalidateRect(hwnd, NULL, TRUE) ? S_OK : E_FAIL;
		}
		EndPaint(hwnd, &ps);
	}
	//delete fruittree;
	std::wstring wfileName(str.begin(), str.end());
	auto res = DeleteFile(wfileName.c_str());
	std::fill(szFilePath, szFilePath + MAX_PATH, 0);
	std::fill(szFolderPath, szFolderPath + MAX_PATH, 0);
}