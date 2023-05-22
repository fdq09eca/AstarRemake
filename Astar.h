#pragma once
#include "Util.h"
#include "Maze.h"


class Astar
{
	using v2i = Vector2i;
	using Dir = MyDirection;
private:
	
	
	Cell& cell(v2i pos_);

public:
	v2i pos{ 0, 0 };
	v2i src{ 0, 0 };
	v2i dst{ 0, 0 };
	std::vector<v2i> bestPath;
	std::vector<v2i> frontiers;
	std::vector<v2i> history;
	std::map<Cell*, Cell*> comeFrom;

	void init(v2i src_, v2i dst_);

	void update();

	Maze& maze();
	const Maze& maze() const;

	void updatePath() ;

	v2i minCostFrontier();

	inline int estCostToDst(const v2i& pos_)		{ return (int)Distance2d::manhattan(pos_, dst); }
	inline int costToNextPos(const v2i& nextPos_)	{ return (int)Distance2d::manhattan(pos, nextPos_) + currentCost(); }
	inline int& visitCost(v2i pos_) { return cell(pos_).visitCost; }
	inline int& currentCost()		{ return visitCost(pos); }

	bool updateVisitCost(v2i pos_);

	void onComplete();

	void draw(HDC hdc);
	

};

