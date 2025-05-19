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

#include <stdio.h>
#include <stdarg.h>

#include "platform.h"

#define DEB printf

int wsprintf(LPSTR lpOut, LPCSTR lpFmt, ...)
{
  va_list args;
  int ret;
  va_start (args, lpFmt);
  ret=vsprintf(lpOut,lpFmt,args);
  va_end (args);
  return ret;
}

void OutputDebugString(char *str)
{
  printf("%s\n",str);
}

#undef malloc
#undef free
#undef realloc
HGLOBAL GlobalAlloc(UINT uFlags, DWORD dwBytes)
{
  return((HGLOBAL)malloc(dwBytes));
}

HGLOBAL GlobalFree(HGLOBAL hMem)
{
  free(hMem);
  return 0;
}

HGLOBAL GlobalReAlloc(HGLOBAL hMem, DWORD dwBytes, UINT uFlags)
{
  return((HGLOBAL)realloc(hMem,dwBytes));
}

WINBASEAPI DWORD WINAPI GetModuleFileName ( HMODULE hModule, LPSTR lpFilename, DWORD nSize )
{
  DEB("WINBASEAPI DWORD WINAPI GetModuleFileName ( HMODULE hModule, LPSTR lpFilename, DWORD nSize )\n");
  return 0;
}

