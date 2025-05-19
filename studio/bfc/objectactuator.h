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

#ifndef __OBJECTACTUATOR_H
#define __OBJECTACTUATOR_H

#include "nakedobject.h"

#define  OBJECTACTUATOR_PARENT NakedObject

// -----------------------------------------------------------------------
// Your wnd object class
class COMEXP ObjectActuator : public OBJECTACTUATOR_PARENT {
  
  public:

    ObjectActuator();
    virtual ~ObjectActuator();

    virtual int onInit();
    virtual int setXuiParam(int xuihandle, int xmlattributeid, const char *xmlattributename, const char *value);

    virtual void actuator_setTarget(const char *value);
    virtual void actuator_setGroup(const char *value);

    virtual int actuator_wantTargetParam() { return 1; }
    virtual int actuator_wantGroupParam() { return 1; }
    virtual int actuator_wantAutoPerform() { return 1; }
    virtual void actuator_onPerform(GuiObject *target) { } // called back n times for n targets found (separated by ';'), guaranteed non NULL

    virtual const char *getActuatorTag(); // for error msgs purposes

protected:
    void performActions();

  private:
    int myxuihandle;

    enum {
      OBJECTACTUATOR_TARGET= 0,
      OBJECTACTUATOR_GROUP,
    };

  String groupid;
  String objectsid;
};


#endif
