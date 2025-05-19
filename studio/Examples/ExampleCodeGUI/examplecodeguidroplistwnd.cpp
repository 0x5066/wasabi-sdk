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
//    NULLSOFT WASABI SDK EXAMPLE PROJECTS
//
//      File:     ExampleCodeGUIDroplistWnd.cpp
//
//!##   Purpose:  This source module contains the functions for the 
//!##             construction of ListWnd and DropList objects.
//
//      Requires: Please read ExampleCodeGUIWnd.cpp first.
//
//      Notes:    A note on the comments in this document:
//
//                Notes that begin with *** are important notes that everyone
//                needs to read.  The other comments assist readability or
//                explain the thinking behind sections of code which may not
//                be immediately obvious to the novice programmer.
//
//                Or I'm just typing to hear myself clickyclack.
//

// Always begin with including std.h
#include "../../bfc/std.h"

#include "examplecodeguiwnd.h"

#include "../../bfc/notifmsg.h"
#include "../../bfc/memblock.h"
#include "../../common/styleposwnd.h"
#include "../../common/xlatstr.h"
#include "../../common/droplist.h"
#include "../../bfc/wnds/listwnd.h"

// ===========================================================================
//
//    *** Module Syllabus
//
//
//    Step 1:  Make a Drop box and a listwnd.  
//						 a - Things in the Dropbox drop into the listwnd on selection (shrug)
//



// ===========================================================================
//
//    *** Constants and Static Globals
//
//
const char *myDroplistTexts[] = {
	"The quick brown fox jumps over the lazy dog.",
	"You have to add to a droplist after it is initialized.",
	"Isn't this the most ree-diculous thing you've ever seen?",
	"How many people are actually ever going to read these things?",
	"I wonder if I'd get fired if I wrote a bunch of rude things here?",
	"Don't you hate rhetorical questions?",
	"So, how's the weather out there?",
	"Do you want to kill me for writing these things as much as I want to kill myself for the same reason?",
	"Help, I am being held captive in the Nullsoft Fortune Cookie Factory.",
	"JAAAAAAAAAAAAANE, STOP THIS CRAZY THING!",
	"This is some kind of weird 20'th century haiku visual arts type thing, isn't it?",
	"By the way, the option was added to ListWnd to disallow multiple selections.",
	"Who wanted that?  I don't remember.  Was it Schweitn?",
	"See, we listen to you.  We want you to succeed!  We want to assist your assimilation!",
	"But don't be scared to email us API suggestions or extensions.",
	"If it's easy for us to make your life easier, we're all happier.",
	"Just don't turn into a whiny thatch of milksops, either.",
	"Sometimes you won't be able to have your way.",
	"Same thing goes for my daughter.",
	"As soon as I procreate.  Let me get back to you on that."
};

const int numDroplistTexts = 20;

