#pragma once

#include "precompile_header.h"

// win32 
#define MY_COLOR_WHITE		RGB(255, 255, 255)
#define MY_COLOR_RED		RGB(255, 0  , 0  )
#define MY_COLOR_GREEN		RGB(0  , 255, 0  )
#define MY_COLOR_DARKGREEN	RGB(3  , 75, 3  )
#define MY_COLOR_SKYBLUE	RGB(136, 26 , 235)
#define MY_COLOR_BLUE		RGB(0  , 0  , 255)
#define MY_COLOR_BLACK		RGB(0  , 0  , 0  )
#define MY_COLOR_GREY		RGB(128, 128, 128)
#define MY_COLOR_BROWN		RGB(124, 71 , 0  )
#define MY_COLOR_YELLOW		RGB(255, 234, 0  )

class NonCopyable {

private:
	NonCopyable(const NonCopyable&) = delete;
	NonCopyable& operator=(const NonCopyable&) = delete;

public:
	NonCopyable() = default;
	~NonCopyable() = default;
};

#define MY_ENUM_OP(E) \
	constexpr E operator& (E x, E y)	{ return static_cast<E>(static_cast<int>(x) & static_cast<int>(y)); } \
	constexpr E operator| (E x, E y)	{ return static_cast<E>(static_cast<int>(x) | static_cast<int>(y)); } \
	constexpr E operator^ (E x, E y)	{ return static_cast<E>(static_cast<int>(x) ^ static_cast<int>(y)); } \
	constexpr E operator~ (E x)			{ return static_cast<E>(~static_cast<int>(x)); } \
	inline void operator&=(E& x, E y)	{ x = x & y; } \
	inline void operator|=(E& x, E y)	{ x = x | y; } \
	inline void operator^=(E& x, E y)	{ x = x ^ y; } \
//--- MY_ENUM_OP ----

enum class MouseButton {
	NA		= 0,
	Left	= 1 << 0,  // 1, 0001
	Middle	= 1 << 1,  // 2, 0010
	Right	= 1 << 2,  // 4, 0100
};
MY_ENUM_OP(MouseButton)

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


class Vec2d {

public:
	double x = 0;
	double y = 0;

	Vec2d(double _x = 0, double _y = 0);
	Vec2d(const Vec2d& v);


	~Vec2d() { x = 0; y = 0; }

	double distance(const Vec2d& p) const;

	double length() const;

	Vec2d unitVector() const;

	double dotProduct(const Vec2d& p) const;

	Vec2d project(const Vec2d& v) const;


	inline void operator=(const Vec2d& p) { *this = Vec2d(p); } //recursive here!!

	inline POINT asPOINT() const { return POINT{ (int)x, (int)y }; }
	inline operator POINT () const { return asPOINT(); }

	inline Vec2d operator+(const Vec2d& v) const { return Vec2d(x + v.x, y + v.y); }
	inline Vec2d operator-(const Vec2d& v) const { return Vec2d(x - v.x, y - v.y); }
	inline Vec2d operator*(const Vec2d& v) const { return Vec2d(x * v.x, y * v.y); }
	inline Vec2d operator/(const Vec2d& v) const { return Vec2d(x / v.x, y / v.y); }

	inline void operator+=(const Vec2d& v) { x += v.x; y += v.y; }
	inline void operator-=(const Vec2d& v) { x -= v.x; y -= v.y; }
	inline void operator*=(const Vec2d& v) { x *= v.x; y *= v.y; }
	inline void operator/=(const Vec2d& v) { x /= v.x; y /= v.y; }

	inline Vec2d operator*(int val) const { return Vec2d(x * val, y * val); }
	inline Vec2d operator/(int val) const { return Vec2d(x / val, y / val); }

	inline void operator*=(int val) { x *= val; y *= val; }
	inline void operator/=(int val) { x /= val; y /= val; }

	inline Vec2d operator*(float val) const { return Vec2d(x * val, y * val); }
	inline Vec2d operator/(float val) const { return Vec2d(x / val, y / val); }

