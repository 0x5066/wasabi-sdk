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

#include "jpgload.h"
#include "loader.h"

static WACNAME wac;
WAComponentClient *the = &wac;

#include "../studio/services/servicei.h"

// {AE04FB30-53F5-4032-BD29-032B87EC3404}
static const GUID guid = 
{ 0xae04fb30, 0x53f5, 0x4032, { 0xbd, 0x29, 0x3, 0x2b, 0x87, 0xec, 0x34, 0x04 } };


WACNAME::WACNAME() {
  registerService(new waServiceT<svc_imageLoader, JpgLoad>);
}

WACNAME::~WACNAME() {
}

GUID WACNAME::getGUID() {
  return guid;
}
