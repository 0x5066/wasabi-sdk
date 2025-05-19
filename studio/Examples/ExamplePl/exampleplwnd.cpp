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

//=================================
//  Project Includes
//=================================
#include "exampleplwnd.h"

//=================================
//  Wasabi Includes
//=================================
#include "../../bfc/wnds/listwnd.h"   //ListWnd
#include "../../common/nsguid.h"      //nsGUID

#include "../../pledit/svc_pldir.h"   //svc_plDir
#include "../../pledit/playlist.h"    //Playlist
#include "../../pledit/plhand.h"      //PlaylistHandle

//=================================
//  Constants
//=================================
static const int MIN_PANEWIDTH = 200;

ExamplePlWnd::ExamplePlWnd() {
  setName("ExamplePl Window");  //Set our window name (it becomes the window title caption).

  rightpane = new ListWnd;      //Create our right pane window.
  leftpane = new ListWnd;       //Create our left pane window.

  setResizeable(TRUE);  //Frames are resizeable.
  setDividerType(DIVIDER_VERTICAL); //Verticla divider.
  setDividerPos(SDP_FROMRIGHT, MIN_PANEWIDTH);  //Divider extends from the right hand side with a minimum width of 200.
  setChildrenRootWnd(leftpane, rightpane);  //Set our left and right pane.

  pldir = NULL; //Set pldir to a known value.
}

ExamplePlWnd::~ExamplePlWnd() {
}

int ExamplePlWnd::onInit() {
  EXAMPLEPLWND_PARENT::onInit();  //Call up the our parent.

  appcmds_addCmd("Clear All", PCMD_CLEARALL, SIDE_RIGHT); //Add the 'clear all' button on the right.
  appcmds_addCmd("Glue All", PCMD_GLUEALL, SIDE_RIGHT);   //Add the 'glue all' button on the right.
  appcmds_addCmd("Read Current", PCMD_READCURRENT);       //Add the 'read current' button on the left.
  appcmds_addCmd("Read List", PCMD_READLIST);             //Add the 'read list' button on the left.

  getGuiObject()->guiobject_addAppCmds(this); //Add our buttons to the window.

  leftpane->addColumn("Track", 150);      //Add a column with the label "Track" in the left pane.
  rightpane->addColumn("Playlists", 200); //Add a column with the label "Playlists" in the right pane.

  return 1; //Finished init'ing? yep!
}

void ExamplePlWnd::appcmds_onCommand(int id, const RECT *buttonRect) {
  if(pldir == NULL) 
    pldir = SvcEnumByGuid<svc_plDir>(svc_plDir::getServiceGuid());  //Playlist Directory service.

  /**
    Note: Please remember that when you instantiate a service, you need to realease it when you're done!
    This can be safely done in the runlevelcb_onShutdown() in the RunlevelCallbackI class. This is
    how we release the service in this example.
  **/

  switch(id) {
    case PCMD_CLEARALL: { //Clear All Playlists
      leftpane->deleteAllItems();   //Clear our leftpane.
      rightpane->deleteAllItems();  //Clear our rightpane.
      pldir->clearAll();  //Clear all playlists.
    }
    break;

    case PCMD_GLUEALL: {  //Glue all playlists into one new playlist.
      PlaylistHandle newplh = pldir->insertPlaylist(NULL, "ExamplePl Glued Pl", NULL, TRUE);  //Create a new playlist with a label and hidden.
      Playlist *newpl = pldir->getPlaylist(newplh); //Get the playlist using it's handle.

      int numpl = pldir->getNumPlaylists(); //Get the number of playlists.

      for(int i = 0; i < numpl; i++) {  //Enumerate all the playlists.
        PlaylistHandle plh = pldir->enumPlaylist(i);  //Get the next playlist's handle.
        Playlist *pl = pldir->getPlaylist(plh); //Get the playlist.
        int numentries = pl->getNumEntries(); //Get the number of entries in the playlist.

        for(int j = 0; j < numentries; j++) { //Enumerate all the entries in the playlist.
          PlaylistEntry *plentry = pl->enumEntry(j);  //Get the next playlist entry.
          newpl->addPlaylistItem(pldir->getPlaystringFromEntry(plentry)); //Add it to the new "glued" playlist.
        }
      }

      pldir->setHidden(newplh, FALSE); //Now that we're done filling our new playlist, let's show it.
    }
    break;

    case PCMD_READCURRENT: {  //Read the currently playing playlist.
      PlaylistHandle plh = pldir->getCurrentlyPlaying();  //Get the handle of the currently playing playlist.
      Playlist *pl = pldir->getPlaylist(plh); //Get the playlist.

      if(leftpane->getNumItems()) //If we have previously read a playlist, clear our left pane of all tracks.
        leftpane->deleteAllItems();
        
      int numentries = pl->getNumEntries(); //Get the number of entries in the playlist.
      for(int i = 0; i < numentries; i++) { //Enumerate all entries in the playlist.
        char buf[4096]={0};
        PlaylistEntry * plentry = pl->enumEntry(i); //Get the next playlist entry.
        
        //Get the full formatted name of the entry from the metadb.
        if(api->metadb_getMetaData(pldir->getPlaystringFromEntry(plentry), MT_NAME, buf, 4095, MDT_STRINGZ)) {
          buf[4095]=0;
          leftpane->addItem(buf, NULL); //Add the formatted name of the entry to our list of tracks.
        }
      }
    }
    break;

    case PCMD_READLIST: {
      int numpl = pldir->getNumPlaylists();

      if(rightpane->getNumItems())
        rightpane->deleteAllItems();

      for(int i = 0; i < numpl; i++) {
        PlaylistHandle plh = pldir->enumPlaylist(i);
        Playlist *pl = pldir->getPlaylist(plh);
        rightpane->addItem(pl->getPlaylistName(), NULL);
      }
    }
    break;
  }

}
/*
  This is where you can safely do service cleanup / releases.
*/
void ExamplePlWnd::runlevelcb_onShutdown() {
  if(pldir != NULL)
    delete pldir; //If we had an instance of the playlist directory service, delete it.
}
