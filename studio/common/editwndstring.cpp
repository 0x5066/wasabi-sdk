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

// ===========================================================================
//
//    NULLSOFT WASABI SDK MODULE & EXAMPLE CODE
//
//      File:     editwndstring.cpp
//
//!##   Purpose:  The EditWndString object both extends the EditWnd object to
//!##             simplify and streamline the use of an EditWnd in your wasabi
//!##             components.  In addition, this module serves as a tutorial
//!##             to instruct wasabi developers on how to properly extend our
//!##             current objects to provide new functionality.
//
//

//CT> not used anymore
#if 0
#include "../common/editwndstring.h"

// ===========================================================================
//
//    Design Notes
//
// EditWndString is intended to be a drop-in replacement for EditWnd, whose
// purpose is to internalize the need to keep around an allocated buffer
// for the internals of EditWnd.  You _aught_ to be able to replace any
// current use of EditWnd with EditWndString without any other modification
// to the code -- if you can't, that's a bug!  Please report it to me.
// The two functionality extensions in EditWndString are:
//
//    1) Package the allocation, deallocation, and assignment of the
//    buffer required by EditWnd into the internals of this object.
//
//    2) Do not require the user to keep the buffer passed into
//      virtual void setBuffer(char * buffer, int len)
//    ie: you can call setBuffer("my text to display",2000) -- which would
//    be VERY BAD if you called that same code on EditWnd.
//
//    3) Add methods that will set the buffer as a String object or
//    get the buffer as a String object.



// ===========================================================================
//
//    Implementation Notes
//
// *** The most important thing to pay attention to, of course, when extending
// a current object to provide new or easier functionality, is that you test
// heavily to ensure that you provide _ALL_ of the functionality already
// provided by the original object.

// *** The second most important thing is to make sure your new object is
// "transparently replacable" with the object you are extending.  In this
// example, for instance, it would not be enough to simply add the String
// parameter methods to EditWnd.  Because the internals of EditWndString
// operate differently from EditWnd, it would create some severe bugs if
// I did not also override the const char * methods to also support the
// functionality presented here.

// Note that this isn't always going to be possible -- but it is of vital
// importance to your mental health later on to attempt to adhere as 
// strictly as possible to these rules.  If you don't abide by these
// rules, you will have a very miserable time debugging your code and
// you'll find that your objects aren't terribly useful beyond the single
// purpose you'd originally envisioned for it.

// All of this type of coding falls into a little-known skills set called
// "Legacy Codebase Management."  And this kind of coding is always more
// difficult (at least on a conceptual level) than simply designing code
// from scratch.  You have to think of the objects you will be supporting
// with your new objects as theoretical Black Boxes.  Break them down into
// who calls them, how they are called, and how they operate as they are
// called -- then make sure you at _least_ provide that functionality back
// in your own objects.

// Good luck, kiddies!

// *** As a final "important note," you may find that you need to have
// some methods on some of our objects promoted to being virtual in order
// to get your extensions properly created (I had to do the same thing
// with EditWnd).  If you are extending one of our objects and need to
// have us change our methods to virtual, send us an email or post in the
// WA3 development thread in the forums.  We'll work with you to try to
// help make sure you get everything done easily and properly.

EditWndString::EditWndString(char * buffer, int buflen) 
  : str_buffer(NULL), str_buflen(0), req_buflen(buflen), 
    EDITWNDSTRING_PARENT(NULL, 0) 
{
  allocFromChar(buffer,buflen);
}

EditWndString::EditWndString(const String & initParam, int buflen)
  : str_buffer(NULL), str_buflen(0), req_buflen(buflen), 
    EDITWNDSTRING_PARENT() 
{
  allocFromString(initParam);
}

EditWndString::~EditWndString() {
  deallocEWS();
}

// I find it often easier to use private "functionality methods"
// which are called by the published API methods.  This often
// gives me much more flexibility in making sure I cover all
// possible uses of my objects from both new and legacy code.
void EditWndString::setBuffer(const String & buffer, int buflen) {
  allocFromString(buffer,buflen);
}

// Note that this is part of why this object is "slow" -- it is
// copying a string as a return value on the stack.  When you
// copy that to your internal string, you then must copy the
// data from one string to the other.  Slow?  Absolutely.
// Easier than getBuffer/setBuffer?  Indubitably.
// CT> un-gay-ed
/*String EditWndString::getBuffer() {
  return String( str_buffer );
}*/

