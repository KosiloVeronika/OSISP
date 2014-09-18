// MiniPaint.cpp: определяет точку входа для приложения.
//

#include "stdafx.h"
#include "MiniPaint.h"

#define MAX_LOADSTRING 100
// Идентификатор ресурса шаблона меню
#define IDM_MYMENURESOURCE 3
#define PENCIL 8
#define CREATE 9
#define OPEN 10
#define BLACK 13
#define LINE 14
#define RECTANGLE 15
#define ELLIPSE 16
#define CIRCLE 17




// Глобальные переменные:
HINSTANCE hInst;								// текущий экземпляр
TCHAR szTitle[MAX_LOADSTRING];					// Текст строки заголовка
TCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна
HMENU Init_MENU();
HWND hWnd;
HWND hDrawWnd;
HPEN MyPen;
RECT rect;
HBRUSH MyBrush;
HDC hDC;
HDC hMemDC;
HBITMAP hBit;
int operation_number = 0, color_number = BLACK, width = 1;
int x, y;
int r=0, g=0, b=0;

// Отправить объявления функций, включенных в этот модуль кода:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
ATOM				RegisterChildWnd(HINSTANCE hInstance);
LRESULT CALLBACK	ChildProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: разместите код здесь.
	MSG msg;
	HACCEL hAccelTable;

	// Инициализация глобальных строк
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_MINIPAINT, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);
	RegisterChildWnd(hInstance);

	// Выполнить инициализацию приложения:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MINIPAINT));

	// Цикл основного сообщения:
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
//  ФУНКЦИЯ: MyRegisterClass()
//
//  НАЗНАЧЕНИЕ: регистрирует класс окна.
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
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDC_icon));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL/*MAKEINTRESOURCE(IDC_MINIPAINT)*/;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

