#pragma once

#include <Windows.h>
#include "../resource.h"
#include "../Infrastructure/Messages/WindowMessages.h"


class DeleteNodeDialogHandler {
private:
	HWND hwnd;

public:
	DeleteNodeDialogHandler(HWND);
	~DeleteNodeDialogHandler();

	// WM_COMMAND: Called when a button, menu button or accelerator was triggered
	LRESULT OnCommand(CommandWindowMessage*);

private:
	LRESULT onOkButtonClick();
	LRESULT onCancelButtonClick();
};