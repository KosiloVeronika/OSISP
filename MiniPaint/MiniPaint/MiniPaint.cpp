// MiniPaint.cpp: определяет точку входа для приложения.
//

#include "stdafx.h"
#include "MiniPaint.h"
#include <commdlg.h>

#define MAX_LOADSTRING 100
// Идентификатор ресурса шаблона меню
#define IDM_MYMENURESOURCE 3
//#define PENCIL 8
//#define CREATE 9
//#define OPEN 10
//#define BLACK 13
//#define LINE 14
//#define RECTANGLE 15
//#define ELLIPSE 16
//#define CIRCLE 17




// Глобальные переменные:
HINSTANCE hInst;								// текущий экземпляр
TCHAR szTitle[MAX_LOADSTRING];					// Текст строки заголовка
TCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна
HWND hWnd;
HWND hDrawWnd;
HWND hText;
HDC hDC;
HDC hMemDC;
HDC hFile;
HBITMAP hBit;
RECT rect;
HPEN myPen;
HBRUSH myBrush;

int operation = ID_PENCIL;
int x0, y0, x1, y1, x2, y2, xZ, yZ;
double m;
int penWidth = 1;
DWORD rgb;

bool flag = false;
bool zoomOrPan = false;
bool saveFirstTime = true;
TCHAR File[MAX_PATH];

// Отправить объявления функций, включенных в этот модуль кода:
ATOM				MyRegisterClass(HINSTANCE hInstance);
ATOM				MyRegisterDrawClass(HINSTANCE hInstance);
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

	// Выполнить инициализацию приложения:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}
	ACCEL accKey[5];
	accKey[0].cmd = ID_CREATE;
	accKey[0].fVirt = FVIRTKEY | FCONTROL;
	accKey[0].key = 0x4E;
	accKey[1].cmd = IDM_EXIT;
	accKey[1].fVirt = FVIRTKEY | FCONTROL;
	accKey[1].key = VK_F4;
	accKey[2].cmd = ID_OPEN;
	accKey[2].fVirt = FVIRTKEY | FCONTROL;
	accKey[2].key = 0x4F;
	accKey[3].cmd = ID_PRINT;
	accKey[3].fVirt = FVIRTKEY | FCONTROL;
	accKey[3].key = 0x50;
	accKey[4].cmd = ID_SAVE;
	accKey[4].fVirt = FVIRTKEY | FCONTROL;
	accKey[4].key = 0x53;
	hAccelTable = CreateAcceleratorTable((LPACCEL)accKey, 5);
	MyRegisterDrawClass(hInst);

	//hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MINIPAINT));

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

	wcex.style			= CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDC_icon));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_MINIPAINT);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

ATOM MyRegisterDrawClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wcex.lpfnWndProc	= ChildProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= NULL;
	wcex.hCursor		= LoadCursor(NULL, IDC_CROSS);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= L"ChildWnd";
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
	HENHMETAFILE h;
	TCHAR szFile[MAX_PATH];

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Разобрать выбор в меню:
		switch (wmId)
		{
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
					1, 1, 1000, 500, hWnd, NULL, hInst, NULL);

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
				1, 1, 1000, 500, hWnd, NULL, hInst, NULL);

			hDC = GetDC(hDrawWnd);
			hMemDC = CreateCompatibleDC(hDC);
			GetClientRect(hDrawWnd, &rect);
			hBit = CreateCompatibleBitmap(hDC, rect.right, rect.bottom);
			myBrush = CreateSolidBrush(RGB(255, 255, 255));
			SelectObject(hMemDC, hBit);
			FillRect(hMemDC, &rect, myBrush);
			DeleteObject(myBrush);
			ReleaseDC(hDrawWnd, hDC);
			ShowWindow(hDrawWnd, SW_SHOW);
			UpdateWindow(hDrawWnd);
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
		case ID_ONE:
			if (hText != NULL)
			{
				SetFocus(NULL);
				DestroyWindow(hText);
			}
			flag = false;
			penWidth = 1;
			break;
		case ID_TWO:
			if (hText != NULL)
			{
				SetFocus(NULL);
				DestroyWindow(hText);
			}
			flag = false;
			penWidth = 2;
			break;
		case ID_THREE:
			if (hText != NULL)
			{
				SetFocus(NULL);
				DestroyWindow(hText);
			}
			flag = false;
			penWidth = 3;
			break;
		case ID_FOUR:
			if (hText != NULL)
			{
				SetFocus(NULL);
				DestroyWindow(hText);
			}
			flag = false;
			penWidth = 4;
			break;
		case ID_FIVE:
			if (hText != NULL)
			{
				SetFocus(NULL);
				DestroyWindow(hText);
			}
			flag = false;
			penWidth = 5;
			break;
		case ID_SIX:
			if (hText != NULL)
			{
				SetFocus(NULL);
				DestroyWindow(hText);
			}
			flag = false;
			penWidth = 6;
			break;
		case ID_SEVEN:
			if (hText != NULL)
			{
				SetFocus(NULL);
				DestroyWindow(hText);
			}
			flag = false;
			penWidth = 7;
			break;
		case ID_EIGHT:
			if (hText != NULL)
			{
				SetFocus(NULL);
				DestroyWindow(hText);
			}
			flag = false;
			penWidth = 8;
			break;
		case ID_NINE:
			if (hText != NULL)
			{
				SetFocus(NULL);
				DestroyWindow(hText);
			}
			flag = false;
			penWidth = 9;
			break;
		case ID_TEN:
			if (hText != NULL)
			{
				SetFocus(NULL);
				DestroyWindow(hText);
			}
			flag = false;
			penWidth = 10;
			break;


		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);
		// TODO: добавьте любой код отрисовки...
		EndPaint(hWnd, &ps);
		break;

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
				hDC = GetDC(hDrawWnd);
				GetClientRect(hDrawWnd, &rect);
				BitBlt(hDC, 0, 0, rect.right, rect.bottom, hMemDC, 0, 0, SRCCOPY);
				SelectObject(hDC, GetStockObject(NULL_BRUSH));
				SelectObject(hDC, myPen);
				Rectangle(hDC, x1, y1, x2, y2);
				ReleaseDC(hDrawWnd, hDC);
				DeleteObject(myPen);
				break;

			case ID_ELLIPSE:
				x2 = LOWORD(lParam);
				y2 = HIWORD(lParam);
				myPen = CreatePen(PS_SOLID, penWidth, rgb);
				hDC = GetDC(hDrawWnd);
				GetClientRect(hDrawWnd, &rect);
				BitBlt(hDC, 0, 0, rect.right, rect.bottom, hMemDC, 0, 0, SRCCOPY);
				SelectObject(hDC, GetStockObject(NULL_BRUSH));
				SelectObject(hDC, myPen);
				Ellipse(hDC, x1, y1, x2, y2);
				ReleaseDC(hDrawWnd, hDC);
				DeleteObject(myPen);
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


