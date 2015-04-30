#pragma once

#include "../Infrastructure/Messages/CreateWindowMessage.h"
#include "../Infrastructure/Messages/PaintWindowMessage.h"

class MainEventHandler{
private:
	int charWidth;
	int charHeight;
	int charCapsWidth;

public:
	// WM_CREATE: Called when the window is being created.
	LRESULT OnCreate(CreateWindowMessage*);
	// WM_PAINT: Called when the window needs to be redrawn
	LRESULT OnPaint(PaintWindowMessage*);
};