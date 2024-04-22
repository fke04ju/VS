#include "framework.h"
#include "Point2D.h"
#include "Engine.h"

#pragma comment(lib, "d2d1")
#pragma comment(lib, "dwrite")

ID2D1SolidColorBrush* m_pRedBrush;
ID2D1SolidColorBrush* m_pWhiteBrush;
ID2D1SolidColorBrush* m_pBlackBrush;
ID2D1SolidColorBrush* m_pDarkRedBrush;
ID2D1SolidColorBrush* m_pYellowBrush;

int winner = 0;
bool largeOrSmall = false;
int selectingFrame = 0;
int currState = ST_PENDING;
int animateEnd = 0;
int AnimateEndTime = 0;

Engine::Engine(HWND hwnd)
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
		hr = CreateDeviceResources(hwnd);
	}


	m_pRT->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), &m_pRedBrush);
	m_pRT->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &m_pWhiteBrush);
	m_pRT->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &m_pBlackBrush);
	m_pRT->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::DarkRed), &m_pDarkRedBrush);
	m_pRT->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Yellow), &m_pYellowBrush);

	DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&m_pWriteFactory));
	m_pWriteFactory->CreateTextFormat(L"Arial", NULL, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 30.0f, L"en-us", &m_pScoreTextFormat);
	m_pScoreTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);
	m_pWriteFactory->CreateTextFormat(L"Arial", NULL, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 30.0f, L"en-us", &m_pWriteTextFormat);
	m_pWriteTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);

	roulettePicture.resize(28);
	score = new Score();
	roulette = new Roulette(m_pD2DFactory, m_pRT, m_pConvertedSourceBitmap, m_pWriteFactory, m_pWriteTextFormat, m_pScoreTextFormat);
	betArea = new BetArea(m_pD2DFactory, m_pRT, m_pConvertedSourceBitmap, m_pWriteFactory, m_pWriteTextFormat, m_pScoreTextFormat);

	isBetStart = false;
	isWinner = false;
	isLargesmallStart = false;
	getPoint = false;
}

Engine::~Engine()
{
	delete score;
	delete roulette;
	delete betArea;
	SafeRelease(m_pConvertedSourceBitmap);
	SafeRelease(m_pIWICFactory);
	SafeRelease(m_pD2DFactory);
	SafeRelease(m_pRT);
}

HRESULT Engine::CreateDeviceResources(HWND hWnd)
{
	HRESULT hr = S_OK;
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

	return hr;
}

HRESULT Engine::InitializeD2D(HWND hwnd)
{
	return S_OK;
}

void Engine::Animater(HWND hwnd)
{
	HRESULT hr = S_OK;
	PAINTSTRUCT ps;
	WCHAR wscore[10] = L"";

	if (BeginPaint(hwnd, &ps)) {
		hr = CreateDeviceResources(hwnd);
		if (SUCCEEDED(hr) && !(m_pRT->CheckWindowState() & D2D1_WINDOW_STATE_OCCLUDED))
		{
			m_pRT->BeginDraw();
			m_pRT->Clear(D2D1::ColorF(D2D1::ColorF::White));

			// Draw Background
			roulette->Background->Draw(hwnd, 0, 0, RESOLUTION_X, RESOLUTION_Y, m_pRT);
			roulette->RouletteBackground->Draw(hwnd, 100, 100, 600, 600, m_pRT);

			// Draw Roulette and Bet area
			roulette->Draw(hwnd);
			betArea->Draw(hwnd);

			// Draw Points
			swprintf_s(wscore, L"%d", score->CurrentWinScore);
			m_pRT->DrawTextW(wscore, wcslen(wscore), m_pScoreTextFormat, D2D1::RectF(score->CurrentPoint.x, score->CurrentPoint.y, score->CurrentPoint.x + 200, score->CurrentPoint.y + 50), m_pRedBrush);
			swprintf_s(wscore, L"%d", score->TotalScore);
			m_pRT->DrawTextW(wscore, wcslen(wscore), m_pScoreTextFormat, D2D1::RectF(score->TotalPoint.x, score->TotalPoint.y, score->TotalPoint.x + 200, score->TotalPoint.y + 50), m_pRedBrush);
			swprintf_s(wscore, L"%d", score->BetScore);
			m_pRT->DrawTextW(wscore, wcslen(wscore), m_pWriteTextFormat, D2D1::RectF(score->LargeSmallPoint.x - 10, score->LargeSmallPoint.y, score->LargeSmallPoint.x + 60, score->LargeSmallPoint.y + 50), m_pRedBrush);
			for (int i = 0; i < 7; i++)
			{
				swprintf_s(wscore, L"%d", score->scorePoints[i]);
				m_pRT->DrawTextW(wscore, wcslen(wscore), m_pWriteTextFormat, D2D1::RectF(score->pointArea[i].x - 10, score->pointArea[i].y, score->pointArea[i].x + 100, score->pointArea[i].y + 100), m_pBlackBrush);
			}
		}
		hr = m_pRT->EndDraw();
		// In case of device loss, discard D2D render target and D2DBitmap
		// They will be re-create in the next rendering pass
		if (hr == D2DERR_RECREATE_TARGET)
		{
			SafeRelease(m_pRT);
			// Force a re-render
		}
		hr = InvalidateRect(hwnd, NULL, TRUE) ? S_OK : E_FAIL;
	}
	EndPaint(hwnd, &ps);

}

