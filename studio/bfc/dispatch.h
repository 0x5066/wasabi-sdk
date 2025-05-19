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

#ifndef _DISPATCH_H
#define _DISPATCH_H

#include "platform/platform.h"

class DispatchableCallback;

#pragma warning(disable: 4275)
// Disable the "identifier was truncated to '255' characters..." warning.
#pragma warning( disable : 4786 )

class NOVTABLE Dispatchable {
public:
//  // fake virtual destructor
//  void destruct() { _voidcall(DESTRUCT); }

  // this is virtual so it is visible across modules
  virtual int _dispatch(int msg, void *retval, void **params=NULL, int nparam=0)=0;

protected:
//  // protected real destructor
//  ~Dispatchable() {}
  // helper templates to implement client-side methods
  int _voidcall(int msg) {
    return _dispatch(msg, NULL);
  }

  template<class PARAM1>
  int _voidcall(int msg, PARAM1 param1) {
    void *params[1] = { &param1 };
    return _dispatch(msg, NULL, params, 1);
  }

  template<class PARAM1, class PARAM2>
  int _voidcall(int msg, PARAM1 param1, PARAM2 param2) {
    void *params[2] = { &param1, &param2 };
    return _dispatch(msg, NULL, params, 2);
  }

  template<class PARAM1, class PARAM2, class PARAM3>
  int _voidcall(int msg, PARAM1 param1, PARAM2 param2, PARAM3 param3) {
    void *params[3] = { &param1, &param2, &param3 };
    return _dispatch(msg, NULL, params, 3);
  }

  template<class PARAM1, class PARAM2, class PARAM3, class PARAM4>
  int _voidcall(int msg, PARAM1 param1, PARAM2 param2, PARAM3 param3, PARAM4 param4) {
    void *params[4] = { &param1, &param2, &param3, &param4 };
    return _dispatch(msg, NULL, params, 4);
  }

  template<class PARAM1, class PARAM2, class PARAM3, class PARAM4, class PARAM5>
  int _voidcall(int msg, PARAM1 param1, PARAM2 param2, PARAM3 param3, PARAM4 param4, PARAM5 param5) {
//    void *params[4] = { &param1, &param2, &param3, &param4, &param5 }; // mig found another bug
    void *params[5] = { &param1, &param2, &param3, &param4, &param5 };
    return _dispatch(msg, NULL, params, 5);
  }

  template<class PARAM1, class PARAM2, class PARAM3, class PARAM4, class PARAM5, class PARAM6>
  int _voidcall(int msg, PARAM1 param1, PARAM2 param2, PARAM3 param3, PARAM4 param4, PARAM5 param5, PARAM6 param6) {
//    void *params[4] = { &param1, &param2, &param3, &param4, &param5, &param6 }; // mig found another bug
    void *params[6] = { &param1, &param2, &param3, &param4, &param5, &param6 };
    return _dispatch(msg, NULL, params, 6);
  }

  template<class PARAM1, class PARAM2, class PARAM3, class PARAM4, class PARAM5, class PARAM6, class PARAM7>
  int _voidcall(int msg, PARAM1 param1, PARAM2 param2, PARAM3 param3, PARAM4 param4, PARAM5 param5, PARAM6 param6, PARAM7 param7) {
    void *params[7] = { &param1, &param2, &param3, &param4, &param5, &param6 , &param7 };
    return _dispatch(msg, NULL, params, 7);
  }

  template<class PARAM1, class PARAM2, class PARAM3, class PARAM4, class PARAM5, class PARAM6, class PARAM7, class PARAM8>
  int _voidcall(int msg, PARAM1 param1, PARAM2 param2, PARAM3 param3, PARAM4 param4, PARAM5 param5, PARAM6 param6, PARAM7 param7, PARAM8 param8) {
    void *params[8] = { &param1, &param2, &param3, &param4, &param5, &param6 , &param7 , &param8 };
    return _dispatch(msg, NULL, params, 8);
  }

  template<class PARAM1, class PARAM2, class PARAM3, class PARAM4, class PARAM5, class PARAM6, class PARAM7, class PARAM8, class PARAM9>
  int _voidcall(int msg, PARAM1 param1, PARAM2 param2, PARAM3 param3, PARAM4 param4, PARAM5 param5, PARAM6 param6, PARAM7 param7, PARAM8 param8, PARAM9 param9) {
    void *params[9] = { &param1, &param2, &param3, &param4, &param5, &param6 , &param7 , &param8 , &param9 };
    return _dispatch(msg, NULL, params, 9);
  }

