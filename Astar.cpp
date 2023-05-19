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
	cell(pos).visitCost = 0;
	//frontiers.insert(pos);
}

v2i Astar::nextPosition() {
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

void Astar::update() {
	if (pos == dst) {
		onComplete();
		return;
	}

	for (Dir d : MyDirectionUtil::entries) {
		v2i dv = MyDirectionUtil::asVector2i(d);
		dv = pos + dv;
		Cell* p = maze().cellPtr(dv);
		if (!p) continue;
		if (p->isBlock()) continue;
		updateVisitCost(dv);
		frontiers.insert(dv);
	}


	return;
}

Maze& Astar::maze() { return App::Instance()->maze; }

const Maze& Astar::maze() const { return App::Instance()->maze; }

bool Astar::updateNeighbours(v2i& nextPos) {
	int minCost = -1;

	for (Dir d : MyDirectionUtil::entries) {
		v2i dv = MyDirectionUtil::asVector2i(d);
		Cell p = cell(dv);
		if (p.isBlock()) continue;
		updateVisitCost(dv);
		frontiers.insert(dv);
		int neighbourCost = totalCost(dv);
		if (neighbourCost < minCost) {
			nextPos = dv;
			minCost = neighbourCost;
		}
	}
	return minCost < 0;
}

void Astar::onVisit(v2i pos_) {
	pos = pos_;
}

void Astar::updateVisitCost(v2i pos_) {
	Cell& c = cell(pos_);
	int newCost = costToNextPos(pos_);
	int oldCost = c.visitCost;


	if (!c.isVisited() || newCost < oldCost) {
		c.visitCost = newCost;
	}
}

void Astar::onComplete() {
	//makeBestPath()
}