// ===========================================================================
//
//    *** ExampleCodeGUIWnd::createDroplistSheet()
//
//  This is the method in which we create the tabsheet for this module.  It is
//  our primary point of entry for instantiation and initialization.  Aren't 
//  you thrilled?
int ExampleCodeGUIWnd::createDroplistSheet() {
  // *** Here is where we create even more useless user interface object hierarchy:

  StylePosWnd *container = new StylePosWnd;
  container->setName(_("Droplist"));

  myDropList = new DropList;
	myDropList->setNotifyId(EXB_DROPLIST);
	myDropList->setNotifyWindow(this);
  container->addChild(StylePosWnd::ChildInfo(myDropList,
    StylePosWnd::ChildEdgeInfo(EXB_MINE_EDGE, StylePosWnd::LOCK_BORDER),     // Left
    StylePosWnd::ChildEdgeInfo(EXB_MINE_EDGE, StylePosWnd::LOCK_BORDER),     // Top
    StylePosWnd::ChildEdgeInfo(-EXB_MINE_EDGE, StylePosWnd::LOCK_BORDER),    // Right
    StylePosWnd::ChildEdgeInfo(EXB_BUTTON_HEIGHT, StylePosWnd::LOOSE)));     // Bottom
  // "Set your left edge 5 pixels to the right of the left border,
  //  set your top edge 5 pixels below the top border,
  //  set your right edge 5 pixels to the left of the right border,
  //  set your bottom edge to create a 25 pixel tall bar."

  myListWnd = new ListWnd;
	myListWnd->setPreventMultipleSelection(1);	// someone wanted to be able to do this, hmm?
  container->addChild(StylePosWnd::ChildInfo(myListWnd,
    StylePosWnd::ChildEdgeInfo(EXB_MINE_EDGE, StylePosWnd::LOCK_BORDER),     // Left
    StylePosWnd::ChildEdgeInfo(EXB_MINE_EDGE, StylePosWnd::LOCK_BOTTOM, myDropList),// Top
    StylePosWnd::ChildEdgeInfo(-EXB_MINE_EDGE, StylePosWnd::LOCK_BORDER),    // Right
    StylePosWnd::ChildEdgeInfo(EXB_MINE_EDGE, StylePosWnd::LOCK_BORDER)));   // Bottom
  // "Set your left edge 5 pixels to the right of the left border,
  //  set your top edge 5 pixels below the droplist object,
  //  set your right edge 5 pixels to the left of the right border,
  //  set your bottom edge 5 pixels above the bottom border."

	addChild(container);
	return 1;
}

int ExampleCodeGUIWnd::postInitDroplist() {
	// *** You cannot add items to DropList until after you call
	// the onInit parent virtual method.  So this method handles
	// that for us here.
	for (int i = 0; i < numDroplistTexts; i++) {
		// So, like, these are the string constants at the top of the module, dude
		myDropList->addItem(_(myDroplistTexts[i]));
	}
	return 1;
}

int ExampleCodeGUIWnd::handleDroplistChanged(int msg, int objId, int param1, int param2) {
	// Hmmm, looks like we're doing an absurd amount of work to make sure that
	// our only possible droplist ID is being passed to us.  What do you think?
	switch (objId) {
		case EXB_DROPLIST: {
			int droplistItem = param1; // clarification assignment

			// Hey, I've got an idea!  Let's get the text of the current droplist selection!

			// First make a buffer to copy the info into
			const int LOCAL_BUFFER_SIZE = 2000;
			MemBlock<char>	buffer(LOCAL_BUFFER_SIZE);  // MemBlock<T> has an implicit cast method to T*
			// Then query the drop list for the item text
			myDropList->getItemText(droplistItem, buffer, LOCAL_BUFFER_SIZE);
			// Enforce a null termination (just in case)
			buffer[LOCAL_BUFFER_SIZE-1]=0;
			// Dump it to the debug window
			OutputDebugString(StringPrintf("New droplist text: %s\n",buffer));
			// Use it as our source string for the listwnd
			populateListWnd(buffer);
			return 1;
		}
		break;
	}
	return 0;
}

void ExampleCodeGUIWnd::populateListWnd(const char *newLameText) {

  // Empty listwnd in case it already contains data
  myListWnd->reset();
  // Eh, I think six is a very tidy number for the column count.
  for (int i=0;i<6;i++) {
    // So, then, six lovely columns it is.
    myListWnd->addColumn(StringPrintf("%d",i), 80);
  }

	int row = 0, column = 0;
	String parseText = newLameText;
  while (parseText.len()) { // while we sill have words to parse
		// grab another word
		String value = parseText.lSpliceChar(' ');
		// output to the debug console
    OutputDebugString(StringPrintf("%s ",value));
		// add a new row
    myListWnd->addItem(StringPrintf("%d",row), NULL);
    // and add the word into a column of the new row.
    myListWnd->setSubItem(row, column+1, value.getNonConstVal());
		// and then twiddle the counters
		row++;
		// counters love twiddling!
		column = (column + 1) % 5;
  }
}