  template<class PARAM1, class PARAM2, class PARAM3, class PARAM4, class PARAM5, class PARAM6, class PARAM7, class PARAM8, class PARAM9, class PARAM10>
  int _voidcall(int msg, PARAM1 param1, PARAM2 param2, PARAM3 param3, PARAM4 param4, PARAM5 param5, PARAM6 param6, PARAM7 param7, PARAM8 param8, PARAM9 param9, PARAM10 param10) {
    void *params[10] = { &param1, &param2, &param3, &param4, &param5, &param6 , &param7 , &param8 , &param9 , &param10 };
    return _dispatch(msg, NULL, params, 10);
  }


  template<class RETURN_TYPE>
  RETURN_TYPE _call(int msg, RETURN_TYPE defval) {
    RETURN_TYPE retval;
    if (_dispatch(msg, &retval)) return retval;
    return defval;
  }

  template<class RETURN_TYPE, class PARAM1>
  RETURN_TYPE _call(int msg, RETURN_TYPE defval, PARAM1 param1) {
    void *params[1] = { &param1 };
    RETURN_TYPE retval;
    if (_dispatch(msg, &retval, params, 1)) return retval;
    return defval;
  }

  template<class RETURN_TYPE, class PARAM1, class PARAM2>
  RETURN_TYPE _call(int msg, RETURN_TYPE defval, PARAM1 param1, PARAM2 param2) {
    void *params[2] = { &param1, &param2 };
    RETURN_TYPE retval;
    if (_dispatch(msg, &retval, params, 2)) return retval;
    return defval;
  }

  template<class RETURN_TYPE, class PARAM1, class PARAM2, class PARAM3>
  RETURN_TYPE _call(int msg, RETURN_TYPE defval, PARAM1 param1, PARAM2 param2, PARAM3 param3) {
    void *params[3] = { &param1, &param2, &param3 };
    RETURN_TYPE retval;
    if (_dispatch(msg, &retval, params, 3)) return retval;
    return defval;
  }

  template<class RETURN_TYPE, class PARAM1, class PARAM2, class PARAM3, class PARAM4>
  RETURN_TYPE _call(int msg, RETURN_TYPE defval, PARAM1 param1, PARAM2 param2, PARAM3 param3, PARAM4 param4) {
    void *params[4] = { &param1, &param2, &param3, &param4 };
    RETURN_TYPE retval;
    if (_dispatch(msg, &retval, params, 4)) return retval;
    return defval;
  }

  template<class RETURN_TYPE, class PARAM1, class PARAM2, class PARAM3, class PARAM4, class PARAM5>
  RETURN_TYPE _call(int msg, RETURN_TYPE defval, PARAM1 param1, PARAM2 param2, PARAM3 param3, PARAM4 param4, PARAM5 param5) {
    void *params[5] = { &param1, &param2, &param3, &param4, &param5 };
    RETURN_TYPE retval;
    if (_dispatch(msg, &retval, params, 5)) return retval;
    return defval;
  }

  template<class RETURN_TYPE, class PARAM1, class PARAM2, class PARAM3, class PARAM4, class PARAM5, class PARAM6>
  RETURN_TYPE _call(int msg, RETURN_TYPE defval, PARAM1 param1, PARAM2 param2, PARAM3 param3, PARAM4 param4, PARAM5 param5, PARAM6 param6) {
    void *params[6] = { &param1, &param2, &param3, &param4, &param5, &param6 };
    RETURN_TYPE retval;
    if (_dispatch(msg, &retval, params, 6)) return retval;
    return defval;
  }

  template<class RETURN_TYPE, class PARAM1, class PARAM2, class PARAM3, class PARAM4, class PARAM5, class PARAM6, class PARAM7>
  RETURN_TYPE _call(int msg, RETURN_TYPE defval, PARAM1 param1, PARAM2 param2, PARAM3 param3, PARAM4 param4, PARAM5 param5, PARAM6 param6, PARAM7 param7) {
    void *params[7] = { &param1, &param2, &param3, &param4, &param5, &param6, &param7 };
    RETURN_TYPE retval;
    if (_dispatch(msg, &retval, params, 7)) return retval;
    return defval;
  }

  template<class RETURN_TYPE, class PARAM1, class PARAM2, class PARAM3, class PARAM4, class PARAM5, class PARAM6, class PARAM7, class PARAM8>
  RETURN_TYPE _call(int msg, RETURN_TYPE defval, PARAM1 param1, PARAM2 param2, PARAM3 param3, PARAM4 param4, PARAM5 param5, PARAM6 param6, PARAM7 param7, PARAM8 param8) {
    void *params[8] = { &param1, &param2, &param3, &param4, &param5, &param6, &param7, &param8 };
    RETURN_TYPE retval;
    if (_dispatch(msg, &retval, params, 8)) return retval;
    return defval;
  }

