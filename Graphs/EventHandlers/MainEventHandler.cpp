#include "MainEventHandler.h"


MainEventHandler::MainEventHandler() {
	this->hwnd = NULL;
	this->hInstance = NULL;
	this->charHeight = 0;
	this->charWidth = 0;

	this->addingNode = false;
	this->tmpNodeName = NULL;
	this->organizeByName = true;
	this->showDijkstraLabels = false;
	this->showVertices = true;
	this->showFloydMatrices = false;
}

MainEventHandler::~MainEventHandler() {

}

LRESULT MainEventHandler::OnCreate(CreateWindowMessage *msg) {
	HDC hdc;

	hdc = GetDC(msg->hwnd);
	this->hwnd = msg->hwnd;
	this->hInstance = msg->createStruct->hInstance;
	this->calculateDefaultFontSizes(hdc);
	this->initCommonVisualControls();
	this->createMainToolbar();
	ReleaseDC(msg->hwnd, hdc);

	return DefWindowProc(msg->hwnd, msg->msg, msg->wParam, msg->lParam);
}

LRESULT MainEventHandler::OnPaint(PaintWindowMessage *msg) {
	PAINTSTRUCT ps;
	HDC         hdc;

	hdc = BeginPaint(msg->hwnd, &ps);
	if (this->showVertices) {
		this->drawNodes(hdc);
	}
	else if (this->showFloydMatrices) {
		this->drawFloydMatrices(hdc);
	}
	EndPaint(msg->hwnd, &ps);

	return 0;
}

LRESULT MainEventHandler::OnResize(ResizeWindowMessage *msg) {
	// Resize the main toolbar
	SendMessage(this->mainWindowControls.mainToolbar, TB_AUTOSIZE, 0, 0);
	return 0;
}

LRESULT MainEventHandler::OnCommand(CommandWindowMessage *msg) {
	switch (msg->buttonIdentifier) {
	case TOOLBAR_BUTTON::ADD_NODE:
		return this->onToolbarAddNodeClick();
	case TOOLBAR_BUTTON::DELETE_NODE:
		return this->onToolbarDeleteNodeClick();
	case TOOLBAR_BUTTON::ADD_VERTEX:
		return this->onToolbarAddVertexClick();
	case TOOLBAR_BUTTON::DELETE_VERTEX:
		return this->onToolbarDeleteVertexClick();
	case TOOLBAR_BUTTON::ORGANIZE_NAME:
		return this->onToolbarOrganizeByNameClick();
	case TOOLBAR_BUTTON::ORGANIZE_TOPOLOGY:
		return this->onToolbarOrganizeByTopologyClick();
	case TOOLBAR_BUTTON::IS_ACYCLIC:
		return this->onToolbarCheckAcyclicityClick();
	case TOOLBAR_BUTTON::SHOW_DIJKSTRA_LABELS:
		return this->onToolbarShowDijkstraLabelsClick();
	case TOOLBAR_BUTTON::SHOW_FLOYD_MATRICES:
		return this->onToolbarShowFloydMatricesClick();
	case TOOLBAR_BUTTON::SHOW_VERTICES:
		return this->onToolbarShowVerticesClick();
	case TOOLBAR_BUTTON::EULER_LOOP:
		return this->onToolbarEulerLoopClick();
	case TOOLBAR_BUTTON::STRONG_COMPONENTS:
		return this->onToolbarStrongComponentsClick();
	case TOOLBAR_BUTTON::EXIT:
		return this->onToolbarExitClick();
	}

	return TRUE;
}

LRESULT MainEventHandler::OnLeftMouseButtonClick(LeftMouseButtonDownMessage* msg) {
	if (this->addingNode) {
		bool result = this->addNode(msg->xCoordinate, msg->yCoordinate);
		// if node already exists with that name
		if (!result) {
			MessageBox(hwnd, TEXT("Node name already exists!"), 0, MB_OK | MB_ICONERROR);
		}
		else 
			InvalidateRect(hwnd, 0, true);
	}
	return 0;
}

