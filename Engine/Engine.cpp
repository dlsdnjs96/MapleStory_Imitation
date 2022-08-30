#include <d3d11.h>
#include <chrono>
#include "Game.h"
#include "Gui.h"
#include "Pipeline.h"


namespace Rendering { void Procedure(HWND const, UINT const, WPARAM const, LPARAM const); }
namespace Input     { void Procedure(HWND const, UINT const, WPARAM const, LPARAM const); }
namespace Time      { void Procedure(HWND const, UINT const, WPARAM const, LPARAM const); void UseNewDelta(); }
namespace Sound     { void Procedure(HWND const, UINT const, WPARAM const, LPARAM const); }

namespace Engine
{

	namespace { Game* Portfolio; static HWND hWindow;	}

	HWND* GetHwnd()
	{
		return &hWindow;
	}
	LRESULT CALLBACK Procedure(HWND const hWindow, UINT const uMessage, WPARAM const wParam, LPARAM const lParam)
	{
		if (Gui::MsgProc(hWindow, uMessage, wParam, lParam))
			return true;

		switch (uMessage)
		{
			case WM_CREATE:
			{
				Sound::Procedure(hWindow, uMessage, wParam, lParam);
				Rendering::Procedure(hWindow, uMessage, wParam, lParam);
				(Portfolio = Initialize())->Start();

				return 0;
			}
			case WM_APP:
			{
				if (Portfolio->Update())
					CloseWindow(hWindow);
				if (Portfolio->LateUpdate())
					CloseWindow(hWindow);

				Time::Procedure(hWindow, uMessage, wParam, lParam);
				Input::Procedure(hWindow, uMessage, wParam, lParam);
				Rendering::Procedure(hWindow, uMessage, wParam, lParam);
				return 0;
			}
			case WM_DESTROY:
			{
				Portfolio->End();
				delete Portfolio;

				Rendering::Procedure(hWindow, uMessage, wParam, lParam);

				PostQuitMessage(0);

				return 0;
			}
			case WM_MOUSEWHEEL: case WM_MOUSEHWHEEL: case WM_MOUSEMOVE:
			case WM_SYSKEYDOWN: case WM_LBUTTONDOWN: case WM_LBUTTONUP:
			case WM_SYSKEYUP  : case WM_RBUTTONDOWN: case WM_RBUTTONUP:
			case WM_KEYUP     : case WM_MBUTTONDOWN: case WM_MBUTTONUP:
			case WM_KEYDOWN   : case WM_XBUTTONDOWN: case WM_XBUTTONUP:
			{
				Input::Procedure(hWindow, uMessage, wParam, lParam);
				return 0;
			}
			case WM_SIZE:
			{
				Rendering::Procedure(hWindow, uMessage, wParam, lParam);

				return 0;
			}
			default :
			{
				return DefWindowProc(hWindow, uMessage, wParam, lParam);
			}
		}
	}
}