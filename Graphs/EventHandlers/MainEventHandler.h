#pragma once


#include <Windows.h>
#include <CommCtrl.h>
#include <tchar.h>

#include "../Infrastructure/Messages/WindowMessages.h"

#include "../DialogProcedures/StrongComponentsDialogProc.h"
#include "../DialogProcedures/DeleteVertexDialogProc.h"
#include "../DialogProcedures/DeleteNodeDialogProc.h"
#include "../DialogProcedures/AddVertexDialogProc.h"
#include "../DialogProcedures/AddNodeDialogProc.h"
#include "../Algorithms/Graphs.h"
#include "../resource.h"


// Manifest to use the latest CommCtl32 library which
// enables the newest visual styles for windows.
// ----------------------------------------------------
// ref: https://msdn.microsoft.com/en-us/library/windows/desktop/bb773175%28v=vs.85%29.aspx
#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")


enum TOOLBAR_BUTTON {
	SHOW_NODES,
	ADD_NODE,
	DELETE_NODE,
	ADD_VERTEX,
	DELETE_VERTEX,
	ORGANIZE_NAME,
	ORGANIZE_TOPOLOGY,
	IS_ACYCLIC,
	SHOW_DIJKSTRA_LABELS,
	SHOW_FLOYD_MATRICES,
	STRONG_COMPONENTS,
	EXIT
};


struct MainWindowControls {
	HWND mainToolbar;
};


class MainEventHandler {
private:
	MainWindowControls  mainWindowControls;
	HINSTANCE           hInstance;
	Graphs              graphs;
	HWND                hwnd;
	int                 charWidth;
	int                 charHeight;

	TCHAR*              tmpNodeName;
	bool                showNodes;
	bool                showFloydMatrices;
	bool                showDijkstraLabels;
	bool                organizeByName;
	bool                addingNode;	

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
	// WM_LBUTTONDOWN: Called when the left mouse button is clicked
	LRESULT OnLeftMouseButtonClick(LeftMouseButtonDownMessage*);

private:
	LRESULT  onToolbarAddNodeClick();
	LRESULT  onToolbarDeleteNodeClick();
	LRESULT  onToolbarAddVertexClick();
	LRESULT  onToolbarDeleteVertexClick();
	LRESULT  onToolbarOrganizeByNameClick();
	LRESULT  onToolbarOrganizeByTopologyClick();
	LRESULT  onToolbarShowDijkstraLabelsClick();
	LRESULT  onToolbarShowFloydMatricesClick();
	LRESULT  onToolbarCheckAcyclicityClick();
	LRESULT  onToolbarStrongComponentsClick();
	LRESULT  onToolbarShowNodesClick();
	LRESULT  onToolbarExitClick();
	BOOL     initCommonVisualControls();
	void     calculateDefaultFontSizes(HDC);	
	void     createMainToolbar();
	void     drawNodes(HDC);
	void     drawFloydMatrices(HDC);
	bool     addNode(int, int);
	bool     addVertex(TCHAR*, TCHAR*, TCHAR*);
	bool     deleteVertex(TCHAR*, TCHAR*);	
};