  template<class RETURN_TYPE, class PARAM1, class PARAM2, class PARAM3, class PARAM4, class PARAM5, class PARAM6, class PARAM7, class PARAM8, class PARAM9>
  RETURN_TYPE _call(int msg, RETURN_TYPE defval, PARAM1 param1, PARAM2 param2, PARAM3 param3, PARAM4 param4, PARAM5 param5, PARAM6 param6, PARAM7 param7, PARAM8 param8, PARAM9 param9) {
    void *params[9] = { &param1, &param2, &param3, &param4, &param5, &param6, &param7, &param8, &param9 };
    RETURN_TYPE retval;
    if (_dispatch(msg, &retval, params, 9)) return retval;
    return defval;
  }

  template<class RETURN_TYPE, class PARAM1, class PARAM2, class PARAM3, class PARAM4, class PARAM5, class PARAM6, class PARAM7, class PARAM8, class PARAM9, class PARAM10>
  RETURN_TYPE _call(int msg, RETURN_TYPE defval, PARAM1 param1, PARAM2 param2, PARAM3 param3, PARAM4 param4, PARAM5 param5, PARAM6 param6, PARAM7 param7, PARAM8 param8, PARAM9 param9, PARAM10 param10) {
    void *params[10] = { &param1, &param2, &param3, &param4, &param5, &param6, &param7, &param8, &param9, &param10 };
    RETURN_TYPE retval;
    if (_dispatch(msg, &retval, params, 10)) return retval;
    return defval;
  }

  template <class CLASSNAME, class RETVAL>
  void cb(RETVAL (CLASSNAME::*fn)(), void *retval, void **params, int nparam) {
    *static_cast<RETVAL*>(retval) = (static_cast<CLASSNAME *>(this)->*fn)();
  }

  template <class CLASSNAME>
  void vcb(void (CLASSNAME::*fn)(), void *retval, void **params, int nparam) {
    (static_cast<CLASSNAME *>(this)->*fn)();
  }

  template <class CLASSNAME, class RETVAL, class PARAM1>
  void cb(RETVAL (CLASSNAME::*fn)(PARAM1), void *retval, void **params, int nparam) {
    PARAM1 *p1 = static_cast<PARAM1*>(params[0]);
    *static_cast<RETVAL*>(retval) = (static_cast<CLASSNAME *>(this)->*fn)(*p1);
  }

  template <class CLASSNAME, class PARAM1>
  void vcb(void (CLASSNAME::*fn)(PARAM1), void *retval, void **params, int nparam) {
    PARAM1 *p1 = static_cast<PARAM1*>(params[0]);
    (static_cast<CLASSNAME *>(this)->*fn)(*p1);
  }

  template <class CLASSNAME, class PARAM1, class PARAM2>
  void vcb(void (CLASSNAME::*fn)(PARAM1, PARAM2), void *retval, void **params, int nparam) {
    PARAM1 *p1 = static_cast<PARAM1*>(params[0]);
    PARAM2 *p2 = static_cast<PARAM2*>(params[1]);
    (static_cast<CLASSNAME *>(this)->*fn)(*p1, *p2);
  }

  template <class CLASSNAME, class RETVAL, class PARAM1, class PARAM2>
  void cb(RETVAL (CLASSNAME::*fn)(PARAM1, PARAM2), void *retval, void **params, int nparam) {
    PARAM1 *p1 = static_cast<PARAM1*>(params[0]);
    PARAM2 *p2 = static_cast<PARAM2*>(params[1]);
    *static_cast<RETVAL*>(retval) = (static_cast<CLASSNAME *>(this)->*fn)(*p1, *p2);
  }

  // 3 params
  template <class CLASSNAME, class PARAM1, class PARAM2, class PARAM3>
  void vcb(void (CLASSNAME::*fn)(PARAM1, PARAM2, PARAM3), void *retval, void **params, int nparam) {
    PARAM1 *p1 = static_cast<PARAM1*>(params[0]);
    PARAM2 *p2 = static_cast<PARAM2*>(params[1]);
    PARAM3 *p3 = static_cast<PARAM3*>(params[2]);
    (static_cast<CLASSNAME *>(this)->*fn)(*p1, *p2, *p3);
  }

  template <class CLASSNAME, class RETVAL, class PARAM1, class PARAM2, class PARAM3>
  void cb(RETVAL (CLASSNAME::*fn)(PARAM1, PARAM2, PARAM3), void *retval, void **params, int nparam) {
    PARAM1 *p1 = static_cast<PARAM1*>(params[0]);
    PARAM2 *p2 = static_cast<PARAM2*>(params[1]);
    PARAM3 *p3 = static_cast<PARAM3*>(params[2]);
    *static_cast<RETVAL*>(retval) = (static_cast<CLASSNAME *>(this)->*fn)(*p1, *p2, *p3);
  }

