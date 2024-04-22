#pragma once
#include "Point2D.h"

class Score
{
public:
	Score();
	~Score();
	void ScoreControl(int id, int flag);
	void ClearBet();
	void GetPoint();
	int BetRoulette();
	bool BetLargeSmall(bool flag);
	bool CheckBet();
	void Comparing();

	vector<int> scorePoints;	// 各水果的數量
	vector<int> scoreRatio;		// 各水果的賠率
	vector<int> specialRatio;	// 特殊賠率

	Point2D CurrentPoint, TotalPoint, LargeSmallPoint;
	vector<Point2D> pointArea;

	int TotalScore;
	int CurrentWinScore;
	int BetScore;

private:
};