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

#include "std.h"
#include "string.h"
#include "../common/nsguid.h"
#include "../bfc/svc_enum.h"

String::String(const char *initial_val) :
  val(NULL)
{
  setValue(initial_val);
}

String::String(const String &s) :
  val(NULL)
{
  setValue(s.getValue());
}

String::String(const String *s) :
  val(NULL)
{
  setValue(s->getValue());
}

String::~String() {
  FREE(val);
}

const char *String::getValueSafe(const char *def_val) const {
  if (val == NULL)
    return def_val;
  else
    return val;
}

int String::getChar(int pos) {
  return val[pos];
}
int String::setChar(int pos, int value) {
  return val[pos] = value;
}

const char *String::setValue(const char *newval) {
  if (newval != val)  {
    if (newval == NULL) {
      FREE(val);
      val = NULL;
    } else {
      int len = STRLEN(newval);
      if (val != NULL)
        val = (char *)REALLOC(val, len+1);
      else
        val = (char *)MALLOC(len+1);
      ASSERT(newval != NULL);
      MEMCPY(val, newval, len+1);
    }
  }
  return getValue();
}

int String::len() const {
  return STRLEN(getValue());
}

int String::isempty() const {
  return (!val || !*val);
}

void String::toupper() {
  if (!isempty()) for (char *p = val; *p; p++) *p = TOUPPER(*p);
}

void String::tolower() {
  if (!isempty()) for (char *p = val; *p; p++) *p = TOLOWER(*p);
}

int String::isequal(const char *otherval) const {
  return !STRCMPSAFE(getValue(), otherval);
}

int String::iscaseequal(const char *otherval) const {
  return !STRICMPSAFE(getValue(), otherval);
}

int String::islessthan(const char *otherval) const {
  return STRCMPSAFE(getValue(), otherval) < 0;
}

void String::changeChar(int from, int to) {
  if (val == NULL) return;
  int length = len();
  for (int i = 0; i < length; i++)
    if (val[i] == from) val[i] = to;
}

int String::lastChar() {
  if (isempty()) return -1;
  return val[len()-1];
}

const char *String::printf(const char *format, ...) {
  va_list args;
  va_start (args, format);
  vsprintf(format,args);
  va_end(args);
  return getValue();
}

const char *String::cat(const char *value) {
  if (value == NULL || *value == 0) return getValue();
  if (val == NULL) return setValue(value);
  return setValue(StringPrintf("%s%s", getValue(), value));
}

const char *String::prepend(const char *value) {
  if (value == NULL || *value == 0) return getValue();
  if (val == NULL) return setValue(value);
  return setValue(StringPrintf("%s%s", value, getValue()));
}

// replaces string with n chars of val or length of val, whichever is less.
const char *String::ncpy(const char *newstr, int numchars) {
  val = (char *)REALLOC(val, numchars+1);
  val[numchars] = 0;
  STRNCPY(val,newstr,numchars);
  return getValue();
}


// -----------------------------------------
// Character based find-n-splice methods --
// "l" and "r" prefixes specify to begin at
// front or back of string:

#ifdef FAST_METHODS
#undef FAST_METHODS
#endif//FAST_METHODS
#ifdef SAFE_METHODS
#undef SAFE_METHODS
#endif//SAFE_METHODS
#ifdef USE
#undef USE
#endif//USE

#define FAST_METHODS 1
#define SAFE_METHODS 0

#define USE FAST_METHODS

// Returns index of first found, -1 if not found.
int String::lFindChar(char findval) {
  int length = len();
  for (int i = 0; i < length; i++) {
    if (val[i] == findval) {
      return i;
    }
  }
  return -1;
}

// Same as above, save the "findval" is a string where it searches
// for any of the characters in the string.
int String::lFindChar(const char * findval) {
  int length = len();
  int numchars = STRLEN(findval);
  for (int i = 0; i < length; i++) {
    for (int j = 0; j < numchars; j++) {
      if (val[i] == findval[j]) {
        return i;
      }
    }
  }
  return -1;
}

int String::rFindChar(char findval) {
  int length = len();
  for (int i = length-1; i > 0; i--) {
    if (val[i] == findval) {
      return i;
    }
  }
  return -1;
}

// Same as above, save the "findval" is a string where it searches
// for any of the characters in the string.
int String::rFindChar(const char * findval) {
  int length = len();
  int numchars = STRLEN(findval);
  for (int i = length-1; i > 0; i--) {
    for (int j = 0; j < numchars; j++) {
      if (val[i] == findval[j]) {
        return i;
      }
    }
  }
  return -1;
}

