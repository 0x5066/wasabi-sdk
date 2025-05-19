#include <lib/std.mi>
#include "exampleobject.mi"

Global ExampleObject obj;

System.onScriptLoaded() {

  obj = new ExampleObject;

  obj.functionABC(32, 75.01248, "Hello, world");
}

System.onScriptLoaded() {
  delete obj;
}

obj.onSomething(String msg) {
  messageBox(msg, "Something happened!", 0, "");
}

