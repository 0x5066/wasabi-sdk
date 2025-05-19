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

#ifndef __CFGSCRIPTOBJ_H
#define __CFGSCRIPTOBJ_H

#include "../../common/script/scriptobj.h"
#include "../../common/script/objcontroller.h"
#include "../../studio/services/svc_scriptobj.h"
#include "../../studio/corecb.h"

class ExampleObject;

extern ScriptObjectController *exampleController;

// -----------------------------------------------------------------------------------------------------
// ScriptObject Provider Service. This class tells the system that we have some ScriptObjectControllers
// to register.

class ExampleScriptObjectSvc : public svc_scriptObjectI {

public:
  ExampleScriptObjectSvc() {};
  virtual ~ExampleScriptObjectSvc() {};

  static const char *getServiceName() { return "Example Script Object Service"; }
  virtual ScriptObjectController *getController(int n);
};       

// -----------------------------------------------------------------------------------------------------
// Script classe GUID

// {9476ADAE-9F8C-4a5b-BA95-C7F8FA76A2F1} // this is the guid to use in your compiler definitions, change it for each new class!
static const GUID EXAMPLE_SCRIPTOBJECT_GUID = 
{ 0x9476adae, 0x9f8c, 0x4a5b, { 0xba, 0x95, 0xc7, 0xf8, 0xfa, 0x76, 0xa2, 0xf1 } };

// -----------------------------------------------------------------------------------------------------
// ScriptObject Interface. This is your script object, you could attach this class to a preexisting object
// you have, as a seconday inheritance, then fill in a few functions and events. Or you could start from the
// scriptobject and turn it into a wrapper to your own object. 
class ExampleObject : public ScriptObjectI, public CoreCallbackI {
  
  public:

    ExampleObject();
    virtual ~ExampleObject();
    
    virtual const char *vcpu_getClassName() { return "ExampleObject"; }
    virtual ScriptObjectController *vcpu_getController() { return exampleController; }

    virtual int functionABC(int a, float b, const char *c);
    virtual void onSomething(const char *msg);

    virtual int corecb_onStarted();

    // These are the exported functions, they do not have to reside in the object class but for convenience we'll have them here
    static scriptVar example_functionABC(SCRIPT_FUNCTION_PARAMS, ScriptObject *o, scriptVar a, scriptVar b, scriptVar c);
    static scriptVar example_eventOnSomething(SCRIPT_FUNCTION_PARAMS, ScriptObject *o, scriptVar msg);
};

// -----------------------------------------------------------------------------------------------------
// This class describes to the system how to handle, create and delete our script class

class ExampleScriptObjectController : public ScriptObjectControllerI {
  public:
    ExampleScriptObjectController() {} // (don't ask my why, the compiler wanted it.)
    virtual const char *getClassName() { return "ExampleObject"; } // has to match the controlled class' vcpu_getClassName() !
    virtual const char *getAncestorClassName() { return "Object"; }  // "Object" to inherit from root, otherwise name of parent class
    virtual ScriptObjectController *getAncestorController() { return NULL; } // NULL if inheriting from root, otherwise, your parent controller class
    virtual int getNumFunctions(); // number of functions and events in table
    virtual const function_descriptor_struct *getExportedFunctions() { return exportedFunction; } // pointer to function table
    virtual GUID getClassGuid() { return EXAMPLE_SCRIPTOBJECT_GUID; } // script class guid
    virtual int getInstantiable() { return 1; } // 1 if using '&' in compiler definitions
    virtual int getReferenceable() { return 1; } // usually 1, or 0 if using _predecl in compiler definitions
    virtual ScriptObject *instantiate() { return new ExampleObject; } // create your object here
    virtual void destroy(ScriptObject *o) { delete static_cast<ExampleObject *>(o); } // delete it here
    virtual void *encapsulate(ScriptObject *o) { return NULL; } // no encapsulation for exampleobject yet
    virtual void deencapsulate(void *o ) {} // no encapsulation for exampleobject yet


  private:
    static function_descriptor_struct exportedFunction[];
};


#endif