  // 4 params
  template <class CLASSNAME, class PARAM1, class PARAM2, class PARAM3, class PARAM4>
  void vcb(void (CLASSNAME::*fn)(PARAM1, PARAM2, PARAM3, PARAM4), void *retval, void **params, int nparam) {
    PARAM1 *p1 = static_cast<PARAM1*>(params[0]);
    PARAM2 *p2 = static_cast<PARAM2*>(params[1]);
    PARAM3 *p3 = static_cast<PARAM3*>(params[2]);
    PARAM4 *p4 = static_cast<PARAM4*>(params[3]);
    (static_cast<CLASSNAME *>(this)->*fn)(*p1, *p2, *p3, *p4);
  }

  template <class CLASSNAME, class RETVAL, class PARAM1, class PARAM2, class PARAM3, class PARAM4>
  void cb(RETVAL (CLASSNAME::*fn)(PARAM1, PARAM2, PARAM3, PARAM4), void *retval, void **params, int nparam) {
    PARAM1 *p1 = static_cast<PARAM1*>(params[0]);
    PARAM2 *p2 = static_cast<PARAM2*>(params[1]);
    PARAM3 *p3 = static_cast<PARAM3*>(params[2]);
    PARAM4 *p4 = static_cast<PARAM4*>(params[3]);
    *static_cast<RETVAL*>(retval) = (static_cast<CLASSNAME *>(this)->*fn)(*p1, *p2, *p3, *p4);
  }

  // 5 params
  template <class CLASSNAME, class PARAM1, class PARAM2, class PARAM3, class PARAM4, class PARAM5>
  void vcb(void (CLASSNAME::*fn)(PARAM1, PARAM2, PARAM3, PARAM4, PARAM5), void *retval, void **params, int nparam) {
    PARAM1 *p1 = static_cast<PARAM1*>(params[0]);
    PARAM2 *p2 = static_cast<PARAM2*>(params[1]);
    PARAM3 *p3 = static_cast<PARAM3*>(params[2]);
    PARAM4 *p4 = static_cast<PARAM4*>(params[3]);
    PARAM5 *p5 = static_cast<PARAM5*>(params[4]);
    (static_cast<CLASSNAME *>(this)->*fn)(*p1, *p2, *p3, *p4, *p5);
  }

  template <class CLASSNAME, class RETVAL, class PARAM1, class PARAM2, class PARAM3, class PARAM4, class PARAM5>
  void cb(RETVAL (CLASSNAME::*fn)(PARAM1, PARAM2, PARAM3, PARAM4, PARAM5), void *retval, void **params, int nparam) {
    PARAM1 *p1 = static_cast<PARAM1*>(params[0]);
    PARAM2 *p2 = static_cast<PARAM2*>(params[1]);
    PARAM3 *p3 = static_cast<PARAM3*>(params[2]);
    PARAM4 *p4 = static_cast<PARAM4*>(params[3]);
    PARAM5 *p5 = static_cast<PARAM5*>(params[4]);
    *static_cast<RETVAL*>(retval) = (static_cast<CLASSNAME *>(this)->*fn)(*p1, *p2, *p3, *p4, *p5);
  }

  // 6 params
  template <class CLASSNAME, class PARAM1, class PARAM2, class PARAM3, class PARAM4, class PARAM5, class PARAM6>
  void vcb(void (CLASSNAME::*fn)(PARAM1, PARAM2, PARAM3, PARAM4, PARAM5, PARAM6), void *retval, void **params, int nparam) {
    PARAM1 *p1 = static_cast<PARAM1*>(params[0]);
    PARAM2 *p2 = static_cast<PARAM2*>(params[1]);
    PARAM3 *p3 = static_cast<PARAM3*>(params[2]);
    PARAM4 *p4 = static_cast<PARAM4*>(params[3]);
    PARAM5 *p5 = static_cast<PARAM5*>(params[4]);
//    PARAM6 *p6 = static_cast<PARAM5*>(params[5]); // hey look, mig found a bug.
    PARAM6 *p6 = static_cast<PARAM6*>(params[5]);
    (static_cast<CLASSNAME *>(this)->*fn)(*p1, *p2, *p3, *p4, *p5, *p6);
  }

