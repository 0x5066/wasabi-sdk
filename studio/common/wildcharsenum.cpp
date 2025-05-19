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


// NONPORTABLE NONPORTABLE NONPORTABLE 

#include <windows.h>
#include "wildcharsenum.h"
#include "../bfc/util/pathparse.h"

#define UTF8 1
#if UTF8
#ifdef WANT_UTF8_WARNINGS
#pragma CHAT("mig", "all", "UTF8 is enabled in wildcharsenum.cpp -- Things might be screwy till it's all debugged?")
#endif
# include "../bfc/encodedstr.h"
#endif

WildcharsEnumerator::WildcharsEnumerator(const char *_selection) : selection(_selection) { 
  // Test if the system supports U16 at initialization.
#if UTF8
  utf16 = Std::encodingSupportedByOS(SvcStrCnv::UTF16);
#endif
  // Then scan.
  rescan();
}

void WildcharsEnumerator::rescan() {
  char p[WA_MAX_PATH]="";
  STRNCPY(p, selection, WA_MAX_PATH-1);

  PathParser parse(selection);
  path = "";

  for (int i=0;i<parse.getNumStrings()-1;i++)
    path.cat(StringPrintf("%s%c", parse.enumString(i), Std::dirChar()));

  finddatalist.removeAll();

  // forcibly convert all slashes to proper directory separator char.
  char *q = p;
  while (*q) {
    if (*q == '/') *q = Std::dirChar();
    if (*q == '\\') *q = Std::dirChar();
    q++;
  }

  // enum files and store a list
#if UTF8
  if (utf16) {
    // CODE FOR NT COMPATIBLE OS'S
    EncodedStr selection16enc;
    int retcode = selection16enc.convertFromUTF8(SvcStrCnv::UTF16, String(selection));
    WCHAR *selection16 = static_cast<WCHAR *>(selection16enc.getEncodedBuffer());
    if (selection16 != NULL) {

      WIN32_FIND_DATAW finddataW;
      int quit = ((findhandle = FindFirstFileW(selection16, &finddataW)) == (HANDLE)-1);
      while (!quit) {
        finddatalistW.addItem(finddataW);
        quit = !FindNextFileW(findhandle, &finddataW);
      }
      FindClose(findhandle);

    } else if (retcode == SvcStrCnv::ERROR_UNAVAILABLE) {

      WIN32_FIND_DATA finddata;
      int quit = ((findhandle = FindFirstFile(selection, &finddata)) == (HANDLE)-1);
      while (!quit) {
        finddatalist.addItem(finddata);
        quit = !FindNextFile(findhandle, &finddata);
      }
      FindClose(findhandle);
      // this shouldn't happen, boyo.
      utf16 = 0;

    }
  } else {
    // CODE FOR 9x COMPATIBLE OS'S
    EncodedStr selectionOSenc;
    int retcode = selectionOSenc.convertFromUTF8(SvcStrCnv::OSNATIVE, String(selection));
    char *selectionOS = static_cast<char *>(selectionOSenc.getEncodedBuffer());
    if (selectionOS != NULL) {
      
      WIN32_FIND_DATA finddata;
      int quit = ((findhandle = FindFirstFile(selectionOS, &finddata)) == (HANDLE)-1);
      while (!quit) {
        finddatalist.addItem(finddata);
        quit = !FindNextFile(findhandle, &finddata);
      }
      FindClose(findhandle);

    } else if (retcode == SvcStrCnv::ERROR_UNAVAILABLE) {
      
      WIN32_FIND_DATA finddata;
      int quit = ((findhandle = FindFirstFile(selection, &finddata)) == (HANDLE)-1);
      while (!quit) {
        finddatalist.addItem(finddata);
        quit = !FindNextFile(findhandle, &finddata);
      }
      FindClose(findhandle);

    }
  }

#else
  WIN32_FIND_DATA finddata;
  int quit = ((findhandle = FindFirstFile(selection, &finddata)) == (HANDLE)-1);
  while (!quit) {
    finddatalist.addItem(finddata);
    quit = !FindNextFile(findhandle, &finddata);
  }
  FindClose(findhandle);
#endif
}

int WildcharsEnumerator::getNumFiles() {
#if UTF8
  if (utf16) {
    return finddatalistW.getNumItems();
  }
#endif
  return finddatalist.getNumItems();
}

const char *WildcharsEnumerator::enumFile(int n) {
#if UTF8
  if (utf16) {
    // CODE FOR NT COMPATIBLE OS'S
    String out8;
    WCHAR *fname16 = finddatalistW.enumItem(n).cFileName;
    EncodedStr selection16enc(SvcStrCnv::UTF16, fname16, (WSTRLEN(fname16)+1)*2, 0/*no delete*/);
    int retcode = selection16enc.convertToUTF8(out8);
    if (retcode > 0) {
      if (!path.isempty()) {
        enumFileString.printf("%s%s", path, out8);
      } else {
        enumFileString = out8;
      }
    } else if (retcode == SvcStrCnv::ERROR_UNAVAILABLE) {
      // Never, ever, ever, get here.  That's BAD.
      ASSERTPR(retcode != SvcStrCnv::ERROR_UNAVAILABLE, "INCONCEIVABLE!");
    }
  } else {
    // CODE FOR 9x COMPATIBLE OS'S
    String out8;
    char *fnameOS = finddatalist.enumItem(n).cFileName;
    EncodedStr selectionOSenc(SvcStrCnv::OSNATIVE, fnameOS, (STRLEN(fnameOS)+1), 0/*no delete*/);
    int retcode = selectionOSenc.convertToUTF8(out8);
    if (retcode > 0) {
      if (!path.isempty()) {
        enumFileString.printf("%s%s", path, out8);
      } else {
        enumFileString = out8;
      }
    } else if (retcode == SvcStrCnv::ERROR_UNAVAILABLE) {
      if (!path.isempty()) {
        enumFileString.printf("%s%s", path, finddatalist.enumItem(n).cFileName);
      } else {
        enumFileString = finddatalist.enumItem(n).cFileName;
      }
    }
  }
  return enumFileString;
#else
  if (!path.isempty()) {
    enumFileString.printf("%s%s", path, finddatalist.enumItem(n).cFileName);
    return enumFileString;
  }
  return finddatalist.enumItem(n).cFileName;
#endif
}

int WildcharsEnumerator::isWildchars(const char *filename) {
  return (STRCHR(filename, '*') || STRCHR(filename, '?'));
}

