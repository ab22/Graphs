#pragma once

#include <Windows.h>

#include "EventHandlers\MainEventHandler.h"
#include "Infrastructure\Messages\WindowMessages.h"

#define APP_CLASS_NAME	TEXT("MAIN_GRAPH_WINDOW")
#define APP_WND_TITLE	TEXT("Graphs Remake")

LRESULT CALLBACK WinProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, char *lpCmdLine, int nShowCmd);