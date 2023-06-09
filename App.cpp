#include "App.h"


App* App::_instance = nullptr;

void App::destroy()
{

	DeleteObject(dashRedPen);
	DeleteObject(solidBlackPen);
	DeleteObject(solidRedBrush);
	KillTimer(_hWnd, timerId);
}

void App::initTimer(int fps) {
	assert(_hWnd != NULL);
	SetTimer(_hWnd, timerId, 0, (TIMERPROC) NULL);
}

void App::init() {
	assert(_instance == nullptr);
	_instance = this;
	
	// create pen and brush..
	solidBlackPen = ::CreatePen(PS_SOLID, 1, RGB(0, 0, 0)); // black solid pen
	dashRedPen = ::CreatePen(PS_DOT, 1, RGB(255, 0, 0)); // red dash pen
	solidRedPen = ::CreatePen(PS_SOLID, 5, RGB(255, 0, 0)); // red solid pen
	solidRedBrush = ::CreateSolidBrush(RGB(255, 0, 0)); // red solid brush

	std::srand(36);

	maze.init(45, 25);
	maze.genRandom(400);
	agent.init({ 5,  5}, { 40, 20 });
}

void App::setHwnd(HWND hWnd_) {
	_hWnd = hWnd_;
	backBuffer.create(_hWnd);
}

void App::draw(HDC hdc_)  {
	backBuffer.clear();

	
	maze.draw(backBuffer.dc());
	agent.draw(backBuffer.dc());
	backBuffer.draw(hdc_);
}


void App::onMouseEvent(const MouseEvent& ev) {

	maze.onMouseEvent(ev);

}


void App::_onWin32MouseEvent(UINT msg, WPARAM wp, LPARAM lp) {

	MouseEvent ev;
	ev.pos = POINT{ GET_X_LPARAM(lp) , GET_Y_LPARAM(lp) };

	switch (msg)
	{
		using B = MouseButton;
		using T = MouseEventType;

	case WM_LBUTTONDOWN: { ev.eventType = T::Down;  ev.button = B::Left;   _mouseButtonState |=  B::Left;	} break;
	case WM_MBUTTONDOWN: { ev.eventType = T::Down;  ev.button = B::Middle; _mouseButtonState |=  B::Middle;	} break;
	case WM_RBUTTONDOWN: { ev.eventType = T::Down;  ev.button = B::Right;  _mouseButtonState |=  B::Right;	} break;
	
	case WM_LBUTTONUP:	 { ev.eventType = T::Up;	ev.button = B::Left;   _mouseButtonState &= ~B::Left;	} break;
	case WM_MBUTTONUP:	 { ev.eventType = T::Up;	ev.button = B::Middle; _mouseButtonState &= ~B::Middle;	} break;
	case WM_RBUTTONUP:	 { ev.eventType = T::Up;	ev.button = B::Right;  _mouseButtonState &= ~B::Right;	} break;

	case WM_MOUSEMOVE: { ev.eventType = T::Move; } break;
	
	default: { assert(false); return; } break;
	}
	


	onMouseEvent(ev);
	InvalidateRect(_hWnd, nullptr, false);

}


