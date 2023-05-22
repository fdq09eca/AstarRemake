#include "Maze.h"


void Maze::init(int nRow_, int nCol_) {
	nRow = nRow_;
	nCol = nCol_;
	cells.resize(nCells());

	//genRandom(MyRandomUtil::RandInt(nCells() / 2));
	genRandom(20);

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

Cell* Maze::cellPtr(Vector2i v) {
	if (v.y >= 0 && v.y < nRow && v.x >= 0 && v.x < nCol)
		return &cells[v.y * nCol + v.x];
	return nullptr;	
}



const Cell& Maze::cell(int r, int c) const {
	auto p = cellPtr(r, c);
	assert(p);
	return *p;
}

Cell& Maze::cell(POINT p) {
	int r = p.x / Cell::size;
	int c = p.y / Cell::size;
	return *cellPtr(r, c);
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
			if (MyVectorUtil::contains(vals, i)) { continue; };
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

POINT Maze::cellPixelPos(Vector2i v) const { return cellPixelPos(v.y, v.x); }

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

bool Maze::onMouseEvent(MouseEvent ev) {

	if (ev.isLButton()) {
		if (ev.isUp()) {
			Cell& c = cell(ev.pos);

			if (c.isBlock()) {
				c.setBlock(false);
				return true;
			}
			else {
				c.setBlock(true);
				return true;
			}
		}
	}
	return false;
}

