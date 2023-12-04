#include "Astar.h"
#include "App.h"

using v2i = Vec2i;

Cell* Astar::cell(v2i pos_) {
	return maze().cell(pos_);
}

void Astar::init(v2i src_, v2i dst_) {
	curPos = src_;
	src = src_;
	dst = dst_;

	if (auto* dstCell = cell(dst)) {
		dstCell->setBlock(false);
	}

	if (auto* srcCell = cell(src)) {
		srcCell->visitCost = 0;
	}
}

void Astar::update() {
	if (curPos == dst) {
		onComplete();
		return;
	}

	//discover adjcent cell;
	for (const Dir& d : MyDirUtil::entries) {
		v2i dv = MyDirUtil::asVector2i(d);
		auto nextPos = curPos + dv;
		
		Cell* nextCell = cell(nextPos);
		if (!nextCell || nextCell->isBlock()) {
			continue;
		};

		if (MyVectorUtil::contains(frontiers, nextPos)) {
			updateVisitCost(nextPos);
			continue;
		}
		
		if (nextCell->isVisited()) {
			if (updateVisitCost(nextPos)) {
				frontiers.emplace_back(nextPos);
			}
			continue;
		}
		
		updateVisitCost(nextPos);
		frontiers.emplace_back(nextPos);
		comeFrom[nextCell] = cell(curPos);
	}

	if (frontiers.empty()) {
		onComplete();
		return;
	}

	v2i nextPos = minCostFrontier();
	curPos = nextPos;
	MyVectorUtil::remove_unordered(frontiers, nextPos);
	return;
}

Maze& Astar::maze() { return App::Instance()->maze; }

const Maze& Astar::maze() const { return App::Instance()->maze; }

void Astar::updatePath() {
	bestPath.clear();
	Cell* p = cell(curPos);
	Cell* s = cell(src);
	while (p != s) {
		bestPath.emplace_back(maze().cellCol(p), maze().cellRow(p));

		auto it = comeFrom.find(p);
		if (it != comeFrom.end()) {
			p = it->second;
		} else {
			assert(false);
		}
	}
}

v2i Astar::minCostFrontier() {
	assert(!frontiers.empty());
	auto minIter = frontiers.begin();

	int visitCostScalar = 1;
	int estCostScalar   = 2;

	int minVisitCost = cell(*minIter)->visitCost * visitCostScalar;
	int minEstCost   = estCostToDst(*minIter)    * estCostScalar;

	for (auto iter = frontiers.begin() + 1; iter != frontiers.end(); iter++) {
		int visitCost = cell(*iter)->visitCost * visitCostScalar;
		int estCost   = estCostToDst(*iter)    * estCostScalar;

		if (visitCost + estCost > minVisitCost + minEstCost) {
			continue;
		}

		if (visitCost + estCost == minVisitCost + minEstCost) {
			if (estCost > minEstCost) {
				continue;
			}
		}

		minIter = iter;
		minVisitCost = visitCost;
		minEstCost   = estCost;
	}
//	printf("===========\n");

	v2i r = *minIter;
	//printf("minCost: %d\n", fCost(r));
	return r;
	//v2i r = * std::min_element(frontiers.begin(), frontiers.end(), [this](const v2i& a, const v2i& b) { return fCost(a) + estCostToDst(a) < fCost(b) + estCostToDst(a); });  //bug here
	//printf("minCost: %d\n", fCost(r));
	//return r;
}

int Astar::visitCost(const v2i& pos_) {

	auto* c = cell(pos_);
	if (!c) {
		assert(false);
		return INT_MAX;
	}
	return c->visitCost;
}

bool Astar::updateVisitCost(v2i nextPos) {
	auto* cell = maze().cell(nextPos);
	if (!cell) return false;

	int newCost = costToNextPos(nextPos);
	int oldCost = cell->visitCost;


	if (!cell->isVisited() || newCost < oldCost) {
		cell->visitCost = newCost;
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

	for (const auto& f : frontiers) {
		drawCell(hdc, f, MY_COLOR_YELLOW, estCostToDst(f));
	}

	//for (const auto& f : history) {
	//	cell(f)->drawAt(hdc, maze().cellPixelPos(f), MY_COLOR_GREY, estCostToDst(f));
	//}

	for (const auto& f : bestPath) {
		drawCell(hdc, f, MY_COLOR_GREEN, estCostToDst(f));
	}

	drawCell(hdc, dst, MY_COLOR_GREEN);
	drawCell(hdc, curPos, MY_COLOR_RED, estCostToDst(curPos));
}

void Astar::drawCell(HDC hdc, const v2i& pos, COLORREF color_) {
	if (auto* c = cell(pos)) {
		c->drawAt(hdc, maze().cellScreenPos(pos), color_);
	}
}

void Astar::drawCell(HDC hdc, const v2i& pos, COLORREF color_, int hCost) {
	if (auto* c = cell(pos)) {
		c->drawAt(hdc, maze().cellScreenPos(pos), color_, hCost);
	}
}
