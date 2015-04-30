#pragma once
#include <Windows.h>

class WinMessageBase{
public:
	HWND hwnd;
	UINT msg;
	WPARAM wParam;
	LPARAM lParam;

	WinMessageBase(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

protected:
	virtual void TranslateMessage() = 0;
};