#pragma once

#include <Windows.h>

#include "EventHandlers\EventHandlers.h"
#include "Infrastructure\Messages\CreateWindowMessage.h"
#include "Infrastructure\Messages\PaintWindowMessage.h"

#define APP_CLASS_NAME TEXT("Main")
#define APP_WND_TITLE TEXT("Grafos")

LRESULT CALLBACK WinProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, char *lpCmdLine, int nShowCmd);