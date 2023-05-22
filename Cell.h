#pragma once
#include "Util.h"


enum class CellType { Unknown = 0, Block, Path };


class Cell
{
private:
	using Type = CellType;
	Type _type = Type::Unknown;
	
public:
	static const int size = 32;
	int visitCost = -1; //
	

	Cell(){ reset(); }
	~Cell() { }
	inline void reset() { setType(Type::Path); visitCost = -1; }
	
	void setType(Type t);
	inline void setAsBlock()			{ setType(Type::Block); }
	inline void setBlock(bool b)			{ b? setType(Type::Block) : setType(Type::Path); }
	inline bool isVisited() const	{ return visitCost >= 0; };
	inline bool isBlock()	const	{ return _type == Type::Block; };
	

	COLORREF color() const;

	void drawAt(HDC hdc, POINT pos) const;
	void drawAt(HDC hdc, POINT pos, COLORREF color_) const;
	void drawAt(HDC hdc, POINT pos, COLORREF color_, int hCost) const;


};