void MainEventHandler::calculateDefaultFontSizes(HDC hdc) {
	TEXTMETRIC tm;

	GetTextMetrics(hdc, &tm);
	this->charWidth = tm.tmAveCharWidth;
	this->charHeight = tm.tmHeight + tm.tmInternalLeading;
}

BOOL MainEventHandler::initCommonVisualControls() {
	INITCOMMONCONTROLSEX	commControls;

	commControls.dwSize = sizeof(INITCOMMONCONTROLSEX);
	commControls.dwICC = ICC_STANDARD_CLASSES | ICC_BAR_CLASSES;
	return InitCommonControlsEx(&commControls);
}

void MainEventHandler::createMainToolbar() {
	const DWORD buttonStyles = BTNS_AUTOSIZE;
	HIMAGELIST  imageList = NULL;
	const int   bitmapSize = 16;
	const int   numButtons = 13;
	const int   imageListId = 0;
	TBBUTTON    tbButtons[numButtons];
	HWND        toolBar = NULL;

	toolBar = CreateWindowEx(
		0,
		TOOLBARCLASSNAME,
		NULL,
		WS_CHILD | WS_VISIBLE | WS_BORDER,
		0,
		0,
		0,
		0,
		this->hwnd,
		NULL,
		0,
		NULL
		);

	imageList = ImageList_Create(
		bitmapSize,
		bitmapSize,
		ILC_COLOR16 | ILC_MASK,
		numButtons,
		0
		);

	SendMessage(toolBar, TB_SETIMAGELIST, (WPARAM)imageListId, (LPARAM)imageList);
	SendMessage(toolBar, TB_LOADIMAGES, (WPARAM)IDB_STD_SMALL_COLOR, (LPARAM)HINST_COMMCTRL);

	tbButtons[0] = { MAKELONG(STD_FILENEW, imageListId), TOOLBAR_BUTTON::ADD_NODE, TBSTATE_ENABLED, buttonStyles, { 0 }, 0, (INT_PTR)TEXT("Add Node") };
	tbButtons[1] = { MAKELONG(STD_CUT, imageListId), TOOLBAR_BUTTON::DELETE_NODE, TBSTATE_ENABLED, buttonStyles, { 0 }, 0, (INT_PTR)L"Delete Node" };
	tbButtons[2] = { MAKELONG(STD_FILENEW, imageListId), TOOLBAR_BUTTON::ADD_VERTEX, TBSTATE_ENABLED, buttonStyles, { 0 }, 0, (INT_PTR)L"Add Vertex" };
	tbButtons[3] = { MAKELONG(STD_CUT, imageListId), TOOLBAR_BUTTON::DELETE_VERTEX, TBSTATE_ENABLED, buttonStyles, { 0 }, 0, (INT_PTR)L"Delete Vertex" };
	tbButtons[4] = { MAKELONG(STD_FILENEW, imageListId), TOOLBAR_BUTTON::ORGANIZE_NAME, TBSTATE_ENABLED, buttonStyles, { 0 }, 0, (INT_PTR)L"Organize by Name" };
	tbButtons[5] = { MAKELONG(STD_CUT, imageListId), TOOLBAR_BUTTON::ORGANIZE_TOPOLOGY, TBSTATE_ENABLED, buttonStyles, { 0 }, 0, (INT_PTR)L"Organize by Topology" };
	tbButtons[6] = { MAKELONG(STD_CUT, imageListId), TOOLBAR_BUTTON::IS_ACYCLIC, TBSTATE_ENABLED, buttonStyles, { 0 }, 0, (INT_PTR)L"Is Acyclic" };
	tbButtons[7] = { MAKELONG(STD_CUT, imageListId), TOOLBAR_BUTTON::SHOW_DIJKSTRA_LABELS, TBSTATE_ENABLED, buttonStyles, { 0 }, 0, (INT_PTR)L"Show/Hide Dijkstra" };
	tbButtons[8] = { MAKELONG(STD_CUT, imageListId), TOOLBAR_BUTTON::SHOW_FLOYD_MATRICES, TBSTATE_ENABLED, buttonStyles, { 0 }, 0, (INT_PTR)L"Show Floyd Matrices" };
	tbButtons[9] = { MAKELONG(STD_CUT, imageListId), TOOLBAR_BUTTON::SHOW_VERTICES, TBSTATE_ENABLED, buttonStyles, { 0 }, 0, (INT_PTR)L"Show Vertices" };
	tbButtons[10] = { MAKELONG(STD_CUT, imageListId), TOOLBAR_BUTTON::EULER_LOOP, TBSTATE_ENABLED, buttonStyles, { 0 }, 0, (INT_PTR)L"Euler Loop" };
	tbButtons[11] = { MAKELONG(STD_CUT, imageListId), TOOLBAR_BUTTON::STRONG_COMPONENTS, TBSTATE_ENABLED, buttonStyles, { 0 }, 0, (INT_PTR)L"Strong Components" };
	tbButtons[12] = { MAKELONG(STD_DELETE, imageListId), TOOLBAR_BUTTON::EXIT, TBSTATE_ENABLED, buttonStyles, { 0 }, 0, (INT_PTR)L"Exit" };

	// Add buttons.
	SendMessage(toolBar, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0);
	SendMessage(toolBar, TB_ADDBUTTONS, (WPARAM)numButtons, (LPARAM)&tbButtons);
	SendMessage(toolBar, TB_AUTOSIZE, 0, 0);

	this->mainWindowControls.mainToolbar = toolBar;
}

