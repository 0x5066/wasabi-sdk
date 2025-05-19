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

//!##  Our subclass of the object to multiplex the callbacks of multiple attributes.
//!##  Reference Stuff is in this module.
#ifndef _EXAMPLEATTRIBHANDLER_H
#define _EXAMPLEATTRIBHANDLER_H

#include "../../attribs/attrhandler.h"
#include "exampleattribwnd.h"

//
// Class Definition
class ExampleAttribHandler : public AttrHandler<ExampleAttribWnd> {
public:
ExampleAttribHandler() :
#if 1 // Example Specific Attribs
//
// In the initialization list of our constructor, we setup our attributes with
// their displayed names and their default values.
//
  intAttr("Example Integer Attribute",0),
  boolAttr("Example Boolean Attribute",0),
  floatAttr("Example Floating Point Attribute",0.5),
  stringAttr("Example String Attribute","Default Value"),
#endif // Cut if you're cloning this object.
  AttrHandler<ExampleAttribWnd>() {
}

#if 1 // Example Specific Attribs
  // We just have our attribs be public members of our handler subclass.
  // you could also make them be globals or something if you like.
  _int intAttr;
  _bool boolAttr;
  _float floatAttr;
  _string stringAttr;
#endif // Cut if you're cloning this object.
};

extern ExampleAttribHandler *attr;

#endif  // _EXAMPLEATTRIBHANDLER_H

#if 0 // Reference Stuff
// ===========================================================================
//
//  EXAMPLEATTRIB: Reference Stuff
//
//    This section should (eventually) provide the reference materials for
//  the following SDK Objects:
//
//    Attribute
//    _int
//    _bool
//    _float
//    _string
//
//    AttrCallback
//    int_attrCB
//    AttrHandler
//
//    As always, if you have any specific questions about these objects,
//  please do feel free to also check the NSDN site, as well as the wasabi
//  developers' forum.



// ===========================================================================
//
//  EXAMPLEATRRIB: Attribute Reference
//
//  Attribute is publicly inherited from DependentI and privately inherited from Named
//
//  Attributes have three primary features:
//    1 - a Type (int, bool, float, or string)
//    2 - a Value (of the preceeding type)
//    3 - a Name (which will be displayed in the configuration component)
//
//  You don't use Attribute directly, you use its children, but these are the
//  methods that _int, _bool, _float, and _string have in common.


  // --------------------------------------------
  // Public Methods

  void setName(const char *newname);
  // Change the name of the object

  const char *getAttributeName();
  // Get the name of the object

  const char *getAttributeDesc();
  // Get the description of the object (not used?  will return NULL with attributes?)

  virtual int getAttributeType();
  // Overridden individually by _int/_bool/_float/_string

  virtual const char *getConfigGroup() { return NULL; }	// override me
  // This is only called if the kernel doesn't have a default config group
  // set for your type already

  int getDataLen();
  int getData(char *data, int data_len);
  int setData(const char *data, bool def=false);  // def specifies to set a new default value
  // Internally the data is saved in string format.
  // These functions get/set the data as string data.

  int getValueAsInt();
  int setValueAsInt(int newval, bool def=false);  // def specifies to set a new default value
  // These functions get/set the data as int data.

  double getValueAsDouble();
  double setValueAsDouble(double newval, bool def=false);  // def specifies to set a new default value
  // These functions get/set the data as float data.



// ===========================================================================
//
//  EXAMPLEATRRIB: _int Reference
//
//  _int is inherited from Attribute
//
//  Guess what its type is?

  
  // --------------------------------------------
  // Constructor

  _int(const char *name=NULL, int default_val=0)
  // Here is where you specify the attribute's name and its default value
  // ie: you don't get to have uninitialized data bugs.  so sorry.


  // --------------------------------------------
  // Public Methods

  virtual int getAttributeType() { return AttributeType::INT; }
  virtual const char *getConfigGroup() { return "studio.configgroup.int"; }
  // Yatta!

  operator int() { return getValueAsInt(); }
  int operator =(int newval) { return setValueAsInt(newval) ? newval : getValueAsInt(); }
  // Convenience operators
  // This allows you to treat this object as if it were a bool type.


  
  
// ===========================================================================
//
//  EXAMPLEATRRIB: _bool Reference
//
//  _bool is inherited from Attribute
//
//  Guess what its type is?

  
  // --------------------------------------------
  // Constructor

  _bool(const char *name=NULL, int default_val=0)
  // Here is where you specify the attribute's name and its default value
  // ie: you don't get to have uninitialized data bugs.  so sorry.


  // --------------------------------------------
  // Public Methods

  virtual int getAttributeType() { return AttributeType::BOOL; }
  virtual const char *getConfigGroup() { return "studio.configgroup.bool"; }
  // Yatta!

  operator bool() { return getValueAsInt(); }
  bool operator =(bool newval) { return setValueAsInt(newval) ? newval : getValueAsInt(); }
  // Convenience operators
  // This allows you to treat this object as if it were a bool type.

  
  
