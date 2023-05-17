#pragma once
#include <stdio.h>
#include <Windows.h>
#include "windowsx.h"
#include <string>
#include <vector>
#include <set>
#include <map>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <fstream>
#include <algorithm>

// win32 
#define MY_COLOR_WHITE		RGB(255,255,255)
#define MY_COLOR_RED		RGB(255,0  ,0  )
#define MY_COLOR_GREEN		RGB(0  ,255,0  )
#define MY_COLOR_BLUE		RGB(0  ,  0,255)
#define MY_COLOR_BLACK		RGB(0  ,0  ,0  )
#define MY_COLOR_GREY		RGB(128,128,128)
#define MY_COLOR_YELLOW	RGB(255,234,0  )

class NonCopyable {

private:
	NonCopyable(const NonCopyable&) = delete;
	NonCopyable& operator=(const NonCopyable&) = delete;

public:
	NonCopyable() = default;
	~NonCopyable() = default;
};

enum class MouseButton {
	NA = 0,
	Left = 1 << 0,    // 1, 0001
	Middle = 1 << 1,  // 2, 0010 
	Right = 1 << 2,   // 4, 0100
};

inline constexpr MouseButton operator& (MouseButton x, MouseButton y) {
	return static_cast<MouseButton>(static_cast<int>(x) & static_cast<int>(y));
}

inline constexpr MouseButton operator| (MouseButton x, MouseButton y) {
	return static_cast<MouseButton>(static_cast<int>(x) | static_cast<int>(y));
}

inline constexpr MouseButton operator^(MouseButton x, MouseButton y) {
	return static_cast<MouseButton> (static_cast<int>(x) ^ static_cast<int>(y));
}

inline constexpr MouseButton operator~(MouseButton x) {
	return static_cast<MouseButton>(~static_cast<int>(x));
}

inline MouseButton& operator&=(MouseButton& x, MouseButton y) {
	x = x & y;
	return x;
}

inline MouseButton& operator|=(MouseButton& x, MouseButton y) {
	x = x | y;
	return x;
}

inline MouseButton& operator^=(MouseButton& x, MouseButton y) {
	x = x ^ y;
	return x;
}


enum class MouseEventType {
	None = 0,
	Up,
	Down,
	Move,
};



class MouseEvent {
public:
	using Type = MouseEventType;
	using Button = MouseButton;
	POINT pos{ 0,0 };
	Type eventType = Type::None;
	Button button = Button::NA;
	Button buttonState = Button::NA;

	bool isUp()	  const { return eventType == Type::Up; }
	bool isDown() const { return eventType == Type::Down; }
	bool isMove() const { return eventType == Type::Move; }

	bool isLButton() const { return button == Button::Left; }
	bool isMButton() const { return button == Button::Middle; }
	bool isRButton() const { return button == Button::Right; }
};

class BackBuffer : NonCopyable {
	HBITMAP _bitmap;
	HBRUSH _bgColor = NULL;
	HDC _dc = NULL;
	int _w = 0;
	int _h = 0;

public:

	~BackBuffer() { destroy(); }

	void create(HWND hWnd_);

	HDC dc() const { return _dc; }

	void destroy();

	void clear();

	void draw(HDC hdc, int x = 0, int y = 0);
};


class Vector2D {

public:
	double x = 0;
	double y = 0;

	Vector2D(double _x = 0, double _y = 0);
	Vector2D(const Vector2D& v);
	

	~Vector2D() { x = 0; y = 0; }

	double distance(const Vector2D& p) const;

	double length() const;

	Vector2D unitVector() const;

	double dotProduct(const Vector2D& p) const;

	Vector2D project(const Vector2D& v) const;


	inline void operator=(const Vector2D& p) { *this = Vector2D(p); } //recursive here!!
	
	inline POINT asPOINT() const { return POINT{ (int)x, (int)y }; }
	inline operator POINT () const { return asPOINT(); }