// Splits string at findval.  Characters passed by search, including the 
// found character, are MOVED to the returned string.  If there is no char
// to be found, the entire string is returnef and the called instance is
// left empty.  (Makes looped splits very easy).
String String::lSplit(int idxval) {
  if (idxval == -1) { // Not Found
    // Copy our contents to return on the stack
    String retval(val);
    // And zero the string.
    if (val) {
      val[0] = 0;
    }
    return retval;
  } else {
    String retval;
    // Copy into retval the number of characters to the found char index.
    retval.ncpy(val,idxval+1);
    {
      String testscope;
      // Copy into retval the number of characters to the found char index.
      testscope.ncpy(val,idxval+1);
    }
#if USE == FAST_METHODS
    int len = strlen(val+idxval+1);
    MEMCPY(val,val+idxval+1,len+1);
#elif USE == SAFE_METHODS
    // Copy from the found index downwards to save for this object
    String temp(val + idxval + 1);
    // And then copy into ourselves the tempspace.
    *this = temp;
#endif
    return retval;
  }
  // this will never be hit.  many compilers are too stupid to realize this.
  return String();
}

String String::lSplitChar(char findval) {
  // The index of the found character
  int idxval = lFindChar(findval);
  return lSplit(idxval);
}

String String::lSplitChar(const char * findval) {
  // The index of the found character
  int idxval = lFindChar(findval);
  return lSplit(idxval);
}

String String::rSplit(int idxval) {
  if (idxval == -1) { // Not Found
    // Copy our contents to return on the stack
    String retval(val);
    // And zero the string.
    val[0] = 0;
    return retval;
  } else {
    // Copy from the found index downwards to the retval
    String retval(val + idxval);
    // Terminate the found char index
    val[idxval] = 0;
    // That was easier, wasn't it?
    return retval;
  }
  // this will never be hit.  many compilers are too stupid to realize this.
  return String();
}

String String::rSplitChar(char findval) {
  // The index of the found character
  int idxval = rFindChar(findval);
  return rSplit(idxval);
}

String String::rSplitChar(const char * findval) {
  // The index of the found character
  int idxval = rFindChar(findval);
  return rSplit(idxval);
}

// Same as split, except the find char is cut completely.
String String::lSpliceChar(char findval) {
  // Auto-scope reference allows us to avoid a copy.
  String & retval = lSplitChar(findval);
  
  // We need to strip the findval char, which is the end char.
  int end = retval.len();
  if (end) {
    if (retval.val[end-1] == findval) {
      retval.val[end-1] = 0;
    }
  }
  return retval;
}

// Same as split, except the find char is cut completely.
String String::lSpliceChar(const char * findval) {
  // Auto-scope reference allows us to avoid a copy.
  String & retval = lSplitChar(findval);
  // We need to strip the findval char, which is the end char.
  int end = retval.len();
  int num = STRLEN(findval);
  if (end) {
    for (int i = 0; i < num; i++) {
      if (retval.val[end-1] == findval[i]) {
        retval.val[end-1] = 0;
      }
    }
  }
  return retval;
}

String String::rSpliceChar(char findval) {
  // Auto-scope reference allows us to avoid a copy.
  String & retval = rSplitChar(findval);
  // We need to strip the findval char, which is the first char.
  // (But we still check for empty string:)
  int end = retval.len();
  if (end) {
    if (retval.val[0] == findval) {
#if USE == FAST_METHODS
      int len = strlen(retval.val+1);
      MEMCPY(retval.val,retval.val+1,len+1);
#elif USE == SAFE_METHODS
      String temp(retval.val + 1);
      retval = temp;
#endif
      return retval;
    }
  }
  return retval;
}

String String::rSpliceChar(const char * findval) {
  // Auto-scope reference allows us to avoid a copy.
  String & retval = rSplitChar(findval);
  // We need to strip the findval char, which is the first char.
  // (But we still check for empty string:)
  int end = retval.len();
  int num = STRLEN(findval);
  if (end) {
    for (int i = 0; i < num; i++) {
      if (retval.val[0] == findval[i]) {
#if USE == FAST_METHODS
      int len = strlen(retval.val+1);
      MEMCPY(retval.val,retval.val+1,len+1);
#elif USE == SAFE_METHODS
        String temp(retval.val + 1);
        retval = temp;
#endif
        return retval;
      }
    }
  }
  return retval;
}

