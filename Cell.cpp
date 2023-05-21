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
}


void Cell::drawAt(HDC hdc, POINT pos, COLORREF color_) const {
	auto oldBrush = SelectObject(hdc, GetStockObject(DC_BRUSH));
	auto oldColor = SetDCBrushColor(hdc, color_);
	::Rectangle(hdc, pos.x, pos.y, pos.x + size, pos.y + size);
	SetDCBrushColor(hdc, oldColor);
	SelectObject(hdc, oldBrush);

	if (!isBlock()) {

		auto gCostPost = Vector2i(pos) + 3;
		auto hCostPost = Vector2i(pos) + 3;
		auto fCostPost = Vector2i(pos) + size / 2;
		hCostPost.x += size / 2 + 4;
		gCostPost.x += 2;
		fCostPost.x -= 8;
		fCostPost.y -= 2;

		auto w_gCost = std::to_wstring(visitCost);
		
		MyWin32GuiUtil::TextOutAt(hdc, gCostPost, size / 3, w_gCost.data(), MY_COLOR_RED);
		
	}
}

void Cell::drawAt(HDC hdc, POINT pos, COLORREF color_, int hCost) const {
	drawAt(hdc, pos, color_);
	
	if (!isBlock()) {
		
		auto gCostPost = Vector2i(pos) + 3;
		auto hCostPost = Vector2i(pos) + 3;
		auto fCostPost = Vector2i(pos) + size/2;
		hCostPost.x += size/2 + 4;
		gCostPost.x += 2;
		fCostPost.x -= 8;
		fCostPost.y -= 2;

		auto w_gCost = std::to_wstring(visitCost);
		auto w_hCost = std::to_wstring(hCost);
		auto w_fCost = std::to_wstring(visitCost + hCost);
		MyWin32GuiUtil::TextOutAt(hdc, gCostPost, size/3, w_gCost.data(), MY_COLOR_BLUE);
		MyWin32GuiUtil::TextOutAt(hdc, hCostPost, size/3, w_hCost.data(), MY_COLOR_BLUE);
		MyWin32GuiUtil::TextOutAt(hdc, fCostPost, size/2, w_fCost.data(), MY_COLOR_BLACK);
	}
}




