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

#ifndef _SIMPLEWNDCREATE_H
#define _SIMPLEWNDCREATE_H

#include "../studio/services/svc_wndcreate.h"
#include "../bfc/ptrlist.h"
#include "../bfc/string.h"
#include "../common/bucketitem.h"

// ===========================================================================
//
//    class ThingerBitmapInfo
//
//  This object simply contains all the info to describe the bitmaps used by
// a component for its display in the Thinger.  This packages it nicely to be
// used as a return value from functions.
class ThingerBitmapInfo {
public:
  ThingerBitmapInfo(HINSTANCE inst, int normal, int pushed, int hilited, int activated) {
    hInst       = inst;
    _normal     = normal;
    _pushed     = pushed;
    _hilited    = hilited;
    _activated  = activated;
  }
public:
  HINSTANCE hInst;
  int _normal;
  int _pushed;
  int _hilited;
  int _activated;
};

// ===========================================================================
//
//    template < class TWindowCreationObj >
//    class SimpleWndCreateSvcBase
//
//  This template is used to streamline the window creation services, allowing
// your component class or window class (or some other class, if you really
// want) to control the creation and destruction of its window instances.
//
template < class TWindowCreationObj >
class SimpleWndCreateSvcBase : public svc_windowCreateI {
public:

  // ===========================================================================
  //
  // Because the instantiation of this template is to create
  // a new service for each window in the system, we take the
  // given "Window Name" and prepend a service tag to it.
  static const char *getServiceName() { 
    String serviceName = "WindowCreationService ";
    serviceName += TWindowCreationObj::getWindowNameStatic();
    return serviceName.getValue(); 
  }


  // ===========================================================================
  //
  // Wasabi is asking us if we can create this GUID's window
  int testGuid(GUID g) {
    // Test the GUID against our retrieved GUID.  If we match, say so.
    if (g == TWindowCreationObj::getGUIDStatic()) {
      return 1;
    }
    // If we do not, then we do not.
    return 0;
  }


  // ===========================================================================
  //
  // The most basic class has no concept of window types or component buckets.
  RootWnd *createWindowOfType(int windowtype, int n, RootWnd *parent) {
    return NULL;
  }


  // ===========================================================================
  //
  // Create window according to GUID: if a window class does not support 
  // multiple instances, you should keep a pointer to it and always return it 
  // for its GUID, otherwise you should keep a list of your pointers for later 
  // deletion.
  RootWnd *createWindowByGuid(GUID g, RootWnd *parent) {
    // Again test the given GUID against the WAC's GUID.
    if (g == TWindowCreationObj::getGUIDStatic()) {
      // If we match, create the window.
      return TWindowCreationObj::createWindowStatic(0, parent);
    }
    // If that's not our guid, something is funky.  Return NULL.
    return NULL; 
  }

  // ===========================================================================
  //
  //  This callback method is called upon program exit, when the user destroys a window,
  // or when the skin is switched.
  //
  // WIP: I'm still confused about the callchain for destroyWindow.  I am not very
  // sure what window pointers I should expect to be called with or what I should
  // do with those pointers.
  int destroyWindow(RootWnd *w) {
    return TWindowCreationObj::destroyWindowStatic(w);
  }
};



// ===========================================================================
//
//    template < class TWindowCreationObj >
//    class ThingerWndCreateSvc
//
//  This template extends the functionality of the above template to include
// an icon into the "Component Bucket" -- Colloquially named "The Thinger"
// in Winamp 3 discourse.  (Actually "The Thinger" is just the name of the
// component whose purpose is to display the components listed in the 
// single Component Bucket in Winamp 3, whereas your program may have multiple
// Component Buckets, etc etc).  So now you know what a "Bucket" is, right?
//
//  Won't that make life so much easier?
template < class TWindowCreationObj >
class ThingerWndCreateSvc : public SimpleWndCreateSvcBase< TWindowCreationObj > {
public:

