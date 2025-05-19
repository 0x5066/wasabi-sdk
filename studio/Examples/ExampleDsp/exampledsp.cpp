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

#include "exampledsp.h" //CHANGE ME!!
#include "dsp.h"

//CHANGE ME!! (Use GUIDGEN.EXE, comes with MSVC++)
// {B7B68BA5-8B0C-4edf-B49C-976F52E292E5}
static const GUID exampledsp_guid = 
{ 0xb7b68ba5, 0x8b0c, 0x4edf, { 0xb4, 0x9c, 0x97, 0x6f, 0x52, 0xe2, 0x92, 0xe5 } };

static WACNAME exampledsp_wac;  //CHANGE ME!!
WAComponentClient *the = &exampledsp_wac; //CHANGE ME!!

WACNAME::WACNAME() : WACPARENT("Example DSP MediaConverter") {  //CHANGE ME!!
  registerService(new waServiceT<svc_mediaConverter, DSP > );
}

WACNAME::~WACNAME() {

}

GUID WACNAME::getGUID() {
  return exampledsp_guid; //CHANGE ME!!
}



_bool dsp_enabled("Enable Dsp", 1);

void WACNAME::onRegisterServices() {
  registerAttribute(&dsp_enabled);
}