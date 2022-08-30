
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
	_In_     HINSTANCE const hInstance,		// 프로그램의 주소값을 저장하고있는 변수
	_In_opt_ HINSTANCE const hprevInstance,	// 윈도우 32bit부터 사용되지 않는 기능
	_In_     LPSTR     const lpCmdLine,		// 명령어로 실행할시 받을 추가 인수 Ex) Game.exe parameter1
	_In_     int       const nShowCmd		// 창의 상태 (최소화, 최대화 등등..)
)
{
	//HWND hWindow = nullptr;

	{
		WNDCLASSEX wndClass = WNDCLASSEX();		// 윈도우 클래스의 설명서 역할

		wndClass.cbSize = sizeof(WNDCLASSEX);									   // 설명서의 크기 지정
		wndClass.lpfnWndProc = Engine::Procedure;								   // 모든 윈도우 메세지를 Engine의 Procedure로 전달
		wndClass.hInstance = hInstance;											   // 프로그램 실행시 받아온 HINSTANCE(프로그램의 주소값)
		wndClass.hIcon = LoadIcon(nullptr, IDI_APPLICATION);					   // 윈도우 창의 아이콘
		wndClass.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);					   // 윈도우 창의 작은 아이콘
		wndClass.hCursor = LoadCursor(nullptr, IDC_ARROW);						   // 윈도우 창에서 표시할 마우스 이미지
		wndClass.hbrBackground = static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH)); // 윈도우 창의 배경색
		wndClass.lpszClassName = "Window";										   // 윈도우 클래스의 명칭

		RegisterClassEx(&wndClass); // 윈도우 클래스 등록
		
	}

	CREATESTRUCT window = CREATESTRUCT();

	window.lpszClass = "Window";								// 클래스 이름 (위에와 같게 설정)
	window.lpszName = "포트폴리오";								// 윈도우 창의 이름 (실행시 창위에 뜨는것)
	window.style = WS_CAPTION | WS_SYSMENU;						// 윈도우 창의 스타일
	window.cx = WINDOW_WIDTH;									// 창 가로길이
	window.cy = WINDOW_HEIGHT;									// 창 세로길이
	// GetSystemMetrics() : 모니터 화면 크기값을 가져오는 함수
	window.x = (GetSystemMetrics(SM_CXSCREEN) - window.cx) / 2; // 창 생성 가로위치
	window.y = 0;// (GetSystemMetrics(SM_CYSCREEN) - window.cy) / 2; // 창 생성 세로위치

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
		window.hwndParent,	// 기본값 : 부모 윈도우 없음
		window.hMenu,		// 기본값 : 메뉴 없음
		window.hInstance,
		window.lpCreateParams
	);

	ShowWindow((*Engine::GetHwnd()), SW_RESTORE); // 창 띄우기


	MSG msg = MSG(); // 윈도우 메세지를 임시로 저장해둘 변수


	//ShowCursor(false); // 커서 없애기
	while (true)
	{
		if (PeekMessage(&msg, HWND(), WM_NULL, WM_NULL, PM_REMOVE))
			// GetMessage는 초당 1000개의 메세지밖에 못받기때문에 PeekMessage이용
		{
			if (msg.message == WM_QUIT) // 윈도우 창 종료
				return static_cast<int>(msg.wParam);

			DispatchMessage(&msg);
		}
		else
		{
			SendMessage((*Engine::GetHwnd()), WM_APP, 0, 0); // 받은 메세지를 Procedure함수로 보내기
		}
	}
	// 과제 : 등록한 클래스를 가지고 창의 객체 생성
	// 창 사이즈 1280, 720
	// 화면 정 가운데에 생성

	return 0;
}
