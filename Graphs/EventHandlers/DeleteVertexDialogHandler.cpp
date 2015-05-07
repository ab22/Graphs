#include "DeleteVertexDialogHandler.h"


DeleteVertexDialogHandler::DeleteVertexDialogHandler(HWND hwnd) {
	this->hwnd = hwnd;
}

DeleteVertexDialogHandler::~DeleteVertexDialogHandler() {

}

LRESULT DeleteVertexDialogHandler::OnCommand(CommandWindowMessage *msg) {
	if (msg->buttonIdentifier == IDOK) {
		return this->onOkButtonClick();
	}
	else if (msg->buttonIdentifier == IDCANCEL) {
		return this->onCancelButtonClick();
	}
	return TRUE;
}

LRESULT DeleteVertexDialogHandler::onOkButtonClick() {
	TCHAR** values = new TCHAR*[2];

	values[0] = this->getTextFromInput(IDC_SOURCE_NODE_EDIT);
	if (values[0] == NULL) {
		MessageBox(this->hwnd, TEXT("Please enter the source node's name!"), TEXT("Source node name is missing!"), MB_OK | MB_ICONERROR);
		return FALSE;
	}

	values[1] = this->getTextFromInput(IDC_DEST_NODE_EDIT);
	if (values[1] == NULL) {
		MessageBox(this->hwnd, TEXT("Please enter the destination node's name!"), TEXT("Destination node's name is missing!"), MB_OK | MB_ICONERROR);
		return FALSE;
	}

	EndDialog(this->hwnd, (INT_PTR)values);
	return TRUE;
}

LRESULT DeleteVertexDialogHandler::onCancelButtonClick() {
	EndDialog(this->hwnd, NULL);
	return TRUE;
}

TCHAR* DeleteVertexDialogHandler::getTextFromInput(int id) {
	TCHAR*  text;
	WORD    textLength;

	textLength = (WORD)SendDlgItemMessage(this->hwnd, id, EM_LINELENGTH, 0, 0);
	if (textLength == 0) {
		return NULL;
	}

	text = new TCHAR[textLength + 1];
	text[0] = textLength;
	text[textLength] = 0;

	SendDlgItemMessage(this->hwnd, id, EM_GETLINE, 0, (LPARAM)text);
	return text;
}