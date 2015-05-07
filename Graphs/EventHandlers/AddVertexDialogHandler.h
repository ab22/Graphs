#pragma once

#include <Windows.h>
#include "../resource.h"
#include "../Infrastructure/Messages/WindowMessages.h"


class AddVertexDialogHandler {
private:
	HWND hwnd;

public:
	AddVertexDialogHandler(HWND);
	~AddVertexDialogHandler();

	// WM_COMMAND: Called when a button, menu button or accelerator was triggered
	LRESULT OnCommand(CommandWindowMessage*);

private:
	LRESULT onOkButtonClick();
	LRESULT onCancelButtonClick();

	TCHAR*  getTextFromInput(int);
};