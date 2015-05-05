#include "AddVertexDialogProc.h"
AddVertexDialogHandler *dialogHandler = NULL;

INT_PTR CALLBACK AddVertexDialogProc(HWND hwndDlg, UINT msg, WPARAM wParam, LPARAM lParam) {
	WinMessageBase* message = NULL;
	INT_PTR         result;

	switch (msg) {
	case WM_INITDIALOG:
		dialogHandler = new AddVertexDialogHandler(hwndDlg);
		return TRUE;
	case WM_COMMAND:
		message = new CommandWindowMessage(hwndDlg, msg, wParam, lParam);
		result = dialogHandler->OnCommand((CommandWindowMessage*)message);
		break;
	case WM_DESTROY:
		if (dialogHandler != NULL) {
			delete dialogHandler;
			dialogHandler = NULL;
		}
		return TRUE;
	default:
		return FALSE;
	}

	if (message != NULL)
		delete message;
	return result;
}