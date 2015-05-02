#pragma once

#include <Windows.h>
#include "../resource.h"
#include "../Infrastructure/Messages/WindowMessages.h"


class AddNodeDialogHandler{
private:
	HWND				hwnd;

public:
	AddNodeDialogHandler(HWND);
	~AddNodeDialogHandler();

	// WM_COMMAND: Called when a button, menu button or accelerator was triggered
	LRESULT OnCommand(CommandWindowMessage*);

private:
};