void Engine::Selecting(int currTime) // mode 2
{
	timestamp.clear();
	roulette->ClearLight();
	betArea->ClearLight();
	roulette->rouletteWinner = score->BetRoulette();
	int later = 1;
	for (int i = 0; i < 28; i++)
	{
		timestamp.push_back({ currTime + later * 500, i, true, 1 });
		timestamp.push_back({ currTime + (later + 1) * 500, i, false, 1 });
		later++;
	}
	for (int i = 0; i <= roulette->rouletteWinner; i++)
	{
		timestamp.push_back({ currTime + later * 500, i, true, 1 });
		timestamp.push_back({ currTime + (later + 1) * 500, i, false, 1 });
		later++;
	}
	AnimateEndTime = currTime + later * 500;
}

void Engine::Winning(int currTime) // mode 3
{
	timestamp.push_back({ currTime + 500, roulette->rouletteWinner, true, 1 });
	timestamp.push_back({ currTime + 1000, roulette->rouletteWinner, false, 1 });
	timestamp.push_back({ currTime + 500, roulette->roulettePicture[roulette->rouletteWinner], true, 2 });
	timestamp.push_back({ currTime + 1000, roulette->roulettePicture[roulette->rouletteWinner], false, 2 });
}

void Engine::LargeAndSmall(int currTime) // mode 4
{
	//largeOrSmall = score->BetLargeSmall(choose);
	timestamp.clear();
	for (int i = 0; i < 10; i++)
	{
		srand(static_cast<unsigned int>((std::time(nullptr) + i)));
		score->Comparing();
		timestamp.push_back({ currTime + 500 * i, score->BetScore, true, (i % 2 == 0) ? 3 : 4 });
	}
	AnimateEndTime = currTime + 5000;
}

