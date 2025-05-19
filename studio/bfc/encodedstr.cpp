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

// someday, there will be a file here.
#include "std.h"
#include "encodedstr.h"
#include "string.h"
#include "../studio/services/svc_stringconverter.h"
#include "../studio/api.h"

EncodedStr::EncodedStr(FOURCC encodingType, void *encodedBuffer, int bufferSize, int deleteIt) {
  encoding_type = encodingType;
  encoded_buffer = encodedBuffer;
  buffer_size = bufferSize;
  delete_it = deleteIt;
}

EncodedStr::~EncodedStr() {
  if (delete_it && (encoded_buffer != NULL)) {
    api->sysFree(encoded_buffer);
  }
}

void EncodedStr::resetBuffer(FOURCC encodingType, void *encodedBuffer, int bufferSize, int deleteIt) {
  // if there's someone already there, toss them.
  if (delete_it && (encoded_buffer != NULL)) {
    api->sysFree(encoded_buffer);
  }
  encoding_type = encodingType;
  encoded_buffer = encodedBuffer;
  buffer_size = bufferSize;
  delete_it = deleteIt;
}

int EncodedStr::convertToUTF8(String &output_str) {
  int retval = 0;
  StringConverterEnum myServiceEnum(encoding_type);
  svc_stringConverter *myConv = myServiceEnum.getFirst();
  if (myConv != NULL) {

    void *in_buffer = encoded_buffer;
    int size_in_bytes = buffer_size; 

    if (encoded_buffer != NULL) {
      // Preflight
      int u8size = myConv->preflightToUTF8(encoding_type, in_buffer, size_in_bytes);
      // Alloc
      char *u8str = reinterpret_cast<char *>(api->sysMalloc(u8size));
      if (u8str != NULL) {
        // Convert
        retval = myConv->convertToUTF8(encoding_type, in_buffer, size_in_bytes, u8str, u8size);
        if (retval < 0) {
          // Clear on error.
          api->sysFree(u8str);
          u8str = NULL;
        }
      } else {
        ASSERTPR(u8str != NULL, "Malloc failed in string converter\n");
      }
      // And call the method to inject the pointer into our string (cleared on error).
      output_str.setValue(u8str);
    }

    // Once we use our service, release our locked instance of it.
    myServiceEnum.release(myConv);
  } else {
    // Clear the string on error.
    retval = SvcStrCnv::ERROR_UNAVAILABLE;
    output_str.setValue(NULL);
  }
  return retval;
}

int EncodedStr::convertFromUTF8(FOURCC encodingType, const String &inputStr) {
  int retval = 0;
  int size = 0, written = 0;
  void *buffer = NULL;
  StringConverterEnum myServiceEnum(encodingType);
  svc_stringConverter *myConv = myServiceEnum.getFirst();

  if (myConv != NULL) {
    if (inputStr != NULL) {
      const char *val = inputStr.getValue();
      int valsize = STRLEN(val) + 1; // count the null char in your size-in-bytes!
      // Preflight
      size = myConv->preflightFromUTF8(encodingType, val, valsize);
      if (size > 0) {
        // Alloc
        buffer = api->sysMalloc(size);
        if (buffer != NULL) {
          // Convert
          written = myConv->convertFromUTF8(encodingType, val, valsize, buffer, size);
          if (written > 0) {
            retval = written;
          } else {
            // Clear on error.
            api->sysFree(buffer);
            buffer = NULL;
            retval = written;
            written = 0;
          }
        } else {
          ASSERTPR(buffer != NULL, "Malloc failed in string converter\n");
        }
      } else {
        // Clear on error.
        buffer = NULL;
        retval = size;
        written = 0;
      }
    }
    // Once we use our service, release our locked instance of it.
    myServiceEnum.release(myConv);
  } else {
    // On error locking down a service, all the default values are errors and called through resetBuffer.
    retval = SvcStrCnv::ERROR_UNAVAILABLE;
  }
  resetBuffer(encodingType, buffer, written);
  return retval;
}

// This is for debugging.
int EncodedStr::operator ==(const EncodedStr &in_string) {
  if (encoding_type == in_string.encoding_type) {
    switch (encoding_type) {
      case SvcStrCnv::OSNATIVE:
        return (STRCMP(reinterpret_cast<char *>(encoded_buffer), reinterpret_cast<char *>(in_string.encoded_buffer)) == 0);
      break;
      default:
        return 0;
      break;
    }
  }
  return 0;
}
