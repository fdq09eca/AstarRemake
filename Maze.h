#pragma once
#include "Cell.h"

class Maze //https://www.youtube.com/watch?v=sVcB8vUFlmU
{
public:
	std::vector<Cell> cells;
	int nRow = 0;
	int nCol = 0;
	int yOffset = 0; 
	int xOffset = 0;
	
	void init(int nRow_ = 10, int nCol_ = 10);

	const Cell* cellPtr(int r, int c) const;

	Cell* cellPtr(int r, int c);
	
	const Cell& cell(int r, int c) const;
	
	const Cell& cell(POINT p) const;

	inline int width() const { return nCol * Cell::size; }
	inline int height() const { return nRow * Cell::size; }
	inline int nCells() const { return nRow * nCol; }
	
	inline bool inRange(int r, int c) const { return (r >= 0 && r < nRow && c >= 0 && c < nCol); }

	inline void reset() { for (auto& c : cells) { c.reset(); } }
	
	void genRandom(int nb);

	void genDefault();

	void restart() { for (auto& c : cells) {  if (!c.isBlock())  c.reset(); } }

	POINT cellPixelPos(int r, int c) const;

	std::vector<Cell*> unVisitedCells() {
		std::vector<Cell*> vec;
		for (auto& c : cells) { 
			if (c.isBlock()) continue;
			if (!c.isVisited()) {  vec.emplace_back(&c);  }
		}
		return vec;
	}
	
	
	inline int cellRow(Cell* p) const {
		int d = static_cast<int>(p - &cells.front());
		int r = d / nCol;
		assert(r >= 0 && r < nRow);
		return r;
	}

	inline int cellCol(Cell* p) const {
		int d = static_cast<int>(p - &cells.front());
		int c = d - cellRow(p) * nCol;
		assert(c >= 0 && c < nCol);
		return c;
	}

	
	void draw(HDC hdc) const;

	
};