void Engine::Pending(int currTime)
{
	for (int i = 0; i < 28; i++)
	{
		if (i % 2 == 0)
		{
			timestamp.push_back({ currTime, i, false , 1 });
			timestamp.push_back({ currTime + 500, i, true , 1 });
			timestamp.push_back({ currTime + 1000, i, false , 1 });
			timestamp.push_back({ currTime + 1500, i, true , 1 });
			timestamp.push_back({ currTime + 2000, i, false , 1 });
			timestamp.push_back({ currTime + 2500, i, true , 1 });
			timestamp.push_back({ currTime + 3000, i, false , 1 });
			timestamp.push_back({ currTime + 3500, i, true , 1 });
			timestamp.push_back({ currTime + 4000, i, false , 1 });
		}
		else
		{
			timestamp.push_back({ currTime, i, false , 1 });
			timestamp.push_back({ currTime + 500, i, false , 1 });
			timestamp.push_back({ currTime + 1000, i, false , 1 });
			timestamp.push_back({ currTime + 1500, i, false , 1 });
			timestamp.push_back({ currTime + 2000, i, false , 1 });
			timestamp.push_back({ currTime + 2500, i, false , 1 });
			timestamp.push_back({ currTime + 3000, i, true , 1 });
			timestamp.push_back({ currTime + 3500, i, false , 1 });
			timestamp.push_back({ currTime + 4000, i, true , 1 });
		}
		//timestamp.push_back({ currTime, i, (i % 2 == 0) ? true : false , 1 });
		//timestamp.push_back({ currTime + 500, i, (i % 2 == 1) ? true : false , 1 });
	}
	for (int i = 0; i < 7; i++)
	{
		timestamp.push_back({ currTime, i, (i % 2 == 0) ? true : false , 2 });
		timestamp.push_back({ currTime + 500, i, (i % 2 == 1) ? true : false , 2 });
		timestamp.push_back({ currTime + 1000, i, (i % 2 == 0) ? true : false , 2 });
		timestamp.push_back({ currTime + 1500, i, (i % 2 == 1) ? true : false , 2 });
		timestamp.push_back({ currTime + 2000, i, (i % 2 == 0) ? true : false , 2 });
		timestamp.push_back({ currTime + 2500, i, (i % 2 == 1) ? true : false , 2 });
		timestamp.push_back({ currTime + 3000, i, (i % 2 == 0) ? true : false , 2 });
		timestamp.push_back({ currTime + 3500, i, (i % 2 == 1) ? true : false , 2 });
		timestamp.push_back({ currTime + 4000, i, (i % 2 == 0) ? true : false , 2 });
	}
}

void Engine::ReadJSON(string filename, HWND hwnd)
{
	Json::Reader reader;
	Json::Value data;
	std::ifstream is;
	is.open(filename, std::ios::binary);
	if (!reader.parse(is, data, false))
	{
		return;
	}

	Json::Value rouletteArr = data["Roulette"];
	for (int i = 0; i < rouletteArr.size(); i++)
	{
		std::string str = rouletteArr[i].asString();
		roulette->roulettePicture[i] = stoi(str);
	}

	for (int i = 0; i < data["Picture"].size(); i++)
	{
		std::string str = data["Picture"][i]["Path"].asString();
		wstring ws(str.begin(), str.end());
		Component* tc = new Component();
		wcscpy_s(tc->componentPath, ws.c_str());
		tc->OpenImageFile(hwnd, tc->componentPath, m_pRT);
		roulette->componentSet.push_back(tc);
		betArea->componentSet.push_back(tc);
		str = data["Picture"][i]["Ratio"].asString();
		score->scoreRatio[i] = stoi(str);
	}

	for (int i = 0; i < data["Special Picture"].size(); i++)
	{
		std::string str = data["Special Picture"][i]["Path"].asString();
		wstring ws(str.begin(), str.end());
		Component* tsc = new Component();
		wcscpy_s(tsc->componentPath, ws.c_str());
		tsc->OpenImageFile(hwnd, tsc->componentPath, m_pRT);
		roulette->specialComponentSet.push_back(tsc);
		str = data["Special Picture"][i]["Ratio"].asString();
		score->specialRatio[i] = stoi(str);
	}

	std::string str = data["Background"]["Path"].asString();
	wstring ws(str.begin(), str.end());
	roulette->Background = new Component();
	wcscpy_s(roulette->Background->componentPath, ws.c_str());
	roulette->Background->OpenImageFile(hwnd, roulette->Background->componentPath, m_pRT);
	str = data["RouletteBackground"]["Path"].asString();
	wstring ws1(str.begin(), str.end());
	roulette->RouletteBackground = new Component();
	wcscpy_s(roulette->RouletteBackground->componentPath, ws1.c_str());
	roulette->RouletteBackground->OpenImageFile(hwnd, roulette->RouletteBackground->componentPath, m_pRT);

	is.close();
}

