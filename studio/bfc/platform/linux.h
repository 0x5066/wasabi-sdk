/*

  Nullsoft WASABI Source File License

  Copyright 1999-2001 Nullsoft, Inc.

    This software is provided 'as-is', without any express or implied
    warranty.  In no event will the authors be held liable for any damages
    arising from the use of this software.

    Permission is granted to anyone to use this software for any purpose,
    including commercial applications, and to alter it and redistribute it
    freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not
       claim that you wrote the original software. If you use this software
       in a product, an acknowledgment in the product documentation would be
       appreciated but is not required.
    2. Altered source versions must be plainly marked as such, and must not be
       misrepresented as being the original software.
    3. This notice may not be removed or altered from any source distribution.


  Brennan Underwood
  brennan@nullsoft.com

*/

#ifndef _LINUX_H_WASABI
#define _LINUX_H_WASABI

#ifndef LINUX
#error this file is only for LINUX
#endif

#ifndef _PLATFORM_H
#error this file should only be included from platform.h
#endif

#ifdef __cplusplus
extern "C" {
#endif

int wsprintf(LPSTR lpOut, LPCSTR lpFmt, ...);
void OutputDebugString(char *str);
HGLOBAL GlobalAlloc(UINT uFlags, DWORD dwBytes);
HGLOBAL GlobalFree(HGLOBAL hMem);
HGLOBAL GlobalReAlloc(HGLOBAL hMem, DWORD dwBytes, UINT uFlags);

WINBASEAPI DWORD WINAPI GetModuleFileName ( HMODULE hModule, LPSTR lpFilename, DWORD nSize );
void _splitpath (const char *, char *, char *, char *, char *);
void _makepath (char *, const char *, const char *, const char *, const char *);
WINBASEAPI BOOL WINAPI SetCurrentDirectory ( LPCSTR lpPathName );
WINUSERAPI int WINAPI LoadString ( HINSTANCE hInstance, UINT uID, LPSTR lpBuffer, int nBufferMax);
WINBASEAPI DWORD WINAPI GetCurrentThreadId ( );
WINUSERAPI UINT WINAPI RegisterWindowMessage ( LPCSTR lpString);
WINOLEAPI void OleInitialize (LPVOID pvReserved);
WINUSERAPI BOOL WINAPI UpdateWindow ( HWND hWnd);
WINUSERAPI HACCEL WINAPI LoadAccelerators ( HINSTANCE hInstance, LPCSTR lpTableName);
WINUSERAPI HWND WINAPI SetFocus ( HWND hWnd);
WINUSERAPI BOOL WINAPI GetMessage ( LPMSG lpMsg, HWND hWnd , UINT wMsgFilterMin, UINT wMsgFilterMax);
WINUSERAPI int WINAPI TranslateAccelerator ( HWND hWnd, HACCEL hAccTable, LPMSG lpMsg);
WINUSERAPI BOOL WINAPI TranslateMessage ( CONST MSG *lpMsg);
WINUSERAPI LONG WINAPI DispatchMessage ( CONST MSG *lpMsg);
#define CreateWindow CreateWindowA
#define CreateWindowEx CreateWindowExA
#define CreateWindowA(lpClassName, lpWindowName, dwStyle, x, y,\
nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam)\
CreateWindowExA(0L, lpClassName, lpWindowName, dwStyle, x, y,\
nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam)
WINUSERAPI HWND WINAPI CreateWindowExA(
    DWORD dwExStyle,
    LPCSTR lpClassName,
    LPCSTR lpWindowName,
    DWORD dwStyle,
    int X,
    int Y,
    int nWidth,
    int nHeight,
    HWND hWndParent ,
    HMENU hMenu,
    HINSTANCE hInstance,
    LPVOID lpParam);
