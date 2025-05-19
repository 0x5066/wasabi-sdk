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

#ifndef _STRING_H
#define _STRING_H

// a very, very basic string... I just got sick of all the STRDUPs --BU
// ok, not so basic any more =) --BU

#include "../common/common.h"

class COMEXP String {
public:
  String(const char *initial_val=NULL);
  String(const String &s);
  String(const String *s);
  ~String();

  const char *getValue() const { return val; }
  operator const char *() const { return getValue(); }
  const char *getValueSafe(const char *def_val="") const; // returns def_val if NULL

  int getChar(int pos);
  int setChar(int pos, int val);

  const char *setValue(const char *newval);
  char *getNonConstVal() { return const_cast<char *>(getValue()); }
  const char *operator =(const char *newval) { return setValue(newval); }
  const char *operator +=(const char *addval) {
    return cat(addval);
  }
  const char *operator +=(char addval);
  const char *operator +=(int value);
  const char *operator +=(GUID guid);

  // copy assignment operator
  String& operator =(const String &s) {
    if (this != &s)
      setValue(s);
    return *this;
  }

  // comparator operators
  inline int operator ==(const char *val) const {
    if(!val) return isempty();
    return isequal(val);
  }
  inline int operator <(const char *val) const {
    return islessthan(val);
  }
  inline int operator !=(const char *val) const {
    if(!val) return !isempty();
    return !isequal(val);
  }
  inline int operator >(const char *val) const {
    return (!islessthan(val)) && (!isequal(val));
  }
  inline String operator + (const char * val) {
    String retval = *this;
    return retval += val;
  }
  inline int operator ==(const String &val) const {
    return isequal(val);
  }
  inline int operator <(const String &val) const {
    return islessthan(val);
  }
  inline int operator !=(const String &val) const {
    return !isequal(val);
  }
  inline int operator >(const String &val) const {
    return (!islessthan(val)) && (!isequal(val));
  }
  inline String operator + (const String &val) {
    String retval = *this;
    return retval += val;
  }
  inline String operator + (const char val) {
    String retval = *this;
    return retval += val;
  }

  int len() const;
  int isempty() const;
  void toupper();	// converts entire string to uppercase
  void tolower();	// converts entire string to lowercase
  int isequal(const char *val) const;	// basically !strcmp
  int iscaseequal(const char *val) const;	// basically !strcasecmp
  int islessthan(const char *val) const;	// basically strcmp < 0

  void changeChar(int from, int to);

  int lastChar();	// -1 if empty

  // replaces string
  const char *printf(const char *format, ...);
  // adds to the end
  const char *cat(const char *value);
  // inserts at beginning
  const char *prepend(const char *value);
  // replaces string with n chars of val or length of val, whichever is less.
  const char *ncpy(const char *newstr, int numchars);

  // -----------------------------------------
  // Character based find-n-splice methods --
  // "l" and "r" prefixes specify to begin at
  // front or back of string:

  // Returns index of first found, -1 if not found.
  int lFindChar(char findval);  
  int lFindChar(const char * findval);  // a list of chars to search for
  int rFindChar(char findval);  
  int rFindChar(const char * findval);  // a list of chars to search for
  
  // Splits string at findval.  Characters passed by search, including the 
  // found character, are MOVED to the returned string.  If there is no char
  // to be found, the entire string is returned and the called instance is
  // left empty.  (Makes looped splits very easy).
  String lSplit(int idxval);  
  String lSplitChar(char findval);  
  String lSplitChar(const char *findval);  
  String rSplit(int idxval);  
  String rSplitChar(char findval);  
  String rSplitChar(const char *findval);  

  // Same as split, except the find char is cut completely.
  String lSpliceChar(char findval);
  String lSpliceChar(const char * findval);
  String rSpliceChar(char findval);  
  String rSpliceChar(const char * findval);

  // returns how many found
  int replace(const char *find, const char *replace);



  // UTF8-Aware "Character Based" Methods
  int numCharacters();
  void trunc(int newlen);


protected:
  void vsprintf(const char *format, va_list args);

  char *val;
  enum { wastage_allowed = 128 };
};

// use this one if you want to use a printf-style constructor
class COMEXP StringPrintf : public String {
public:
  StringPrintf(const char *format=NULL, ...);
  StringPrintf(int value);
  StringPrintf(double value);
  StringPrintf(GUID g);
};

class StringToLower : public String {
public:
  StringToLower(const char *val=NULL) : String(val) {
    tolower();
  }
};
class StringToUpper : public String {
public:
  StringToUpper(const char *val=NULL) : String(val) {
    toupper();
  }
};

#define RecycleString String

inline const char *String::operator +=(int value)
{
  return cat(StringPrintf("%i",value));
}

inline const char *String::operator +=(char value)
{
  return cat(StringPrintf("%c",value));  // Uhm.  Shouldn't the string be given
                                         // the "Fast" methods and the Printf be
                                         // built off of that?
}

inline const char *String::operator +=(GUID guid) {
  return cat(StringPrintf(guid));
}

//
//  Global operator overrides to allow string to take over for
//  the use of standard operators with const char pointers as
//  left hand operands.
inline int operator ==(const char *v1, const String &v2) {
  return String(v1) == v2;
}
inline int operator !=(const char *v1, const String &v2) {
  return String(v1) != v2;
}
inline int operator <(const char *v1, const String &v2) {
  return String(v1) < v2;
}
inline int operator >(const char *v1, const String &v2) {
  return String(v1) > v2;
}




class StringComparator {
public:
  // comparator for sorting
  static int compareItem(String *p1, String* p2) {
    return STRCMP(p1->getValue(), p2->getValue());
  }
  // comparator for searching
  static int compareAttrib(char *attrib, String *item) {
    return STRCMP(attrib, item->getValue());
  }
};

#endif