LRESULT MainEventHandler::onToolbarAddNodeClick() {
	TCHAR* nodeName;

	nodeName = (TCHAR*)DialogBox(this->hInstance, MAKEINTRESOURCE(IDD_ADD_NODE_DIALOG), this->hwnd, AddNodeDialogProc);
	if (nodeName != NULL) {
		this->addingNode = true;
		this->tmpNodeName = nodeName;
	}
	return TRUE;
}

LRESULT MainEventHandler::onToolbarDeleteNodeClick() {
	TCHAR* nodeName;
	int    result;

	nodeName = (TCHAR*)DialogBox(this->hInstance, MAKEINTRESOURCE(IDD_DELETE_NODE_DIALOG), this->hwnd, DeleteNodeDialogProc);
	if (nodeName == NULL)
		return TRUE;

	result = this->graphs.borrar(nodeName);
	if (result == -1) {
		MessageBox(hwnd, TEXT("Node does not exists!"), TEXT("Node not found"), MB_OK | MB_ICONERROR);
		return  TRUE;
	}
	
	InvalidateRect(hwnd, 0, true);
	delete[] nodeName;
	return TRUE;
}

LRESULT MainEventHandler::onToolbarAddVertexClick() {
	const int     amountValues = 3;
	TCHAR**       values;
	bool          vertexAdded;	

	values = (TCHAR**)DialogBox(this->hInstance, MAKEINTRESOURCE(IDD_ADD_VERTEX_DIALOG), this->hwnd, AddVertexDialogProc);
	if (values == NULL)
		return TRUE;

	vertexAdded = this->addVertex(values[0], values[1], values[2]);
	if (!vertexAdded) {
		MessageBox(this->hwnd, TEXT("Invalid vertices!"), TEXT("Error"), MB_OK | MB_ICONERROR);
	}
	
	for (int i = 0; i < amountValues; i++)
		delete[] values[i];
	delete[] values;

	return TRUE;
}

LRESULT MainEventHandler::onToolbarDeleteVertexClick() {
	const int     amountValues = 2;
	TCHAR**       values;
	bool          vertexDeleted;

	values = (TCHAR**)DialogBox(this->hInstance, MAKEINTRESOURCE(IDD_DELETE_VERTEX_DIALOG), this->hwnd, DeleteVertexDialogProc);
	if (values == NULL)
		return TRUE;

	vertexDeleted = this->deleteVertex(values[0], values[1]);
	if (!vertexDeleted) {
		MessageBox(this->hwnd, TEXT("Invalid vertices!"), TEXT("Error"), MB_OK | MB_ICONERROR);
	}

	for (int i = 0; i < amountValues; i++)
		delete[] values[i];
	delete[] values;

	return TRUE;
}

