// hclMail.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "hclMail.h"

#define MAX_LOADSTRING 100
#define minWidth 50

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPTSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_HCLMAIL, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_HCLMAIL));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_HCLMAIL));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCE(IDC_HCLMAIL);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;

	hInst = hInstance; // Store instance handle in our global variable

	hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	RECT		rect;
	static HCURSOR 	hCursor;
	static BOOL	bSplitterMoving;
	static DWORD	dwSplitterPos1, dwSplitterPos2;
	static HWND	hWnd1, hWnd2, hWnd3;
	static bool flagBar1, flagBar2;
	static int wnd2Width;

	switch (message)
	{
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_CREATE:
		hInst = ((LPCREATESTRUCT)lParam)->hInstance;

		hWnd1 = CreateWindowEx(WS_EX_CLIENTEDGE, "edit", NULL,
			WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | ES_MULTILINE | WS_VSCROLL,
			0, 0, 0, 0,
			hWnd, (HMENU)1,
			hInst, NULL);

		hWnd2 = CreateWindowEx(WS_EX_CLIENTEDGE, "edit", NULL,
			WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | ES_MULTILINE | WS_VSCROLL,
			0, 0, 0, 0,
			hWnd, (HMENU)2,
			hInst, NULL);

		hWnd3 = CreateWindowEx(WS_EX_CLIENTEDGE, "edit", NULL,
			WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | ES_MULTILINE | WS_VSCROLL,
			0, 0, 0, 0,
			hWnd, (HMENU)3,
			hInst, NULL);

		hCursor = LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZEWE));
		bSplitterMoving = FALSE;

		dwSplitterPos1 = 130;
		dwSplitterPos2 = 300;
		wnd2Width = dwSplitterPos2 - dwSplitterPos1 - 2;
		flagBar1 = true;
		flagBar2 = true;

		return 0;

	case WM_SIZE:
		/*
		If window is shrunk so that splitter now lies outside the
		window boundaries, move the splitter within the window.
		*/
		if ((wParam != SIZE_MINIMIZED))
		{
			if (LOWORD(lParam) / 2 < dwSplitterPos1)
				dwSplitterPos1 = LOWORD(lParam) / 2;
			if (LOWORD(lParam) < dwSplitterPos2)
				dwSplitterPos2 = LOWORD(lParam) - minWidth;
		}

		/* Adjust the children's size and position */
		MoveWindow(hWnd1, 0, 0, dwSplitterPos1 - 1, HIWORD(lParam), TRUE);

		if (flagBar1)
		{
			dwSplitterPos2 = dwSplitterPos1 + wnd2Width + 4;
			MoveWindow(hWnd2, dwSplitterPos1 + 2, 0, wnd2Width, HIWORD(lParam), TRUE);
			MoveWindow(hWnd3, dwSplitterPos2 + 2, 0, LOWORD(lParam) - dwSplitterPos1 - wnd2Width - 6, HIWORD(lParam), TRUE);
		}

		if (flagBar2)
		{
			MoveWindow(hWnd2, dwSplitterPos1 + 2, 0, dwSplitterPos2 - dwSplitterPos1 - 2, HIWORD(lParam), TRUE);
			MoveWindow(hWnd3, dwSplitterPos2 + 2, 0, LOWORD(lParam) - dwSplitterPos2 - 2, HIWORD(lParam), TRUE);
			wnd2Width = dwSplitterPos2 - dwSplitterPos1 - 2;
		}

		flagBar1 = false;
		flagBar2 = false;

		return 0;


	case WM_MOUSEMOVE:
		SetCursor(hCursor);
		if ((wParam == MK_LBUTTON) && bSplitterMoving)
		{
			GetClientRect(hWnd, &rect);
			int newPos = LOWORD(lParam);
			if (newPos > rect.right - minWidth)
				return 0;

			if ((abs(static_cast<LONG>(newPos - dwSplitterPos1)) <= 10) && (newPos > minWidth) && ((dwSplitterPos2 - newPos) > minWidth))
			{
				dwSplitterPos1 = newPos;
				flagBar1 = true;
			}
			else if ((abs(static_cast<LONG>(newPos - dwSplitterPos2)) <= 10) && (newPos > dwSplitterPos1 + minWidth))
			{
				dwSplitterPos2 = newPos;
				flagBar2 = true;
			}
			SendMessage(hWnd, WM_SIZE, 0, MAKELPARAM(rect.right, rect.bottom));
		}

		return 0;


	case WM_LBUTTONDOWN:
		SetCursor(hCursor);
		bSplitterMoving = TRUE;
		SetCapture(hWnd);
		return 0;


	case WM_LBUTTONUP:
		ReleaseCapture();
		bSplitterMoving = FALSE;
		return 0;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
