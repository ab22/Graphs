#include "AddNodeDialogHandler.h"


AddNodeDialogHandler::AddNodeDialogHandler(HWND hwnd) {
	this->hwnd = hwnd;
}

AddNodeDialogHandler::~AddNodeDialogHandler() {

}

LRESULT AddNodeDialogHandler::OnCommand(CommandWindowMessage *msg) {
	if (msg->buttonIdentifier == IDOK) {
		return this->onOkButtonClick();
	}
	else if (msg->buttonIdentifier == IDCANCEL) {
		return this->onCancelButtonClick();
	}
	return TRUE;
}

LRESULT AddNodeDialogHandler::onOkButtonClick() {
	TCHAR*	nodeName;
	WORD	nodeNameLength;

	nodeNameLength = (WORD)SendDlgItemMessage(this->hwnd, IDC_NODE_NAME_EDIT, EM_LINELENGTH, 0, 0);

	if (nodeNameLength == 0) {
		MessageBox(this->hwnd, TEXT("Please enter the new node's name!"), TEXT("Error"), MB_OK | MB_ICONERROR);
		return TRUE;
	}

	nodeName = new TCHAR[nodeNameLength + 1];
	// Null terminated string.
	nodeName[nodeNameLength] = 0;
	// Set first byte to the size of the string array.
	// This is required by the EM_GETLINE message.
	nodeName[0] = nodeNameLength;
	SendDlgItemMessage(this->hwnd, IDC_NODE_NAME_EDIT, EM_GETLINE, 0, (LPARAM)nodeName);
	EndDialog(this->hwnd, (INT_PTR)nodeName);
	
	return TRUE;
}

LRESULT AddNodeDialogHandler::onCancelButtonClick() {
	EndDialog(this->hwnd, NULL);
	return TRUE;
}