  // ===========================================================================
  //
  // Populate a component bucket with a specific type of window. This function is called
  // as long as you do not return NULL, with n incremented for each pass.
  //
  // NOTE: The only window type you will be called with, for now, is a type of
  // "WindowTypes::BUCKETITEM" -- and unless you want to use multiple window types,
  // don't worry about returning anything but NULL for values of n greater than 0;
  RootWnd *createWindowOfType(int windowtype, int n, RootWnd *parent) {
    switch (windowtype) {
      case WindowTypes::BUCKETITEM: {  
        switch (n) {
          case 0: {
            // If you wanna play with the kids in the thinger, you'd better
            // be prepared to fly your colors, proudly.
            ThingerBitmapInfo bmp = TWindowCreationObj::getThingerBitmapInfoStatic();

            BucketItem *bucketitem = new BucketItem;
            bucketitem->setAutoOpen(TWindowCreationObj::getGUIDStatic());
            bucketitem->setAutoText(TWindowCreationObj::getWindowNameStatic());
            bucketitem->setBitmaps(bmp.hInst, bmp._normal, bmp._pushed, bmp._hilited, bmp._activated);
            bucketitem_list.addItem(bucketitem);
            return bucketitem;
          }
        }
      }
    }
    return NULL;
  }

  // ===========================================================================
  //
  //  This callback method is called upon program exit, when the user destroys 
  // a window, or when the skin is switched.
  //
  // WIP: SHOULD THIS CALL THE DESTROY WINDOW STATIC?
  // FG> Not yet but it will have to, I'll change it when needed
  int destroyWindow(RootWnd *w) {
    // Doublecheck that our bucketitem_list has this window pointer.
    if (bucketitem_list.haveItem(w)) { 
      bucketitem_list.removeItem(w); 
      delete static_cast<BucketItem *>(w); // This is safe, because we already checked it.
    } else {
      // If we don't have it, we can't delete it.  Tell the API.
      return 0;
    }
    return 1;
  }

private:
  PtrList<RootWnd> bucketitem_list;
};


#if 0 // Example code
//
// This class shows what static methods must be provided in the class
// used to handle the window creation.  How this info is actually used
// for making WASABI componentry is discussed in Example1 and ExampleB.
//
// Please see how this code is ACTUALLY used in BOTH of those examples
// before attempting to use this code on your own.  There are two standard
// ways for providing the object functionality to come from the component
// class or from the window class.
//
// ===========================================================================
//
class SampleWindowCreationObj {
private:
  static SampleWindowCreationObj *     myInstance;
public:
  SampleWindowCreationObj() {
    // Only allow a single instance to be constructed.
    ASSERT( myInstance == NULL );
    myInstance = this;
  }

  static SampleWindowInfoObj & Main() {
    // Make that instance available as a static item.
    return * myInstance;
  }
  //  
  //  Therefore, if we wish, we can call down to that instance
  // to handle any of the necessary function calls required by
  // the template.  Hooray for legacy code support.
  //

  //
  //  The rest of the methods listed here are the required static
  // methods for any object that is passed into the above template.
  //
  // =========================================================================
  //
  static const char *getWindowNameStatic() { 
    return Main().getWindowName(); 
  }
  //
  static GUID getGUIDStatic() { 
    return Main().getGUID(); 
  }
  //
  static RootWnd *createWindowStatic(int n, RootWnd *parentWnd) {
    return Main().createWindow(n, parentWnd);
  }
  //
  static int destroyWindowStatic(RootWnd *deadWnd) {
    return Main().destroyWindow(deadWnd);
  }
  //
  // If you are going to be using the Thinger template, 
  // you also must provide the following method:
  //
  static ThingerBitmapInfo TWindowCreationObj::getThingerBitmapInfoStatic() {
    return Main().getThingerBitmapInfo();
  }
}
#endif// Example code

#endif//_SIMPLEWNDCREATE_H