  template <class CLASSNAME, class RETVAL, class PARAM1, class PARAM2, class PARAM3, class PARAM4, class PARAM5, class PARAM6>
  void cb(RETVAL (CLASSNAME::*fn)(PARAM1, PARAM2, PARAM3, PARAM4, PARAM5, PARAM6), void *retval, void **params, int nparam) {
    PARAM1 *p1 = static_cast<PARAM1*>(params[0]);
    PARAM2 *p2 = static_cast<PARAM2*>(params[1]);
    PARAM3 *p3 = static_cast<PARAM3*>(params[2]);
    PARAM4 *p4 = static_cast<PARAM4*>(params[3]);
    PARAM5 *p5 = static_cast<PARAM5*>(params[4]);
//    PARAM6 *p6 = static_cast<PARAM5*>(params[5]); // hey look, mig found a bug.
    PARAM6 *p6 = static_cast<PARAM6*>(params[5]);
    *static_cast<RETVAL*>(retval) = (static_cast<CLASSNAME *>(this)->*fn)(*p1, *p2, *p3, *p4, *p5, *p6);
  }

  // 7 params
  template <class CLASSNAME, class PARAM1, class PARAM2, class PARAM3, class PARAM4, class PARAM5, class PARAM6, class PARAM7>
  void vcb(void (CLASSNAME::*fn)(PARAM1, PARAM2, PARAM3, PARAM4, PARAM5, PARAM6, PARAM7), void *retval, void **params, int nparam) {
    PARAM1 *p1 = static_cast<PARAM1*>(params[0]);
    PARAM2 *p2 = static_cast<PARAM2*>(params[1]);
    PARAM3 *p3 = static_cast<PARAM3*>(params[2]);
    PARAM4 *p4 = static_cast<PARAM4*>(params[3]);
    PARAM5 *p5 = static_cast<PARAM5*>(params[4]);
    PARAM6 *p6 = static_cast<PARAM6*>(params[5]);
    PARAM7 *p7 = static_cast<PARAM7*>(params[6]);
    (static_cast<CLASSNAME *>(this)->*fn)(*p1, *p2, *p3, *p4, *p5, *p6, *p7);
  }

  template <class CLASSNAME, class RETVAL, class PARAM1, class PARAM2, class PARAM3, class PARAM4, class PARAM5, class PARAM6, class PARAM7>
  void cb(RETVAL (CLASSNAME::*fn)(PARAM1, PARAM2, PARAM3, PARAM4, PARAM5, PARAM6, PARAM7), void *retval, void **params, int nparam) {
    PARAM1 *p1 = static_cast<PARAM1*>(params[0]);
    PARAM2 *p2 = static_cast<PARAM2*>(params[1]);
    PARAM3 *p3 = static_cast<PARAM3*>(params[2]);
    PARAM4 *p4 = static_cast<PARAM4*>(params[3]);
    PARAM5 *p5 = static_cast<PARAM5*>(params[4]);
    PARAM6 *p6 = static_cast<PARAM6*>(params[5]);
    PARAM7 *p7 = static_cast<PARAM7*>(params[6]);
    *static_cast<RETVAL*>(retval) = (static_cast<CLASSNAME *>(this)->*fn)(*p1, *p2, *p3, *p4, *p5, *p6, *p7);
  }

  // 8 params
  template <class CLASSNAME, class PARAM1, class PARAM2, class PARAM3, class PARAM4, class PARAM5, class PARAM6, class PARAM7, class PARAM8>
  void vcb(void (CLASSNAME::*fn)(PARAM1, PARAM2, PARAM3, PARAM4, PARAM5, PARAM6, PARAM7, PARAM8), void *retval, void **params, int nparam) {
    PARAM1 *p1 = static_cast<PARAM1*>(params[0]);
    PARAM2 *p2 = static_cast<PARAM2*>(params[1]);
    PARAM3 *p3 = static_cast<PARAM3*>(params[2]);
    PARAM4 *p4 = static_cast<PARAM4*>(params[3]);
    PARAM5 *p5 = static_cast<PARAM5*>(params[4]);
    PARAM6 *p6 = static_cast<PARAM6*>(params[5]);
    PARAM7 *p7 = static_cast<PARAM7*>(params[6]);
    PARAM8 *p8 = static_cast<PARAM8*>(params[7]);
    (static_cast<CLASSNAME *>(this)->*fn)(*p1, *p2, *p3, *p4, *p5, *p6, *p7, *p8);
  }