// Because the "fast" version of this object remains unchanged, you
// can (pretty much) just use the other one if you need higher
// performance from your objects.  But how much performance does
// one really need to squeeze out of one's Edit UI object?

//
//  These methods are overridden virtually to provide backwards
// compatibility with legacy code - even if you only have a pointer
// to EditWnd and not EditWndString.  This is an easily overlooked
// but very important issue in legacy codebase management.
void EditWndString::getBuffer(char *outbuf, int len) {
  STRNCPY( outbuf, str_buffer, len );
}

void EditWndString::setBuffer(char * buffer, int len) {
  allocFromChar(buffer,len);
}

void EditWndString::allocFromString(const String & fromString, int buflen) {
  // Default size of buffer is twice the current string size,
  // or the value of req_buflen -- whichever is larger.
  int strSize = fromString.len() * 2;

  // Calculate something tolerable for a buffer length.
  req_buflen = max(max(strSize,req_buflen),buflen);

  // Sets the new values for str_buffer, str_buflen
  allocEWS(req_buflen);

  // Check to make sure the alloc worked, and copy the data over
  if (str_buffer) {
    STRNCPY(str_buffer,fromString,str_buflen);
  }

  // Sets the actual EditWnd with our new values.
  EDITWNDSTRING_PARENT::setBuffer(str_buffer,str_buflen);
}

// AllocFromChar is a bit more complicated, since we have to handle
// all of the legacy calls to EditWnd through this function and there's
// lots more different code paths and odd special cases, here.
void EditWndString::allocFromChar(const char * fromChar, int len) {
  // If there's actually a string,
  if (fromChar != NULL) {
    // Interestingly, we may be called to submit our object from our own
    // buffer pointer.  Wheee!  If this is the case, we're going to be
    // VERY safe and copy our buffer to another buffer before we realloc.
    if (fromChar == str_buffer) {
      // If there is no given length, that's ODD.  Just leave it be.
      if (len) {
        // Make some tempspace, copy it here.
        char * temp_buffer = (char *)MALLOC(len+1);//new char[len];
        STRNCPY(temp_buffer,str_buffer,len);
        // Then realloc
        allocEWS(len);
        // And copy again (yawn.  repmovsd.)
        if (str_buffer) {
          STRNCPY(str_buffer,temp_buffer,str_buflen);
        }
        // Enforce the null termination where it was before.
        int str_len = STRLEN(temp_buffer);
        if (str_len) {
          str_buffer[ str_len - 1 ] = 0;
        } else {
          str_buffer[ 0 ] = 0;
        }
        // don't forget to delete our tempspace.
//        delete[] temp_buffer;
        FREE(temp_buffer);

        // And then fall through to the call to the parent object.
      }
    } else {
      // Otherwise, we just do our normal routine.

      // How big do we want our object to be?
      req_buflen = len; // while stupid, this can exist as size 0 without errors.
      // Sets the new values for str_buffer, str_buflen
      allocEWS(req_buflen);
      // And copy the thing over
      if (str_buffer) {
        STRNCPY(str_buffer,fromChar,str_buflen);
      }
       // And then fall through to the call to the parent object.
    }
  } 
  // Otherwise, if there's still a requested length,
  else if (len != 0) {
    // Again, set the object size
    req_buflen = len;
    // Sets the new values for str_buffer, str_buflen
    allocEWS(req_buflen);
    // And then fall through to the call to the parent object.
  } 
  // But if the string is null and the length is 0, collapse.
  else {
    deallocEWS();
  }

  // Sets the actual EditWnd with our new values.
  if ( (str_buffer != NULL) && (str_buflen != 0) )  {
    EDITWNDSTRING_PARENT::setBuffer(str_buffer,str_buflen);
  }
}

// Memory deallocation specific to this object & any descendants.
void EditWndString::deallocEWS() {
  if (str_buffer != NULL) {
//    delete[] str_buffer;
    FREE(str_buffer); //CT> un-gay-ed
    str_buffer = NULL;
  }
  str_buflen = 0;
}

// Memory allocation specific to this object & any descendants.
void EditWndString::allocEWS(int size) {
  // First free whatever was there before.
  deallocEWS();
  // Then make some new stuff.
//  str_buffer = new char[size];
  str_buffer = (char *)MALLOC(size+1); //CT> un-gay-ed (new char[] is BAD)
  str_buffer[size] = 0; // null terminate or we have problems.
  if (str_buffer) {
    str_buflen = size;
  }
}
#endif