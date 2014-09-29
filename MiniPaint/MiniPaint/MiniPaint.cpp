// MiniPaint.cpp: определяет точку входа для приложения.
//

#include "stdafx.h"
#include "MiniPaint.h"
<<<<<<< HEAD
#include <commdlg.h>
#include "Resource.h"
=======
>>>>>>> parent of 8b04d33... Lab 1

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
<<<<<<< HEAD
RECT rect;
HPEN myPen;
HBRUSH myBrush;
LOGBRUSH logbrush;

int operation = ID_PENCIL;
int x0, y0, x1, y1, x2, y2, xZ, yZ;
double m;
int penWidth = 1;
DWORD rgb;
DWORD fillRGB = 0xffffff;

bool flag = false;
bool zoomOrPan = false;
bool saveFirstTime = true;
TCHAR File[MAX_PATH];
=======
int operation_number = 0, color_number = BLACK, width = 1;
int x, y;
int r=0, g=0, b=0;
>>>>>>> parent of 8b04d33... Lab 1

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
<<<<<<< HEAD
	case WM_CREATE:
		logbrush.lbStyle = BS_HOLLOW;
		break;
=======
		//
>>>>>>> parent of 8b04d33... Lab 1
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Разобрать выбор в меню:
		switch (wmId)
		{
<<<<<<< HEAD
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;

		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;

		case ID_OPEN:
			operation = 0;
			OPENFILENAME OpenDialog;
			TCHAR szFile[MAX_PATH];
			ZeroMemory(&OpenDialog, sizeof(OpenDialog));
			OpenDialog.lStructSize = sizeof(OpenDialog);
			OpenDialog.hwndOwner = hWnd;
			OpenDialog.lpstrFile = szFile;
			OpenDialog.lpstrFile[0] = '\0';
			OpenDialog.nMaxFile = sizeof(szFile);
			OpenDialog.lpstrFilter = L"Enchanced metafiles (*.emf)\0*.emf\0All files\0*.*\0";
			OpenDialog.nFilterIndex = 1;
			OpenDialog.lpstrFileTitle = NULL;
			OpenDialog.nMaxFileTitle = 255;
			OpenDialog.lpstrInitialDir = L"";
			OpenDialog.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
			if (GetOpenFileName(&OpenDialog))
			{
				if (hDrawWnd != NULL)
				{
					DeleteObject(hDC);
					DeleteObject(hMemDC);
					DeleteObject(hBit);
				}
				if (hText != NULL)
				{
					SetFocus(NULL);
					DestroyWindow(hText);
				}
				hDrawWnd = CreateWindow(L"ChildWnd", NULL, WS_CHILD | WS_BORDER,
					1, 1, 800, 400, hWnd, NULL, hInst, NULL);

				hDC = GetDC(hDrawWnd);
				hMemDC = CreateCompatibleDC(hDC);
				GetClientRect(hDrawWnd, &rect);
				hBit = CreateCompatibleBitmap(hDC, rect.right, rect.bottom);
				myBrush = CreateSolidBrush(RGB(255, 255, 255));
				SelectObject(hMemDC, hBit);
				FillRect(hMemDC, &rect, myBrush);
				DeleteObject(myBrush);
				h = GetEnhMetaFile(szFile);
				PlayEnhMetaFile(hMemDC, h, &rect);
				DeleteEnhMetaFile(h);

				ReleaseDC(hDrawWnd, hDC);
				ShowWindow(hDrawWnd, SW_SHOW);

				BitBlt(hDC, 0, 0, rect.right, rect.bottom, hMemDC, 0, 0, SRCCOPY);
				UpdateWindow(hDrawWnd);
				InvalidateRect(hDrawWnd, &rect, true);
				m = 1;
				xZ = 0;
				yZ = 0;
				for (int i = 0; i <= _tcslen(szFile); i++)
					File[i] = szFile[i];
				saveFirstTime = false;				
			}
			break;

		case ID_SAVE:
			if (hDrawWnd == NULL)
				break;
			if (saveFirstTime)
			{
				OPENFILENAME SaveDialog;
				ZeroMemory(&SaveDialog, sizeof(SaveDialog));
				SaveDialog.lStructSize = sizeof(SaveDialog);
				SaveDialog.hwndOwner = hWnd;
				SaveDialog.lpstrFile = szFile;
				SaveDialog.lpstrFile[0] = '\0';
				SaveDialog.nMaxFile = sizeof(szFile);
				SaveDialog.lpstrFilter = L"Enchanced metafiles (*.emf)\0*.emf\0";
				SaveDialog.nFilterIndex = 1;
				SaveDialog.lpstrFileTitle = NULL;
				SaveDialog.nMaxFileTitle = 255;
				SaveDialog.lpstrInitialDir = L"";
				SaveDialog.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
				if (GetSaveFileName(&SaveDialog))
				{
					int len = _tcslen(szFile);
					if (len > 1)
					{
						if ((szFile[len - 1] == 'f') && (szFile[len - 2] == 'm') && (szFile[len - 3] == 'e') && (szFile[len - 4] == '.'))
						{
						}
						else
						{
							szFile[len] = '.';
							szFile[len + 1] = 'e';
							szFile[len + 2] = 'm';
							szFile[len + 3] = 'f';
							szFile[len + 4] = '\0';
						}
					}
					hDC = GetDC(hDrawWnd);
					GetClientRect(hDrawWnd, &rect);
					hFile = CreateEnhMetaFile(hDC, szFile, NULL, NULL);
					BitBlt(hFile, 0, 0, rect.right, rect.bottom, hMemDC, 0, 0, SRCCOPY);
					ReleaseDC(hDrawWnd, hDC);
					h = CloseEnhMetaFile(hFile);
					DeleteEnhMetaFile(h);
					for (int i = 0; i <= _tcslen(szFile); i++)
						File[i] = szFile[i];
					saveFirstTime = false;
				}
			}
			else
			{
				hDC = GetDC(hDrawWnd);
				GetClientRect(hDrawWnd, &rect);
				hFile = CreateEnhMetaFile(hDC, File, NULL, NULL);
				BitBlt(hFile, 0, 0, rect.right, rect.bottom, hMemDC, 0, 0, SRCCOPY);
				ReleaseDC(hDrawWnd, hDC);
				h = CloseEnhMetaFile(hFile);
				DeleteEnhMetaFile(h);
			}
			break;

		case ID_SAVE_AS:
			if (hDrawWnd == NULL)
				break;
			OPENFILENAME SaveDialog;
			ZeroMemory(&SaveDialog, sizeof(SaveDialog));
			SaveDialog.lStructSize = sizeof(SaveDialog);
			SaveDialog.hwndOwner = hWnd;
			SaveDialog.lpstrFile = szFile;
			SaveDialog.lpstrFile[0] = '\0';
			SaveDialog.nMaxFile = sizeof(szFile);
			SaveDialog.lpstrFilter = L"Enchanced metafiles (*.emf)\0*.emf\0";
			SaveDialog.nFilterIndex = 1;
			SaveDialog.lpstrFileTitle = NULL;
			SaveDialog.nMaxFileTitle = 255;
			SaveDialog.lpstrInitialDir = L"";
			SaveDialog.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
			if (GetSaveFileName(&SaveDialog))
			{
				int len = _tcslen(szFile);
				if (len > 1)
				{
					if ((szFile[len - 1] == 'f') && (szFile[len - 2] == 'm') && (szFile[len - 3] == 'e') && (szFile[len - 4] == '.'))
					{
					}
					else
					{
						szFile[len] = '.';
						szFile[len + 1] = 'e';
						szFile[len + 2] = 'm';
						szFile[len + 3] = 'f';
						szFile[len + 4] = '\0';
					}
				}
				hDC = GetDC(hDrawWnd);
				GetClientRect(hDrawWnd, &rect);
				hFile = CreateEnhMetaFile(hDC, szFile, NULL, NULL);
				BitBlt(hFile, 0, 0, rect.right, rect.bottom, hMemDC, 0, 0, SRCCOPY);
				ReleaseDC(hDrawWnd, hDC);
				h = CloseEnhMetaFile(hFile);
				DeleteEnhMetaFile(h);
				for (int i = 0; i <= _tcslen(szFile); i++)
					File[i] = szFile[i];
				saveFirstTime = false;
			}
			break;

		case ID_PRINT:
			if (hDrawWnd == NULL)
				break;
			PRINTDLG PrintDialog;
			ZeroMemory(&PrintDialog, sizeof(PRINTDLG));
			PrintDialog.lStructSize = sizeof(PRINTDLG);
			PrintDialog.hwndOwner = hWnd;
			PrintDialog.hDevMode = NULL;
			PrintDialog.hDevNames = NULL;
			PrintDialog.Flags = PD_USEDEVMODECOPIESANDCOLLATE | PD_RETURNDC | PD_PRINTTOFILE;
			PrintDialog.nCopies = 1;
			PrintDialog.nFromPage = 0xFFFF;
			PrintDialog.nToPage = 0xFFFF;
			PrintDialog.nMinPage = 1;
			PrintDialog.nMaxPage = 0xFFFF;
			if (PrintDlg(&PrintDialog) == TRUE)
			{
				DOCINFO doc;
				GlobalUnlock(PrintDialog.hDevMode);
				DEVNAMES * pdn = (DEVNAMES *)GlobalLock(PrintDialog.hDevNames);
				ZeroMemory(&doc, sizeof(doc));
				doc.cbSize = sizeof(doc);
				doc.lpszDatatype = _T("RAW");
				doc.lpszOutput = (TCHAR *)pdn + pdn->wOutputOffset;
				StartDoc(PrintDialog.hDC, &doc);
				StartPage(PrintDialog.hDC);
				GetClientRect(hDrawWnd, &rect);
				BitBlt(PrintDialog.hDC, 0, 0, rect.right, rect.bottom, hMemDC, 0, 0, SRCCOPY);
				EndPage(PrintDialog.hDC);
				EndDoc(PrintDialog.hDC);
				DeleteDC(PrintDialog.hDC);
			}
			break;

		case ID_CREATE:
			if (hDrawWnd != NULL)
			{
				DeleteObject(hDC);
				DeleteObject(hMemDC);
				DeleteObject(hBit);
			}
			if (hText != NULL)
			{
				SetFocus(NULL);
				DestroyWindow(hText);
			}


			hDrawWnd = CreateWindow(L"ChildWnd", NULL, WS_CHILD | WS_BORDER,
				1, 1, 800, 400, hWnd, NULL, hInst, NULL);

=======
		case CREATE:
			//hDrawWnd = CreateBitmap(600,400,1,100,NULL)
			hDrawWnd = CreateWindow(L"ChildWindow", NULL, WS_CHILD | WS_BORDER, 1, 1, 600, 400, hWnd, NULL, hInst, NULL);
>>>>>>> parent of 8b04d33... Lab 1
			hDC = GetDC(hDrawWnd);
			hMemDC = CreateCompatibleDC(hDC);//
			GetClientRect(hDrawWnd, &rect);
			hBit = CreateCompatibleBitmap(hDC, rect.right, rect.bottom);
<<<<<<< HEAD
			myBrush = CreateSolidBrush(RGB(255,255,255));
=======
			MyBrush = CreateSolidBrush(RGB(255, 255, 255));
>>>>>>> parent of 8b04d33... Lab 1
			SelectObject(hMemDC, hBit);
			FillRect(hMemDC, &rect, MyBrush);
			DeleteObject(MyBrush);
			ReleaseDC(hDrawWnd, hDC);
			ShowWindow(hDrawWnd, SW_SHOW);
			UpdateWindow(hDrawWnd);
<<<<<<< HEAD
			m = 1;
			xZ = 0;
			yZ = 0;

			break;
		case ID_PENCIL:
			if (hText != NULL)
			{
				SetFocus(NULL);
				DestroyWindow(hText);
			}
			flag = false;
			operation = ID_PENCIL;
			break;
		case ID_LINE:
			if (hText != NULL)
			{
				SetFocus(NULL);
				DestroyWindow(hText);
			}
			flag = false;
			operation = ID_LINE;
			break;
		case ID_POLYLINE:
			if (hText != NULL)
			{
				SetFocus(NULL);
				DestroyWindow(hText);
			}
			flag = false;
			operation = ID_POLYLINE;
			break;
		case ID_ELLIPSE:
			if (hText != NULL)
			{
				SetFocus(NULL);
				DestroyWindow(hText);
			}
			flag = false;
			operation = ID_ELLIPSE;
			break;
		case ID_RECT:
			if (hText != NULL)
			{
				SetFocus(NULL);
				DestroyWindow(hText);
			}
			flag = false;
			operation = ID_RECT;
			break;
		case ID_POLYGON:
			if (hText != NULL)
			{
				SetFocus(NULL);
				DestroyWindow(hText);
			}
			flag = false;
			operation = ID_POLYGON;
			break;
		case ID_TEXT:
			if (hText != NULL)
			{
				SetFocus(NULL);
				DestroyWindow(hText);
			}
			flag = false;
			operation = ID_TEXT;
			break;
		case ID_COLOR:
			if (hText != NULL)
			{
				SetFocus(NULL);
				DestroyWindow(hText);
			}
			CHOOSECOLOR cc;
			COLORREF arrCustColor[16];
			ZeroMemory(&cc, sizeof(CHOOSECOLOR));
			cc.lStructSize = sizeof(CHOOSECOLOR);
			cc.hwndOwner = hWnd;
			cc.lpCustColors = arrCustColor;
			cc.Flags = CC_FULLOPEN | CC_RGBINIT;
			if (ChooseColor(&cc) == TRUE)
			{
				rgb = cc.rgbResult;
			}
			break;
		case ID_FILL_COLOR:
			if (hText != NULL)
			{
				SetFocus(NULL);
				DestroyWindow(hText);
			}
			CHOOSECOLOR ccf;
			COLORREF arrCustColorFill[16];
			ZeroMemory(&ccf, sizeof(CHOOSECOLOR));
			ccf.lStructSize = sizeof(CHOOSECOLOR);
			ccf.hwndOwner = hWnd;
			ccf.lpCustColors = arrCustColorFill;
			ccf.Flags = CC_FULLOPEN | CC_RGBINIT;
			if (ChooseColor(&ccf) == TRUE)
			{
				fillRGB = ccf.rgbResult;
				logbrush.lbStyle = BS_SOLID;
			}
			break;
		case ID_ONE:
			if (hText != NULL)
			{
				SetFocus(NULL);
				DestroyWindow(hText);
			}
			flag = false;
			penWidth = 1;
=======
>>>>>>> parent of 8b04d33... Lab 1
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
<<<<<<< HEAD

	case WM_LBUTTONDOWN:
		if (zoomOrPan)
			break;
		if (!flag)
		{
			x0 = LOWORD(lParam);
			y0 = HIWORD(lParam);
		}
		if ((operation == ID_TEXT) && (hText != NULL))
		{
			SetFocus(NULL);
			DestroyWindow(hText);
		}
		if ((operation == ID_POLYLINE) || (operation == ID_POLYGON))
		{
			if (flag)
			{
				hDC = GetDC(hDrawWnd);
				GetClientRect(hDrawWnd, &rect);
				BitBlt(hMemDC, 0, 0, rect.right, rect.bottom, hDC, 0, 0, SRCCOPY);
			}
			else
				flag = true;
		}

		x1 = LOWORD(lParam);
		y1 = HIWORD(lParam);


		break;

	case WM_MOUSEMOVE:
		if (zoomOrPan)
			break;
		if (hDrawWnd == NULL)
			break;
		if (MK_LBUTTON == wParam)
		{
			switch (operation)
			{
			case ID_PENCIL:
				x2 = LOWORD(lParam);
				y2 = HIWORD(lParam);
				myPen = CreatePen(PS_SOLID, penWidth, rgb);
				hDC = GetDC(hWnd);
				GetClientRect(hWnd, &rect);
				SelectObject(hMemDC, myPen);
				MoveToEx(hMemDC, x1, y1, NULL);
				LineTo(hMemDC, x2, y2);
				ReleaseDC(hWnd, hDC);
				DeleteObject(myPen);
				BitBlt(hDC, 0, 0, rect.right, rect.bottom, hMemDC, 0, 0, SRCCOPY);

				x1 = x2;
				y1 = y2;
				break;

			case ID_LINE:
				x2 = LOWORD(lParam);
				y2 = HIWORD(lParam);

				myPen = CreatePen(PS_SOLID, penWidth, rgb);
				hDC = GetDC(hDrawWnd);
				GetClientRect(hDrawWnd, &rect);
				BitBlt(hDC, 0, 0, rect.right, rect.bottom, hMemDC, 0, 0, SRCCOPY);
				SelectObject(hDC, myPen);
				MoveToEx(hDC, x1, y1, NULL);
				LineTo(hDC, x2, y2);
				ReleaseDC(hDrawWnd, hDC);
				DeleteObject(myPen);
				break;

			case ID_RECT:
				x2 = LOWORD(lParam);
				y2 = HIWORD(lParam);
				myPen = CreatePen(PS_SOLID, penWidth, rgb);
				logbrush.lbColor = fillRGB;
				myBrush = CreateBrushIndirect(&logbrush);
				hDC = GetDC(hDrawWnd);
				GetClientRect(hDrawWnd, &rect);
				BitBlt(hDC, 0, 0, rect.right, rect.bottom, hMemDC, 0, 0, SRCCOPY);
				SelectObject(hDC, myBrush);
				SelectObject(hDC, myPen);
				Rectangle(hDC, x1, y1, x2, y2);
				ReleaseDC(hDrawWnd, hDC);
				DeleteObject(myPen);
				DeleteObject(myBrush);
				break;

			case ID_ELLIPSE:
				x2 = LOWORD(lParam);
				y2 = HIWORD(lParam);
				myPen = CreatePen(PS_SOLID, penWidth, rgb);
				myBrush = CreateSolidBrush(fillRGB);
				hDC = GetDC(hDrawWnd);
				GetClientRect(hDrawWnd, &rect);
				BitBlt(hDC, 0, 0, rect.right, rect.bottom, hMemDC, 0, 0, SRCCOPY);
				SelectObject(hDC, myBrush);
				SelectObject(hDC, myPen);
				Ellipse(hDC, x1, y1, x2, y2);
				ReleaseDC(hDrawWnd, hDC);
				DeleteObject(myPen);
				DeleteObject(myBrush);
				break;
			}
		}
		else
		{
			switch (operation)
			{
			case ID_POLYLINE:
				if (flag == false)
					break;
				x2 = LOWORD(lParam);
				y2 = HIWORD(lParam);

				myPen = CreatePen(PS_SOLID, penWidth, rgb);
				hDC = GetDC(hDrawWnd);
				GetClientRect(hDrawWnd, &rect);
				BitBlt(hDC, 0, 0, rect.right, rect.bottom, hMemDC, 0, 0, SRCCOPY);
				SelectObject(hDC, myPen);
				MoveToEx(hDC, x1, y1, NULL);
				LineTo(hDC, x2, y2);
				ReleaseDC(hDrawWnd, hDC);
				DeleteObject(myPen);
				break;

			case ID_POLYGON:
				if (flag == false)
					break;
				x2 = LOWORD(lParam);
				y2 = HIWORD(lParam);

				myPen = CreatePen(PS_SOLID, penWidth, rgb);
				hDC = GetDC(hDrawWnd);
				GetClientRect(hDrawWnd, &rect);
				BitBlt(hDC, 0, 0, rect.right, rect.bottom, hMemDC, 0, 0, SRCCOPY);
				SelectObject(hDC, myPen);
				MoveToEx(hDC, x1, y1, NULL);
				LineTo(hDC, x2, y2);
				ReleaseDC(hDrawWnd, hDC);
				DeleteObject(myPen);
				break;

			}
		}


		break;

	case WM_RBUTTONUP:
		if ((operation == ID_POLYGON) && (flag))
		{
			myPen = CreatePen(PS_SOLID, penWidth, rgb);
			hDC = GetDC(hDrawWnd);
			GetClientRect(hDrawWnd, &rect);
			BitBlt(hDC, 0, 0, rect.right, rect.bottom, hMemDC, 0, 0, SRCCOPY);
			SelectObject(hDC, myPen);
			MoveToEx(hDC, x1, y1, NULL);
			LineTo(hDC, x0, y0);
			ReleaseDC(hDrawWnd, hDC);
			DeleteObject(myPen);
		}
		flag = false;
		hDC = GetDC(hDrawWnd);
		GetClientRect(hDrawWnd, &rect);
		BitBlt(hMemDC, 0, 0, rect.right, rect.bottom, hDC, 0, 0, SRCCOPY);
		break;

	case WM_LBUTTONUP:
		if (zoomOrPan)
			break;
		if (operation == ID_TEXT)
		{

			hText = CreateWindow(L"Edit", NULL, WS_CHILD | WS_BORDER | WS_VISIBLE | ES_LEFT, x1, y1, 200, 20, hDrawWnd, NULL, hInst, NULL);
			SetFocus(hText);
		}
		else
		{
			hDC = GetDC(hDrawWnd);
			GetClientRect(hDrawWnd, &rect);
			BitBlt(hMemDC, 0, 0, rect.right, rect.bottom, hDC, 0, 0, SRCCOPY);
		}
		break;

	case WM_MOUSEWHEEL:
		int delta;
		zoomOrPan = true;
		if (hDrawWnd == NULL)
			break;
		delta = GET_WHEEL_DELTA_WPARAM(wParam);

		hDC = GetDC(hDrawWnd);
		GetClientRect(hDrawWnd, &rect);
		myBrush = CreateSolidBrush(RGB(255, 255, 255));
		if (GET_KEYSTATE_WPARAM(wParam) == MK_CONTROL)
		{
			if (delta == 120)
			{
				xZ = xZ + 30;
				if (xZ >= 1000)
				{
					ReleaseDC(hDrawWnd, hDC);
					DeleteObject(myBrush);
					xZ = 1000;
					break;
				}
				SelectObject(hDC, myBrush);
				FillRect(hDC, &rect, myBrush);
				StretchBlt(hDC, 0, 0, rect.right, rect.bottom, hMemDC, 0 + xZ, 0 + yZ, (int)rect.right*m, (int)rect.bottom*m, SRCCOPY);
			}
			else
			{
				xZ = xZ - 30;
				if (xZ <= -1000)
				{
					ReleaseDC(hDrawWnd, hDC);
					DeleteObject(myBrush);
					xZ = -1000;
					break;
				}
				SelectObject(hDC, myBrush);
				FillRect(hDC, &rect, myBrush);
				StretchBlt(hDC, 0, 0, rect.right, rect.bottom, hMemDC, 0 + xZ, 0 + yZ, (int)rect.right*m, (int)rect.bottom*m, SRCCOPY);
			}
			break;
		}
		else if (GET_KEYSTATE_WPARAM(wParam) == MK_SHIFT)
		{
			if (delta == 120)
			{
				yZ = yZ + 30;
				if (yZ >= 500)
				{
					ReleaseDC(hDrawWnd, hDC);
					DeleteObject(myBrush);
					yZ = 600;
					break;
				}
				SelectObject(hDC, myBrush);
				FillRect(hDC, &rect, myBrush);
				StretchBlt(hDC, 0, 0, rect.right, rect.bottom, hMemDC, 0 + xZ, 0 + yZ, (int)rect.right*m, (int)rect.bottom*m, SRCCOPY);
			}
			else
			{
				yZ = yZ - 30;
				if (yZ <= -1000)
				{
					ReleaseDC(hDrawWnd, hDC);
					DeleteObject(myBrush);
					yZ = -1000;
					break;
				}
				SelectObject(hDC, myBrush);
				FillRect(hDC, &rect, myBrush);
				StretchBlt(hDC, 0, 0, rect.right, rect.bottom, hMemDC, 0 + xZ, 0 + yZ, (int)rect.right*m, (int)rect.bottom*m, SRCCOPY);
			}
			break;
		}
		else if (delta == 120)
		{
			m = m - 0.15;
			if (m <= 0.1)
			{
				ReleaseDC(hDrawWnd, hDC);
				DeleteObject(myBrush);
				m = 0.15;
				break;
			}
			SelectObject(hDC, myBrush);
			FillRect(hDC, &rect, myBrush);
			StretchBlt(hDC, 0 + xZ, 0 + yZ, rect.right, rect.bottom, hMemDC, 0 + xZ, 0 + yZ, (int)rect.right*m, (int)rect.bottom*m, SRCCOPY);
		}
		else
		{
			m = m + 0.15;
			if (m >= 4)
			{
				ReleaseDC(hDrawWnd, hDC);
				DeleteObject(myBrush);
				m = 3.75;
				break;
			}
			SelectObject(hDC, myBrush);
			FillRect(hDC, &rect, myBrush);
			StretchBlt(hDC, 0 + xZ, 0 + yZ, rect.right, rect.bottom, hMemDC, 0 + xZ, 0 + yZ, (int)rect.right*m, (int)rect.bottom*m, SRCCOPY);
		}
		ReleaseDC(hDrawWnd, hDC);

		break;

=======
>>>>>>> parent of 8b04d33... Lab 1
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
<<<<<<< HEAD
				myPen = CreatePen(PS_SOLID, penWidth, rgb);
				logbrush.lbColor = fillRGB;
				//logbrush.lbStyle = BS_HOLLOW;
				myBrush = CreateBrushIndirect(&logbrush);
				hDC = GetDC(hDrawWnd);
				GetClientRect(hDrawWnd, &rect);
				BitBlt(hDC, 0, 0, rect.right, rect.bottom, hMemDC, 0, 0, SRCCOPY);
				SelectObject(hDC, myBrush);
				SelectObject(hDC, myPen);
				Rectangle(hDC, x1, y1, x2, y2);
				ReleaseDC(hDrawWnd, hDC);
				DeleteObject(myPen);
				DeleteObject(myBrush);
				break;
=======
				hDC = GetDC(hDrawWnd);
				BitBlt(hDC,rect.left, rect.top, rect.right, rect.bottom, hMemDC,rect.left, rect.top, SRCCOPY);
				MoveToEx(hDC, x, y, NULL);
				LineTo(hDC, x2, y2);
				ReleaseDC(hDrawWnd, hDC);*/
>>>>>>> parent of 8b04d33... Lab 1

				x2 = LOWORD(lParam);
				y2 = HIWORD(lParam);
<<<<<<< HEAD
				myPen = CreatePen(PS_SOLID, penWidth, rgb);
				logbrush.lbColor = fillRGB;
				//logbrush.lbStyle = BS_HOLLOW;
				myBrush = CreateBrushIndirect(&logbrush);
				hDC = GetDC(hDrawWnd);
				GetClientRect(hDrawWnd, &rect);
				BitBlt(hDC, 0, 0, rect.right, rect.bottom, hMemDC, 0, 0, SRCCOPY);
				SelectObject(hDC, myBrush);
				SelectObject(hDC, myPen);
				Ellipse(hDC, x1, y1, x2, y2);
				ReleaseDC(hDrawWnd, hDC);
				DeleteObject(myPen);
				DeleteObject(myBrush);
				break;	
			}
		}
		else
		{
			switch (operation)
			{
			case ID_POLYLINE:
				if (flag == false)
					break;
				x2 = LOWORD(lParam);
				y2 = HIWORD(lParam);

				myPen = CreatePen(PS_SOLID, penWidth, rgb);
				hDC = GetDC(hDrawWnd);
				GetClientRect(hDrawWnd, &rect);
				BitBlt(hDC, 0, 0, rect.right, rect.bottom, hMemDC, 0, 0, SRCCOPY);
				SelectObject(hDC, myPen);
				MoveToEx(hDC, x1, y1, NULL);
=======
				MyPen = CreatePen(PS_SOLID, width, RGB(r, g, b));
				hDC = GetDC(hDrawWnd);
				GetClientRect(hDrawWnd, &rect);
				BitBlt(hDC, 0, 0, rect.right - rect.left, rect.bottom - rect.top, hMemDC, 0, 0, SRCCOPY);
				MoveToEx(hDC, x, y, NULL);
>>>>>>> parent of 8b04d33... Lab 1
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