LRESULT MainEventHandler::onToolbarOrganizeByNameClick() {
	if (this->organizeByName)
		return TRUE;
	this->organizeByName = true;
	InvalidateRect(this->hwnd, 0, true);
	return TRUE;
}

LRESULT MainEventHandler::onToolbarOrganizeByTopologyClick() {
	if (!this->organizeByName)
		return TRUE;
	this->organizeByName = false;
	InvalidateRect(this->hwnd, 0, true);
	return TRUE;
}

LRESULT MainEventHandler::onToolbarShowDijkstraLabelsClick() {
	this->showDijkstraLabels = !this->showDijkstraLabels;
	InvalidateRect(this->hwnd, 0, true);
	return TRUE;
}

LRESULT MainEventHandler::onToolbarShowFloydMatricesClick() {
	this->showFloydMatrices = true;
	this->showVertices = false;
	InvalidateRect(this->hwnd, 0, true);
	return TRUE;
}

LRESULT MainEventHandler::onToolbarShowVerticesClick() {
	this->showFloydMatrices = false;
	this->showVertices = true;
	InvalidateRect(this->hwnd, 0, true);
	return TRUE;
}

LRESULT MainEventHandler::onToolbarCheckAcyclicityClick() {
	const int  messageSize = 64;
	TCHAR*     message = new TCHAR[messageSize];

	if (this->graphs.nVertices == 0) {
		_stprintf_s(message, messageSize, TEXT("There are no nodes added!"));
	}
	else if (this->graphs.esAciclico()) {
		_stprintf_s(message, messageSize, TEXT("No infinite cycles!"));
	}
	else {
		_stprintf_s(message, messageSize, TEXT("Graph contains infinite cycles!"));
	}

	MessageBox(this->hwnd, message, TEXT("Acyclicity"), MB_OK | MB_ICONINFORMATION);
	delete[] message;

	return TRUE;
}

LRESULT MainEventHandler::onToolbarEulerLoopClick() {

	return TRUE;
}

LRESULT MainEventHandler::onToolbarStrongComponentsClick() {
	DialogBoxParam(this->hInstance, MAKEINTRESOURCE(IDD_STRONG_COMPONENTS), this->hwnd, StrongComponentsDialogProc, (LPARAM)&this->graphs);
	return TRUE;
}

LRESULT MainEventHandler::onToolbarExitClick() {
	SendMessage(this->hwnd, WM_CLOSE, 0, 0);
	return TRUE;
}

bool MainEventHandler::addNode(int x, int y) {
	bool result = graphs.agregar(this->tmpNodeName, x, y);

	// if node already exists with that name
	if (!result) {
		delete[] this->tmpNodeName;
		this->tmpNodeName = NULL;		
	}
	this->addingNode = false;
	return result;
}