void _splitpath (const char *path, char *drive, char *dir, char *fname, char *ext)
{
  DEB("void _splitpath (const char *, char *, char *, char *, char *)\n");
  return;
}
void _makepath (char *path, const char *drive, const char *dir, const char *fname, const char *ext)
{
  DEB("void _makepath (char *, const char *, const char *, const char *, const char *)\n");
  return;
}
WINBASEAPI BOOL WINAPI SetCurrentDirectory ( LPCSTR lpPathName )
{
  DEB("WINBASEAPI BOOL WINAPI SetCurrentDirectory ( LPCSTR lpPathName )\n");
  return 0;
}
WINUSERAPI int WINAPI LoadString ( HINSTANCE hInstance, UINT uID, LPSTR lpBuffer, int nBufferMax)
{
  DEB("WINUSERAPI int WINAPI LoadString ( HINSTANCE hInstance, UINT uID, LPSTR lpBuffer, int nBufferMax)\n");
  return 0;
}
WINBASEAPI DWORD WINAPI GetCurrentThreadId ( VOID )
{
  DEB("WINBASEAPI DWORD WINAPI GetCurrentThreadId ( VOID )\n");
  return 0;
}
WINUSERAPI UINT WINAPI RegisterWindowMessage ( LPCSTR lpString)
{
  DEB("WINUSERAPI UINT WINAPI RegisterWindowMessage ( LPCSTR lpString)\n");
  return 0;
}
WINOLEAPI VOID OleInitialize (LPVOID pvReserved)
{
  DEB("WINOLEAPI OleInitialize (LPVOID pvReserved)\n");
  return;
}
WINUSERAPI BOOL WINAPI UpdateWindow ( HWND hWnd)
{
  DEB("WINUSERAPI BOOL WINAPI UpdateWindow ( HWND hWnd)\n");
  return 0;
}
WINUSERAPI HACCEL WINAPI LoadAccelerators ( HINSTANCE hInstance, LPCSTR lpTableName)
{
  DEB("WINUSERAPI HACCEL WINAPI LoadAccelerators ( HINSTANCE hInstance, LPCSTR lpTableName)\n");
  return 0;
}
WINUSERAPI HWND WINAPI SetFocus ( HWND hWnd)
{
  DEB("WINUSERAPI HWND WINAPI SetFocus ( HWND hWnd)\n");
  return 0;
}
WINUSERAPI BOOL WINAPI GetMessage ( LPMSG lpMsg, HWND hWnd , UINT wMsgFilterMin, UINT wMsgFilterMax)
{
  DEB("WINUSERAPI BOOL WINAPI GetMessage ( LPMSG lpMsg, HWND hWnd , UINT wMsgFilterMin, UINT wMsgFilterMax)\n");
  return 0;
}
WINUSERAPI int WINAPI TranslateAccelerator ( HWND hWnd, HACCEL hAccTable, LPMSG lpMsg)
{
  DEB("WINUSERAPI int WINAPI TranslateAccelerator ( HWND hWnd, HACCEL hAccTable, LPMSG lpMsg)\n");
  return 0;
}
WINUSERAPI BOOL WINAPI TranslateMessage ( CONST MSG *lpMsg)
{
  DEB("WINUSERAPI BOOL WINAPI TranslateMessage ( CONST MSG *lpMsg)\n");
  return 0;
}
WINUSERAPI LONG WINAPI DispatchMessage ( CONST MSG *lpMsg)
{
  DEB("WINUSERAPI LONG WINAPI DispatchMessage ( CONST MSG *lpMsg)\n");
  return 0;
}
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
    LPVOID lpParam)
{
  DEB("WINUSERAPI HWND WINAPI CreateWindowExA\n");
  return 0;
}
WINUSERAPI BOOL WINAPI SetWindowPos ( HWND hWnd, HWND hWndInsertAfter , int X, int Y, int cx, int cy, UINT uFlags)
{
  DEB("WINUSERAPI BOOL WINAPI SetWindowPos ( HWND hWnd, HWND hWndInsertAfter , int X, int Y, int cx, int cy, UINT uFlags)\n");
  return 0;
}
WINUSERAPI DWORD WINAPI GetClassLong ( HWND hWnd, int nIndex)
{
  DEB("WINUSERAPI DWORD WINAPI GetClassLong ( HWND hWnd, int nIndex)\n");
  return 0;
}
WINUSERAPI int WINAPI GetWindowText ( HWND hWnd, LPSTR lpString, int nMaxCount)
{
  DEB("WINUSERAPI int WINAPI GetWindowText ( HWND hWnd, LPSTR lpString, int nMaxCount)\n");
  return 0;
}
WINUSERAPI LRESULT WINAPI SendMessage ( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
  DEB("WINUSERAPI LRESULT WINAPI SendMessage ( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)\n");
  return 0;
}
WINUSERAPI BOOL WINAPI GetClientRect ( HWND hWnd, LPRECT lpRect)
{
  DEB("WINUSERAPI BOOL WINAPI GetClientRect ( HWND hWnd, LPRECT lpRect)\n");
  return 0;
}
WINUSERAPI BOOL WINAPI InvalidateRect ( HWND hWnd , CONST RECT *lpRect, BOOL bErase)
{
  DEB("WINUSERAPI BOOL WINAPI InvalidateRect ( HWND hWnd , CONST RECT *lpRect, BOOL bErase)\n");
  return 0;
}
WINUSERAPI BOOL WINAPI EnumChildWindows ( HWND hWndParent, WNDENUMPROC lpEnumFunc, LPARAM lParam)
{
  DEB("WINUSERAPI BOOL WINAPI EnumChildWindows ( HWND hWndParent, WNDENUMPROC lpEnumFunc, LPARAM lParam)\n");
  return 0;
}
WINUSERAPI BOOL WINAPI ShowWindow ( HWND hWnd, int nCmdShow)
{
  DEB("WINUSERAPI BOOL WINAPI ShowWindow ( HWND hWnd, int nCmdShow)\n");
  return 0;
}
WINUSERAPI BOOL WINAPI GetWindowRect ( HWND hWnd, LPRECT lpRect)
{
  DEB("WINUSERAPI BOOL WINAPI GetWindowRect ( HWND hWnd, LPRECT lpRect)\n");
  return 0;
}
WINUSERAPI BOOL WINAPI EnableWindow ( HWND hWnd, BOOL bEnable)
{
  DEB("WINUSERAPI BOOL WINAPI EnableWindow ( HWND hWnd, BOOL bEnable)\n");
  return 0;
}
WINUSERAPI VOID WINAPI PostQuitMessage ( int nExitCode)
{
  DEB("WINUSERAPI VOID WINAPI PostQuitMessage ( int nExitCode)\n");
  return;
}
WINBASEAPI HMODULE WINAPI LoadLibrary ( LPCSTR lpLibFileName )
{
  DEB("WINBASEAPI HMODULE WINAPI LoadLibrary ( LPCSTR lpLibFileName )\n");
  return 0;
}
WINBASEAPI FARPROC WINAPI GetProcAddress ( HMODULE hModule, LPCSTR lpProcName )
{
  DEB("WINBASEAPI FARPROC WINAPI GetProcAddress ( HMODULE hModule, LPCSTR lpProcName )\n");
  return 0;
}
WINUSERAPI int WINAPI MessageBox ( HWND hWnd , LPCSTR lpText, LPCSTR lpCaption, UINT uType)
{
  DEB("WINUSERAPI int WINAPI MessageBox ( HWND hWnd , LPCSTR lpText, LPCSTR lpCaption, UINT uType)\n");
  return 0;
}
WINBASEAPI VOID WINAPI ExitProcess ( UINT uExitCode )
{
  DEB("WINBASEAPI VOID WINAPI ExitProcess ( UINT uExitCode )\n");
  exit(uExitCode);
  return;
}
WINBASEAPI BOOL WINAPI FreeLibrary ( HMODULE hLibModule )
{
  DEB("WINBASEAPI BOOL WINAPI FreeLibrary ( HMODULE hLibModule )\n");
  return 0;
}