	inline Vector2D operator+(const Vector2D& v) const { return Vector2D(x + v.x, y + v.y); }
	inline Vector2D operator-(const Vector2D& v) const { return Vector2D(x - v.x, y - v.y); }
	inline Vector2D operator*(const Vector2D& v) const { return Vector2D(x * v.x, y * v.y); }
	inline Vector2D operator/(const Vector2D& v) const { return Vector2D(x / v.x, y / v.y); }

	inline void operator+=(const Vector2D& v) { x += v.x; y += v.y; }
	inline void operator-=(const Vector2D& v) { x -= v.x; y -= v.y; }
	inline void operator*=(const Vector2D& v) { x *= v.x; y *= v.y; }
	inline void operator/=(const Vector2D& v) { x /= v.x; y /= v.y; }

	inline Vector2D operator*(int val) const { return Vector2D(x * val, y * val); }
	inline Vector2D operator/(int val) const { return Vector2D(x / val, y / val); }

	inline void operator*=(int val) { x *= val; y *= val; }
	inline void operator/=(int val) { x /= val; y /= val; }

	inline Vector2D operator*(float val) const { return Vector2D(x * val, y * val); }
	inline Vector2D operator/(float val) const { return Vector2D(x / val, y / val); }

	inline Vector2D operator*(double val) const { return Vector2D(x * val, y * val); }
	inline Vector2D operator/(double val) const { return Vector2D(x / val, y / val); }

	inline bool operator!=(const Vector2D& v) const { return v.x != x || v.y != y; }
	inline bool operator==(const Vector2D& v) const { return !operator!=(v); }
};


class Vector2i {

public:
	int x = 0;
	int y = 0;

	inline POINT asPOINT() const { return POINT{ (int)x, (int)y }; }
	inline operator POINT () const { return asPOINT(); }

	Vector2i(int x_ = 0, int y_ = 0) : x(x_), y(y_) {};
	Vector2i(const POINT& p) : Vector2i(p.x, p.y) {};
	
	inline bool operator!=(const Vector2i& v) const { return v.x != x || v.y != y; }
	inline bool operator==(const Vector2i& v) const { return !operator!=(v); }

	inline Vector2i operator+(const Vector2i& v) const { return Vector2i(x + v.x, y + v.y); }
	inline Vector2i operator-(const Vector2i& v) const { return Vector2i(x - v.x, y - v.y); }
	inline Vector2i operator*(const Vector2i& v) const { return Vector2i(x * v.x, y * v.y); }
	inline Vector2i operator/(const Vector2i& v) const { return Vector2i(x / v.x, y / v.y); }

	inline Vector2i operator+(int v) const { return Vector2i(x + v, y + v); }
	inline Vector2i operator-(int v) const { return Vector2i(x - v, y - v); }
	inline Vector2i operator*(int v) const { return Vector2i(x * v, y * v); }
	inline Vector2i operator/(int v) const { return Vector2i(x / v, y / v); }
	

};



template<class T>
inline void my_bzero(T& s) { memset(&s, 0, sizeof(s)); }

template<class T>
inline bool contains(const std::vector<T>& vec, T val) {
	for (const auto& v : vec) {
		if (v == val)
			return true;
	}
	return false;
}

enum class MyDirection { N, E, S, W };

struct MyDirectionUtil {
	using MyDir = MyDirection;
	static constexpr MyDir entries[4]{ MyDir::N, MyDir::E, MyDir::S, MyDir::W };


	MyDir opposite(MyDirection d) {
		switch (d)
		{
		case MyDir::N:	return MyDir::S;
		case MyDir::E:	return MyDir::W;
		case MyDir::S:	return MyDir::N;
		case MyDir::W:	return MyDir::E;
		default:
			assert(false);
		}
	}
};


// Helpers
struct MyRandomUtil {
	static int RandIntInRange(int min_, int max_);
	static int RandInt(int max_ = RAND_MAX);

};

struct MyWin32GuiUtil {
	static void TextOutAt(HDC hdc, Vector2i pos, int fontSize, const wchar_t* text, COLORREF color = MY_COLOR_BLACK);
};