  template <class CLASSNAME, class RETVAL, class PARAM1, class PARAM2, class PARAM3, class PARAM4, class PARAM5, class PARAM6, class PARAM7, class PARAM8>
  void cb(RETVAL (CLASSNAME::*fn)(PARAM1, PARAM2, PARAM3, PARAM4, PARAM5, PARAM6, PARAM7, PARAM8), void *retval, void **params, int nparam) {
    PARAM1 *p1 = static_cast<PARAM1*>(params[0]);
    PARAM2 *p2 = static_cast<PARAM2*>(params[1]);
    PARAM3 *p3 = static_cast<PARAM3*>(params[2]);
    PARAM4 *p4 = static_cast<PARAM4*>(params[3]);
    PARAM5 *p5 = static_cast<PARAM5*>(params[4]);
    PARAM6 *p6 = static_cast<PARAM6*>(params[5]);
    PARAM7 *p7 = static_cast<PARAM7*>(params[6]);
    PARAM8 *p8 = static_cast<PARAM8*>(params[7]);
    *static_cast<RETVAL*>(retval) = (static_cast<CLASSNAME *>(this)->*fn)(*p1, *p2, *p3, *p4, *p5, *p6, *p7, *p8);
  }

  // 9 params
  template <class CLASSNAME, class PARAM1, class PARAM2, class PARAM3, class PARAM4, class PARAM5, class PARAM6, class PARAM7, class PARAM8, class PARAM9>
  void vcb(void (CLASSNAME::*fn)(PARAM1, PARAM2, PARAM3, PARAM4, PARAM5, PARAM6, PARAM7, PARAM8, PARAM9), void *retval, void **params, int nparam) {
    PARAM1 *p1 = static_cast<PARAM1*>(params[0]);
    PARAM2 *p2 = static_cast<PARAM2*>(params[1]);
    PARAM3 *p3 = static_cast<PARAM3*>(params[2]);
    PARAM4 *p4 = static_cast<PARAM4*>(params[3]);
    PARAM5 *p5 = static_cast<PARAM5*>(params[4]);
    PARAM6 *p6 = static_cast<PARAM6*>(params[5]);
    PARAM7 *p7 = static_cast<PARAM7*>(params[6]);
    PARAM8 *p8 = static_cast<PARAM8*>(params[7]);
    PARAM9 *p9 = static_cast<PARAM9*>(params[8]);
    (static_cast<CLASSNAME *>(this)->*fn)(*p1, *p2, *p3, *p4, *p5, *p6, *p7, *p8, *p9);
  }

  template <class CLASSNAME, class RETVAL, class PARAM1, class PARAM2, class PARAM3, class PARAM4, class PARAM5, class PARAM6, class PARAM7, class PARAM8, class PARAM9>
  void cb(RETVAL (CLASSNAME::*fn)(PARAM1, PARAM2, PARAM3, PARAM4, PARAM5, PARAM6, PARAM7, PARAM8, PARAM9), void *retval, void **params, int nparam) {
    PARAM1 *p1 = static_cast<PARAM1*>(params[0]);
    PARAM2 *p2 = static_cast<PARAM2*>(params[1]);
    PARAM3 *p3 = static_cast<PARAM3*>(params[2]);
    PARAM4 *p4 = static_cast<PARAM4*>(params[3]);
    PARAM5 *p5 = static_cast<PARAM5*>(params[4]);
    PARAM6 *p6 = static_cast<PARAM6*>(params[5]);
    PARAM7 *p7 = static_cast<PARAM7*>(params[6]);
    PARAM8 *p8 = static_cast<PARAM8*>(params[7]);
    PARAM9 *p9 = static_cast<PARAM9*>(params[8]);
    *static_cast<RETVAL*>(retval) = (static_cast<CLASSNAME *>(this)->*fn)(*p1, *p2, *p3, *p4, *p5, *p6, *p7, *p8, *p9);
  }

  // 10 params
  template <class CLASSNAME, class PARAM1, class PARAM2, class PARAM3, class PARAM4, class PARAM5, class PARAM6, class PARAM7, class PARAM8, class PARAM9, class PARAM10>
  void vcb(void (CLASSNAME::*fn)(PARAM1, PARAM2, PARAM3, PARAM4, PARAM5, PARAM6, PARAM7, PARAM8, PARAM9, PARAM10), void *retval, void **params, int nparam) {
    PARAM1 *p1 = static_cast<PARAM1*>(params[0]);
    PARAM2 *p2 = static_cast<PARAM2*>(params[1]);
    PARAM3 *p3 = static_cast<PARAM3*>(params[2]);
    PARAM4 *p4 = static_cast<PARAM4*>(params[3]);
    PARAM5 *p5 = static_cast<PARAM5*>(params[4]);
    PARAM6 *p6 = static_cast<PARAM6*>(params[5]);
    PARAM7 *p7 = static_cast<PARAM7*>(params[6]);
    PARAM8 *p8 = static_cast<PARAM8*>(params[7]);
    PARAM9 *p9 = static_cast<PARAM9*>(params[8]);
    PARAM10 *p10 = static_cast<PARAM10*>(params[9]);
    (static_cast<CLASSNAME *>(this)->*fn)(*p1, *p2, *p3, *p4, *p5, *p6, *p7, *p8, *p9, *p10);
  }

