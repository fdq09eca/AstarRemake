#include "Util.h"

int MyRandomUtil::RandIntInRange(int min_, int max_) {

	/*
	* // exclusive
	* // not uniformly distributed.
	* v1 = rand() % 100;         // v1 in the range 0 to 99
	* v2 = rand() % 100 + 1;     // v2 in the range 1 to 100
	* v3 = rand() % 30 + 1985;   // v3 in the range 1985-2014
	*/

	assert(min_ < max_);
	int range = max_ - min_;
	if (!range) return min_;
	int num = rand() % range + min_;
	return num;
}

int MyRandomUtil::RandInt(int max_) { return RandIntInRange(0, max_); }


void BackBuffer::create(HWND hWnd_) {
	destroy();
	HDC wndDC = GetDC(hWnd_);
	_dc = CreateCompatibleDC(wndDC);

	RECT rc;
	GetClientRect(hWnd_, &rc);
	_h = rc.bottom - rc.top;
	_w = rc.right - rc.left;
	_bitmap = CreateCompatibleBitmap(wndDC, _w, _h); //https://stackoverflow.com/questions/7134465/the-result-of-createcompatibledc-only-has-two-colors
	SelectObject(_dc, _bitmap);
	_bgColor = (HBRUSH)GetStockObject(WHITE_BRUSH);

	ReleaseDC(hWnd_, wndDC);
}

void BackBuffer::destroy() {
	if (_dc) {
		DeleteDC(_dc);
		_dc = NULL;
	}

	if (_bitmap) {
		DeleteObject(_bitmap);
		_bitmap = NULL;
	}
}

void BackBuffer::clear() {
	RECT rc{ 0, 0, _w, _h };
	FillRect(_dc, &rc, _bgColor);
}

void BackBuffer::draw(HDC hdc, int x, int y) {
	BitBlt(hdc, x, y, _w, _h, _dc, 0, 0, SRCCOPY);
}

Vector2D::Vector2D(double _x, double _y) : x(_x), y(_y) { }

Vector2D::Vector2D(const Vector2D& v) { //copy
	x = v.x;
	y = v.y;
}

double Vector2D::distance(const Vector2D& p) const {
	return (p - *this).length();
}

double Vector2D::length() const {
	return sqrt(x * x + y * y);
}

Vector2D Vector2D::unitVector() const {
	return *this / length();
}

double Vector2D::dotProduct(const Vector2D& p) const {
	return x * p.x + y * p.y;
}

Vector2D Vector2D::project(const Vector2D& v) const {
	Vector2D u = v.unitVector();
	double projectLen = dotProduct(u);
	return u * projectLen;
}
