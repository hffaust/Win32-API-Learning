#include <windows.h>
#include <stdlib.h>


#define FILE_MENU_NEW 1
#define FILE_MENU_OPEN 2
#define FILE_MENU_EXIT 3
#define GENERATE_BUTTON 4

LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);

void AddMenus(HWND);
void AddControls(HWND);
void loadImages();
void registerDialogClass(HINSTANCE);
void displayDialog(HWND);

HMENU hMenu;
HWND hName, hAge, hOut, hLogo, hMainWindow;
HBITMAP hLogoImage, hGenerateImage;

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow)
{
	// See {Ref 3} on how to disable unicode mode to make char compatible with LPCWSTR
	//MessageBox(NULL, "HELLO WORLD", "My first GUI", MB_OK);

	WNDCLASSW wc = { 0 };
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.hCursor = LoadCursor(NULL, IDC_HAND);
	wc.hInstance = hInst;
	wc.lpszClassName = L"myWindowClass";
	wc.lpfnWndProc = WindowProcedure;  // Pointer to a funciton so no parentheses are used

	if (!RegisterClassW(&wc))
		return -1;

	registerDialogClass(hInst);

	// If Registration of the Window was successful, we can create the window from our class.
	hMainWindow = CreateWindowW(L"myWindowClass", L"My Window", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, 500, 500,
		NULL, NULL, NULL, NULL);

	// Any function that ends with a "W" is a "W-POSTFIX" funciton which means it needs 'w-chars' (i think wide chars)
	// w-chars are designated by preceding the quotes with a 'L' like L"some string"


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
	int val;
	switch (msg)
	{
	case WM_COMMAND:

		switch (wp)
		{
		case FILE_MENU_EXIT:
			val = MessageBoxW(hWnd, L"Are you sure?", L"Wait!", MB_YESNO | MB_ICONEXCLAMATION);
			
			/*
			* IDABORT 3
			* IDCANCEL 2
			* IDCONTINUE 11
			* IDIGNORE 5
			* IDNO 7
			* IDOK 1
			* IDRETRY 4
			* IDTRYAGAIN 10
			* IDYES 6
			*/
			if (val == IDYES)
			{
				DestroyWindow(hWnd);
			}
			else if (val == IDNO)
			{
				// Do nothing if no was pressed
			}
			break;
		case FILE_MENU_NEW:
			displayDialog(hWnd);
			MessageBeep(MB_ICONINFORMATION);
			break;
		case GENERATE_BUTTON:
			char name[30], age[10], out[50];
			GetWindowText(hName, name, 30);
			GetWindowText(hAge, age, 10);

			if (strcmp(name, "") == 0 || strcmp(age, "") == 0)
			{
				val = MessageBoxW(hWnd, L"You did not enter anything!", NULL, MB_ABORTRETRYIGNORE | MB_ICONERROR);
				switch (val)
				{
				case IDABORT:
					DestroyWindow(hWnd);
					break;
				case IDRETRY:
					// return our of the funciton so rest of code is not run and loop restarts
					return 0;
				case IDIGNORE:
					// ignore the warning and process the empty data
					break;
				}
			}
			
			strcpy(out, name);
			strcat(out, " is ");
			strcat(out, age);
			strcat(out, " years old.");
			// Insecure strcat usage, to allow have to enable the _CRT_SECURE_NO_WARNINGS compilation flag
			// {Ref 4}

			SetWindowText(hOut, out);

			break;
		
		}

		break;
	case WM_CREATE:
		
		loadImages();
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
	AppendMenu(hFileMenu, MF_POPUP, (UINT_PTR)hSubMenu, "Open SubMenu");
	AppendMenu(hFileMenu, MF_SEPARATOR, NULL, NULL); // Separator line in menu
	AppendMenu(hFileMenu, MF_STRING, FILE_MENU_EXIT, "Exit");

	// Append items to the main menu
	AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hFileMenu, "File");
	AppendMenu(hMenu, MF_STRING, NULL, "Help");

	SetMenu(hWnd, hMenu);
}