  template <class CLASSNAME, class RETVAL, class PARAM1, class PARAM2, class PARAM3, class PARAM4, class PARAM5, class PARAM6, class PARAM7, class PARAM8, class PARAM9, class PARAM10>
  void cb(RETVAL (CLASSNAME::*fn)(PARAM1, PARAM2, PARAM3, PARAM4, PARAM5, PARAM6, PARAM7, PARAM8, PARAM9, PARAM10), void *retval, void **params, int nparam) {
    PARAM1 *p1 = static_cast<PARAM1*>(params[0]);
    PARAM2 *p2 = static_cast<PARAM2*>(params[1]);
    PARAM3 *p3 = static_cast<PARAM3*>(params[2]);
    PARAM4 *p4 = static_cast<PARAM4*>(params[3]);
    PARAM5 *p5 = static_cast<PARAM5*>(params[4]);
    PARAM6 *p6 = static_cast<PARAM6*>(params[5]);
    PARAM7 *p7 = static_cast<PARAM7*>(params[6]);
    PARAM8 *p8 = static_cast<PARAM8*>(params[7]);
    PARAM9 *p9 = static_cast<PARAM9*>(params[8]);
    PARAM10 *p10 = static_cast<PARAM10*>(params[9]);
    *static_cast<RETVAL*>(retval) = (static_cast<CLASSNAME *>(this)->*fn)(*p1, *p2, *p3, *p4, *p5, *p6, *p7, *p8, *p9, *p10);
  }

  enum { DESTRUCT=0xffff };
};
#define CB(x, y) case (x): cb(&CBCLASS::y, retval, params, nparam); break;
#define VCB(x, y) case (x): vcb(&CBCLASS::y, retval, params, nparam); break;

#define RECVS_DISPATCH virtual int _dispatch(int msg, void *retval, void **params=NULL, int nparam=0)

#define START_DISPATCH \
  int CBCLASS::_dispatch(int msg, void *retval, void **params, int nparam) { \
    switch (msg) {
//FINISH      case DESTRUCT: delete this; return 1;
#define END_DISPATCH \
      default: return 0; \
    } \
    return 1; \
  }
#define FORWARD_DISPATCH(x) \
      default: return x::_dispatch(msg, retval, params, nparam); \
    } \
    return 1; \
  }


#if 0//CUT
// stupid VC++ needs this sometimes... :(
template<class RETURN_TYPE, class PARAM1, class PARAM2, class PARAM3>
RETURN_TYPE _extcall(Dispatchable *dp, int msg, RETURN_TYPE defval, PARAM1 param1, PARAM2 param2, PARAM3 param3) {
  void *params[3] = { &param1, &param2, &param3 };
  RETURN_TYPE retval;
  if (dp->_dispatch(msg, &retval, params, 3)) return retval;
  return defval;
}

// helper classes to implement server-side methods
class DispatchableCallback {
protected:
  DispatchableCallback(int _msgid) : msgid(_msgid) {}
public:
  virtual ~DispatchableCallback() {}

  int getMsgId() const { return msgid; }
  virtual void _callback(void *obj, void *retval, void **param, int nparam)=0;

protected:
  int msgid;
};

// no params
template<class CLASSNAME, class RETVAL>
class callback0 : public DispatchableCallback {
public:
  typedef RETVAL (CLASSNAME::*fnPtrType)();
  callback0(int msgid, fnPtrType _fnptr) :
    DispatchableCallback(msgid),
    fnptr(_fnptr) {}
  virtual void _callback(void *obj, void *retval, void **param, int nparam) {
    CLASSNAME *objptr = static_cast<CLASSNAME *>(obj);
    RETVAL *ret = static_cast<RETVAL *>(retval);
    *ret = (objptr->*fnptr)();
  }
private:
  fnPtrType fnptr;
};

// 1 param
template<class CLASSNAME, class RETVAL, class PARAM1>	// exp
class callback1 : public DispatchableCallback {
public:
  typedef RETVAL (CLASSNAME::*fnPtrType)(PARAM1);	// exp
  callback1(int msgid, fnPtrType _fnptr) :
    DispatchableCallback(msgid),
    fnptr(_fnptr) {}
  virtual void _callback(void *obj, void *retval, void **param, int nparam) {
    CLASSNAME *objptr = static_cast<CLASSNAME *>(obj);
    RETVAL *ret = static_cast<RETVAL *>(retval);
    PARAM1 *p1 = static_cast<PARAM1*>(param[0]);	// exp
    *ret = (objptr->*fnptr)(*p1);			// exp
  }
private:
  fnPtrType fnptr;
};

