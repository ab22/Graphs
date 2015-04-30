#include "PaintWindowMessage.h"


PaintWindowMessage::PaintWindowMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) : WinMessageBase(hwnd, msg, wParam, lParam) {
	this->TranslateMessage();
}


void PaintWindowMessage::TranslateMessage() {
	/*
		-WPARAM: Not used
		-LPARAM: Not used
	*/
}