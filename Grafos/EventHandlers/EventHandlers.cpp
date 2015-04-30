#include <Windows.h>
#include "EventHandlers.h"


LRESULT MainEventHandler::OnCreate(CreateWindowMessage *msg) {
	return DefWindowProc(msg->hwnd, msg->msg, msg->wParam, msg->lParam);
}

LRESULT MainEventHandler::OnPaint(PaintWindowMessage *msg) {
	HDC			hdc;
	PAINTSTRUCT ps;

	hdc = BeginPaint(msg->hwnd, &ps);
	EndPaint(msg->hwnd, &ps);
	return 0;
}