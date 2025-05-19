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

#ifndef _SVC_STRINGCONVERTER_H
#define _SVC_STRINGCONVERTER_H

#include "../../bfc/dispatch.h"
#include "services.h"
#include "svc_stringtypes.h"

// Porting Rule: A new service to respond to at least OSNATIVE and UTF16 shall be
// provided for every new platform.  This service should provide transcoding
// to and from the platform's native internationalized string encoding format
// (ie: MBCS under Win9x, WorldScript on the Mac, etc etc etc).and to and from
// UTF16.  If the OSNATIVE string _IS_ UTF16, just respond that you can convert
// both.

class NOVTABLE svc_stringConverter : public Dispatchable {
public:
  static FOURCC getServiceType() { return WaSvc::STRINGCONVERTER; }

  // test the type.
  int canConvert(FOURCC encoding_type);

  // The return code is the number of bytes written to the output buffer, or the error.
  // A return code of 0 is not an error, other than you passing NULL or a size 0 buffer won't do much interesting.
  int convertToUTF8(FOURCC encoding_type, const void *in_buffer, int size_in_bytes, char *out_buffer, int size_out_bytes);
  int preflightToUTF8(FOURCC encoding_type, const void *in_buffer, int size_in_bytes);
  int convertFromUTF8(FOURCC encoding_type, const char *in_buffer, int size_in_bytes, void *out_buffer, int size_out_bytes);
  int preflightFromUTF8(FOURCC encoding_type, const char *in_buffer, int size_in_bytes);

protected:
  enum {
    CANCONVERT,
    CONVERTTOUTF8,
    PREFLIGHTTOUTF8,
    CONVERTFROMUTF8,
    PREFLIGHTFROMUTF8
  };
};


inline
int svc_stringConverter::canConvert(FOURCC encoding_type) {
  return _call(CANCONVERT, (int)0, encoding_type);
}

inline
int svc_stringConverter::convertToUTF8(FOURCC encoding_type, const void *in_buffer, int size_in_bytes, char *out_buffer, int size_out_bytes) {
  return _call(CONVERTTOUTF8, (int)0, encoding_type, in_buffer, size_in_bytes, out_buffer, size_out_bytes);
}

inline
int svc_stringConverter::preflightToUTF8(FOURCC encoding_type, const void *in_buffer, int size_in_bytes) {
  return _call(PREFLIGHTTOUTF8, (int)0, encoding_type, in_buffer, size_in_bytes);
}

inline
int svc_stringConverter::convertFromUTF8(FOURCC encoding_type, const char *in_buffer, int size_in_bytes, void *out_buffer, int size_out_bytes) {
  return _call(CONVERTFROMUTF8, (int)0, encoding_type, in_buffer, size_in_bytes, out_buffer, size_out_bytes);
}

inline
int svc_stringConverter::preflightFromUTF8(FOURCC encoding_type, const char *in_buffer, int size_in_bytes) {
  return _call(PREFLIGHTFROMUTF8, (int)0, encoding_type, in_buffer, size_in_bytes);
}

// implementor derives from this one
class NOVTABLE svc_stringConverterI : public svc_stringConverter {
public:

  // test the type
  virtual int canConvert(FOURCC encoding_type) = 0;

  virtual int convertToUTF8(FOURCC encoding_type, const void *in_buffer, int size_in_bytes, char *out_buffer, int size_out_bytes) = 0;
  virtual int preflightToUTF8(FOURCC encoding_type, const void *in_buffer, int size_in_bytes) = 0;
  virtual int convertFromUTF8(FOURCC encoding_type, const char *in_buffer, int size_in_bytes, void *out_buffer, int size_out_bytes) = 0;
  virtual int preflightFromUTF8(FOURCC encoding_type, const char *in_buffer, int size_in_bytes) = 0;

protected:
  RECVS_DISPATCH;
};

#include "../../bfc/svc_enum.h"

class StringConverterEnum : public SvcEnumT<svc_stringConverter> {
public:
  StringConverterEnum(FOURCC enc_type) : encoding_type(enc_type) {}
protected:
  virtual int testService(svc_stringConverter *svc) {
    return (svc->canConvert(encoding_type));
  }
private:
  FOURCC encoding_type;
};

#endif // _SVC_STRINGCONVERTER_H
