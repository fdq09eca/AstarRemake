#include "Maze.h"


void Maze::init(int nRow_, int nCol_) {
	nRow = nRow_;
	nCol = nCol_;
	cells.resize(nCells());

	genRandom(MyRandomUtil::RandInt(nCells()/2));

}

const Cell* Maze::cellPtr(int r, int c) const {
	if (r >= 0 && r < nRow && c >= 0 && c < nCol)
		return &cells[r * nCol + c];
	return nullptr;
}

Cell* Maze::cellPtr(int r, int c) {
	if (r >= 0 && r < nRow && c >= 0 && c < nCol)
		return &cells[r * nCol + c];
	return nullptr;
}



const Cell& Maze::cell(int r, int c) const {
	auto p = cellPtr(r, c);
	assert(p);
	return *p;
}

const Cell& Maze::cell(POINT p) const {
	int r = p.y / Cell::size;
	int c = p.x / Cell::size;
	return cell(r, c);
}

void Maze::genRandom(int nb) {
	// https://www.youtube.com/watch?v=Y37-gB83HKE&t=514s
	reset();
	std::vector<int> vals;
	int i;
	for (int n = 0; n < nb; n++) {
		Cell* p = nullptr;
		while (true) {
			i = MyRandomUtil::RandInt(nRow * nCol - 1);
			if (contains(vals, i)) { continue; };
			break;
		}
		vals.emplace_back(i);
		cells[i].setAsBlock();
	}
}

POINT Maze::cellPixelPos(int r, int c)  const {
	POINT pos;
	pos.x = r * Cell::size + xOffset;
	pos.y = c * Cell::size + yOffset;
	return pos;
}

void Maze::draw(HDC hdc) const {
	for (int c = 0; c < nCol; c++) {
		for (int r = 0; r < nRow; r++) {
			auto p = cellPtr(r, c);
			assert(p);
			POINT pos = cellPixelPos(r, c);
			p->drawAt(hdc, pos);
		}
	}
}
