#pragma once
#include "Util.h"
#include "Maze.h"


class Astar
{
	using v2i = Vec2i;
	using Dir = MyDir;
private:
	
	
	Cell* cell(v2i pos_);

public:
	v2i curPos{ 0, 0 };
	v2i src{ 0, 0 };
	v2i dst{ 0, 0 };
	std::vector<v2i> bestPath;
	std::vector<v2i> frontiers;
	std::map<Cell*, Cell*> comeFrom;

	void init(v2i src_, v2i dst_);

	void update();

	Maze& maze();
	const Maze& maze() const;

	void updatePath() ;

	v2i minCostFrontier();

	inline int estCostToDst(const v2i& pos_) {
		auto d = abs(dst - pos_);
		return d.x + d.y;
	}

	inline int costToNextPos(const v2i& nextPos_) {
		auto d = abs(nextPos_ - curPos);
		return d.x + d.y + currentCost(); 
	}

	int visitCost(const v2i& pos_);
	inline int currentCost()		{ return visitCost(curPos); }

	bool updateVisitCost(v2i pos_);

	void onComplete();

	void draw(HDC hdc);
	void drawCell(HDC hdc, const v2i& pos, COLORREF color_);
	void drawCell(HDC hdc, const v2i& pos, COLORREF color_, int hCost);
};