WINBASEAPI BOOL WINAPI WritePrivateProfileString ( LPCSTR lpAppName, LPCSTR lpKeyName, LPCSTR lpString, LPCSTR lpFileName )
{
  DEB("WINBASEAPI BOOL WINAPI WritePrivateProfileString ( LPCSTR lpAppName, LPCSTR lpKeyName, LPCSTR lpString, LPCSTR lpFileName )\n");
  return 0;
}
WINBASEAPI BOOL WINAPI WritePrivateProfileStruct ( LPCSTR lpszSection, LPCSTR lpszKey, LPVOID lpStruct, UINT uSizeStruct, LPCSTR szFile )
{
  DEB("WINBASEAPI BOOL WINAPI WritePrivateProfileStruct ( LPCSTR lpszSection, LPCSTR lpszKey, LPVOID lpStruct, UINT uSizeStruct, LPCSTR szFile )\n");
  return 0;
}
WINBASEAPI DWORD WINAPI GetPrivateProfileString ( LPCSTR lpAppName, LPCSTR lpKeyName, LPCSTR lpDefault, LPSTR lpReturnedString, DWORD nSize, LPCSTR lpFileName )
{
  DEB("WINBASEAPI DWORD WINAPI GetPrivateProfileString ( LPCSTR lpAppName, LPCSTR lpKeyName, LPCSTR lpDefault, LPSTR lpReturnedString, DWORD nSize, LPCSTR lpFileName )\n");
  return 0;
}
WINBASEAPI UINT WINAPI GetPrivateProfileInt ( LPCSTR lpAppName, LPCSTR lpKeyName, INT nDefault, LPCSTR lpFileName )
{
  DEB("WINBASEAPI UINT WINAPI GetPrivateProfileInt ( LPCSTR lpAppName, LPCSTR lpKeyName, INT nDefault, LPCSTR lpFileName )\n");
  return 0;
}
WINBASEAPI BOOL WINAPI GetPrivateProfileStruct ( LPCSTR lpszSection, LPCSTR lpszKey, LPVOID lpStruct, UINT uSizeStruct, LPCSTR szFile )
{
  DEB("WINBASEAPI BOOL WINAPI GetPrivateProfileStruct ( LPCSTR lpszSection, LPCSTR lpszKey, LPVOID lpStruct, UINT uSizeStruct, LPCSTR szFile )\n");
  return 0;
}
WINUSERAPI SHORT WINAPI GetKeyState ( int nVirtKey)
{
  DEB("WINUSERAPI SHORT WINAPI GetKeyState ( int nVirtKey)\n");
  return 0;
}
WINBASEAPI HRSRC WINAPI FindResource ( HMODULE hModule, LPCSTR lpName, LPCSTR lpType )
{
  DEB("WINBASEAPI HRSRC WINAPI FindResource ( HMODULE hModule, LPCSTR lpName, LPCSTR lpType )\n");
  return 0;
}
WINBASEAPI HGLOBAL WINAPI LoadResource ( HMODULE hModule, HRSRC hResInfo )
{
  DEB("WINBASEAPI HGLOBAL WINAPI LoadResource ( HMODULE hModule, HRSRC hResInfo )\n");
  return 0;
}
WINBASEAPI LPVOID WINAPI LockResource ( HGLOBAL hResData )
{
  DEB("WINBASEAPI LPVOID WINAPI LockResource ( HGLOBAL hResData )\n");
  return 0;
}
WINBASEAPI BOOL WINAPI FreeResource ( HGLOBAL hResData )
{
  DEB("WINBASEAPI BOOL WINAPI FreeResource ( HGLOBAL hResData )\n");
  return 0;
}
WINBASEAPI DWORD WINAPI GetTempPath ( DWORD nBufferLength, LPSTR lpBuffer )
{
  DEB("WINBASEAPI DWORD WINAPI GetTempPath ( DWORD nBufferLength, LPSTR lpBuffer )\n");
  return 0;
}
WINBASEAPI DWORD WINAPI SizeofResource ( HMODULE hModule, HRSRC hResInfo )
{
  DEB("WINBASEAPI DWORD WINAPI SizeofResource ( HMODULE hModule, HRSRC hResInfo )\n");
  return 0;
}

