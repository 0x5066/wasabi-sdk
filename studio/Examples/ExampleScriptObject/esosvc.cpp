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

#include "esosvc.h"
#include "eso.h"

static ExampleScriptObjectController _exampleController;
ScriptObjectController *exampleController = &_exampleController;

// -----------------------------------------------------------------------------------------------------
// Service

ScriptObjectController *ExampleScriptObjectSvc::getController(int n) {
  switch (n) {
    case 0:
      return exampleController;
    // add more controllers here
  }
  return NULL;
}

// -----------------------------------------------------------------------------------------------------
// ExampleObject

ExampleObject::ExampleObject() {
  api->core_addCallback(0, this); 
}

ExampleObject::~ExampleObject() {
  api->core_delCallback(0, this);
}

// This is a simple function that a script can use off your object to do stuff
int ExampleObject::functionABC(int a, float b, const char *c) {
  api->messageBox(StringPrintf("a = %d, b = %f, c = %s", a, b, c), "functionABC", 0, NULL, NULL);
  return 31337;
}

// This is your event, when 'something' happens in your code, just call this function and
// any script intercepting this event on this object will be called.
void ExampleObject::onSomething(const char *msg) {
  example_eventOnSomething(SCRIPT_CALL, this, MAKE_SCRIPT_STRING(msg));  
}

// Here we'll just trap the starting of the core and fire the event whenever that happens
int ExampleObject::corecb_onStarted() {
  onSomething("Hey, you just started to play something :)");
  return 0;
}

// Table of exported functions and events
// "function name",  n. params, function_pointer
function_descriptor_struct ExampleScriptObjectController::exportedFunction[] = {
  {"functionABC",            3, ExampleObject::example_functionABC},
  {"onSomething",            1, ExampleObject::example_eventOnSomething},
};

ExampleScriptObjectController::getNumFunctions() {
  return sizeof(exportedFunction) / sizeof(function_descriptor_struct); 
}

// This is called when a script calls ExampleObject.functionABC(...)
scriptVar ExampleObject::example_functionABC(SCRIPT_FUNCTION_PARAMS, ScriptObject *o, scriptVar a, scriptVar b, scriptVar c) {
  // the following line is mandatory at the start of all exported script function/events
  SCRIPT_FUNCTION_INIT   
  // this calls the actual function off the object
  return MAKE_SCRIPT_INT((static_cast<ExampleObject *>(o))->functionABC(GET_SCRIPT_INT(a), GET_SCRIPT_FLOAT(b), GET_SCRIPT_STRING(c)));
}

scriptVar ExampleObject::example_eventOnSomething(SCRIPT_FUNCTION_PARAMS, ScriptObject *o, scriptVar msg) {
  // the following line is mandatory at the start of all exported script function/events
  SCRIPT_FUNCTION_INIT  
  // this needs to be called after script_function_init for all events, using the object pointer passed to the function, the scriptController pointer, and your event parameters
  // note the '1' at the end of the macro name, this is the number of parameters in your event
  PROCESS_HOOKS1(o, exampleController, msg); 
  // this prevents the scripting engine to run events that are not trapped by anyone
  SCRIPT_FUNCTION_CHECKABORTEVENT;
  // this actually runs all scripts associated with the event, note '1' in the macro, this is again the number of parameters in your event
  SCRIPT_EXEC_EVENT1(o, msg);
}



