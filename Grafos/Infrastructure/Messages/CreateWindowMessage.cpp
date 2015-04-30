#include "CreateWindowMessage.h"


CreateWindowMessage::CreateWindowMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam): WinMessageBase(hwnd, msg, wParam, lParam) {
	this->TranslateMessage();
}


void CreateWindowMessage::TranslateMessage() {
	/* 
		-WPARAM: Not used
		-LPARAM: Pointer to a CREATESTRUCT structure that contains information
				 about the window being created.
	*/
	this->createStruct = (CREATESTRUCT *)lParam;
}