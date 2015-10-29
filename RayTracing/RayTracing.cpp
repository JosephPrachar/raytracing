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

// Forward declarations of functions included in this code module:

void printHeader(ofstream* myFile, int width, int height);

void readSetupFile(std::string file, Window* view, Point* eye, Color* ambient, Light* light);
void readShapeFile(std::string file, std::vector<Shape*>* list);

// I have a feeling that most of these functions should be placed in another file, just not sure where to put them
Sphere* parseSphere(std::string line, bool* good);
Window parseWindow(std::string line, bool* good);
Point parsePoint(std::string line, bool* good);
Light parseLight(std::string line, bool* good);
Color parseColor(std::string line, bool* good);
vector<float> parseFloats(std::string line, int expected);

std::vector<std::string> split(const std::string &s, char delim);
void &split(const std::string &s, char delim, std::vector<std::string> &elems);


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

	Window* view = new Window(-10, 10, -7.5, 7.5, 1024, 768);
	Point* eye = new Point(0, 0, -14);
	Color* ambientColor = new Color(1, 1, 1);
	Light* pointLight = new Light(Point(0, 0, -100), Color(1.5, 1.5, 1.5));

	std::vector<Shape*> shapes = std::vector<Shape*>();
	readShapeFile("", &shapes);

	rc = new RayCaster(*view, *eye, shapes, *ambientColor, *pointLight);


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
		if (wParam == 'S') {		
			rc->castAllRays();
			needsRedraw = true;
			InvalidateRect(hWnd, NULL, TRUE);
		}
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		if (needsRedraw) {
			rc->printPicture(hdc);
			needsRedraw = false;
		}

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



void readSetupFile(std::string file, Window* view, Point* eye, Color* ambient, Light* light){
	// todo: check if successful input on each line
	
	string line;
	bool good = true;

	std::ifstream f(file);

	if (f.is_open()){
		getline(f, line);
		*view = parseWindow(line, &good);

		getline(f, line);
		*eye = parsePoint(line, &good);

		getline(f, line);
		*ambient = parseColor(line, &good);

		getline(f, line);
		*light = parseLight(line, &good);
	}

	f.close();
}
void readShapeFile(string pointsFile, string verticies, vector<Triangle>* list){
	string line;

	ifstream f(pointsFile);
	int count = 0;
	vector<Point*> points = vector<Point*>();
	if (f.is_open()){
		while(getline(f, line)){
			bool good = false;
			Point* temp = parsePoint(line, &good);

			if (good == true){
				points.push_back(temp);
			}
		}
		f.close();
	}

	//list->push_back(new Triangle(Color(0,0,1), Finish(.2, .4, .5, .05), Point(0,0,0), Point(1,0,-2), Point(0,1,-1)));
	//list->push_back(new Triangle(Color(1,0,0), Finish(.2, .4, .5, .05), Point(4,4,0), Point(4,0,-10), Point(0,4,-10)));
	//list->push_back(new Triangle(Color(0,1,0), Finish(.2, .4, .5, .05), Point(1,2,0), Point(-1,0,0), Point(0,-1,0)));
	//list->push_back(new Sphere(Point(-1, 1, 5), 2, Color(1, 0, 1), Finish(.2, .4, .5, .05)));
	//list->clear();

	//list->push_back(new Triangle(Color(1,0,0), Finish(.2, .4, .5, .05), Point(0,0,-1), Point(0,1,0), Point(1,0,0)));
	//list->push_back(new Triangle(Color(1,0,0), Finish(.2, .4, .5, .05), Point(0,0,-1), Point(-1,0,0), Point(0,1,0)));
	//list->push_back(new Triangle(Color(1,0,0), Finish(.2, .4, .5, .05), Point(0,0,-1), Point(0,-1,0), Point(-1,0,0)));
	//list->push_back(new Triangle(Color(1,0,0), Finish(.2, .4, .5, .05), Point(0,0,-1), Point(1,0,0), Point(0,-1,0)));

	//list->push_back(new Triangle(Color(0,0,0), Finish(.2, .4, .5, .05), Point(0,0,0), Point(0,1,0), Point(1,0,0)));
	//list->push_back(new Triangle(Color(0,0,0), Finish(.2, .4, .5, .05), Point(0,0,0), Point(-1,0,0), Point(0,1,0)));
	//list->push_back(new Triangle(Color(0,0,0), Finish(.2, .4, .5, .05), Point(0,0,0), Point(0,-1,0), Point(-1,0,0)));
	//list->push_back(new Triangle(Color(0,0,0), Finish(.2, .4, .5, .05), Point(0,0,0), Point(1,0,0), Point(0,-1,0)));

	//list->push_back(new Sphere(Point(8, -10, 110), 100, Color(.2, .2, .6), Finish(.4, .8, 0, .05)));
}

Sphere* parseSphere(std::string line, bool* good){
	vector<float> vals = parseFloats(line, 11);
	if (vals.size() != 0){
		*good = true;
		return new Sphere(Point(vals[0], vals[1], vals[2]), vals[3], Color(vals[4], vals[5], vals[6]), Finish(vals[7], vals[8], vals[9], vals[10]));
	}

	*good = false;
	return new Sphere();
}
Window parseWindow(std::string line, bool* good){
	vector<float> vals = parseFloats(line, 6);
	if (vals.size() != 0){
		*good = true;
		return Window(vals[0], vals[1], vals[2], vals[3], (int)vals[4], (int)vals[5]);
	}

	*good = false;
	return Window(0,0,0,0,0,0);
}
Point* parsePoint(std::string line, bool* good){
	vector<float> vals = parseFloats(line, 3);
	if (vals.size() != 0){
		*good = true;
		return new Point(vals[0], vals[1], vals[2]);
	}

	*good = false;
	return new Point(0,0,0);
}
Light parseLight(std::string line, bool* good){
		vector<float> vals = parseFloats(line, 6);
	if (vals.size() != 0){
		*good = true;
		return Light(Point(vals[0], vals[1], vals[2]), Color(vals[3], vals[4], vals[5]));
	}

	*good = false;
	return Light(Point(0,0,0), Color(0,0,0));
}
Color parseColor(std::string line, bool* good){
	vector<float> vals = parseFloats(line, 3);
	if (vals.size() != 0){
		*good = true;
		return Color(vals[0], vals[1], vals[2]);
	}

	*good = false;
	return Color(0,0,0);
}
vector<float> parseFloats(std::string line, int expected){
	vector<string> chars = split(line, ' ');
	vector<float> nums = vector<float>();

	if (chars.size() == expected){
		try{
			for (int i = 0; i < expected; ++i){
				float f = std::stof(chars[i]);
				nums.push_back(f);
			}
		}
		catch (invalid_argument e){
			return vector<float>();
		}
	}

	return nums;
}

std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}
void &split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}