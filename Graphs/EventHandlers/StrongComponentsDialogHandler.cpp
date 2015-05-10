#include "StrongComponentsDialogHandler.h"


StrongComponentsDialogHandler::StrongComponentsDialogHandler(HWND hwnd) {
	this->hwnd = hwnd;
}

StrongComponentsDialogHandler::~StrongComponentsDialogHandler() {

}

LRESULT StrongComponentsDialogHandler::OnInit(InitDialogMessage* msg) {
	Graphs*   graphs;
	graphs = (Graphs*)msg->lParam;

	this->addStrongComponents(graphs);
	return TRUE;
}

LRESULT StrongComponentsDialogHandler::OnCommand(CommandWindowMessage *msg) {
	if (msg->buttonIdentifier == IDOK) {
		return this->onOkButtonClick();
	}
	return TRUE;
}

LRESULT StrongComponentsDialogHandler::onOkButtonClick() {
	EndDialog(this->hwnd, NULL);
	return TRUE;
}

void StrongComponentsDialogHandler::addStrongComponents(Graphs* graphs) {
	const int vertexNameLength = 2;
	TCHAR     vertexName[vertexNameLength];
	HWND      listBox;
	HWND      txtEulerCycle;
	int*      strongComponents = NULL;
	int       vertexAmount;
	int       space;

	if (graphs->nVertices == 0)
		return;

	listBox = GetDlgItem(this->hwnd, IDC_COMPONENTS_LIST);
	txtEulerCycle = GetDlgItem(this->hwnd, IDC_EULER_CYCLE_TEXT);

	strongComponents = graphs->componentesFuertes();
	space = 0;
	for (int i = 0; i < graphs->nVertices; i++) {
		if (strongComponents[i] == 1) {
			_stprintf_s(vertexName, 2, TEXT("%c"), graphs->vertices[i][0]);
			SendMessage(listBox, LB_ADDSTRING, NULL, (LPARAM)vertexName);
			space++;
		}
	}
	vertexAmount = graphs->contarAristas();
	if (space == 1 && vertexAmount % 2 == 0) {
		SetWindowText(txtEulerCycle, TEXT("Euler cycle exists!"));
	}
	else
		SetWindowText(txtEulerCycle, TEXT("Euler cycle exists!"));

	if (strongComponents != NULL)
		delete[] strongComponents;
}