// HexCalc.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "HexCalc.h"

#define MAX_LOADSTRING 100

// グローバル変数:
HINSTANCE hInst;                                // 現在のインターフェイス
WCHAR szTitle[MAX_LOADSTRING];                  // タイトル バーのテキスト
WCHAR szWindowClass[MAX_LOADSTRING];            // メイン ウィンドウ クラス名

// このコード モジュールに含まれる関数の宣言を転送します:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: ここにコードを挿入してください。

    // グローバル文字列を初期化する
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_HEXCALC, szWindowClass, MAX_LOADSTRING);
	ATOM asdf = MyRegisterClass(hInstance);

    // アプリケーション初期化の実行:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_HEXCALC));

    MSG msg;

    // メイン メッセージ ループ:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}

void ShowNumber(HWND hwnd, UINT iNumber)
{
	TCHAR szBuffer[20];
	wsprintf(szBuffer, TEXT("%X"), iNumber);
	SetDlgItemText(hwnd, VK_ESCAPE, szBuffer);
}
DWORD CalcIt(UINT iFirstNum, int iOperation, UINT iNum)
{
	switch (iOperation)
	{
	case '=': return iNum;
	case '+': return iFirstNum + iNum;
	case '-': return iFirstNum - iNum;
	case '*': return iFirstNum * iNum;
	case '&': return iFirstNum & iNum;
	case '|': return iFirstNum | iNum;
	case '^': return iFirstNum ^ iNum;
	case '<': return iFirstNum << iNum;
	case '>': return iFirstNum >> iNum;
	case '/': return iNum ? iFirstNum / iNum : MAXDWORD;
	case '%': return iNum ? iFirstNum % iNum : MAXDWORD;
	default: return 0;
	}
}

//
//  関数: MyRegisterClass()
//
//  目的: ウィンドウ クラスを登録します。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = DLGWINDOWEXTRA;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_HEXCALC));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_BTNFACE + 1);
    wcex.lpszMenuName   = NULL;
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   関数: InitInstance(HINSTANCE, int)
//
//   目的: インスタンス ハンドルを保存して、メイン ウィンドウを作成します
//
//   コメント:
//
//        この関数で、グローバル変数でインスタンス ハンドルを保存し、
//        メイン プログラム ウィンドウを作成および表示します。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // グローバル変数にインスタンス ハンドルを格納する

   HWND hWnd = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_HEXCALC), 0, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  関数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的: メイン ウィンドウのメッセージを処理します。
//
//  WM_COMMAND  - アプリケーション メニューの処理
//  WM_PAINT    - メイン ウィンドウを描画する
//  WM_DESTROY  - 中止メッセージを表示して戻る
//
//
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static BOOL bNewNumber = TRUE;
	static int iOperation = '=';
	static UINT iNumber, iFirstNum;
	HWND hButton;

    switch (message)
    {
	case WM_KEYDOWN: // left arrow --> backspace
		if (wParam != VK_LEFT)
			break;
		wParam = VK_BACK;
		// fall through
	case WM_CHAR:
		if ((wParam = (WPARAM)CharUpper((TCHAR *)wParam)) ==
			VK_RETURN)
			wParam = '=';

		if (hButton = GetDlgItem(hwnd, wParam))
		{
			SendMessage(hButton, BM_SETSTATE, 1, 0);
			Sleep(100);
			SendMessage(hButton, BM_SETSTATE, 0, 0);
		}
		else
		{
			MessageBeep(0);
			break;
		}
		// fall through
	case WM_COMMAND:
		SetFocus(hwnd);

		if (LOWORD(wParam) == VK_BACK) //backspace
			ShowNumber(hwnd, iNumber /= 16);

		else if (LOWORD(wParam) == VK_ESCAPE) // escape
			ShowNumber(hwnd, iNumber = 0);

		else if (isxdigit(LOWORD(wParam))) // hex digit
		{
			if (bNewNumber)
			{
				iFirstNum = iNumber;
				iNumber = 0;
			}
			bNewNumber = FALSE;
			if (iNumber <= MAXDWORD >> 4)
				ShowNumber(hwnd, iNumber = 16 * iNumber + wParam - (isdigit(wParam) ? '0' : 'A' - 10));
			else
				MessageBeep(0);
		}
		else // operation
		{
			if (!bNewNumber)
				ShowNumber(hwnd, iNumber = CalcIt(iFirstNum, iOperation, iNumber));
			bNewNumber = TRUE;
			iOperation = LOWORD(wParam);
		}
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}