// 2 params
template<class CLASSNAME, class RETVAL, class PARAM1, class PARAM2>	// exp
class callback2 : public DispatchableCallback {
public:
  typedef RETVAL (CLASSNAME::*fnPtrType)(PARAM1, PARAM2);	// exp
  callback2(int msgid, fnPtrType _fnptr) :
    DispatchableCallback(msgid),
    fnptr(_fnptr) {}
  virtual void _callback(void *obj, void *retval, void **param, int nparam) {
    CLASSNAME *objptr = static_cast<CLASSNAME *>(obj);
    RETVAL *ret = static_cast<RETVAL *>(retval);
    PARAM1 *p1 = static_cast<PARAM1*>(param[0]);	// exp
    PARAM2 *p2 = static_cast<PARAM2*>(param[1]);	// exp
    *ret = (objptr->*fnptr)(*p1, *p2);			// exp
  }
private:
  fnPtrType fnptr;
};

// 2 params, no return val
template<class CLASSNAME, class PARAM1, class PARAM2>	// exp
class voidcallback2 : public DispatchableCallback {
public:
  typedef void (CLASSNAME::*fnPtrType)(PARAM1, PARAM2);	// exp
  voidcallback2(int msgid, fnPtrType _fnptr) :
    DispatchableCallback(msgid),
    fnptr(_fnptr) {}
  virtual void _callback(void *obj, void *retval, void **param, int nparam) {
    CLASSNAME *objptr = static_cast<CLASSNAME *>(obj);
    PARAM1 *p1 = static_cast<PARAM1*>(param[0]);	// exp
    PARAM2 *p2 = static_cast<PARAM2*>(param[1]);	// exp
    (objptr->*fnptr)(*p1, *p2);			// exp
  }
private:
  fnPtrType fnptr;
};

// 3 params
template<class CLASSNAME, class RETVAL, class PARAM1, class PARAM2, class PARAM3>	// exp
class callback3 : public DispatchableCallback {
public:
  typedef RETVAL (CLASSNAME::*fnPtrType)(PARAM1, PARAM2, PARAM3);	// exp
  callback3(int msgid, fnPtrType _fnptr) :
    DispatchableCallback(msgid),
    fnptr(_fnptr) {}
  virtual void _callback(void *obj, void *retval, void **param, int nparam) {
    CLASSNAME *objptr = static_cast<CLASSNAME *>(obj);
    RETVAL *ret = static_cast<RETVAL *>(retval);
    PARAM1 *p1 = static_cast<PARAM1*>(param[0]);	// exp
    PARAM2 *p2 = static_cast<PARAM2*>(param[1]);	// exp
    PARAM3 *p3 = static_cast<PARAM3*>(param[2]);	// exp
    *ret = (objptr->*fnptr)(*p1, *p2, *p3);		// exp
  }
private:
  fnPtrType fnptr;
};

// 4 params
template<class CLASSNAME, class RETVAL, class PARAM1, class PARAM2, class PARAM3, class PARAM4>	// exp
class callback4 : public DispatchableCallback {
public:
  typedef RETVAL (CLASSNAME::*fnPtrType)(PARAM1, PARAM2, PARAM3, PARAM4);	// exp
  callback4(int msgid, fnPtrType _fnptr) :
    DispatchableCallback(msgid),
    fnptr(_fnptr) {}
  virtual void _callback(void *obj, void *retval, void **param, int nparam) {
    CLASSNAME *objptr = static_cast<CLASSNAME *>(obj);
    RETVAL *ret = static_cast<RETVAL *>(retval);
    PARAM1 *p1 = static_cast<PARAM1*>(param[0]);	// exp
    PARAM2 *p2 = static_cast<PARAM2*>(param[1]);	// exp
    PARAM3 *p3 = static_cast<PARAM3*>(param[2]);	// exp
    PARAM4 *p4 = static_cast<PARAM4*>(param[3]);	// exp
    *ret = (objptr->*fnptr)(*p1, *p2, *p3, *p4);		// exp
  }
private:
  fnPtrType fnptr;
};

inline int Dispatchable::_dispatch(int msg, void *retval, void **params, int nparam) {
  DispatchableCallback **dlist = dispatchable_getCallbackList();
  for (int i = 0; ; i++) {
    DispatchableCallback *rec = dlist[i];
    if (rec == NULL) break;
    if (rec->getMsgId() == msg) {
      rec->_callback(this, retval, params, nparam);
      return 1;
    }
  }
  return 0;
}
#endif


#endif
