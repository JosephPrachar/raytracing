// RayTracing.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "RayTracing.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
RayCaster* rc;

ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

bool needsRedraw = true;

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	//Window* view = new Window(-10, 70, -10, 50, 801, 600);
	Camera* cam = new Camera();
	Point* eye = new Point(0, 0, 100);
	Color* ambientColor = new Color(1, 1, 1);
	Light* pointLight = new Light(Point(0, 0, 100), Color(1.5, 1.5, 1.5));

	vector<Triangle*> *shapes = new vector<Triangle*>();
	readTriangles("C:\\Users\\joseph\\Documents\\Projects\\RayTracing\\C++\\RayTracing\\Debug\\meshPoints.in",
		"C:\\Users\\joseph\\Documents\\Projects\\RayTracing\\C++\\RayTracing\\Debug\\meshVerticies.in", *shapes);

	rc = new RayCaster(*cam, *eye, shapes, *ambientColor, *pointLight);


	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_RAYTRACING, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_RAYTRACING));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
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

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_RAYTRACING));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_RAYTRACING);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

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
      0, 0, 1150, 850, NULL, NULL, hInstance, NULL);

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
	int savedDC;
	int width;
	int height;
	PAINTSTRUCT ps;
	HDC hdc;
	HDC backbuffDC;
	HBITMAP backbuffer;
	HBRUSH hBrush;
	RECT rect;
	Camera* cam = rc->getCamera();

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
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
	case WM_KEYDOWN:		
		if (wParam == 'Z') {		
		rc->castAllRays();

		//std::string str = std::to_string(rc->computeTime);
		//MessageBoxA(hWnd, str.c_str (), NULL, 0);
		needsRedraw = true;
		InvalidateRect(hWnd, NULL, TRUE);

		} else if (wParam == 'Q') { // X+   Point move
			cam->moveBy(Vector(1, 0, 0));
		} else if (wParam == 'A') { // X-
			cam->moveBy(Vector(-1, 0, 0));
		} else if (wParam == 'W') { // Y+
			cam->moveBy(Vector(0, 1, 0));
		} else if (wParam == 'S') { // Y-
			cam->moveBy(Vector(0, -1, 0));
		} else if (wParam == 'E') { // Z+
			cam->moveBy(Vector(0, 0, 1));
		} else if (wParam == 'D') { // Z-
			cam->moveBy(Vector(0, 0, -1));
		} else if (wParam == 'R') { // X+   Rotate
			cam->rotate(Vector(.1, 0, 0));
		} else if (wParam == 'F') { // X-
			cam->rotate(Vector(-.1, 0, 0));
		} else if (wParam == 'T') { // Y+
			cam->rotate(Vector(0, .1, 0));
		} else if (wParam == 'G') { // Y-
			cam->rotate(Vector(0, -.1, 0));
		} else if (wParam == 'Y') { // Z+
			cam->rotate(Vector(0, 0, .1));
		} else if (wParam == 'H') { // Z-
			cam->rotate(Vector(0, 0, -.1));
		}

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		
        GetClientRect(hWnd, &rect);
        width=rect.right;
        height=rect.bottom;

        backbuffDC = CreateCompatibleDC(hdc);

        backbuffer = CreateCompatibleBitmap( hdc, width, height);

        savedDC = SaveDC(backbuffDC);
        SelectObject( backbuffDC, backbuffer );
        hBrush = CreateSolidBrush(RGB(255,255,255));
        FillRect(backbuffDC,&rect,hBrush);
        DeleteObject(hBrush);

		if (needsRedraw) {
			rc->printPicture(backbuffDC);
			needsRedraw = false;
		}

        BitBlt(hdc,0,0,width,height,backbuffDC,0,0,SRCCOPY);
        RestoreDC(backbuffDC,savedDC);

        DeleteObject(backbuffer);
        DeleteDC(backbuffDC);

		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
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
