#include "Astar.h"
#include "App.h"

using v2i = Vector2i;

Cell& Astar::cell(v2i pos_) {
	Cell* p = maze().cellPtr(pos_);
	assert(p);
	return *p;
}

void Astar::init(v2i src_, v2i dst_) {
	pos = src_;
	src = src_;
	dst = dst_;

	if (cell(dst).isBlock()) 
		cell(dst).setBlock(false);
	cell(pos).visitCost = 0;
	history.emplace_back(pos);
	//frontiers.insert(pos);
}

void Astar::update() {
	if (pos == dst) {
		onComplete();
		return;
	}

	//disvoer adjcent cell;
	for (Dir d : MyDirectionUtil::entries) {
		v2i dv = MyDirectionUtil::asVector2i(d);
		dv = pos + dv;
		Cell* p = maze().cellPtr(dv);
		if (!p || p->isBlock()) {
			continue;
		};

 		if (MyVectorUtil::contains(history, dv)) {
			continue;
		}
		
		if (MyVectorUtil::contains(frontiers, dv)) {
			//assert(frontiers.find(dv) == frontiers.end()); //should not exist in frontiers.
			if (!updateVisitCost(dv)) {
				MyVectorUtil::remove(frontiers, dv);
				addHistory(dv);
				continue;
			}
			//frontiers.emplace_back(dv);
		}
		else {
			updateVisitCost(dv);
			frontiers.emplace_back(dv);
		}

	}

	if (frontiers.empty()) {
		onComplete();
		return;
	}

	v2i nextPos = minCostFrontier();
	history.emplace_back(pos);
	pos = nextPos;
	MyVectorUtil::remove(frontiers, nextPos);
	
	return;
}

Maze& Astar::maze() { return App::Instance()->maze; }

const Maze& Astar::maze() const { return App::Instance()->maze; }




v2i Astar::minCostFrontier() {
	assert(!frontiers.empty());
	/*auto minIter = frontiers.begin();
	int minCost = totalCost(*minIter);
	
	for (auto iter = frontiers.begin() + 1; iter != frontiers.end(); iter++) {
		auto _minCost = totalCost(*iter);
		if (_minCost < minCost) {
			minIter = iter;
		}
	}*/
	return *std::min_element(frontiers.begin(), frontiers.end(), [this](const auto& a, const auto& b) { return totalCost(a) < totalCost(b); });
	
	//return *minIter;
}


bool Astar::updateVisitCost(v2i pos_) {
	Cell& c = cell(pos_);
	int newCost = costToNextPos(pos_);
	int oldCost = c.visitCost;


	if (!c.isVisited() || newCost < oldCost) {
		c.visitCost = newCost;
		return true;
	}

	return false;
}

void Astar::onComplete() {
	assert(false);
}

void Astar::draw(HDC hdc) {
	cell(pos).drawAt(hdc, maze().cellPixelPos(pos), MY_COLOR_RED, estCostToDst(pos));

	for (const auto& f : frontiers) {
		cell(f).drawAt(hdc, maze().cellPixelPos(f), MY_COLOR_YELLOW, estCostToDst(f));
	}

	cell(dst).drawAt(hdc, maze().cellPixelPos(dst), MY_COLOR_GREEN);
}
