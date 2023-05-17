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
	
	if (!isBlock()) {
		wchar_t buff[4];
		swprintf(buff, 4, L"%d", visitCost);
		auto gCostPost = Vector2i(pos) + 3;
		auto hCostPost = Vector2i(pos) + 3;
		hCostPost.x += size/2 + 4;
		gCostPost.x += 2;
		
		MyWin32GuiUtil::TextOutAt(hdc, gCostPost, size/3, buff, MY_COLOR_RED);
		MyWin32GuiUtil::TextOutAt(hdc, hCostPost, size/3, buff, MY_COLOR_RED);
	}

	

}


void Cell::drawAt(HDC hdc, POINT pos, COLORREF color_) const {
	auto oldBrush = SelectObject(hdc, GetStockObject(DC_BRUSH));
	auto oldColor = SetDCBrushColor(hdc, color_);
	::Rectangle(hdc, pos.x, pos.y, pos.x + size, pos.y + size);
	SetDCBrushColor(hdc, oldColor);
	SelectObject(hdc, oldBrush);
}
