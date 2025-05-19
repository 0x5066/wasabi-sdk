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
//		NULLSOFT WASABI SDK EXAMPLE PROJECTS
//
//			File:			ExampleMetaData.cpp
//
//			Purpose:	Show how to add custom metadata to play items.
//
//			Notes:		A note on the comments in this document:
//
//								Notes that begin with *** are important notes that everyone
//								needs to read.  The other comments assist readability or
//								explain the thinking behind sections of code which may not
//								be immediately obvious to the novice programmer.
//
//                Or I'm just typing to hear myself clickyclack.  Deal.
//
//                Because Example1 is the first of a string of example
//                projects, there are more useless comments in this
//                set of modules than any others.
//


//
// Always start with std.h
#include "../../bfc/std.h"
//
#include "examplemetadata.h"			//EDITME
//
#include "../../common/xlatstr.h"
#include "../../common/metadatasvc.h"
#include "../../common/metatags.h"
#include "../../common/nsGUID.h"

const char *myMetadataName = "LastDatePlayed";

static WACNAME wac;
WACPARENT *the = &wac;										 

//  *** This is MY GUID.  Get your own.   // {E9FFDFFC-14C1-47dd-B786-CDD2052AD785}
static const GUID guid = 
{ 0xe9ffdffc, 0x14c1, 0x47dd, { 0xb7, 0x86, 0xcd, 0xd2, 0x5, 0x2a, 0xd7, 0x85 } };


WACNAME::WACNAME() : WACPARENT(/**/"Example Meta Data"/*EDITME*/) {
  //
  // *** Note that we now register our service in the constructor.

  // Don't let the guid scare you.  Just make one and put it there.
  // You don't have to worry about it again.  It's just a magic number for the engine.
  MetaDataSvc *mds = new MetaDataSvc("ExampleMetadata", nsGUID::fromChar("{E111917C-97CE-40dc-9F8F-7FCAB4033FF6}"));

  // Then we add a column to the metadata service for the data we will track.

  // We're just going to track a single piece of string information:
  // The last date a track was played.
  mds->addColumn(myMetadataName, MDT_STRINGZ, FALSE, FALSE);

  // And we register the service using a MetaDataSvcFactory
  registerService(new MetaDataSvcFactory(mds));
}

WACNAME::~WACNAME() {
}

GUID WACNAME::getGUID() {
  return guid;
}

void WACNAME::onCreate() {
  // *** Do startup stuff here that doesn't require you to have a window yet
  WACPARENT::onCreate();
}

void WACNAME::onDestroy() {
  WACPARENT::onDestroy();
}

//
//  *** PSSST, DOWN HERE!!!
//
//
//  Okay, so, when the title changes, we get the date as a string
//  and stuff that into the metadata.  Simple, right?
int WACNAME::corecb_onTitleChange(const char *title) {
  char * date = "Today's Date :)";
//
// From api.h
//
//  int metadb_setMetaData(const char *playstring, const char *name,
//     const char *data, int data_len, int data_type);
//
  api->metadb_setMetaData(title, myMetadataName, date, STRLEN(date), MDT_STRINGZ);

  return 0;
}
