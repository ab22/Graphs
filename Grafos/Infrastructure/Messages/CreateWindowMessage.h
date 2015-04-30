#pragma once

#include "WinMessageBase.h"


class CreateWindowMessage : public WinMessageBase {
public:
	CreateWindowMessage(HWND, UINT, WPARAM, LPARAM);

protected:
	void TranslateMessage();

private: 
	CREATESTRUCT *createStruct;
};