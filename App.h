#pragma once
#include "Maze.h"
#include "Astar.h"



class App : NonCopyable
{
private:
	static App* _instance;
	HWND _hWnd = NULL;
	BackBuffer backBuffer;
	MouseButton _mouseButtonState = MouseButton::NA;

public:
	HPEN dashRedPen = NULL;
	HPEN solidRedPen = NULL;
	HPEN solidBlackPen = NULL;
	HBRUSH solidRedBrush = NULL;
	Maze maze;
	Astar agent;
	
	App() = default;
	
	static App* Instance() { return _instance; }
	
	~App() { destroy(); }

	void init();
	void destroy();
	
	void initTimer(int fps = 10);
	
	void update() { 
		agent.update();
		//printf("update\n");
		
	};

	void setHwnd(HWND hWnd_);

	void draw(HDC hdc_) ;

	void onMouseEvent(const MouseEvent& ev);

	void _onWin32MouseEvent(UINT msg, WPARAM wp, LPARAM lp);


	MouseButton mouseButtonState() const { return _mouseButtonState; }

	inline void save(const wchar_t* fpath) const { };
	inline void load(const wchar_t* fpath) { };
	
};