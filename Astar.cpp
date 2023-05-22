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

 		if (MyVectorUtil::contains(history, dv)) { continue; }
		
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
	int tc = fCost(nextPos);
	if (tc < 0) {
		printf("tc: %d < 0", tc);
	}
	if (comeFrom.find(&cell(nextPos)) == comeFrom.end()) {
		comeFrom[&cell(nextPos)] = &cell(pos);
	}
	else {
		auto p = comeFrom[&cell(nextPos)];
		auto pv = v2i(maze().cellCol(p), maze().cellRow(p));
		auto old_cost = fCost(pv) + estCostToDst(pv);
		auto new_cost = fCost(pos) + estCostToDst(pos);
		if (new_cost < old_cost) {
			comeFrom[&cell(nextPos)] = &cell(pos);
		}


	}

	
	history.emplace_back(pos);
	pos = nextPos;
	MyVectorUtil::remove(frontiers, nextPos);
	//updatePath();
	
	return;
}

Maze& Astar::maze() { return App::Instance()->maze; }

const Maze& Astar::maze() const { return App::Instance()->maze; }




void Astar::updatePath() {
	Cell* p = &cell(dst);
	Cell* s = &cell(src);


	while (p != s) {

		bestPath.emplace_back(maze().cellCol(p), maze().cellRow(p));
		p = comeFrom[p];
	}

}

v2i Astar::minCostFrontier() {
	assert(!frontiers.empty());
	auto minIter = frontiers.begin();
	int minCost = cell(*minIter).visitCost + estCostToDst(*minIter);
	
	for (auto iter = frontiers.begin() + 1; iter != frontiers.end(); iter++) {
		auto minCost_i = cell(*iter).visitCost + estCostToDst(*iter);
		printf("(%d, %d) minCost_i: %d\n", (*iter).x, (*iter).y, minCost_i);
		
		//printf("(%d, %d) hCost: %d\n", (*iter).x, (*iter).y, estCostToDst(*iter));
		if (minCost_i < minCost) {
			minIter = iter;
			minCost = minCost_i;
		}

		if (minCost_i == minCost) {
			if (estCostToDst(*iter) < estCostToDst(*minIter)) {
				minIter = iter;
				minCost = minCost_i;
			}
		}
		
	}
		printf("===========\n");
	
	v2i r = *minIter;
	//printf("minCost: %d\n", fCost(r));
	return r;
	//v2i r = * std::min_element(frontiers.begin(), frontiers.end(), [this](const v2i& a, const v2i& b) { return fCost(a) + estCostToDst(a) < fCost(b) + estCostToDst(a); });  //bug here
	//printf("minCost: %d\n", fCost(r));
	//return r;
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
	updatePath();
	return;
	//assert(false);
}

void Astar::draw(HDC hdc) {
	cell(pos).drawAt(hdc, maze().cellPixelPos(pos), MY_COLOR_RED, estCostToDst(pos));

	for (const auto& f : frontiers) {
		cell(f).drawAt(hdc, maze().cellPixelPos(f), MY_COLOR_YELLOW, estCostToDst(f));
	}

	for (const auto& f : history) {
		cell(f).drawAt(hdc, maze().cellPixelPos(f), MY_COLOR_GREY, estCostToDst(f));
	}

	for (const auto& f : bestPath) {
		cell(f).drawAt(hdc, maze().cellPixelPos(f), MY_COLOR_GREEN, estCostToDst(f));
	}

	cell(dst).drawAt(hdc, maze().cellPixelPos(dst), MY_COLOR_GREEN);
	
}
