#include "Cell.h"



void Cell::setType(Type t) {
	_type = t;
	if (t == Type::Block) { visitCost = -1; }
}

COLORREF Cell::color() const {
	if (isBlock())
		return MY_COLOR_BLACK;
	return isVisited() ? MY_COLOR_GREY : MY_COLOR_WHITE;
}

void Cell::drawAt(HDC hdc, POINT pos) const {
	drawAt(hdc, pos, color());

	TextOut(hdc, pos.x + 3, pos.y + 3, L"cost", 4);

}


void Cell::drawAt(HDC hdc, POINT pos, COLORREF color_) const {
	auto oldBrush = SelectObject(hdc, GetStockObject(DC_BRUSH));
	auto oldColor = SetDCBrushColor(hdc, color_);
	::Rectangle(hdc, pos.x, pos.y, pos.x + size, pos.y + size);
	SetDCBrushColor(hdc, oldColor);
	SelectObject(hdc, oldBrush);
}
