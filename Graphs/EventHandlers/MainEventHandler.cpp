#include "MainEventHandler.h"


MainEventHandler::MainEventHandler() {
	this->hwnd = NULL;
	this->hInstance = NULL;
	this->charHeight = 0;
	this->charWidth = 0;
}

MainEventHandler::~MainEventHandler() {

}

LRESULT MainEventHandler::OnCreate(CreateWindowMessage *msg) {
	HDC hdc;

	hdc = GetDC(msg->hwnd);
	this->hwnd = msg->hwnd;
	this->hInstance = msg->createStruct->hInstance;
	this->calculateDefaultFontSizes(hdc);
	this->initCommonVisualControls();
	this->createMainToolbar();
	ReleaseDC(msg->hwnd, hdc);

	return DefWindowProc(msg->hwnd, msg->msg, msg->wParam, msg->lParam);
}

LRESULT MainEventHandler::OnPaint(PaintWindowMessage *msg) {
	PAINTSTRUCT ps;
	HDC         hdc;

	hdc = BeginPaint(msg->hwnd, &ps);
	EndPaint(msg->hwnd, &ps);

	return 0;
}

LRESULT MainEventHandler::OnResize(ResizeWindowMessage *msg) {
	// Resize the main toolbar
	SendMessage(this->mainWindowControls.mainToolbar, TB_AUTOSIZE, 0, 0);
	return 0;
}

LRESULT MainEventHandler::OnCommand(CommandWindowMessage *msg) {
	if (msg->buttonIdentifier == TOOLBAR_BUTTON::ADD_NODE) {
		return this->onToolbarAddNodeClick();
	}
	else if (msg->buttonIdentifier == TOOLBAR_BUTTON::DELETE_NODE) {
		return this->onToolbarDeleteNodeClick();
	}
	else if (msg->buttonIdentifier == TOOLBAR_BUTTON::EXIT) {
		return this->onToolbarExitClick();
	}
	return FALSE;
}

void MainEventHandler::calculateDefaultFontSizes(HDC hdc) {
	TEXTMETRIC tm;

	GetTextMetrics(hdc, &tm);
	this->charWidth = tm.tmAveCharWidth;
	this->charHeight = tm.tmHeight + tm.tmInternalLeading;
}

BOOL MainEventHandler::initCommonVisualControls() {
	INITCOMMONCONTROLSEX	commControls;
	commControls.dwSize = sizeof(INITCOMMONCONTROLSEX);
	commControls.dwICC = ICC_STANDARD_CLASSES | ICC_BAR_CLASSES;
	return InitCommonControlsEx(&commControls);
}

void MainEventHandler::createMainToolbar() {
	const DWORD buttonStyles = BTNS_AUTOSIZE;
	HIMAGELIST  imageList = NULL;
	const int   bitmapSize = 16;
	const int   numButtons = 3;
	const int   imageListId = 0;
	TBBUTTON    tbButtons[3];
	HWND        toolBar = NULL;

	toolBar = CreateWindowEx(
		0,
		TOOLBARCLASSNAME,
		NULL,
		WS_CHILD | WS_VISIBLE | WS_BORDER,
		0,
		0,
		0,
		0,
		this->hwnd,
		NULL,
		0,
		NULL
		);

	imageList = ImageList_Create(
		bitmapSize,
		bitmapSize,
		ILC_COLOR16 | ILC_MASK,
		numButtons,
		0
		);

	SendMessage(toolBar, TB_SETIMAGELIST, (WPARAM)imageListId, (LPARAM)imageList);
	SendMessage(toolBar, TB_LOADIMAGES, (WPARAM)IDB_STD_SMALL_COLOR, (LPARAM)HINST_COMMCTRL);

	tbButtons[0] = { MAKELONG(STD_FILENEW, imageListId), TOOLBAR_BUTTON::ADD_NODE, TBSTATE_ENABLED, buttonStyles, { 0 }, 0, (INT_PTR)TEXT("Add Node") };
	tbButtons[1] = { MAKELONG(STD_CUT, imageListId), TOOLBAR_BUTTON::DELETE_NODE, TBSTATE_ENABLED, buttonStyles, { 0 }, 0, (INT_PTR)L"Delete Node" };
	tbButtons[2] = { MAKELONG(STD_DELETE, imageListId), TOOLBAR_BUTTON::EXIT, TBSTATE_ENABLED, buttonStyles, { 0 }, 0, (INT_PTR)L"Exit" };

	// Add buttons.
	SendMessage(toolBar, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0);
	SendMessage(toolBar, TB_ADDBUTTONS, (WPARAM)numButtons, (LPARAM)&tbButtons);
	SendMessage(toolBar, TB_AUTOSIZE, 0, 0);

	this->mainWindowControls.mainToolbar = toolBar;
}

LRESULT MainEventHandler::onToolbarAddNodeClick() {
	TCHAR* nodeName;

	nodeName = (TCHAR*)DialogBox(this->hInstance, MAKEINTRESOURCE(IDD_ADD_NODE_DIALOG), this->hwnd, AddNodeDialogProc);
	if (nodeName != NULL) {
		MessageBox(hwnd, nodeName, TEXT("New node name"), MB_OK | MB_ICONINFORMATION);
		delete nodeName;
	}
	else {
		MessageBox(hwnd, TEXT("No node was specified!"), TEXT("No new node!"), MB_OK | MB_ICONINFORMATION);
	}
	return TRUE;
}

LRESULT MainEventHandler::onToolbarDeleteNodeClick() {
	MessageBox(this->hwnd, TEXT("Show delete node dialog!"), TEXT("Delete node"), MB_OK | MB_ICONINFORMATION);
	return TRUE;
}

LRESULT MainEventHandler::onToolbarExitClick() {
	SendMessage(this->hwnd, WM_CLOSE, 0, 0);
	return TRUE;
}