void AddControls(HWND hWnd)
{
	// The window control is basically just a element inside the window that does something.
	// Static Controls contain static text in the control.
	//  WS_VISIBLE | WS_CHILD muat be included in all the controls at a minimum. 
	
	CreateWindowW(L"Static", L"Name :", WS_VISIBLE | WS_CHILD, 100, 50, 98, 38, hWnd, NULL, NULL, NULL);
	hName = CreateWindowW(L"Edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 200, 50, 98, 38, hWnd, NULL, NULL, NULL);

	CreateWindowW(L"Static", L"Age :", WS_VISIBLE | WS_CHILD, 100, 90, 98, 38, hWnd, NULL, NULL, NULL);
	hAge = CreateWindowW(L"Edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 200, 90, 98, 38, hWnd, NULL, NULL, NULL);

	HWND hBut = CreateWindowW(L"Button", NULL, WS_VISIBLE | WS_CHILD | BS_BITMAP, 150, 140, 98, 38, hWnd, (HMENU) GENERATE_BUTTON, NULL, NULL);
	SendMessageW(hBut, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hGenerateImage);

	hOut = CreateWindowW(L"Edit", L"", WS_VISIBLE | WS_CHILD, 100, 200, 300, 200, hWnd, NULL, NULL, NULL);
	
	hLogo = CreateWindowW(L"Static", NULL, WS_VISIBLE | WS_CHILD | SS_BITMAP, 350, 60, 200, 200, hWnd, NULL, NULL, NULL);
	SendMessageW(hLogo, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM) hLogoImage);
}

void loadImages()
{
	// For some stupid reason it will only load the file if its called "star2.bmp" or anything other than just "star.bmp"
	// Also I think it wanted the BMP files to not be 32bit bitmap files.
	hLogoImage = (HBITMAP) LoadImageW(NULL, L"star2.bmp", IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE);
	if (hLogoImage == NULL)
		MessageBox(NULL, "Null Logo Image", "Error", MB_OK);	


	hGenerateImage = (HBITMAP) LoadImageW(NULL, L"check2.bmp", IMAGE_BITMAP, 98, 38, LR_LOADFROMFILE);
	if (hGenerateImage == NULL)
		MessageBox(NULL, "Null Button Image", "Error", MB_OK);

		
}

LRESULT CALLBACK DialogProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
	case WM_COMMAND:
		switch (wp)
		{
		case 1:
			EnableWindow(hMainWindow, true); // Re-enables parent window once dialog box is closed. 
			DestroyWindow(hWnd);
			break;
		}
	case WM_CLOSE:
		EnableWindow(hMainWindow, true);
		DestroyWindow(hWnd);
	default:
		return DefWindowProcW(hWnd, msg, wp, lp);
	}
}

void registerDialogClass(HINSTANCE hInst)
{
	WNDCLASSW dialog = { 0 };
	dialog.hbrBackground = (HBRUSH)COLOR_WINDOW;
	dialog.hCursor = LoadCursor(NULL, IDC_CROSS);
	dialog.hInstance = hInst;
	dialog.lpszClassName = L"myDialogClass";
	dialog.lpfnWndProc = DialogProcedure;  // Pointer to a funciton so no parentheses are used

	RegisterClassW(&dialog);
}

void displayDialog(HWND hWnd)
{
	HWND hDlg = CreateWindowW(L"myDialogClass", L"Dialog", WS_VISIBLE | WS_OVERLAPPEDWINDOW, 400, 400, 200, 200, hWnd, NULL, NULL, NULL);

	CreateWindowW(L"Button", L"Close", WS_VISIBLE | WS_CHILD, 20, 20, 100, 40, hDlg, (HMENU)1, NULL, NULL);

	EnableWindow(hWnd, false);	// Passing false disables the parent window when dialog box is displayed. 
	// Have to reneable window once dialog box is closed. 
}

/*
* References:
*	1. https://docs.microsoft.com/en-us/cpp/windows/walkthrough-creating-windows-desktop-applications-cpp?view=msvc-160
*	2. https://www.youtube.com/watch?v=7K6HCeog09c
*	3. https://stackoverflow.com/questions/21834833/argument-of-type-const-char-is-incompatible-with-parameter-of-type-lpcwstr/44942751
*	4. https://stackoverflow.com/questions/22450423/how-to-use-crt-secure-no-warnings
*	
*
*/