WINBASEAPI VOID WINAPI Sleep ( DWORD dwMilliseconds )
{
  usleep(dwMilliseconds);
  return;
}
WINBASEAPI DWORD WINAPI GetTickCount ( )
{
  DEB("WINBASEAPI DWORD WINAPI GetTickCount ( )\n");
  return 0;
}
WINUSERAPI BOOL WINAPI SystemParametersInfo ( UINT uiAction, UINT uiParam, PVOID pvParam, UINT fWinIni)
{
  DEB("WINUSERAPI BOOL WINAPI SystemParametersInfo ( UINT uiAction, UINT uiParam, PVOID pvParam, UINT fWinIni)\n");
  return 0;
}
WINUSERAPI UINT WINAPI GetDoubleClickTime ()
{
  DEB("WINUSERAPI UINT WINAPI GetDoubleClickTime ()\n");
  return 0;
}
WINUSERAPI int WINAPI GetSystemMetrics ( int nIndex)
{
  DEB("WINUSERAPI int WINAPI GetSystemMetrics ( int nIndex)\n");
  return 0;
}
WINUSERAPI BOOL WINAPI SetWindowText ( HWND hWnd, LPCSTR lpString)
{
  DEB("WINUSERAPI BOOL WINAPI SetWindowText ( HWND hWnd, LPCSTR lpString)\n");
  return 0;
}
WINUSERAPI BOOL WINAPI PtInRect ( CONST RECT *lprc, POINT pt)
{
  DEB("WINUSERAPI BOOL WINAPI PtInRect ( CONST RECT *lprc, POINT pt)\n");
  return 0;
}
WINUSERAPI HWND WINAPI GetCapture ()
{
  DEB("WINUSERAPI HWND WINAPI GetCapture ()\n");
  return 0;
}
WINUSERAPI BOOL WINAPI ReleaseCapture ()
{
  DEB("WINUSERAPI BOOL WINAPI ReleaseCapture ()\n");
  return 0;
}
WINUSERAPI BOOL WINAPI DestroyWindow ( HWND hWnd)
{
  DEB("WINUSERAPI BOOL WINAPI DestroyWindow ( HWND hWnd)\n");
  return 0;
}
WINUSERAPI UINT WINAPI SetTimer ( HWND hWnd , UINT nIDEvent, UINT uElapse, TIMERPROC lpTimerFunc)
{
  DEB("WINUSERAPI UINT WINAPI SetTimer ( HWND hWnd , UINT nIDEvent, UINT uElapse, TIMERPROC lpTimerFunc)\n");
  return 0;
}
WINUSERAPI BOOL WINAPI KillTimer ( HWND hWnd, UINT uIDEvent)
{
  DEB("WINUSERAPI BOOL WINAPI KillTimer ( HWND hWnd, UINT uIDEvent)\n");
  return 0;
}
WINUSERAPI BOOL WINAPI InvalidateRgn ( HWND hWnd, HRGN hRgn, BOOL bErase)
{
  DEB("WINUSERAPI BOOL WINAPI InvalidateRgn ( HWND hWnd, HRGN hRgn, BOOL bErase)\n");
  return 0;
}
WINUSERAPI BOOL WINAPI ValidateRect ( HWND hWnd , CONST RECT *lpRect)
{
  DEB("WINUSERAPI BOOL WINAPI ValidateRect ( HWND hWnd , CONST RECT *lpRect)\n");
  return 0;
}
WINUSERAPI BOOL WINAPI ClientToScreen ( HWND hWnd, LPPOINT lpPoint)
{
  DEB("WINUSERAPI BOOL WINAPI ClientToScreen ( HWND hWnd, LPPOINT lpPoint)\n");
  return 0;
}
WINUSERAPI BOOL WINAPI ScreenToClient ( HWND hWnd, LPPOINT lpPoint)
{
  DEB("WINUSERAPI BOOL WINAPI ScreenToClient ( HWND hWnd, LPPOINT lpPoint)\n");
  return 0;
}
WINUSERAPI HWND WINAPI SetParent ( HWND hWndChild, HWND hWndNewParent)
{
  DEB("WINUSERAPI HWND WINAPI SetParent ( HWND hWndChild, HWND hWndNewParent)\n");
  return 0;
}
WINUSERAPI BOOL WINAPI PostMessage ( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
  DEB("WINUSERAPI BOOL WINAPI PostMessage ( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)\n");
  return 0;
}
WINUSERAPI BOOL WINAPI GetUpdateRect ( HWND hWnd, LPRECT lpRect, BOOL bErase)
{
  DEB("WINUSERAPI BOOL WINAPI GetUpdateRect ( HWND hWnd, LPRECT lpRect, BOOL bErase)\n");
  return 0;
}
WINUSERAPI BOOL WINAPI IsWindowVisible ( HWND hWnd)
{
  DEB("WINUSERAPI BOOL WINAPI IsWindowVisible ( HWND hWnd)\n");
  return 0;
}
WINUSERAPI int WINAPI GetUpdateRgn ( HWND hWnd, HRGN hRgn, BOOL bErase)
{
  DEB("WINUSERAPI int WINAPI GetUpdateRgn ( HWND hWnd, HRGN hRgn, BOOL bErase)\n");
  return 0;
}
WINUSERAPI BOOL WINAPI DrawFocusRect ( HDC hDC, CONST RECT * lprc)
{
  DEB("WINUSERAPI BOOL WINAPI DrawFocusRect ( HDC hDC, CONST RECT * lprc)\n");
  return 0;
}
WINUSERAPI BOOL WINAPI SetRect ( LPRECT lprc, int xLeft, int yTop, int xRight, int yBottom)
{
  DEB("WINUSERAPI BOOL WINAPI SetRect ( LPRECT lprc, int xLeft, int yTop, int xRight, int yBottom)\n");
  return 0;
}
WINUSERAPI BOOL WINAPI SubtractRect ( LPRECT lprcDst, CONST RECT *lprcSrc1, CONST RECT *lprcSrc2)
{
  DEB("WINUSERAPI BOOL WINAPI SubtractRect ( LPRECT lprcDst, CONST RECT *lprcSrc1, CONST RECT *lprcSrc2)\n");
  return 0;
}

