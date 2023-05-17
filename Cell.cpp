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
	Vector2i txtPos = pos;
	MyWin32GuiUtil::TextOutAt(hdc, Vector2i(pos) + 3, size/3, L"12", MY_COLOR_RED);

	

}


void Cell::drawAt(HDC hdc, POINT pos, COLORREF color_) const {
	auto oldBrush = SelectObject(hdc, GetStockObject(DC_BRUSH));
	auto oldColor = SetDCBrushColor(hdc, color_);
	::Rectangle(hdc, pos.x, pos.y, pos.x + size, pos.y + size);
	SetDCBrushColor(hdc, oldColor);
	SelectObject(hdc, oldBrush);
}
