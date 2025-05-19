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

#ifndef __VCPUTYPES_H
#define __VCPUTYPES_H

#include "../../bfc/platform/platform.h"

#pragma pack (push, 1)

// Basic types
#define SCRIPT_FIRSTTYPE        SCRIPT_VOID
#define SCRIPT_VOID             0
#define SCRIPT_EVENT            1
#define SCRIPT_INT              2
#define SCRIPT_FLOAT            3
#define SCRIPT_DOUBLE           4
#define SCRIPT_BOOLEAN          5
#define SCRIPT_STRING           6
#define SCRIPT_OBJECT           7 
#define SCRIPT_ANY              8
#define SCRIPT_LASTTYPE         SCRIPT_ANY

static const GUID guidNull = 
{ 0x00000000, 0x0000, 0x0000, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } };

#include "scriptvar.h"


typedef struct {
  scriptVar v;
  int scriptId;
  int varId;
  char transcient;
  char isaclass;
  char isstatic;
} VCPUscriptVar;

typedef struct {
  int varId;
  int pointer;
  int DLFid;
  int scriptId;
} VCPUeventEntry;

typedef struct {
  char *functionName;
  int basetype;
  int scriptId;
  int DLFid;
  void *ptr;
  int nparams;
} VCPUdlfEntry;

typedef struct {
  char *codeBlock;
  int scriptId;
  int dlfBase;
  int varBase;
} VCPUcodeBlock;

typedef struct {
  int cmd;
  int id;
} maki_cmd;

#pragma pack (pop)

#endif

