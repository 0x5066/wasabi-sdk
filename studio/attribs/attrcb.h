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

#ifndef _ATTRCB_H
#define _ATTRCB_H

#include "attribute.h"

/**
  Enables you to register callbacks on
  a specific attribute to monitor if it's
  value has been changed by the user or other.

  This class is not meant to be used on it's own.
  Please derive from it instead.
  
  @short Attribute callback
  @ver 1.0
  @author Nullsoft
  @see Attribute
  @see int_attrCB
  @see _int
  @see _float
  @see _string
  @see _bool   
*/
class AttrCallback {
public:
  /**
    Does nothing.
  */
  virtual ~AttrCallback() {}
  
  /**
    Event triggered when the value of the attribute,
    for which this callback has been registered, changes.
    
    This is a pure virtual, please override to implement
    your custom behavior.
    
    @param attr Attribute for which the value has changed.
  */
  virtual void onValueChange(Attribute *attr)=0;
};

/**
  Enables you to register callbacks on a specific
  integer or boolean attribute to monitor if the
  value has been changed by the user or other.
  
  @short Integer or Boolean attribute Callback.
  @ver 1.0
  @author Nullsoft
  @see Attribute
  @see _int
  @see _bool   
*/
class int_attrCB : public AttrCallback {
  typedef void (*fnPtrType)(int);
public:
  /**
    Upon construction, you must specify which
    function will be called when the value of
    the attribute has indeed changed.
    
    This is done using a pointer to the function.
    The function must accept one parameter of type
    int, like so: void myfunc(int val);
    
    @param _fn Pointer to the function to use on value change.
  */
  int_attrCB(fnPtrType _fn) { fnptr = _fn; }
  
  /**
    Event triggered when the value of the attribute,
    for which this callback has been registered, changes.
    
    Override this to implement your own behavior.
    The default is to send the new value of the attribute
    to a function which you specify upon construction
    of this object.
        
    @param attr Attribute for which the value has changed.
  */
  virtual void onValueChange(Attribute *attr) {
    ASSERT(attr->getAttributeType() == AttributeType::INT ||
           attr->getAttributeType() == AttributeType::BOOL);
    (*fnptr)(attr->getValueAsInt());
  }
private:
  fnPtrType fnptr;
};

#endif