WINBASEAPI VOID WINAPI InitializeCriticalSection ( LPCRITICAL_SECTION lpCriticalSection )
{
  DEB("WINBASEAPI VOID WINAPI InitializeCriticalSection ( LPCRITICAL_SECTION lpCriticalSection )\n");
  return;
}
WINBASEAPI VOID WINAPI DeleteCriticalSection ( LPCRITICAL_SECTION lpCriticalSection )
{
  DEB("WINBASEAPI VOID WINAPI DeleteCriticalSection ( LPCRITICAL_SECTION lpCriticalSection )\n");
  return;
}
WINBASEAPI VOID WINAPI EnterCriticalSection ( LPCRITICAL_SECTION lpCriticalSection )
{
  DEB("WINBASEAPI VOID WINAPI EnterCriticalSection ( LPCRITICAL_SECTION lpCriticalSection )\n");
  return;
}
WINBASEAPI VOID WINAPI LeaveCriticalSection ( LPCRITICAL_SECTION lpCriticalSection )
{
  DEB("WINBASEAPI VOID WINAPI LeaveCriticalSection ( LPCRITICAL_SECTION lpCriticalSection )\n");
  return;
}

WINUSERAPI HANDLE WINAPI LoadImage ( HINSTANCE hinst, LPCSTR lpszName, UINT uType, int cxDesired, int cyDesired, UINT fuLoad)
{
  DEB("WINUSERAPI HANDLE WINAPI LoadImage ( HINSTANCE hinst, LPCSTR lpszName, UINT uType, int cxDesired, int cyDesired, UINT fuLoad)\n");
  return 0;
}
WINGDIAPI HDC WINAPI CreateCompatibleDC (HDC hdc)
{
  DEB("WINGDIAPI HDC WINAPI CreateCompatibleDC (HDC hdc)\n");
  return 0;
}
WINGDIAPI HBITMAP WINAPI CreateDIBSection (HDC hdc, CONST BITMAPINFO *pbmi, UINT iUsage, VOID **ppvBits, HANDLE hSection, DWORD dwOffset)
{
  DEB("WINGDIAPI HBITMAP WINAPI CreateDIBSection (HDC hdc, CONST BITMAPINFO *pbmi, UINT iUsage, VOID **ppvBits, HANDLE hSection, DWORD dwOffset)\n");
  return 0;
}
WINGDIAPI HGDIOBJ WINAPI SelectObject (HDC hdc, HGDIOBJ hgdiobj)
{
  DEB("WINGDIAPI HGDIOBJ WINAPI SelectObject (HDC hdc, HGDIOBJ hgdiobj)\n");
  return 0;
}
WINGDIAPI int WINAPI SetDIBits (HDC hdc, HBITMAP hbmp, UINT uStartScan, UINT cScanLines, CONST VOID *lpvBits, CONST BITMAPINFO *lpbmi, UINT fuColorUse)
{
  DEB("WINGDIAPI int WINAPI SetDIBits (HDC hdc, HBITMAP hbmp, UINT uStartScan, UINT cScanLines, CONST VOID *lpvBits, CONST BITMAPINFO *lpbmi, UINT fuColorUse)\n");
  return 0;
}
WINGDIAPI BOOL WINAPI DeleteDC (HDC hdc)
{
  DEB("WINGDIAPI BOOL WINAPI DeleteDC (HDC hdc)\n");
  return 0;
}
WINGDIAPI BOOL WINAPI DeleteObject (HGDIOBJ hObject)
{
  DEB("WINGDIAPI BOOL WINAPI DeleteObject (HGDIOBJ hObject)\n");
  return 0;
}
WINGDIAPI int WINAPI GetObject (HGDIOBJ hgdiobj, int cbBuffer, LPVOID lpvObject)
{
  DEB("WINGDIAPI int WINAPI GetObject (HGDIOBJ, int, LPVOID)\n");
  return 0;
}
WINGDIAPI BOOL WINAPI GdiFlush ()
{
  DEB("WINGDIAPI BOOL WINAPI GdiFlush ()\n");
  return 0;
}
WINGDIAPI int WINAPI GetDIBits (HDC hdc, HBITMAP hbmp, UINT uStartScan, UINT cScanLines, LPVOID lpvBits, LPBITMAPINFO lpbi, UINT uUsage)
{
  DEB("WINGDIAPI int WINAPI GetDIBits (HDC hdc, HBITMAP hbmp, UINT uStartScan, UINT cScanLines, LPVOID lpvBits, LPBITMAPINFO lpbi, UINT uUsage)\n");
  return 0;
}
