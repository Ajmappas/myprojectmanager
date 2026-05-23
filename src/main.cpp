#include <iostream>
#include <Windows.h>
#include <CommCtrl.h>

#pragma comment(lib, "comctl32.lib")

#define ID_LISTVIEW 2000

#define FILE_MENU_NEWPROJECT 1
#define FILE_MENU_OPENPROJECT 2

HWND CreateProjectListView(HWND hwnd) {
	INITCOMMONCONTROLSEX icex;
	icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
	icex.dwICC = ICC_LISTVIEW_CLASSES;
	InitCommonControlsEx(&icex);

	RECT rcClient;
	GetClientRect(hwnd, &rcClient);

	HWND hWndListView = CreateWindowEx(
		0,
		WC_LISTVIEW,
		L"",
		WS_CHILD | WS_VISIBLE | WS_BORDER | LVS_REPORT | LVS_EDITLABELS,
		0, 0, rcClient.right, rcClient.bottom,
		hwnd,
		(HMENU)ID_LISTVIEW,
		(HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
		NULL
	);

	ListView_SetExtendedListViewStyle(hWndListView, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	return hWndListView;
}

void InitListViewColumns(HWND hWndListView) {
	LVCOLUMN lvc;

	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvc.fmt = LVCFMT_LEFT;

	// Column 1: Project Name
	lvc.iSubItem = 0;
	lvc.pszText = (LPWSTR)L"Project Name";
	lvc.cx = 250; // Width in pixels
	ListView_InsertColumn(hWndListView, 0, &lvc);

	// Column 2: Status
	lvc.iSubItem = 1;
	lvc.pszText = (LPWSTR)L"Status";
	lvc.cx = 100;
	ListView_InsertColumn(hWndListView, 1, &lvc);

	// Column 3: Last Modified
	lvc.iSubItem = 2;
	lvc.pszText = (LPWSTR)L"Last Modified";
	lvc.cx = 150;
	ListView_InsertColumn(hWndListView, 2, &lvc);
}

void AddProjectItem(HWND hWndListView, int rowIndex, LPCWSTR name, LPCWSTR status, LPCWSTR date) {
	LVITEM lvi = { 0 };

	// 1. Insert the main item (Column 0)
	lvi.mask = LVIF_TEXT;
	lvi.iItem = rowIndex;
	lvi.iSubItem = 0;
	lvi.pszText = (LPWSTR)name;
	ListView_InsertItem(hWndListView, &lvi);

	// 2. Set the text for the subitems (Columns 1 and 2)
	// We use the ListView_SetItemText macro for cleaner code
	ListView_SetItemText(hWndListView, rowIndex, 1, (LPWSTR)status);
	ListView_SetItemText(hWndListView, rowIndex, 2, (LPWSTR)date);
}

HMENU hmenu;
HMENU hFileMenu;
void AddMenus(HWND hwnd) {
	hmenu = CreateMenu();
	hFileMenu = CreateMenu();

	AppendMenuW(hFileMenu, MF_STRING, FILE_MENU_NEWPROJECT, L"New Project");
	AppendMenuW(hFileMenu, MF_STRING, FILE_MENU_OPENPROJECT, L"Open Project");

	AppendMenuW(hmenu, MF_POPUP, (UINT_PTR)hFileMenu, L"File");

	SetMenu(hwnd, hmenu);
}

HWND hListView;
int index = 1;

LRESULT wndproc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam) {
	switch (umsg) {
	case WM_COMMAND:
	{
		switch (wparam) {
		case FILE_MENU_NEWPROJECT:
			MessageBox(hwnd, L"Sometime soon this is going to create a project", L"New Project", MB_OK);
			break;
		case FILE_MENU_OPENPROJECT:
			MessageBox(hwnd, L"Sometime soon this is going to open a project", L"Open Project", MB_OK);
			AddProjectItem(hListView, index, L"Test", L"IN PROGRESS", L"2026-05-23");
			index++;
			break;
		}

		break;
	}
	case WM_CREATE:
	{
		AddMenus(hwnd);

		hListView = CreateProjectListView(hwnd);
		InitListViewColumns(hListView);

		AddProjectItem(hListView, 0, L"TEST", L"IN PROGRESS", L"2026-05-23");

		break;
	}
	case WM_SIZE:
	{
		HWND hListView = GetDlgItem(hwnd, ID_LISTVIEW);
		//MoveWindow(hListView, 0, 0, LOWORD(lparam), HIWORD(lparam), TRUE);

		break;
	}
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