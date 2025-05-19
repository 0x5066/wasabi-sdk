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

#ifndef __AUTOQUERYLIST_H
#define __AUTOQUERYLIST_H

#include "itemlistwnd.h"
#include "../db/multiqueryserver.h"
#include "../ptrlist.h"
#include "../string.h"
#include "../timeslicer.h"
#include "../appcmds.h"
#include "../../pledit/plhand.h"
#include "../nakedobject.h"

class svc_plDir;
class Playlist;

#define AUTOQUERYLIST_PARENT NakedObject
#define AUTOQUERYLIST_DBPARENTSRV MultiQueryServerI

/**
  Automates the display of the result set of
  a query. 
  
  Simply create an AutoQueryList instance,
  set it's query with setQuery().
  
  Can send results to the playlist directory
  for automatic playlist creation based on
  the result of the query.
  
  @short Automated query result display window.
  @author Nullsoft
  @ver 1.0
  @see AutoFilterList
*/
class AutoQueryList : public AUTOQUERYLIST_PARENT, 
                      public AUTOQUERYLIST_DBPARENTSRV, AppCmdsI {

  public:
  /**
    Initializes member data.
  */
  AutoQueryList();
  
  /**
    Releases playlist directory service and removes
    the AppCmd button.
  */
  virtual ~AutoQueryList();

  /**
    Initializes the auto query list.
    
    ret 1
  */
  virtual int onInit();

  /**
    Event is triggered when a playstring is added to the result list.
    
    Updates the progress bar at the bottom of the window (from 0 to 100%).
    
    @param playstring The playstring to being added to the result list.
    @param nitems     The number of items (total) in the result list.
    @param thispos    The current position in the list of items.
  */
  virtual void mqs_onAddPlaystring(const char *playstring, int nitems, int thispos);
  
  /**
    Event is triggered when a new query is set.
    
    Updates the progress bar to 0%, resets the playlist associated
    with this auto query list and resets the number of items found
    for the query to zero.
  */
  virtual void mqs_onNewMultiQuery();
  
  /**
    Event is triggered when a query completes.
    
    Updates the progress bar to 100% and resets the last
    known progress count.
  */
  virtual void mqs_onCompleteMultiQuery();

protected:
  /**
    Event is triggered when an AppCmd button (which belongs to this
    auto query list) is pushed.
    
    Only one command is handled by the auto query list which causes the query
    to be reset. The id of the command is 0.
    
    @param id   The id of the command associated with the button that was pressed.
    @param rect The RECT of the button that was pushed.
  */
  virtual void appcmds_onCommand(int id, const RECT *buttonRect);

private:

  int need_startslicer;
  int lastpc;
  int nfound;
  Playlist *playlist;
  svc_plDir *pldir;
};

#endif
