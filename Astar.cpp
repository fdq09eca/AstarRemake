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
			if (updateVisitCost(dv)) {
				MyVectorUtil::remove(history, dv);
				frontiers.emplace_back(dv);
				continue; 
			}
		} 
		else if (MyVectorUtil::contains(frontiers, dv)) {
			//assert(frontiers.find(dv) == frontiers.end()); //should not exist in frontiers.
			updateVisitCost(dv);
				//MyVectorUtil::remove(frontiers, dv);
				//addHistory(dv);
			continue;
			//frontiers.emplace_back(dv);
		}
		else {
			updateVisitCost(dv);
			frontiers.emplace_back(dv);
			comeFrom[&cell(dv)] = &cell(pos);
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

	history.emplace_back(pos);
	pos = nextPos;
	MyVectorUtil::remove(frontiers, nextPos);
	

	return;
}

Maze& Astar::maze() { return App::Instance()->maze; }

const Maze& Astar::maze() const { return App::Instance()->maze; }




void Astar::updatePath() {
	bestPath.clear();
	Cell* p = &cell(pos);
	Cell* s = &cell(src);
	while (p != s) {
		bestPath.emplace_back(maze().cellCol(p), maze().cellRow(p));
		p = comeFrom[p];
	}
}

v2i Astar::minCostFrontier() {
	assert(!frontiers.empty());
	auto minIter = frontiers.begin();
	int minfCost = cell(*minIter).visitCost + estCostToDst(*minIter);

	for (auto iter = frontiers.begin() + 1; iter != frontiers.end(); iter++) {
		auto minfCost_i = cell(*iter).visitCost + estCostToDst(*iter);
		printf("(%d, %d) minCost_i: %d\n", (*iter).x, (*iter).y, minfCost_i);

		//printf("(%d, %d) hCost: %d\n", (*iter).x, (*iter).y, estCostToDst(*iter));
		if (minfCost_i < minfCost) {
			minIter = iter;
			minfCost = minfCost_i;
		}

		if (minfCost_i == minfCost) {
			if (estCostToDst(*iter) < estCostToDst(*minIter)) {
				minIter = iter;
				minfCost = minfCost_i;
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