LRESULT CALLBACK ChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;

	switch (message)
	{
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Разобрать выбор в меню:
		switch (wmId)
		{
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);

		hDC = GetDC(hDrawWnd);
		GetClientRect(hDrawWnd, &rect);
		myBrush = CreateSolidBrush(RGB(255, 255, 255));
		SelectObject(hDC, myBrush);
		FillRect(hDC, &rect, myBrush);
		StretchBlt(hDC, 0 + xZ, 0 + yZ, rect.right, rect.bottom, hMemDC, 0 + xZ, 0 + yZ, (int)rect.right*m, (int)rect.bottom*m, SRCCOPY);
		ReleaseDC(hDrawWnd, hDC);
		// TODO: добавьте любой код отрисовки...
		EndPaint(hWnd, &ps);
		break;

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
				hDC = GetDC(hDrawWnd);
				GetClientRect(hDrawWnd, &rect);
				SelectObject(hMemDC, myPen);
				MoveToEx(hMemDC, x1, y1, NULL);
				LineTo(hMemDC, x2, y2);

				BitBlt(hDC, 0, 0, rect.right, rect.bottom, hMemDC, 0, 0, SRCCOPY);
				ReleaseDC(hDrawWnd, hDC);
				DeleteObject(myPen);


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
				hDC = GetDC(hDrawWnd);
				GetClientRect(hDrawWnd, &rect);
				BitBlt(hDC, 0, 0, rect.right, rect.bottom, hMemDC, 0, 0, SRCCOPY);
				SelectObject(hDC, GetStockObject(NULL_BRUSH));
				SelectObject(hDC, myPen);
				Rectangle(hDC, x1, y1, x2, y2);
				ReleaseDC(hDrawWnd, hDC);
				DeleteObject(myPen);
				break;

			case ID_ELLIPSE:
				x2 = LOWORD(lParam);
				y2 = HIWORD(lParam);
				myPen = CreatePen(PS_SOLID, penWidth, rgb);
				hDC = GetDC(hDrawWnd);
				GetClientRect(hDrawWnd, &rect);
				BitBlt(hDC, 0, 0, rect.right, rect.bottom, hMemDC, 0, 0, SRCCOPY);
				SelectObject(hDC, GetStockObject(NULL_BRUSH));
				SelectObject(hDC, myPen);
				Ellipse(hDC, x1, y1, x2, y2);
				ReleaseDC(hDrawWnd, hDC);
				DeleteObject(myPen);
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
		if (zoomOrPan)
		{
			hDC = GetDC(hDrawWnd);
			GetClientRect(hDrawWnd, &rect);
			BitBlt(hDC, 0, 0, rect.right, rect.bottom, hMemDC, 0, 0, SRCCOPY);
			ReleaseDC(hDrawWnd, hDC);
			xZ = 0;
			yZ = 0;
			m = 1;
			zoomOrPan = false;
		}
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
		if (operation == ID_TEXT)
		{
			if (hText == NULL)
				break;
			TCHAR s[255] = L"";
			RECT r;
			GetClientRect(hText, &r);
			GetWindowText(hText, s, 255);
			hDC = GetDC(hDrawWnd);
			SetFocus(NULL);
			DestroyWindow(hText);
			hText = NULL;
			r.left = x1;
			r.top = y1;
			r.bottom = r.top + 20;
			r.right = r.left + 200;
			SetBkMode(hMemDC, TRANSPARENT);
			DrawText(hMemDC, s, 255, &r, DT_LEFT | DT_TOP);
			BitBlt(hDC, 0, 0, rect.right, rect.bottom, hMemDC, 0, 0, SRCCOPY);
			ReleaseDC(hDrawWnd, hDC);
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

			hText = CreateWindow(L"Edit", NULL, WS_CHILD | WS_BORDER | WS_VISIBLE | ES_LEFT , x1, y1, 200, 20, hDrawWnd, NULL, hInst, NULL);
			SetFocus(hText);
		}
		else
		{
			hDC = GetDC(hDrawWnd);
			GetClientRect(hDrawWnd, &rect);
			BitBlt(hMemDC, 0, 0, rect.right, rect.bottom, hDC, 0, 0, SRCCOPY);
		}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
