#pragma once

#include <Windows.h>
#include <tchar.h>

#include "../Algorithms/Graphs.h"
#include "../resource.h"
#include "../Infrastructure/Messages/WindowMessages.h"


class StrongComponentsDialogHandler {
private:
	HWND hwnd;

public:
	StrongComponentsDialogHandler(HWND);
	~StrongComponentsDialogHandler();

	// WM_INITDIALOG: Called when the dialog is created. Basically a constructor.
	LRESULT OnInit(InitDialogMessage*);
	// WM_COMMAND: Called when a button, menu button or accelerator was triggered
	LRESULT OnCommand(CommandWindowMessage*);

private:
	LRESULT  onOkButtonClick();
	void      addStrongComponents(Graphs*);
};