void Engine::updateState(int currentTime)
{
	int i = 0;
	for (i = 0;i<timestamp.size();)
	{
		if (timestamp[i].timeToDo <= currentTime)
		{
			if (timestamp[i].RorB == 1)
			{
				roulette->ChangeState(timestamp[i]);
			}
			else if (timestamp[i].RorB == 2)
			{
				betArea->ChangeState(timestamp[i]);
			}
			else
			{
				score->BetScore = timestamp[i].lightNumber;
				roulette->ChangeLargeSmallState(timestamp[i]);
			}
			timestamp.erase(timestamp.begin()+i);
		}
		else
		{
			i++;
		}
	}
}

void Engine::PressEvent(int xPos, int yPos)
{
	betArea->Press(xPos, yPos);
	if (betArea->num >= 0)
	{
		if (betArea->isOne)
		{
			score->ScoreControl(betArea->num, 1);
		}
		else if (betArea->isTen)
		{
			score->ScoreControl(betArea->num, 10);
		}
		else if (betArea->isClear)
		{
			score->ScoreControl(betArea->num, 0);
		}
	}
}

void Engine::Logic()
{
	int currentTime = GetTickCount64();
	updateState(currentTime);
	// State machine
	if (currState == ST_PENDING)
	{
		if (timestamp.empty())
		{
			Pending(currentTime);
		}
		if (isBetStart)
		{
			if (score->CheckBet())
			{
				currState = ST_ENOUGHMONEYTOBET;
			}
			else
			{
				MessageBox(NULL, L"請先下注!", L"ERROR", MB_OK);
				isBetStart = false;
			}
		}
	}
	else if (currState == ST_ENOUGHMONEYTOBET)
	{
		currState = ST_SELECTING;
	}
	else if (currState == ST_SELECTING)
	{
		if (isBetStart)
		{
			isBetStart = false;
			Selecting(currentTime);
			isWinner = true;
		}
		// animate end
		if (isWinner && currentTime > AnimateEndTime)
		{
			currState = ST_WINNING;
		}
	}
	else if (currState == ST_WINNING)
	{
		if (timestamp.empty())
		{
			Winning(currentTime);
		}
		if (isWinner)
		{
			int winPicID = roulette->roulettePicture[roulette->rouletteWinner];
			if (winPicID > 7) // sad
			{
				score->CurrentWinScore = 0;
			}
			else
			{
				score->CurrentWinScore = (score->scorePoints[winPicID] * score->scoreRatio[winPicID]);
			}
			score->ClearBet();
			isWinner = false;
		}
		if (isLargesmallStart) // start to bet large or small
		{
			isLargesmallStart = false;
			if (score->CurrentWinScore > 0)
			{
				isCompareStart = true;
				currState = ST_ENOUGHMONEYTOCOMPARE;
			}
			else
			{
				MessageBox(NULL, L"沒有足夠金額!", L"ERROR", MB_OK);
				currState = ST_PENDING;
			}
			isWinner = false;
		}
		if (getPoint)
		{
			getPoint = false;
			GetPoint();
			currState = ST_PENDING;
		}
	}
	else if (currState == ST_ENOUGHMONEYTOCOMPARE)
	{
		currState = ST_COMPARE_SELECTING;
	}
	else if (currState == ST_COMPARE_SELECTING)
	{
		if (isCompareStart)
		{
			isCompareStart = false;
			// large small animation
			LargeAndSmall(currentTime);
			isComparing = true;
		}
		if (isComparing && currentTime > AnimateEndTime)
		{
			currState = ST_COMPARE_WINNING;
		}
	}
	else if (currState == ST_COMPARE_WINNING)
	{
		if (isComparing)
		{
			isComparing = false;
			// large small winner
			if (score->BetLargeSmall(guess))
			{
				score->CurrentWinScore = score->CurrentWinScore * 2;
				isLSWinner = true;
			}
			else
			{
				score->CurrentWinScore = 0;
				currState = ST_PENDING;
			}
		}
		if (isLSWinner)
		{
			if (isLargesmallStart)
			{
				currState = ST_WINNING;
			}
		}
		else
		{
			currState = ST_PENDING;
		}
	}
}

void Engine::AutoMode()
{

}

void Engine::GetPoint()
{
	score->GetPoint();
}
