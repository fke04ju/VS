#include "framework.h"
#include "Score.h"

Score::Score()
{
	scorePoints.resize(8);
	scoreRatio.resize(8);
	specialRatio.resize(3);
	for (int i = 0; i < 8; i++)
	{
		scorePoints[i] = 0;
		scoreRatio[i] = 0;
	}
	TotalScore = 10000;
	CurrentWinScore = 0;
	BetScore = 0;
	CurrentPoint = { 150, 200 };
	TotalPoint = { 450, 200 };
	LargeSmallPoint = { 375, 500 };
	pointArea.resize(8);
	pointArea[0] = { 815, 125 };
	pointArea[1] = { 915, 125 };
	pointArea[2] = { 1015, 125 };
	pointArea[3] = { 1115, 125 };
	pointArea[4] = { 815, 425 };
	pointArea[5] = { 915, 425 };
	pointArea[6] = { 1015, 425 };
	pointArea[7] = { 1115, 425 };
}

Score::~Score()
{

}

void Score::ScoreControl(int id, int flag)
{
	if (flag == 1) // one point
	{
		scorePoints[id] ++;
		TotalScore--;
	}
	else if (flag == 10) // ten point
	{
		scorePoints[id] += 10;
		TotalScore -= 10;
	}
	else if (flag == 0) // clear
	{
		TotalScore += scorePoints[id];
		scorePoints[id] = 0;
	}
}

void Score::ClearBet()
{
	for (int i = 0; i < 7; i++)
	{
		scorePoints[i] = 0;
	}
}

void Score::GetPoint()
{
	TotalScore += CurrentWinScore;
	CurrentWinScore = 0;
}

int Score::BetRoulette()
{
	srand((unsigned)time(NULL));
	return rand() % 28;
}

void Score::Comparing()
{
	BetScore = rand() % 10;
}

bool Score::BetLargeSmall(bool flag)
{
	srand((unsigned)time(NULL));
	BetScore = rand() % 10;

	if (flag == true) // large
	{
		if (BetScore >= 5)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else // small
	{
		if (BetScore >= 5)
		{
			return false;
		}
		else
		{
			return true;
		}
	}
}

bool Score::CheckBet()
{
	bool hasMoney = false;
	for (int i = 0; i < 7; i++)
	{
		if (scorePoints[i] > 0)
		{
			hasMoney = true;
		}
	}
	return hasMoney;
}