WINUSERAPI BOOL WINAPI SetWindowPos ( HWND hWnd, HWND hWndInsertAfter , int X, int Y, int cx, int cy, UINT uFlags);
WINUSERAPI DWORD WINAPI GetClassLong ( HWND hWnd, int nIndex);
WINUSERAPI int WINAPI GetWindowText ( HWND hWnd, LPSTR lpString, int nMaxCount);
WINUSERAPI LRESULT WINAPI SendMessage ( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
WINUSERAPI BOOL WINAPI GetClientRect ( HWND hWnd, LPRECT lpRect);
WINUSERAPI BOOL WINAPI InvalidateRect ( HWND hWnd , CONST RECT *lpRect, BOOL bErase);
WINUSERAPI BOOL WINAPI EnumChildWindows ( HWND hWndParent, WNDENUMPROC lpEnumFunc, LPARAM lParam);
WINUSERAPI BOOL WINAPI ShowWindow ( HWND hWnd, int nCmdShow);
WINUSERAPI BOOL WINAPI GetWindowRect ( HWND hWnd, LPRECT lpRect);
WINUSERAPI BOOL WINAPI EnableWindow ( HWND hWnd, BOOL bEnable);
WINUSERAPI VOID WINAPI PostQuitMessage ( int nExitCode);
WINBASEAPI HMODULE WINAPI LoadLibrary ( LPCSTR lpLibFileName );
WINBASEAPI FARPROC WINAPI GetProcAddress ( HMODULE hModule, LPCSTR lpProcName );
WINUSERAPI int WINAPI MessageBox ( HWND hWnd , LPCSTR lpText, LPCSTR lpCaption, UINT uType);
WINBASEAPI VOID WINAPI ExitProcess ( UINT uExitCode );
WINBASEAPI BOOL WINAPI FreeLibrary ( HMODULE hLibModule );

WINBASEAPI BOOL WINAPI WritePrivateProfileString ( LPCSTR lpAppName, LPCSTR lpKeyName, LPCSTR lpString, LPCSTR lpFileName );
WINBASEAPI BOOL WINAPI WritePrivateProfileStruct ( LPCSTR lpszSection, LPCSTR lpszKey, LPVOID lpStruct, UINT uSizeStruct, LPCSTR szFile );
WINBASEAPI DWORD WINAPI GetPrivateProfileString ( LPCSTR lpAppName, LPCSTR lpKeyName, LPCSTR lpDefault, LPSTR lpReturnedString, DWORD nSize, LPCSTR lpFileName );
WINBASEAPI UINT WINAPI GetPrivateProfileInt ( LPCSTR lpAppName, LPCSTR lpKeyName, INT nDefault, LPCSTR lpFileName );
WINBASEAPI BOOL WINAPI GetPrivateProfileStruct ( LPCSTR lpszSection, LPCSTR lpszKey, LPVOID lpStruct, UINT uSizeStruct, LPCSTR szFile );

WINUSERAPI SHORT WINAPI GetKeyState ( int nVirtKey);
WINBASEAPI HRSRC WINAPI FindResource ( HMODULE hModule, LPCSTR lpName, LPCSTR lpType );
WINBASEAPI HGLOBAL WINAPI LoadResource ( HMODULE hModule, HRSRC hResInfo );
WINBASEAPI LPVOID WINAPI LockResource ( HGLOBAL hResData );
WINBASEAPI BOOL WINAPI FreeResource ( HGLOBAL hResData );
WINBASEAPI DWORD WINAPI GetTempPath ( DWORD nBufferLength, LPSTR lpBuffer );
WINBASEAPI DWORD WINAPI SizeofResource ( HMODULE hModule, HRSRC hResInfo );

WINBASEAPI VOID WINAPI Sleep ( DWORD dwMilliseconds );
WINBASEAPI DWORD WINAPI GetTickCount ( );
WINUSERAPI BOOL WINAPI SystemParametersInfo ( UINT uiAction, UINT uiParam, PVOID pvParam, UINT fWinIni);
WINUSERAPI UINT WINAPI GetDoubleClickTime ();
WINUSERAPI int WINAPI GetSystemMetrics ( int nIndex);
WINUSERAPI BOOL WINAPI SetWindowText ( HWND hWnd, LPCSTR lpString);
WINUSERAPI BOOL WINAPI PtInRect ( CONST RECT *lprc, POINT pt);
WINUSERAPI HWND WINAPI GetCapture ();
WINUSERAPI BOOL WINAPI ReleaseCapture ();
WINUSERAPI BOOL WINAPI DestroyWindow ( HWND hWnd);
WINUSERAPI UINT WINAPI SetTimer ( HWND hWnd , UINT nIDEvent, UINT uElapse, TIMERPROC lpTimerFunc);
WINUSERAPI BOOL WINAPI KillTimer ( HWND hWnd, UINT uIDEvent);
WINUSERAPI BOOL WINAPI InvalidateRgn ( HWND hWnd, HRGN hRgn, BOOL bErase);
WINUSERAPI BOOL WINAPI ValidateRect ( HWND hWnd , CONST RECT *lpRect);
WINUSERAPI BOOL WINAPI ClientToScreen ( HWND hWnd, LPPOINT lpPoint);
WINUSERAPI BOOL WINAPI ScreenToClient ( HWND hWnd, LPPOINT lpPoint);
WINUSERAPI HWND WINAPI SetParent ( HWND hWndChild, HWND hWndNewParent);
WINUSERAPI BOOL WINAPI PostMessage ( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
WINUSERAPI BOOL WINAPI GetUpdateRect ( HWND hWnd, LPRECT lpRect, BOOL bErase);
WINUSERAPI BOOL WINAPI IsWindowVisible ( HWND hWnd);
WINUSERAPI int WINAPI GetUpdateRgn ( HWND hWnd, HRGN hRgn, BOOL bErase);
WINUSERAPI BOOL WINAPI DrawFocusRect ( HDC hDC, CONST RECT * lprc);
WINUSERAPI BOOL WINAPI SetRect ( LPRECT lprc, int xLeft, int yTop, int xRight, int yBottom);
WINUSERAPI BOOL WINAPI SubtractRect ( LPRECT lprcDst, CONST RECT *lprcSrc1, CONST RECT *lprcSrc2);

WINBASEAPI VOID WINAPI InitializeCriticalSection ( LPCRITICAL_SECTION lpCriticalSection );
WINBASEAPI VOID WINAPI DeleteCriticalSection ( LPCRITICAL_SECTION lpCriticalSection );
WINBASEAPI VOID WINAPI EnterCriticalSection ( LPCRITICAL_SECTION lpCriticalSection );
WINBASEAPI VOID WINAPI LeaveCriticalSection ( LPCRITICAL_SECTION lpCriticalSection );

#define UnlockResource(hResData) ((hResData), 0)
WINUSERAPI HANDLE WINAPI LoadImage ( HINSTANCE hinst, LPCSTR lpszName, UINT uType, int cxDesired, int cyDesired, UINT fuLoad);
WINGDIAPI HDC WINAPI CreateCompatibleDC (HDC hdc);
WINGDIAPI HBITMAP WINAPI CreateDIBSection (HDC hdc, CONST BITMAPINFO *pbmi, UINT iUsage, VOID **ppvBits, HANDLE hSection, DWORD dwOffset);
WINGDIAPI HGDIOBJ WINAPI SelectObject (HDC hdc, HGDIOBJ hgdiobj);
WINGDIAPI int WINAPI SetDIBits (HDC hdc, HBITMAP hbmp, UINT uStartScan, UINT cScanLines, CONST VOID *lpvBits, CONST BITMAPINFO *lpbmi, UINT fuColorUse);
WINGDIAPI BOOL WINAPI DeleteDC (HDC hdc);
WINGDIAPI BOOL WINAPI DeleteObject (HGDIOBJ hObject);
WINGDIAPI int WINAPI GetObject (HGDIOBJ, int, LPVOID);
WINGDIAPI BOOL WINAPI GdiFlush ();
WINGDIAPI int WINAPI GetDIBits (HDC hdc, HBITMAP hbmp, UINT uStartScan, UINT cScanLines, LPVOID lpvBits, LPBITMAPINFO lpbi, UINT uUsage);

#define ZeroMemory RtlZeroMemory
#define RGB(r,g,b) ((COLORREF)(((BYTE)(r)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(b))<<16)))
#define min(a,b) (((a) < (b)) ? (a) : (b))
#define max(a,b) (((a) > (b)) ? (a) : (b))

#ifdef __cplusplus
};
#endif

#endif
