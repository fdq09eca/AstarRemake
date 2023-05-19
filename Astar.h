#pragma once
#include "Util.h"
#include "App.h"

class Astar
{
	using v2i = Vector2i;
	using Dir = MyDirection;
private:
	std::vector<v2i> _neighbours;
	Cell& cell(v2i pos_) {
		Cell* p = maze().cellPtr(pos_);
		assert(p);
		return *p;
	}

public:
	v2i pos{ 0, 0 };
	v2i src{ 0, 0 };
	v2i dst{ 0, 0 };
	std::vector<v2i> bestPath;
	std::set<v2i> frontiers;


	void init(v2i src_, v2i dst_) {
		pos = src_;
		src = src_;
		dst = dst_;
		frontiers.insert(pos);
	}

	v2i nextPosition() {
		int i = 0;
		v2i nextPos = *frontiers.begin();
		int nextCost = totalCost(nextPos);

		for (v2i f : frontiers) {
			if (totalCost(f) < nextCost) {
				nextPos = f;
			}
		}
		return nextPos;
	}

	void update() {
		if (frontiers.size() == 0 || pos == dst) {
			onComplete();
			return;
		}

		//find out the cheapestFrontier
		v2i nextPos = nextPosition();
		

		frontiers.erase(nextPos);





	}

	Maze& maze() { return App::Instance()->maze; }
	const Maze& maze() const { return App::Instance()->maze; }


	bool neighbours() {
		_neighbours.clear();

		for (Dir d : MyDirectionUtil::entries) {
			v2i dv = MyDirectionUtil::asVector2i(d);
			Cell p = cell(dv);
			if (p.isBlock()) continue;
			_neighbours.emplace_back(dv);
		}
	}



	inline int estCostToDst(const v2i& pos_) { return (int)Distance2d::manhattan(pos_, dst); }
	inline int costToNextPos(const v2i& nextPos_) { return (int)Distance2d::manhattan(pos, nextPos_) + currentCost(); }
	inline int totalCost(const v2i& nextPos_) { return costToNextPos(nextPos_) + estCostToDst(nextPos_); }

	int& visitCost(v2i pos_) { return cell(pos_).visitCost; }
	int& currentCost() { visitCost(pos); }




	void updateVisitCost(v2i pos) {
		Cell& c = cell(pos);
		int newCost = costToNextPos(pos);
		int oldCost = c.visitCost;


		if (!c.isVisited() || newCost < oldCost) {
			cell(pos).visitCost = newCost;
		}
	}

	void onComplete() {
		//makeBestPath()
	}
	

};

