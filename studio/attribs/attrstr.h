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

#ifndef _ATTRSTR_H
#define _ATTRSTR_H

#include "attribute.h"

#include "../bfc/string.h"
#include "../common/common.h"

/**
  String configuration attributes, can have any string value 
  of any length. They can be used like any other config item.
  
  @short String configuration attribute.
  @ver 1.0
  @author Nullsoft
  @see _int
  @see _bool
  @see _float
*/
class _string : public Attribute {
public:
  /**
    Optionally set the name and default value of 
    your configuration attribute during construction.
      
    @param name
    @param default_val
  */
  _string(const char *name=NULL, const char *default_val=NULL)
    : Attribute(name) {
    setData(default_val, true);
  }

  /**
    Get the attribute type. This will return 
    a constant representing the attribute type.
    
    These constants can be: BOOL, FLOAT, STRING and INT.
    
    @see AttributeType
    @ret The attribute type.
  */
  virtual int getAttributeType() { return AttributeType::STRING; }
  
  /**
    Get the configuration group to be used to represent
    this attribute in the registry.
    
    @ret Config group to be used.
  */
  virtual const char *getConfigGroup() { return "studio.configgroup.string"; }

//CUT  virtual int getPermissions() { return ATTR_PERM_ALL; }

  /**
    Get the value of the attribute.
    
    @ret The value of the attribute
  */
  const char *getValue();
  
  /**
    Set the value of the attribute.
    
    @param val The value you want to set.
    @ret 1, success; 0, failure;
  */
  int setValue(const char *val) { return setData(val); }

  // convenience operators
  /**
    Get the value of the attribute.
  */
  operator const char *() { return getValue(); }

  /**
    Set the value of the attribute.
  */
  const char *operator =(const char *newval) { return setValue(newval) ? newval : getValue(); }

private:
  String returnval;
};

#endif
