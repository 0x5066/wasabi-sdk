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

#ifndef _ENCODEDSTR_H
#define _ENCODEDSTR_H

#include "../studio/services/svc_stringtypes.h"

class String;

class COMEXP EncodedStr {
public:
  // The EncodedStr object will automatically delete its ram, unless you 
  // specify 0 in that "delete it" parameter there, partner.
  EncodedStr(FOURCC encodingType = 0, void *encodedBuffer = NULL, int bufferSize = 0, int deleteIt = 1);
  ~EncodedStr();

  // A "reset" will ensure any previously set buffer will be deleted
  void resetBuffer(FOURCC encodingType, void *encodedBuffer, int bufferSize, int deleteIt = 1);

  // All the calls to the service level functions are through here.
  int convertToUTF8(String &output_str);
  // This method will reset this object (ie: delete RAM if necessary)
  int convertFromUTF8(FOURCC encodingType, const String &input_str);

  // Accessor inlines
  inline FOURCC getEncodingType() { return encoding_type; }
  inline void * getEncodedBuffer() { return encoded_buffer; }
  inline int getBufferSize() { return buffer_size; }
  inline int getDeleteIt() { return delete_it; }

// This is for debugging.
int operator ==(const EncodedStr &in_string);

private:
  FOURCC encoding_type;
  void *encoded_buffer;
  int buffer_size;
  int delete_it;
};


#endif//_ENCODEDSTR_H