void MainEventHandler::drawNodes(HDC hdc) {
	/*
		Old code to draw the nodes.
		TODO: 
			Refactor ._. 
	*/
	if (this->graphs.nVertices == 0)
		return;

	HRGN hRgn = NULL;

	if (!this->organizeByName) {
		graphs.recursoTopologico();
	}

	for (int x = 0; x < this->graphs.nVertices; x++) {
		for (int y = 0; y < this->graphs.nVertices; y++) {
			if (this->graphs.aristas[x][y] == 1) {
				MoveToEx(hdc, this->graphs.cc[y].x + 10, this->graphs.cc[y].y + 10, 0);
				LineTo(hdc, this->graphs.cc[x].x + 10, this->graphs.cc[x].y - 10);
				hRgn = CreateRectRgn(this->graphs.cc[x].x + 10, this->graphs.cc[x].y - 10, this->graphs.cc[x].x + 15, this->graphs.cc[x].y + 15);
				FillRgn(hdc, hRgn, (HBRUSH)GetStockObject(LTGRAY_BRUSH));
				int dPosX, dPosY;
				if (this->graphs.cc[x].x >= this->graphs.cc[y].x) {
					dPosX = (this->graphs.cc[x].x - this->graphs.cc[y].x) / 2;
					dPosX = this->graphs.cc[x].x - dPosX;
					dPosX -= this->charWidth * 2;
				}
				else {
					dPosX = (this->graphs.cc[y].x - this->graphs.cc[x].x) / 2;
					dPosX = this->graphs.cc[x].x + dPosX;
					dPosX += this->charWidth * 2;
				}
				if (this->graphs.cc[x].y >= this->graphs.cc[x].y) {
					dPosY = (this->graphs.cc[x].y - this->graphs.cc[y].y) / 2;
					dPosY = this->graphs.cc[x].y - dPosY;
				}
				else {
					dPosY = (this->graphs.cc[y].y - this->graphs.cc[x].y) / 2;
					dPosY = this->graphs.cc[x].y + dPosY;
				}

				WCHAR *distancia = new WCHAR[10];
				wsprintf(distancia, TEXT("%d"), this->graphs.distancias[x][y]);
				TextOut(hdc, dPosX + 1, dPosY + 1, distancia, lstrlen(distancia));

				delete[] distancia;
				DeleteObject(hRgn);
				hRgn = NULL;
			}
		}
	}

	for (int i = 0; i < this->graphs.nVertices; i++) {
		Ellipse(hdc, this->graphs.cc[i].x - 1, this->graphs.cc[i].y - 1, this->graphs.cc[i].x + 25, this->graphs.cc[i].y + 25);
		if (this->organizeByName) {
			TextOut(hdc, this->graphs.cc[i].x + 8, this->graphs.cc[i].y + 5, this->graphs.vertices[i], 1);
		}
		else {
			TCHAR *nodeName = new TCHAR[10];
			_stprintf_s(nodeName, 10, TEXT("%d"), graphs.topologia[i]);
			TextOut(hdc, graphs.cc[i].x + 8, graphs.cc[i].y + 5, nodeName, _tcslen(nodeName));
			delete[] nodeName;
		}
		
	}

	if (this->showDijkstraLabels) {
		const int labelsLength = 32;
		Dijkstra* dijkstra = new Dijkstra[this->graphs.nVertices];
		TCHAR*    label = new TCHAR[labelsLength];

		this->graphs.algoritmoDijkstra(dijkstra);
		for (int i = 0; i < this->graphs.nVertices; i++) {
			_stprintf_s(label, labelsLength, TEXT("[%d,%c](%d)"), dijkstra[i].distancia, this->graphs.vertices[dijkstra[i].procede][0], dijkstra[i].iteracion);
			TextOut(hdc, this->graphs.cc[i].x + 20, this->graphs.cc[i].y + 20, label, _tcslen(label));
		}
		delete[] label;
		delete[] dijkstra;
	}
}

