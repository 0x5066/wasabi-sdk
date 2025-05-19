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

#ifndef _SVC_EVAL_H
#define _SVC_EVAL_H

#include "../../bfc/dispatch.h"
#include "services.h"

class NOVTABLE svc_evaluator : public Dispatchable {
public:
  static FOURCC getServiceType() { return WaSvc::EVALUATOR; }

  const char *getEvalType();	// "php", "perl", "math", "xmlgen", etc.

// these are for future optimization -- BU
  //  void assignVar(const char *name, const char *value);
  //  const char *getVarValue(const char *name);
  //  int getVarIndex(const char *name);
  //  const char *getVarValueByIndex(int pos);

  int setEvalString(const char *string, int length, BOOL isBinary);
  // free the returned memory with api->sysFree()
  const char *evaluate(int *length, BOOL *isBinary);

protected:
  enum {
    GETEVALTYPE, ASSIGNVAR, GETVARVALUE, GETVARINDEX, GETVARVALUEBYINDEX,
    SETEVALSTRING, EVALUATE
  };
};

inline
const char *svc_evaluator::getEvalType() {
  return _call(GETEVALTYPE, (const char *)NULL);
}

inline
int svc_evaluator::setEvalString(const char *string, int length, BOOL isBinary){
  return _call(SETEVALSTRING, FALSE, string, length, isBinary);
}

inline
const char *svc_evaluator::evaluate(int *length, BOOL *isBinary) {
  return _call(EVALUATE, (const char *)NULL, length, isBinary);
}

// implementor derives from this one
class NOVTABLE svc_evaluatorI : public svc_evaluator {
public:
  virtual const char *getEvalType()=0;

//  void assignVar(const char *name, const char *value);
//  const char *getVarValue(const char *name);
//  int getVarIndex(const char *name);
//  const char *getVarValueByIndex(int pos);

  // implementor should make a copy of the string (if needed)
  virtual int setEvalString(const char *string, int length, BOOL isBinary)=0;
  // implementor should alloc returned mem w/ api->sysMalloc()
  virtual const char *evaluate(int *length, BOOL *isBinary)=0;

protected:
  RECVS_DISPATCH;
};

#endif
