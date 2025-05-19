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

#ifndef _GUISTATUSCB_H
#define _GUISTATUSCB_H

#include "dispatch.h"

class AppCmds;
class Dependent;

class GuiStatusCallback : public Dispatchable {
  public:
    void onSetStatusText(const char *text, int overlay);
    void onAddAppCmds(AppCmds *commands);
    void onRemoveAppCmds(AppCmds *commands);
    void pushCompleted(int max=100);
    void incCompleted(int add=1);
    void setCompleted(int pos);
    void popCompleted();

    Dependent *status_getDependencyPtr();
  enum {
    STATUS_ONSETTEXT   = 101,
    STATUS_GETDEP      = 200,
    STATUS_ADDCTXTCMDS = 300,
    STATUS_REMCTXTCMDS = 400,
    STATUS_PUSHCOMPLETED = 500,
    STATUS_INCCOMPLETED = 600,
    STATUS_SETCOMPLETED = 700,
    STATUS_POPCOMPLETED = 800,
  };
};

inline void GuiStatusCallback ::onSetStatusText(const char *text, int overlay) {
  _voidcall(STATUS_ONSETTEXT, text, overlay);
}

inline Dependent *GuiStatusCallback ::status_getDependencyPtr() {
  return  _call(STATUS_GETDEP, (Dependent *)NULL);
}

inline void GuiStatusCallback ::onAddAppCmds(AppCmds *commands) {
  _voidcall(STATUS_ADDCTXTCMDS, commands);
}

inline void GuiStatusCallback ::onRemoveAppCmds(AppCmds *commands) {
  _voidcall(STATUS_ADDCTXTCMDS, commands);
}

inline
void GuiStatusCallback::pushCompleted(int max) {
  _voidcall(STATUS_PUSHCOMPLETED, max);
}

inline
void GuiStatusCallback::incCompleted(int add) {
  _voidcall(STATUS_INCCOMPLETED, add);
}

inline
void GuiStatusCallback::setCompleted(int pos) {
  _voidcall(STATUS_SETCOMPLETED, pos);
}

inline
void GuiStatusCallback::popCompleted() {
  _voidcall(STATUS_POPCOMPLETED);
}

class GuiStatusCallbackI : public GuiStatusCallback {
  public:
    virtual void onSetStatusText(const char *text, int overlay)=0;
    virtual Dependent *status_getDependencyPtr()=0;
    virtual void onAddAppCmds(AppCmds *commands)=0;
    virtual void onRemoveAppCmds(AppCmds *commands)=0;
    virtual void pushCompleted(int max=100)=0;
    virtual void incCompleted(int add=1)=0;
    virtual void setCompleted(int pos)=0;
    virtual void popCompleted()=0;

  protected:
    RECVS_DISPATCH;
};

#endif