ATOM RegisterChildWnd(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW ;
	wcex.lpfnWndProc	= ChildProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= NULL;
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(6);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= L"ChildWindow";
	wcex.hIconSm		= NULL;

	return RegisterClassEx(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   НАЗНАЧЕНИЕ: сохраняет обработку экземпляра и создает главное окно.
//
//   КОММЕНТАРИИ:
//
//        В данной функции дескриптор экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится на экран главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Сохранить дескриптор экземпляра в глобальной переменной

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, Init_MENU(), hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  НАЗНАЧЕНИЕ:  обрабатывает сообщения в главном окне.
//
//  WM_COMMAND	- обработка меню приложения
//  WM_PAINT	-Закрасить главное окно
//  WM_DESTROY	 - ввести сообщение о выходе и вернуться.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hDC;

	switch (message)
	{
		//
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Разобрать выбор в меню:
		switch (wmId)
		{
		case CREATE:
			//hDrawWnd = CreateBitmap(600,400,1,100,NULL)
			hDrawWnd = CreateWindow(L"ChildWindow", NULL, WS_CHILD | WS_BORDER, 1, 1, 600, 400, hWnd, NULL, hInst, NULL);
			hDC = GetDC(hDrawWnd);
			hMemDC = CreateCompatibleDC(hDC);//
			GetClientRect(hDrawWnd, &rect);
			hBit = CreateCompatibleBitmap(hDC, rect.right, rect.bottom);
			MyBrush = CreateSolidBrush(RGB(255, 255, 255));
			SelectObject(hMemDC, hBit);
			FillRect(hMemDC, &rect, MyBrush);
			DeleteObject(MyBrush);
			ReleaseDC(hDrawWnd, hDC);
			ShowWindow(hDrawWnd, SW_SHOW);
			UpdateWindow(hDrawWnd);
			break;
		case PENCIL:
			operation_number = PENCIL;
			break;
		case LINE:
			operation_number = LINE;
			break;
		case RECTANGLE:
			operation_number = RECTANGLE;
			break;
		case CIRCLE:
			operation_number = CIRCLE;
			break;
		case ELLIPSE:
			operation_number = ELLIPSE;
			break;
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
		hDC = BeginPaint(hWnd, &ps);
		// TODO: добавьте любой код отрисовки...
			ValidateRect(hWnd, NULL);
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

// Обработчик сообщений для окна "О программе".
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
HMENU Init_MENU()
{
	HMENU hMenu = CreateMenu();
	HMENU hPopUp = CreatePopupMenu();
	HMENU hCPopUp = CreatePopupMenu();
	HMENU hWid = CreatePopupMenu();

	AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hPopUp, L"File");
	AppendMenu(hMenu, MF_STRING,PENCIL,L"Pencil");
	AppendMenu(hMenu, MF_STRING,LINE,L"Line");
	AppendMenu(hMenu, MF_STRING,ELLIPSE,L"Ellipse");
	AppendMenu(hMenu, MF_STRING,RECTANGLE,L"Rectangle");
	AppendMenu(hMenu, MF_STRING,CIRCLE,L"Circle");

	AppendMenu(hPopUp, MF_STRING, CREATE, L"Create");
	AppendMenu(hPopUp, MF_SEPARATOR, NULL, L"");
	AppendMenu(hPopUp, MF_STRING, OPEN, L"Open...");
	//AppendMenu(hPopUp, MF_STRING, SAVE, L"Save");
	//AppendMenu(hPopUp, MF_STRING, SAVE_AS, L"Save As...");
	AppendMenu(hPopUp, MF_SEPARATOR, NULL, L"");
	AppendMenu(hPopUp, MF_STRING, IDM_EXIT, L"Exit");
	AppendMenu(hPopUp, MF_SEPARATOR, NULL, L"");
	AppendMenu(hPopUp, MF_STRING, IDM_ABOUT, L"About...");
	return hMenu;
}

LRESULT CALLBACK ChildProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	int x2, y2;

	switch (Message)
	{
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		switch (wmId)
		{

		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;

		default:
			return DefWindowProc(hWnd, Message, wParam, lParam);
		}
		break;

	case WM_LBUTTONDOWN:
		//SetCapture(hWnd);
		x = LOWORD(lParam);
		y = HIWORD(lParam);
		break;

	case WM_MOUSEMOVE:
		if (MK_LBUTTON == wParam)
		{
			switch (operation_number)
			{
			case PENCIL:
				x2 = LOWORD(lParam);
				y2 = HIWORD(lParam);
				MyPen = CreatePen(PS_SOLID, width, RGB(r, g, b));
				hDC = GetDC(hDrawWnd);
				GetClientRect(hDrawWnd, &rect);
				BitBlt(hDC,rect.left, rect.top, rect.right, rect.bottom, hMemDC,rect.left, rect.top, SRCCOPY);
				//SelectClipRgn(hDC,rect);
				MoveToEx(hMemDC, x, y, NULL);
				LineTo(hMemDC, x2, y2);
				MoveToEx(hDC, x, y, NULL);
				LineTo(hDC, x2, y2);
				ReleaseDC(hDrawWnd, hDC);
				DeleteObject(MyPen);
				x = x2;
				y = y2;
				break;
			case LINE:
				/*x2 = LOWORD(lParam);
				y2 = HIWORD(lParam);
				hDC = GetDC(hDrawWnd);
				BitBlt(hDC,rect.left, rect.top, rect.right, rect.bottom, hMemDC,rect.left, rect.top, SRCCOPY);
				MoveToEx(hDC, x, y, NULL);
				LineTo(hDC, x2, y2);
				ReleaseDC(hDrawWnd, hDC);*/

				x2 = LOWORD(lParam);
				y2 = HIWORD(lParam);
				MyPen = CreatePen(PS_SOLID, width, RGB(r, g, b));
				hDC = GetDC(hDrawWnd);
				GetClientRect(hDrawWnd, &rect);
				BitBlt(hDC, 0, 0, rect.right - rect.left, rect.bottom - rect.top, hMemDC, 0, 0, SRCCOPY);
				MoveToEx(hDC, x, y, NULL);
				LineTo(hDC, x2, y2);
				ReleaseDC(hDrawWnd, hDC);
				DeleteObject(MyPen);
				break;
			case CIRCLE:

				break;
			case RECTANGLE:
				x2 = LOWORD(lParam);
				y2 = HIWORD(lParam);
				MyPen = CreatePen(PS_SOLID, width, RGB(r, g, b));
				hDC = GetDC(hDrawWnd);
				GetClientRect(hDrawWnd, &rect);
				BitBlt(hDC, 0, 0, rect.right - rect.left, rect.bottom - rect.top, hMemDC, 0, 0, SRCCOPY);
				Rectangle(hDC, x, y, x2, y2);
				ReleaseDC(hDrawWnd, hDC);
				DeleteObject(MyPen);
				break;
			default:
				break;
			}
		}
	case WM_LBUTTONUP:
			//ClipCursor(NULL);
			hDC = GetDC(hDrawWnd);
			BitBlt(hMemDC, 0, 0, rect.right - rect.left, rect.bottom - rect.top, hDC, 0, 0, SRCCOPY);
			ReleaseDC(hDrawWnd, hDC);
			ValidateRect(hWnd, NULL);
			//ClipCursor(NULL);
			break;

	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);
			ValidateRect(hWnd, NULL);	// TODO: добавьте любой код отрисовки...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, Message, wParam, lParam);
	}

	return 0;
}