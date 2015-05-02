#pragma once

#include <Windows.h>

// Make these definitions global for the other classes
// and headers in case we need them.
#define APP_CLASS_NAME  TEXT("MAIN_GRAPH_WINDOW")
#define APP_WND_TITLE   TEXT("Graphs Remake")

#include "EventHandlers\MainEventHandler.h"
#include "Infrastructure\Messages\WindowMessages.h"

LRESULT CALLBACK WinProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, char *lpCmdLine, int nShowCmd);