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

//!##  An object to multiplex the callbacks of multiple attributes.
#ifndef _ATTRHANDLER_H
#define _ATTRHANDLER_H

// This class is meant to be subclassed.  The methods you must provide
// are given as pure virtuals.  See ExampleAttrib for more details, and
// an example subclass that you can copy for your own use.

#include "../attribs/attrcb.h"
#include "../attribs/attrint.h"
#include "../attribs/attrbool.h"
#include "../attribs/attrfloat.h"
#include "../attribs/attrstr.h"
#include "../bfc/map.h"

//
// Forward References
class WAComponentClient;
class Attribute;


//
// Class Definition
template <class TCallback>
class AttrHandler {
protected:
  // Heh, oops.  Can't have one AttrCallback handle lots of different attribs, anymore.  I fix.
  class AttrHandlerChild : public AttrCallback {
    public:
      AttrHandlerChild(AttrHandler *_parent) : AttrCallback() {
        ASSERT(_parent != NULL);
        recursion = 0;
        callback = NULL;
        parent = _parent;
      }

      // Here is where we split out the different value types
      virtual void onValueChange(Attribute *attr) {
        if (!recursion) {
          // protect our programmers from stack overflow, please.
          recursion = 1;
          if ((callback != NULL) && (parent != NULL)) {
            int id;
            // find the id from the map (friendly)
            int success = parent->attribmap.getItem(attr,&id);
            if (success) {
              // and send it to the proper handling function (poorman's RTTI)
              switch (attr->getAttributeType()) {
                case AttributeType::INT:
                  callback->onIntChange(id,*static_cast<_int *>(attr));
                break;
                case AttributeType::BOOL:
                  callback->onBoolChange(id, *static_cast<_bool *>(attr));
                break;
                case AttributeType::FLOAT:
                  callback->onFloatChange(id, *static_cast<_float *>(attr));
                break;
                case AttributeType::STRING:
                  callback->onStringChange(id, *static_cast<_string *>(attr));
                break;
              }
            }
          }
          recursion = 0;
        }
      }

      virtual void bindCallbackObj(TCallback *callbackobj) {
        // Be advised, this may be null.  That's okay, we test for it above.
        callback = callbackobj;
      }
    private:
      int recursion;
      TCallback *callback;
      AttrHandler *parent;
  };

public:
  AttrHandler() {
    component = NULL;
    callback = NULL;
  }

  // Call this method to bind your component (in your component's constructor)
  virtual void bindComponent(WAComponentClient *parentcomponent) {
    component = parentcomponent;
  }

  // Call this method to bind your callback object (usually your window in its constructor)
  virtual void bindCallbackObj(TCallback *callbackobj) {
    // Bind ourselves.
    callback = callbackobj;

    // Then go through and rebind any children.
    int i, num = attrchildren.getNumItems();
    for (i = 0; i < num; i++) {
      AttrHandlerChild *child = attrchildren.enumItem(i);
      child->bindCallbackObj(callback);
    }
  }

  // Call this method to register each attribute.
  virtual void registerAttribute(Attribute *attr, int id) {
    ASSERTPR(component != NULL, "BIND YOUR COMPONENT before registering Attributes to a Handler.");
    // register the attrib with a child object as its callback
    AttrHandlerChild *child = new AttrHandlerChild(this);
    attrchildren.addItem(child);
    component->registerAttribute(attr, child);
    // and save its id mapping
    attribmap.addItem(attr, id);
  }

#if 0  
  // Your callback object (probably your primary window class) must implement
  // its own versions of these methods here.  They will be called by the 
  // switch statement below.
  virtual void onIntChange(int id, int *attr);
  virtual void onBoolChange(int id, bool *attr);
  virtual void onFloatChange(int id, double *attr);
  virtual void onStringChange(int id, const char *attr);
#endif

  
private:
  friend AttrHandlerChild;
  TCallback *callback;
  WAComponentClient *component;
  Map< Attribute *, int >  attribmap;
  PtrList<AttrHandlerChild>  attrchildren;
};

#endif  // _ATTRHANDLER_H
