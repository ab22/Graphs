#include "AddNodeDialogHandler.h"


AddNodeDialogHandler::AddNodeDialogHandler(HWND hwnd) {
	this->hwnd = hwnd;
}

AddNodeDialogHandler::~AddNodeDialogHandler() {

}

LRESULT AddNodeDialogHandler::OnCommand(CommandWindowMessage *msg) {
	if (msg->buttonIdentifier == IDOK) {
		TCHAR	*nodeName;
		WORD	nodeNameLength;

		nodeNameLength = (WORD)SendDlgItemMessage(msg->hwnd, IDC_NODE_NAME_EDIT, EM_LINELENGTH, 0, 0);
		if (nodeNameLength == 0) {
			MessageBox(msg->hwnd, TEXT("Please enter the new node's name!"), TEXT("Error"), MB_OK | MB_ICONERROR);
			return TRUE;
		}
		nodeName = new TCHAR[nodeNameLength + 1];
		nodeName[0] = nodeNameLength;
		nodeName[nodeNameLength] = 0;
		SendDlgItemMessage(msg->hwnd, IDC_NODE_NAME_EDIT, EM_GETLINE, 0, (LPARAM)nodeName);
		EndDialog(msg->hwnd, (INT_PTR)nodeName);
	}
	else if (msg->buttonIdentifier == IDCANCEL) {
		EndDialog(msg->hwnd, NULL);
	}
	return TRUE;
}