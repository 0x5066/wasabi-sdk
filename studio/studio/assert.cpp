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

#include <windows.h>

#include "assert.h"

#ifdef ASSERTS_ENABLED

static int in_assert = 0;

void _assert_handler(const char *reason, const char *file, int line) {
  if (in_assert) __asm { int 3 };
  in_assert = 1;
  char msg[4096];
  wsprintf(msg, "Expression: %s\nFile: %s\nLine: %d", reason, file, line);
  OutputDebugString(msg);
  OutputDebugString("\n");

  ATOM a = AddAtom("BYPASS_DEACTIVATE_MGR"); // so we don't need to call api->appdeactivation_setbypass
  if (MessageBox(NULL, msg, "Assertion failed", MB_OKCANCEL|MB_TASKMODAL) == IDCANCEL) {
    __asm { int 0x3 };
  } else
    ExitProcess(0);
  DeleteAtom(a);
  in_assert = 0;
}

void _assert_handler_str(const char *string, const char *reason, const char *file, int line) {
  if (in_assert) __asm { int 3 };
  in_assert = 1;
  char msg[4096];
  wsprintf(msg, "\"%s\"\nExpression: %s\nFile: %s\nLine: %d", string, reason ? reason : "", file, line);
  OutputDebugString(msg);
  OutputDebugString("\n");

  ATOM a = AddAtom("BYPASS_DEACTIVATE_MGR");
  if (MessageBox(NULL, msg, "Assertion failed", MB_OKCANCEL|MB_TASKMODAL) == IDCANCEL) {
    __asm { int 0x3 };
  } else
    ExitProcess(0);
  DeleteAtom(a);
  in_assert = 0;
}
#endif