int String::replace(const char *find, const char *replace) {
  ASSERT( find != NULL );
  ASSERT( replace != NULL );

  if ( len() == 0 ) 
    return 0;

  int find_count = 0;

  char *p, *p2;
  int rep_len = STRLEN( replace );
  int find_len = STRLEN( find );
  int size_diff = rep_len - find_len;

  if ( size_diff == 0 ) {
    p = val;
    while( p = STRSTR( p, find  ) ) {
      STRNCPY( p, replace, rep_len );
      p += find_len;
      find_count++;
    }
  } else {

    char *new_buf, *in;

    p = val;
    while( p = STRSTR( p, find ) ) {
      find_count++;
      p += find_len;
    }

    new_buf = (char *)MALLOC( len() + find_count * size_diff + 1 );

    p = val;
    in = new_buf;
    while( p2 = STRSTR( p, find ) ) {
      STRNCPY( in, p, p2 - p );
      in += p2 - p;
      STRNCPY( in, replace, rep_len ); 
      in += rep_len;
      p = p2 + find_len;
    }
    STRCPY( in, p );
    new_buf[ len() + find_count * size_diff ] = 0;

    setValue( new_buf );
    FREE( new_buf );
  }
  return find_count;
}

#undef USE
#undef SAFE_METHODS
#undef FAST_METHODS


int String::numCharacters() {
  // count newsize characters over how many bytes?
  int count, bytes;
  for (bytes = 0, count = 0; val[bytes]; count++, bytes++) {
    // If we encounter a lead byte, skip over the trail bytes.
    switch (val[bytes] & 0xC0) {
      case 0x80: // trail bytes
        // THIS SHOULD NEVER EVER EVER EVER HAPPEN!
        // but we'll fall through anyhow, just in case someone
        // sends us non-UTF8.
      case 0xC0: // lead bytes
        do {
          bytes++;
          if (val[bytes] == 0) {
            // if people are giving us lame encodings, break here.
            break;
          }
        } while ((val[bytes+1] & 0xC0) == 0x80);
      break;
    }
  }
  return count;
}


void String::trunc(int newlen) {
  if (val == NULL) return;
  int oldlen = numCharacters();
  if (newlen < 0) newlen = MAX(oldlen + newlen, 0);
  if (newlen >= oldlen) return;

  // count newsize characters over how many bytes?
  int count, bytes;
  for (bytes = 0, count = 0; count < newlen; count++, bytes++) {
    // If we encounter a lead byte, skip over the trail bytes.
    switch (val[bytes] & 0xC0) {
      case 0x80: // trail bytes
        // THIS SHOULD NEVER EVER EVER EVER HAPPEN!
        // but we'll fall through anyhow, just in case someone
        // sends us non-UTF8.
      case 0xC0: // lead bytes
        do {
          bytes++;
        } while ((val[bytes+1] & 0xC0) == 0x80);
      break;
    }
  }

  val[bytes] = 0;
}

void String::vsprintf(const char *format, va_list args) {
  if(!format) return;
  
  va_list saveargs=args;
  // roughly evaluate size of dest string
  const char *p=format;
  int length=0;
  while(*p) {
    if(*(p++)!='%') length++;
    else {
      void *arg=va_arg(args, void *);
      for (;;) {
        const char f=*p++;
        if(f=='c') length++;
        else if(f=='i') length+=16;
        else if(f=='u') length+=16;
        else if(f=='d' || f=='f') length+=32;
        else if(f=='x') length+=32; // Hex with LC Alphas: 0x0009a64c
        else if(f=='X') length+=32; // Hex with UC Alphas: 0x0009A64C
        else if(f=='s') { // ::vsrintf can properly handle null.
          if (arg == NULL) {
            length+=STRLEN("(null)"); // Just to be explicit.
          } else {
            length+=STRLEN((const char *)arg);
          }
        }
        else if(ISDIGIT(f)) continue;
        else if(f=='%') length++;
        else ASSERTPR(0,"undefined format passed to stringprintf!");
        break;
      }
    }
  }
  if(val) {
    if(len() < length) 
      val=(char *)REALLOC(val,length+1);
  } else val=(char *)MALLOC(length+1);

  // now write the string in val
  int real_len = ::vsprintf(val,format,saveargs);
  ASSERTPR(real_len <= length, "String.printf overflow");
}


StringPrintf::StringPrintf(const char *format, ...) {
  va_list args;
  va_start (args, format);
  vsprintf(format,args);
  va_end(args);
}

StringPrintf::StringPrintf(int value) {
  *this += value;
}

StringPrintf::StringPrintf(double value) {
  *this += StringPrintf("%f", value);
}

StringPrintf::StringPrintf(GUID g) {
  char splab[nsGUID::GUID_STRLEN+1];
  nsGUID::toChar(g, splab);
  cat(splab);
}
