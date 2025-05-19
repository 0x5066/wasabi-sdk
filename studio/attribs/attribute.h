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

#ifndef _ATTRIBUTE_H
#define _ATTRIBUTE_H

#include "../bfc/depend.h"
#include "../bfc/string.h"
#include "../bfc/named.h"
#include "../common/common.h"

class CfgItemI;

// lowercase types are reserved for official Nullsoft use
// uppercase are 3rd-party defined
namespace AttributeType {
  /**
    Attribute types.
  */
  enum {
    NONE = 0,
    INT = 'int',
    STRING = 'str',
    BOOL = 'bool',
    FLOAT = 'flot',
  };
};

/**
  Generic configuration attribute. 
  
  Configuration attributes enable you to store 
  uniquely identifiable values that get pushed 
  to a configuration file automatically upon shutdown 
  of any Wasabi application.
  
  You shouldn't normally use this on 
  it's own, look at the CfgItemI class 
  instead.

  @short Generic configuration attribute.
  @ver 1.0
  @author Nullsoft
  @see _float
  @see _int
  @see _bool
  @see _string
  @see CfgItemI
*/
class NOVTABLE Attribute : public DependentI, private Named {
public:
  static const GUID *depend_getClassGuid() {
    // {5AB601D4-1628-4604-808A-7ED899849BEB}
    static const GUID ret = 
    { 0x5ab601d4, 0x1628, 0x4604, { 0x80, 0x8a, 0x7e, 0xd8, 0x99, 0x84, 0x9b, 0xeb } };
    return &ret;
  }
protected:
  
  /**
    Optionally set the name and default value of 
    your configuration attribute during construction.
    
    @param name Name of the configuration attribute.
    @param default_val Default value.
  */
  Attribute(const char *name=NULL, const char *desc=NULL);
  
public:
  /**
    Does nothing.
  */
  virtual ~Attribute();

  /**
    Set the name of the configuration
    attribute.
    
    @param newname Name of the attribute.
  */
  void setName(const char *newname);
  
  /**
    Get the name of the configuration
    attribute.
    
    @ret Name of the attribute.
  */
  const char *getAttributeName();

  /**
    Get the attribute's description.
    
    @ret Attribute's description.
  */
  const char *getAttributeDesc();

  /**
    Get the attribute type. Override
    this for your custom attribute type.
    
    @ret Attribute type.
  */
  virtual int getAttributeType()=0;	// override me
  
  /**
    Get the configuration group to be used to represent
    this attribute in the registry. 
    
    This is only called if the kernel doesn't have a default 
    config group set for your type already.
    
    @ret Config group to be used.
  */
  virtual const char *getConfigGroup() { return NULL; }	// override me

  /**
    Get the attribute's value as signed integer.
    
    @ret Attribute value, as a signed integer.
  */
  int getValueAsInt();
  
  /**
    Set the attribute's value with a signed integer while
    also being able to replace the default value previously
    set.
    
    @param newval Attribute's new value.
    @param def true, replace the current default value; false, leave the default value unchanged;
  */
  int setValueAsInt(int newval, bool def=false);

  /**
    Get the attribute's value as signed double.
    
    @ret Attribute value, as a signed double.
  */
  double getValueAsDouble();
  
  /**
    Set the attribute's value with a signed double while
    also being able to replace the default value previously
    set.
    
    @param newval Attribute's new value.
    @param def true, replace the current default value; false, leave the default value unchanged;
  */
  double setValueAsDouble(double newval, bool def=false);

  /**
    Get the length of the attribute's value (data)
    in bytes.
    
    @ret Attribute value (data) length, in bytes.
  */
  int getDataLen();

  /**
    Get the attribute's raw data. 
    
    This will return the data the attribute is storing
    in a char buffer you hand to it.
    
    @ret Attribute value, as a signed double.
    @param data Pointer to a char buffer.
    @param data_len The maximum amount of bytes the char buffer can hold.
  */
  int getData(char *data, int data_len);
  
  /**
    Set the attribute's value with a zero terminated string. Also 
    enables you to replace the default value previously
    set.
    
    @param newval Attribute's new value.
    @param def true, replace the current default value; false, leave the default value unchanged;
  */
  int setData(const char *data, bool def=false);

  enum {
    Event_DATACHANGE=100,
  };
protected:
  friend class CfgItemI;
  
  /**
    Set the attribute's value without causing 
    a callback.

    @ret 1.
    @param data Attribute's new value.
  */
  int setDataNoCB(const char *data);
  
  /**
    Set the configuration item associated with this
    attribute.
  */
  void setCfgItem(CfgItemI *item);

private:
  String desc;
  String default_val;
  CfgItemI *cfgitemi;
};

#define ATTR_PERM_READ	1
#define ATTR_PERM_WRITE	2

#define ATTR_PERM_ALL	(~0)

// render hints for getRenderHint
enum {
  ATTR_RENDER_HINT_INT_CHECKMARK
};

#endif
