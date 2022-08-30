#pragma once
#include <Windows.h>

namespace Engine
{
	LRESULT CALLBACK Procedure(HWND const hWindow, UINT const uMessage, WPARAM const wParam, LPARAM const lParam);
	HWND* GetHwnd();
}
