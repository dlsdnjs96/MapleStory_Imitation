
#ifdef _DEBUG
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")

#endif

#include <Windows.h>

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

namespace Engine
{
	LRESULT CALLBACK Procedure(HWND const, UINT const, WPARAM const, LPARAM const);
	HWND* GetHwnd();
}



int APIENTRY WinMain(
	_In_     HINSTANCE const hInstance,		// ���α׷��� �ּҰ��� �����ϰ��ִ� ����
	_In_opt_ HINSTANCE const hprevInstance,	// ������ 32bit���� ������ �ʴ� ���
	_In_     LPSTR     const lpCmdLine,		// ��ɾ�� �����ҽ� ���� �߰� �μ� Ex) Game.exe parameter1
	_In_     int       const nShowCmd		// â�� ���� (�ּ�ȭ, �ִ�ȭ ���..)
)
{
	//HWND hWindow = nullptr;

	{
		WNDCLASSEX wndClass = WNDCLASSEX();		// ������ Ŭ������ ���� ����

		wndClass.cbSize = sizeof(WNDCLASSEX);									   // ������ ũ�� ����
		wndClass.lpfnWndProc = Engine::Procedure;								   // ��� ������ �޼����� Engine�� Procedure�� ����
		wndClass.hInstance = hInstance;											   // ���α׷� ����� �޾ƿ� HINSTANCE(���α׷��� �ּҰ�)
		wndClass.hIcon = LoadIcon(nullptr, IDI_APPLICATION);					   // ������ â�� ������
		wndClass.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);					   // ������ â�� ���� ������
		wndClass.hCursor = LoadCursor(nullptr, IDC_ARROW);						   // ������ â���� ǥ���� ���콺 �̹���
		wndClass.hbrBackground = static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH)); // ������ â�� ����
		wndClass.lpszClassName = "Window";										   // ������ Ŭ������ ��Ī

		RegisterClassEx(&wndClass); // ������ Ŭ���� ���
		
	}

	CREATESTRUCT window = CREATESTRUCT();

	window.lpszClass = "Window";								// Ŭ���� �̸� (������ ���� ����)
	window.lpszName = "��Ʈ������";								// ������ â�� �̸� (����� â���� �ߴ°�)
	window.style = WS_CAPTION | WS_SYSMENU;						// ������ â�� ��Ÿ��
	window.cx = WINDOW_WIDTH;									// â ���α���
	window.cy = WINDOW_HEIGHT;									// â ���α���
	// GetSystemMetrics() : ����� ȭ�� ũ�Ⱚ�� �������� �Լ�
	window.x = (GetSystemMetrics(SM_CXSCREEN) - window.cx) / 2; // â ���� ������ġ
	window.y = 0;// (GetSystemMetrics(SM_CYSCREEN) - window.cy) / 2; // â ���� ������ġ

	(*Engine::GetHwnd()) = CreateWindowEx
	(
		window.dwExStyle,
		window.lpszClass,
		window.lpszName,
		window.style,
		window.x,
		window.y,
		window.cx,
		window.cy,
		window.hwndParent,	// �⺻�� : �θ� ������ ����
		window.hMenu,		// �⺻�� : �޴� ����
		window.hInstance,
		window.lpCreateParams
	);

	ShowWindow((*Engine::GetHwnd()), SW_RESTORE); // â ����


	MSG msg = MSG(); // ������ �޼����� �ӽ÷� �����ص� ����


	//ShowCursor(false); // Ŀ�� ���ֱ�
	while (true)
	{
		if (PeekMessage(&msg, HWND(), WM_NULL, WM_NULL, PM_REMOVE))
			// GetMessage�� �ʴ� 1000���� �޼����ۿ� ���ޱ⶧���� PeekMessage�̿�
		{
			if (msg.message == WM_QUIT) // ������ â ����
				return static_cast<int>(msg.wParam);

			DispatchMessage(&msg);
		}
		else
		{
			SendMessage((*Engine::GetHwnd()), WM_APP, 0, 0); // ���� �޼����� Procedure�Լ��� ������
		}
	}
	// ���� : ����� Ŭ������ ������ â�� ��ü ����
	// â ������ 1280, 720
	// ȭ�� �� ����� ����

	return 0;
}
