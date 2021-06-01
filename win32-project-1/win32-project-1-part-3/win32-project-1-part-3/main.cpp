#include <windows.h>
#include <stdio.h>

#define OPEN_FILE_BUTTON 1
#define SAVE_FILE_BUTTON 2

LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);

void AddControls(HWND);

HWND hMainWindow, hEdit;

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow)
{
	WNDCLASSW wc = { 0 };

	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hInstance = hInst;
	wc.lpszClassName = L"myWindowClass";
	wc.lpfnWndProc = WindowProcedure;  // Pointer to a funciton so no parentheses are used

	if (!RegisterClassW(&wc))
		return -1;

	hMainWindow = CreateWindowW(L"myWindowClass", L"My Window", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, 500, 500,
		NULL, NULL, NULL, NULL);

	MSG msg = { 0 };

	while (GetMessage(&msg, NULL, NULL, NULL))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}

void display_file(char* path)
{
	FILE* file;
	file = fopen(path, "rb");
	fseek(file, 0, SEEK_END);
	int _size = ftell(file);
	rewind(file);
	char* data = new char[_size + 1]; // size + 1 because we also need a terminating character for the string.
	//char* data = reinterpret_cast<char*>(malloc(_size + 1));  // alternative to line above.
	fread(data, _size, 1, file);
	// Insecure fread usage, to allow have to enable the _CRT_SECURE_NO_WARNINGS compilation flag
	// {Ref 1}
	data[_size] = '\0';

	SetWindowText(hEdit, data);
	fclose(file);
	//delete data;

}

void open_file(HWND hWnd)
{
	OPENFILENAME ofn;

	char file_name[100];

	ZeroMemory(&ofn,sizeof(OPENFILENAME));

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFile = file_name;	// stores the path of the file that the user has selected.
	ofn.lpstrFile[0] = '\0'; // sets the first character of the string to a terminating character so there is no default file.
	ofn.nMaxFile = 100;	// maximum size of the file path that can be read.
	ofn.lpstrFilter = "All Files\0*.*\0Source Files\0*.CPP\0Text Files\0*.TXT\0";
	// File filter in file explorer; <Title\0Extension\0> is the format.
	ofn.nFilterIndex = 1; // default index, not zero indexed. 

	GetOpenFileName(&ofn);
	// If this function returns successfully, it will automatically set the file path of the selected file in ofn.lpstrFile.
	//MessageBox(NULL, ofn.lpstrFile, "", MB_OK); // Test to see if the file path is loaded.
	// To use GetOpenFileName(&ofn) you may have to add a path in the "Linker Settings" of your compiler options...
	// The path to the file is ..../MinGW64/x86_64-w64-mingw32/lib/libcomdlg32.a

	display_file(ofn.lpstrFile);
}

void write_file(char* path)
{
	FILE* file;
	file = fopen(path, "w");

	int _size = GetWindowTextLength(hEdit);
	char* data = new char[_size + 1];

	GetWindowText(hEdit, data, _size+1);

	fwrite(data, _size + 1, 1, file);
	fclose(file);
}

void save_file(HWND hWnd)
{
	OPENFILENAME ofn;

	char file_name[100];

	ZeroMemory(&ofn, sizeof(OPENFILENAME));

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFile = file_name;	// stores the path of the file that the user has selected.
	ofn.lpstrFile[0] = '\0'; // sets the first character of the string to a terminating character so there is no default file.
	ofn.nMaxFile = 100;	// maximum size of the file path that can be read.
	ofn.lpstrFilter = "All Files\0*.*\0Source Files\0*.CPP\0Text Files\0*.TXT\0";
	ofn.nFilterIndex = 1; 

	GetSaveFileName(&ofn); 
	// Path that is set in ofn.lpstrFile is the path to which the user wants to save a file.
	

	write_file(ofn.lpstrFile);
}

LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
	case WM_COMMAND:
		// Run this code when a button is clicked. 
		// To know what button was clicked you have to check the W-Parameters, aka the 'wp' var
		switch (wp)
		{
		case OPEN_FILE_BUTTON:
			// When open-file button is clicked we have to call another function that will display the open file dialog box on the screen.
			open_file(hWnd);
			break;
		
		case SAVE_FILE_BUTTON:
			save_file(hWnd);
			break;
		}
		break;
	case WM_CREATE:
		AddControls(hWnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProcW(hWnd, msg, wp, lp);
	}
}

void AddControls(HWND hWnd)
{
	CreateWindowW(L"Button", L"Open FIle", WS_VISIBLE | WS_CHILD, 10, 10, 150, 36, hWnd, (HMENU) OPEN_FILE_BUTTON, NULL, NULL);
	CreateWindowW(L"Button", L"Save FIle", WS_VISIBLE | WS_CHILD, 170, 10, 150, 36, hWnd, (HMENU)SAVE_FILE_BUTTON, NULL, NULL);
	hEdit = CreateWindowW(L"Edit", L"", WS_VISIBLE | WS_CHILD | ES_MULTILINE | WS_BORDER | WS_VSCROLL | WS_HSCROLL, 
		10, 50, 400, 300, hWnd, NULL, NULL, NULL);
}

/*
* References:
*	1. https://stackoverflow.com/questions/22450423/how-to-use-crt-secure-no-warnings
*
*/