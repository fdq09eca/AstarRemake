#include "Maze.h"


void Maze::init(int nCol_, int nRow_) {
	nRow = nRow_;
	nCol = nCol_;
	cells.resize(nCells());

	//genRandom(MyRandomUtil::RandInt(nCells() / 2));
//	genRandom(200);

}

Cell* Maze::cell(Vec2i v) {
	if (v.x >= 0 && v.x < nCol && v.y >= 0 && v.y < nRow)
		return &cells[v.y * nCol + v.x];
	return nullptr;
}

Cell* Maze::cellFromScreenPoint(POINT p) {
	int r = p.x / Cell::size;
	int c = p.y / Cell::size;
	return cell({c, r});
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

POINT Maze::cellScreenPos(const Vec2i& v) const { 
	POINT pos;
	pos.x = v.x * Cell::size + xOffset;
	pos.y = v.y * Cell::size + yOffset;
	return pos;
}

void Maze::draw(HDC hdc) {
	for (int r = 0; r < nRow; r++) {
		for (int c = 0; c < nCol; c++) {
			auto* p = cell({c, r});
			assert(p);
			POINT pos = cellScreenPos({c, r});
			p->drawAt(hdc, pos);
		}
	}
}

bool Maze::onMouseEvent(MouseEvent ev) {

	if (ev.isLButton() && ev.isUp()) {
		if (Cell* c = cell(ev.pos)) {
			c->setBlock(!c->isBlock());
			return true;
		}
	}
	return false;
}