void MainEventHandler::drawFloydMatrices(HDC hdc) {
	WCHAR*   distance;
	WCHAR*   iteration;
	Floyd*   floyd;
	int      espacio;
	int      espacio2;
	if (this->graphs.nVertices == 0)
		return;

	floyd = new Floyd;
	this->graphs.algoritmoFloyd(floyd);
	espacio = 0;
	espacio2 = 0;
	distance = new WCHAR[10];
	iteration = new WCHAR[10];

	TextOut(hdc, this->charWidth * 15, this->charHeight * 8, TEXT("Floyd"), 5);

	TextOut(hdc, this->charWidth * 17, this->charHeight * 9, TEXT("Distancias"), 10);
	TextOut(hdc, this->charWidth * 90, this->charHeight * 9, TEXT("Iteraciones"), 11);

	TextOut(hdc, this->charWidth * 17, this->charHeight * 20, TEXT("Matriz Adyacencia"), 17);
	TextOut(hdc, this->charWidth * 90, this->charHeight * 20, TEXT("Matriz Incidencia"), 17);
	for (int i = 0; i < graphs.nVertices; i++) {
		TextOut(hdc, this->charWidth*(16 + espacio), this->charHeight * 10, graphs.vertices[i], 1);
		TextOut(hdc, this->charWidth*(15 - 1), this->charHeight*(11 + espacio2), graphs.vertices[i], 1);

		TextOut(hdc, this->charWidth*(91 + espacio), this->charHeight * 10, graphs.vertices[i], 1);
		TextOut(hdc, this->charWidth*(90 - 1), this->charHeight*(11 + espacio2), graphs.vertices[i], 1);

		TextOut(hdc, this->charWidth*(16 + espacio), this->charHeight * 21, graphs.vertices[i], 1);
		TextOut(hdc, this->charWidth*(15 - 1), this->charHeight*(22 + espacio2), graphs.vertices[i], 1);


		TextOut(hdc, this->charWidth*(90 - 1), this->charHeight*(22 + espacio2), graphs.vertices[i], 1);

		espacio += 3;
		espacio2++;
	}
	espacio = espacio2 = 0;
	for (int x = 0; x < graphs.nVertices; x++) {
		for (int y = 0; y < graphs.nVertices; y++) {
			if (floyd->distancias[x][y] == INF) {
				TextOut(hdc, this->charWidth*(16 + espacio), this->charHeight*(11 + espacio2), TEXT("Inf"), 3);
				wsprintf(iteration, TEXT("%d"), floyd->iteraciones[x][y]);
				TextOut(hdc, this->charWidth*(91 + espacio), this->charHeight*(11 + espacio2), iteration, lstrlen(iteration));
			}
			else if (floyd->distancias[x][y] == EMPTY) {
				TextOut(hdc, this->charWidth*(16 + espacio), this->charHeight*(11 + espacio2), TEXT("--"), 2);
				TextOut(hdc, this->charWidth*(91 + espacio), this->charHeight*(11 + espacio2), TEXT("--"), 2);
			}
			else {
				wsprintf(distance, TEXT("%d"), floyd->distancias[x][y]);
				wsprintf(iteration, TEXT("%d"), floyd->iteraciones[x][y]);
				TextOut(hdc, this->charWidth*(16 + espacio), this->charHeight*(11 + espacio2), distance, lstrlen(distance));
				TextOut(hdc, this->charWidth*(91 + espacio), this->charHeight*(11 + espacio2), iteration, lstrlen(iteration));
			}
			TextOut(hdc, this->charWidth*(16 + espacio), this->charHeight*(22 + espacio2), graphs.aristas[x][y] == 1 ? TEXT("1") : TEXT("0"), 1);

			espacio += 3;
		}
		espacio2++;
		espacio = 0;
	}
	espacio = espacio2 = 0;

	for (int x = 0; x < graphs.nVertices; x++) {
		for (int y = 0; y < graphs.nAristas; y++) {
			WCHAR buf[3];
			wsprintf(buf, TEXT("%d"), y + 1);
			TextOut(hdc, this->charWidth*(91 + espacio), this->charHeight * 21, buf, 1);
			TextOut(hdc, this->charWidth*(91 + espacio), this->charHeight*(22 + espacio2), (graphs.inc1[y] == x || graphs.inc2[y] == x) ? TEXT("1") : TEXT("0"), 1);
			espacio += 3;
		}
		espacio2++;
		espacio = 0;
	}
	delete[]distance;
	delete[]iteration;
}

bool MainEventHandler::addVertex(TCHAR* source, TCHAR* destination, TCHAR* distance) {
	bool vertexAdded = this->graphs.agregarArista(source, destination, distance);
	if (vertexAdded)
		InvalidateRect(this->hwnd, 0, true);
	return vertexAdded;
}

bool MainEventHandler::deleteVertex(TCHAR* source, TCHAR* destination) {
	bool deleted = this->graphs.borrarArista(source, destination);
	if (deleted)
		InvalidateRect(this->hwnd, 0, true);
	return deleted;
}