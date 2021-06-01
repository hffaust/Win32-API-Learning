#include <windows.h>

#define FILE_MENU_NEW 1
#define FILE_MENU_OPEN 2
#define FILE_MENU_EXIT 3
#define CHANGE_TITLE 4

LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);

void AddMenus(HWND);
void AddControls(HWND);

HMENU hMenu;
HWND hEdit;

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow)
{
	// https://stackoverflow.com/questions/21834833/argument-of-type-const-char-is-incompatible-with-parameter-of-type-lpcwstr/44942751
	//MessageBox(NULL, "HELLO WORLD", "My first GUI", MB_OK);
	
	WNDCLASSW wc = { 0 };
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.hCursor = LoadCursor(NULL, IDC_HAND);
	wc.hInstance = hInst;
	wc.lpszClassName = L"myWindowClass";
	wc.lpfnWndProc = WindowProcedure;  // Pointer to a funciton so no parentheses are used

	if (!RegisterClassW(&wc))
		return -1;

	// If Registration of the Window was successful, we can create the window from our class.
	CreateWindowW(L"myWindowClass", L"My Window", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, 500, 500,
		NULL, NULL, NULL, NULL);

	MSG	msg = { 0 };

	while (GetMessage(&msg, NULL, NULL, NULL))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}

LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
	case WM_COMMAND:

		switch (wp)
		{
		case FILE_MENU_EXIT:
			DestroyWindow(hWnd);
			break;
		case FILE_MENU_NEW:
			MessageBeep(MB_ICONINFORMATION);
			break;
		case CHANGE_TITLE:
			wchar_t text[100];
			GetWindowTextW(hEdit, text, 100);
			// The 'text' variable is the text that is present in the hEdit control.
			SetWindowTextW(hWnd, text);
			// Sets the text to be the title of the window.
			break;

		}

		break;
	case WM_CREATE:
		AddMenus(hWnd);
		AddControls(hWnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProcW(hWnd, msg, wp, lp);
	}
}

void AddMenus(HWND hWnd)
{
	// Main Menu object
	hMenu = CreateMenu();

	// Sub-menu under file option
	HMENU hFileMenu = CreateMenu();

	// A Sub-Sub-menu
	HMENU hSubMenu = CreateMenu();

	AppendMenu(hSubMenu, MF_STRING, NULL, "SubMenu Item");

	// Append items to the sub-menu, before appending sub-menu to main menu
	AppendMenu(hFileMenu, MF_STRING, FILE_MENU_NEW, "New");
	AppendMenu(hFileMenu, MF_POPUP, (UINT_PTR) hSubMenu, "Open SubMenu");
	AppendMenu(hFileMenu, MF_SEPARATOR, NULL, NULL); // Separator line in menu
	AppendMenu(hFileMenu, MF_STRING, FILE_MENU_EXIT, "Exit");

	// Append items to the main menu
	AppendMenu(hMenu, MF_POPUP, (UINT_PTR) hFileMenu, "File");
	AppendMenu(hMenu, MF_STRING, NULL, "Help");

	SetMenu(hWnd, hMenu);
}

void AddControls(HWND hWnd)
{
	// The window control is basically just a element inside the window that does something.
	// Static Controls contain static text in the control.
	CreateWindowW(L"static", L"Enter Text Here :", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER, 200, 100, 100, 50, hWnd,
		NULL, NULL, NULL);
	//  WS_VISIBLE | WS_CHILD muat be included in all the controls at a minimum. 
	hEdit = CreateWindowW(L"Edit", L"...", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL, 200, 152, 100, 50, hWnd,
		NULL, NULL, NULL);

	CreateWindowW(L"Button", L"Change Title", WS_VISIBLE | WS_CHILD, 200, 204, 100, 50, hWnd,
		(HMENU) CHANGE_TITLE, NULL, NULL);
}

/*
* References:
*	1. https://docs.microsoft.com/en-us/cpp/windows/walkthrough-creating-windows-desktop-applications-cpp?view=msvc-160
*	2. https://www.youtube.com/watch?v=7K6HCeog09c
*
*/