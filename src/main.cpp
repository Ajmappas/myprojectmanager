#include <iostream>
#include <Windows.h>
	
LRESULT wndproc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam) {
	switch (umsg) {
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);

		FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

		EndPaint(hwnd, &ps);

		break;
	}
	default:
		return DefWindowProc(hwnd, umsg, wparam, lparam);
	}
}

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
	
	const wchar_t CLASSNAME[] = L"MyClass";
	WNDCLASS wc = {};
	wc.lpfnWndProc = wndproc;
	wc.hInstance = hInstance;
	wc.lpszClassName = CLASSNAME;

	RegisterClass(&wc);

	HWND hwnd = CreateWindowEx(0, CLASSNAME, L"Window", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, NULL, NULL, hInstance, NULL);
	if (hwnd == NULL) return MessageBox(NULL, L"ERROR", L"The window failed to initialize!", MB_OK);

	ShowWindow(hwnd, nShowCmd);

	MSG msg = {};
	while (GetMessage(&msg, NULL, 0, 0) > 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

}