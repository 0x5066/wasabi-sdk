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

#ifndef _ATTRFLOAT_H
#define _ATTRFLOAT_H

#include "attribute.h"

// actually it's a double :)

class _float : public Attribute {
public:
  /**
    Optionally set the name and default value of 
    your configuration attribute during construction.
    
    @param name Name of the configuration attribute.
    @param default_val Default value.
  */
  _float(const char *name=NULL, double default_val=0.f) : Attribute(name) {
    setValueAsDouble(default_val, true);
  }
  
  /**
    Get the attribute type. This will return 
    a constant representing the attribute type.
    
    These constants can be: BOOL, FLOAT, STRING and INT.
    
    @see AttributeType
    @ret The attribute type.
  */
  virtual int getAttributeType() { return AttributeType::FLOAT; }
  
  /**
    Get the configuration group to be used to represent
    this attribute in the registry.
    
    @ret Config group to be used.
  */
  virtual const char *getConfigGroup() { return "studio.configgroup.float"; }

  // convenience operators
  /**
    Get the value of the attribute.
  */
  operator double() { return getValueAsDouble(); }
  
  /**
    Set the value of the attribute.
  */
  double operator =(double newval) { return setValueAsDouble(newval) ? newval : getValueAsDouble(); }
};

#endif
