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


#include "readdir.h"

#define UTF8 1
#if UTF8
#ifdef WANT_UTF8_WARNINGS
#pragma CHAT("mig", "all", "UTF8 is enabled in readdir.cpp -- Things might be screwy till it's all debugged?")
#endif
# include "../bfc/encodedstr.h"
#endif

ReadDir::ReadDir(const char *_path, const char *_match, BOOL _skipdots) :
  skipdots(_skipdots), files(INVALID_HANDLE_VALUE), first(1),
  path(_path), match(_match)
{
  utf16 = Std::encodingSupportedByOS(SvcStrCnv::UTF16);
  if (match.isempty()) match = MATCHALLFILES;
//CUT  path.printf("%s%s%s", _path, DIRCHARSTR, match),
  ZERO(data);
}

ReadDir::~ReadDir() {
  if (files != INVALID_HANDLE_VALUE) FindClose(files);
}

int ReadDir::next() {
  for (;;) {
    if (first) {
      StringPrintf fullpath("%s%s%s", path.getValue(), DIRCHARSTR, match.getValue());
#if UTF8
      if (utf16) {
        // CODE FOR NT COMPATIBLE OS'S
        EncodedStr path16enc;
        int retcode = path16enc.convertFromUTF8(SvcStrCnv::UTF16, fullpath);
        WCHAR *path16 = static_cast<WCHAR *>(path16enc.getEncodedBuffer());
        if (path16 != NULL) {
          files = FindFirstFileW(path16, &dataW);
        } else if (retcode == SvcStrCnv::ERROR_UNAVAILABLE) {
          files = FindFirstFile(fullpath, &data);
          // yea... and we shouldn't be in the 16 side of the family, either.
          utf16 = 0;
        }
      } else {
        // CODE FOR 9x COMPATIBLE OS'S
        EncodedStr pathOSenc;
        int retcode = pathOSenc.convertFromUTF8(SvcStrCnv::OSNATIVE, fullpath);
        char *pathOS = static_cast<char *>(pathOSenc.getEncodedBuffer());
        if (pathOS != NULL) {
          files = FindFirstFile(pathOS, &data);
        } else if (retcode == SvcStrCnv::ERROR_UNAVAILABLE) {
          files = FindFirstFile(fullpath, &data);
        }
      }
#else
      files = FindFirstFile(fullpath, &data);
#endif
    }
    if (files == INVALID_HANDLE_VALUE) return 0;
    if (first) {
      first = 0;
      if (skipdots && (isDotDir() || isDotDotDir())) continue;
      return 1;
    }
#if UTF8
    if (utf16) {
      // CODE FOR NT COMPATIBLE OS'S
      if (!FindNextFileW(files, &dataW)) return 0;
    } else {
      // CODE FOR 9x COMPATIBLE OS'S
      if (!FindNextFile(files, &data)) return 0;
    }
#else
    if (!FindNextFile(files, &data)) return 0;
#endif
    if (skipdots && (isDotDir() || isDotDotDir())) continue;
    return 1;
  }
}

const char *ReadDir::getFilename() {
  if (first) if (!next()) return NULL;
#if UTF8
  if (utf16) {
    // CODE FOR NT COMPATIBLE OS'S
    EncodedStr fileenc(SvcStrCnv::UTF16, dataW.cFileName, (WSTRLEN(dataW.cFileName)+1)*2, 0/*no delete!!*/);
    int retcode = fileenc.convertToUTF8(filename8);
    if (retcode == SvcStrCnv::ERROR_UNAVAILABLE) {
      return data.cFileName;
    }
  } else {
    // CODE FOR 9x COMPATIBLE OS'S
    EncodedStr fileenc(SvcStrCnv::OSNATIVE, data.cFileName, (STRLEN(data.cFileName)+1), 0/*no delete!!*/);
    int retcode = fileenc.convertToUTF8(filename8);
    if (retcode == SvcStrCnv::ERROR_UNAVAILABLE) {
      return data.cFileName;
    }
  }
  return filename8;
#else
  return data.cFileName;
#endif
}

int ReadDir::isDir() {
  if (files == INVALID_HANDLE_VALUE) return 0;
#if UTF8
  if (utf16) {
    return !!(dataW.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY);
  } else {
    return !!(data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY);
  }
#else
  return !!(data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY);
#endif
}

int ReadDir::isReadonly() {
  if (files == INVALID_HANDLE_VALUE) return 0;
#if UTF8
  if (utf16) {
    return !!(dataW.dwFileAttributes & FILE_ATTRIBUTE_READONLY);
  } else {
    return !!(data.dwFileAttributes & FILE_ATTRIBUTE_READONLY);
  }
#else
  return !!(data.dwFileAttributes & FILE_ATTRIBUTE_READONLY);
#endif
}

int ReadDir::isDotDir() {
  if (files == INVALID_HANDLE_VALUE) return 0;
#if UTF8
  if (utf16) {
    return !WSTRCMP7(dataW.cFileName, DOTDIR);
  } else {
    return !STRCMP(data.cFileName, DOTDIR);
  }
#else
  return !STRCMP(data.cFileName, DOTDIR);
#endif
}

int ReadDir::isDotDotDir() {
  if (files == INVALID_HANDLE_VALUE) return 0;
#if UTF8
  if (utf16) {
    return !WSTRCMP7(dataW.cFileName, DOTDOTDIR);
  } else {
    return !STRCMP(data.cFileName, DOTDOTDIR);
  }
#else
  return !STRCMP(data.cFileName, DOTDOTDIR);
#endif
}
