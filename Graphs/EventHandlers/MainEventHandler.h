#pragma once


#include <Windows.h>
#include <CommCtrl.h>

#include "../Infrastructure/Messages/WindowMessages.h"
#include "../DialogProcedures/AddNodeDialogProc.h"
#include "../Algorithms/Graphs.h"
#include "../resource.h"


// Manifest to use the latest CommCtl32 library which
// enables the newest windows' Visual styles
// ----------------------------------------------------
// ref: https://msdn.microsoft.com/en-us/library/windows/desktop/bb773175%28v=vs.85%29.aspx
#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")


enum TOOLBAR_BUTTON {
	ADD_NODE,
	DELETE_NODE,
	EXIT
};


struct MainWindowControls {
	HWND mainToolbar;
};


class MainEventHandler {
private:
	MainWindowControls  mainWindowControls;
	HINSTANCE           hInstance;
	Grafos              graphs;
	HWND                hwnd;
	int                 charWidth;
	int                 charHeight;

public:
	MainEventHandler();
	~MainEventHandler();

	// WM_CREATE: Called when the window is being created.
	LRESULT OnCreate(CreateWindowMessage*);
	// WM_PAINT: Called when the window needs to be redrawn.
	LRESULT OnPaint(PaintWindowMessage*);
	// WM_SIZE: Called when the window was resized;
	LRESULT OnResize(ResizeWindowMessage*);
	// WM_COMMAND: Called when a button, menu button or accelerator was triggered
	LRESULT OnCommand(CommandWindowMessage*);

private:
	LRESULT  onToolbarAddNodeClick();
	LRESULT  onToolbarDeleteNodeClick();
	LRESULT  onToolbarExitClick();
	BOOL     initCommonVisualControls();
	void     calculateDefaultFontSizes(HDC);	
	void     createMainToolbar();
	
};