	inline Vec2d operator*(double val) const { return Vec2d(x * val, y * val); }
	inline Vec2d operator/(double val) const { return Vec2d(x / val, y / val); }

	inline bool operator!=(const Vec2d& v) const { return v.x != x || v.y != y; }
	inline bool operator==(const Vec2d& v) const { return !operator!=(v); }
};


class Vec2i {

public:
	int x = 0;
	int y = 0;

	inline POINT asPOINT() const { return POINT{ (int)x, (int)y }; }
	inline operator POINT () const { return asPOINT(); }

	inline Vec2d asVector2d() const { return Vec2d{ (double)x, (double)y }; }
	inline operator Vec2d() const { return asVector2d(); }

	Vec2i(int x_ = 0, int y_ = 0) : x(x_), y(y_) {};
	Vec2i(const POINT& p) : Vec2i(p.x, p.y) {};




	inline bool operator!=(const Vec2i& v) const { return v.x != x || v.y != y; }
	inline bool operator==(const Vec2i& v) const { return !operator!=(v); }

	inline bool operator<(const Vec2i& v) const { 
		if (v.x < x) return true;
		if (v.x > x) return false;

		return v.y < y;
	}

	inline Vec2i operator+(const Vec2i& v) const { return Vec2i(x + v.x, y + v.y); }
	inline Vec2i operator-(const Vec2i& v) const { return Vec2i(x - v.x, y - v.y); }
	inline Vec2i operator*(const Vec2i& v) const { return Vec2i(x * v.x, y * v.y); }
	inline Vec2i operator/(const Vec2i& v) const { return Vec2i(x / v.x, y / v.y); }

	inline Vec2i operator+(int v) const { return Vec2i(x + v, y + v); }
	inline Vec2i operator-(int v) const { return Vec2i(x - v, y - v); }
	inline Vec2i operator*(int v) const { return Vec2i(x * v, y * v); }
	inline Vec2i operator/(int v) const { return Vec2i(x / v, y / v); }
};

inline Vec2i abs(const Vec2i& v) {
	return Vec2i(abs(v.x), abs(v.y));
}


template<class T>
inline void my_bzero(T& s) { memset(&s, 0, sizeof(s)); }


struct MyVectorUtil {

	template<class T>
	static inline bool contains(const std::vector<T>& vec, const T& val) {
		for (const auto& v : vec) {
			if (v == val)
				return true;
		}
		return false;
	}


	template<class T>
	static inline bool remove_unordered(std::vector<T>& vec, T& val) {
		if (vec.size() == 1) {
			if (vec[0] == val) {
				vec.clear();
				return true;
			}
		} else {
			for (auto iter = vec.begin(); iter < vec.end(); iter++) {
				if (*iter == val) {
					*iter = vec.back();
					vec.resize(vec.size() - 1);
					return true;
				}
			}
		}
		return false;
	}


};

enum class MyDir { N, E, S, W };

struct MyDirUtil {
	using MyDir = MyDir;
	using v2i = Vec2i;

	static constexpr MyDir	entries[4]{ MyDir::N, MyDir::E, MyDir::S, MyDir::W };


	MyDir opposite(MyDir d) {
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

	static v2i asVector2i(MyDir d) {
		// { MyDir::N  , MyDir::E , MyDir::S , MyDir::W	    };
		static const v2i dirVectors[4]{ v2i(0, -1), v2i(1, 0), v2i(0, 1), v2i(-1, 0) };
		return dirVectors[static_cast<int>(d)];
	}
};

// Helpers
struct MyRandomUtil {
	static int RandIntInRange(int min_, int max_);
	static int RandInt(int max_ = RAND_MAX);

};

struct MyWin32GuiUtil {
	static void TextOutAt(HDC hdc, Vec2i pos, int fontSize, const wchar_t* text, COLORREF color = MY_COLOR_BLACK);
};
