#include "Main.h"

MainEventHandler *mainEventHandler = NULL;

LRESULT CALLBACK WinProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	WinMessageBase *message = NULL;
	LRESULT			result  = 0;
	switch (msg) {
	case WM_CREATE:
		message = new CreateWindowMessage(hwnd, msg, wParam, lParam);
		result = mainEventHandler->OnCreate((CreateWindowMessage *)message);
		break;
	case WM_COMMAND:
		message = new CommandWindowMessage(hwnd, msg, wParam, lParam);
		result = mainEventHandler->OnCommand((CommandWindowMessage *)message);
		break;
	case WM_PAINT:
		message = new PaintWindowMessage(hwnd, msg, wParam, lParam);
		result = mainEventHandler->OnPaint((PaintWindowMessage *)message);
		break;
	case WM_SIZE:
		message = new ResizeWindowMessage(hwnd, msg, wParam, lParam);
		result = mainEventHandler->OnResize((ResizeWindowMessage *)message);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return result;
	default: 
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}

	if (message != NULL)
		delete message;
	return result;
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, char *lpCmdLine, int nShowCmd) {
	mainEventHandler = new MainEventHandler();

	WNDCLASS wc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = CreateSolidBrush(RGB(245, 245, 245));
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hIcon = LoadIcon(0, IDI_WINLOGO);
	wc.hInstance = hInstance;
	wc.lpfnWndProc = WinProc;
	wc.lpszClassName = APP_CLASS_NAME;
	wc.lpszMenuName = NULL;
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	RegisterClass(&wc);

	HWND hwnd = CreateWindow(
		APP_CLASS_NAME,
		APP_WND_TITLE,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		0,
		0,
		hInstance,
		0
	);
	ShowWindow(hwnd, SW_MAXIMIZE);
	UpdateWindow(hwnd);

	MSG msg;
	while (GetMessage(&msg, 0, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	delete mainEventHandler;
	return msg.wParam;
}