// ===========================================================================
//
//  EXAMPLEATRRIB: _float Reference
//
//  _float is inherited from Attribute

  
  // --------------------------------------------
  // Constructor

  _float(const char *name=NULL, double default_val=0.0)
  // Here is where you specify the attribute's name and its default value
  // ie: you don't get to have uninitialized data bugs.  so sorry.


  // --------------------------------------------
  // Public Methods

  virtual int getAttributeType() { return AttributeType::FLOAT; }
  virtual const char *getConfigGroup() { return "studio.configgroup.float"; }
  // Yatta!

  operator double() { return getValueAsDouble(); }
  double operator =(double newval) { return setValueAsDouble(newval) ? newval : getValueAsDouble(); }
  // Convenience operators
  // This allows you to treat this object as if it were a double type.


  
  
// ===========================================================================
//
//  EXAMPLEATRRIB: _string Reference
//
//  _string is inherited from Attribute

  
  // --------------------------------------------
  // Constructor

  _string(const char *name=NULL, const char *default_val=NULL)
  // Here is where you specify the attribute's name and its default value
  // ie: you don't get to have uninitialized data bugs.  so sorry.


  // --------------------------------------------
  // Public Methods

  virtual int getAttributeType() { return AttributeType::STRING; }
  virtual const char *getConfigGroup() { return "studio.configgroup.string"; }
  // Yatta!

  const char *getValue();
  int setValue(const char *val);
  // Set and get the value using method calls

  operator const char *() { return getValue(); }
  const char *operator =(double newval) { return setValue(newval) ? newval : getValue(); }
  // Convenience operators
  // This allows you to treat this object as if it were a const char * type.
  // (But not a string, so don't use the == operator on it unless you REALLY
  // mean to compare your pointer to the pointer this object carries.  got it?)


  
  
// ===========================================================================
//
//  EXAMPLEATRRIB: AttrCallback Reference
//
//  AttrCallback is inherited from DependentViewerT<Attribute> (an icky, horrid, thing-you-don't-ever-want-to-look-at :)
//
//  AttrCallback objects are meant to be subclassed to provide your own
//  functionality in the onValueChange call.  They are bound to a single
//  attribute using the component's registerAttribute(Attribute*,AttrCallback*)
//  method (defined in CfgItemI, one of your component's grandparents)
//
//  You don't need to use this object, easier to use int_attrCB or AttribHandler.


  // --------------------------------------------
  // Public Methods

  virtual void onValueChange(Attribute *attr)=0;
  // Override this method to get told that your attribute changed.

  void setAttribute(Attribute *attr);
  // This method is called by the component to bind the attribute to the callback.
  // It is not necessary to call or override this method.



// ===========================================================================
//
//  EXAMPLEATRRIB: int_attrCB Reference
//
//  int_attrCB is inherited from AttrCallback
//
//  int_attrCB is a simple callback object to bind a function call (off one
//  of your own objects) to a _bool or _int attribute.


  // --------------------------------------------
  // Type definitions.

  typedef void (*fnPtrType)(int);
  // This is the type of function you must provide.


  // --------------------------------------------
  // Public Methods

  int_attrCB(fnPtrType _fn);
  // And you simply specify it in the constructor.
  // It'll get called with the new value if it changes.



// ===========================================================================
//
//  EXAMPLEATRRIB: AttrHandler Reference
//
//  AttrHandler is inherited from AttrCallback
//
//  This object allows you to centralize the callback handling if you have a
//  large number of attribute variables to keep track of.
//
//  AttrHandler is a template which allows you to bind index values to your
//  Attributes and will then call 4 methods on the templated object to inform
//  you when your attribute values are changed (by the configuration editor
//  window, et al).
//
//  It handles some of the housekeeping issues for you, but you have to bind
//  your component and your callback object into it (using the provided methods)
//  or it obviously won't work.
//
//  Lastly, while your attribs themselves need to be unique objects with custom
//  initialization (and can be globals, if you prefer), this object MUST be
//  allocated new (in your component's constructor).  The API will delete it,
//  so you can't have it be anything static.

  template <class TCallback>  // :)
  // --------------------------------------------
  // Public Methods

  virtual void bindComponent(WAComponentClient *parentcomponent);
  // Call this method to bind your component

  virtual void bindCallbackObj(TCallback *callbackobj);
  // Call this method to bind your callback

  virtual void registerAttribute(Attribute *attr, int id) {
  // Call this method in your component's constructor to register each attribute.

#if 0  
  virtual void onIntChange(int id, int *attr);
  virtual void onBoolChange(int id, bool *attr);
  virtual void onFloatChange(int id, double *attr);
  virtual void onStringChange(int id, const char *attr);
  // Your callback object (probably your primary window class) must implement
  // its own versions of these methods here.  They will be called by the 
  // switch statement below.
#endif

  virtual void onValueChange(Attribute *attr);
  // This is an override of AttrCallback where we multiplex the callback events. 
  // You don' needta do nothink vid dat one, mang.





#endif// Reference Stuff
