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


#include "pngload.h"

#include "loader.h"

#include "../studio/services/servicei.h"

static WACPngLoad wac;
WAComponentClient *the = &wac;

// {5E04FB28-53F5-4032-BD29-032B87EC3725}
static const GUID guid = 
{ 0x5e04fb28, 0x53f5, 0x4032, { 0xbd, 0x29, 0x3, 0x2b, 0x87, 0xec, 0x37, 0x25 } };


WACPngLoad::WACPngLoad() {
  registerService(new waServiceT<svc_imageLoader, PngLoad>);
}

GUID WACPngLoad::getGUID() {
  return guid;
}
