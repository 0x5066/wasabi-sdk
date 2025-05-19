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

// This is a sample video input source to the core.

//
// Always start with std.h
#include "../../bfc/std.h"
#include "../../bfc/memblock.h"
#include "../../bfc/timerclient.h"
//
#include "examplevidsource.h"

#include "../../studio/services/svc_mediaconverter.h"
#include "../../studio/services/creators.h"

static WACNAME wac;
WACPARENT *the = &wac;                     

//  *** This is MY GUID.  Get your own!
// {A2D4EE2E-029B-4315-98D5-0E98C0F2246B}
static const GUID guid =  //EDITME (hint: use guidgen.exe)
{ 0xa2d4ee2e, 0x29b, 0x4315, { 0x98, 0xd5, 0xe, 0x98, 0xc0, 0xf2, 0x24, 0x6b } };

//
//  *** So.  I like purple.  Suffer.
//
// In order to get this whole purple thing going, you have to compile this project
// and when you run the app, hit Ctrl-L for the locations dialog, and type in:
//
//         purple://
//
// as the url you want to connect to.  This object will respond through its canConvertFrom
// method and will then start getting called to be a video source (it is output to the video
// window because we return "VIDEO" in getConverterTo).
//
// In order to not waste our time and our CPU drawing too too much, we're going to "fake"
// the fact that we're only a 30FPS video source by using a timer client.
//

static ARGB32 purples[] = { 
  0x00800080, 
  0x00700070, 
  0x00600060,
  0x00500050,
  0x00400040,
  0x00500050,
  0x00600060,
  0x00700070, 
};
const numPurples = sizeof(purples)/sizeof(ARGB32);

class svc_purpleVid : public svc_mediaConverterI, public TimerClientI, public DependentI {

public:
  enum {
    WIDTH = 320,
    HEIGHT = 200,
    THE_COLOR_PURPLE = 0x00800080,
    MS_DELAY = 33, // 30 fps
  };

public:
	svc_purpleVid() {
    // Setup the buffer we're going to blit continually.
    purplebuf.setSize(WIDTH * HEIGHT);
    // Fill it with the color purple.
    MEMFILL<ARGB32>(purplebuf.getMemory(), THE_COLOR_PURPLE, WIDTH * HEIGHT);
    // Get your timer going.
    timerclient_setTimer(THE_COLOR_PURPLE, MS_DELAY);
    // And be ready to blit (this is for the timer)
    okToBlit = 1;
    // And, for fun let's count how often our timer callback hits.
    timecount = 0;   
	}

	~svc_purpleVid() {
	}

	static const char *getServiceName() {
    return "Purple Video"; 
  }

  virtual int canConvertFrom(svc_fileReader *reader, const char *name, const char *chunktype) {
	  if (name && !stricmp(name,"purple://")) return 1;
	  if (chunktype && !stricmp(chunktype,"purple")) return 1;
	  return 0;
  }

  virtual const char *getConverterTo() {
    return "VIDEO";
  }
  
  virtual int getInfos(MediaInfo *infos) {
    return 0;
  }

	virtual int processData(MediaInfo *infos, ChunkList *chunk_list, bool *killswitch) {

    if (!okToBlit) {
      chunk_list->setChunk("VIDEO", NULL, NULL);
      // If you don't Sleep(1) you kill the CPU.
      Sleep(1);
		  return 1;
    }
    okToBlit = 0;

    OutputDebugString(StringPrintf("REAL PURPLE BLIT #%04d\n",timecount));

    MEMFILL<ARGB32>(purplebuf.getMemory(), purples[timecount%numPurples], WIDTH * HEIGHT);

		ChunkInfosI *chunkinfo = new ChunkInfosI();

    // 32bpp video
    chunkinfo->addInfo("type", (int)"RGB32"); 

    // Aspect ratio is a float packed into an int.
    float aspectFloat = 1.33333F;
    chunkinfo->addInfo("aspect", *((int*)&aspectFloat));

    // Width is whatever your video source is.  For us, 320
    chunkinfo->addInfo("width", WIDTH);

    // Height is whatever your video source is.  For us, 200
    chunkinfo->addInfo("height", HEIGHT);

    // Flipv specifies whether or not to flip your video vertically.  For us, no
    chunkinfo->addInfo("flipv", 0);

    // Yes, all of our frames are keyframes.
    chunkinfo->addInfo("keyframe", 1);

    // How much time in ms has elapsed?
    chunkinfo->addInfo("time", timecount * MS_DELAY);

    // After we setup the chunk, feed it back to the core for more processing.
    chunk_list->setChunk("VIDEO", (void *)purplebuf.getMemory(), WIDTH * HEIGHT, chunkinfo);

		return 1;
	}

  virtual void timerclient_timerCallback(int id) {
    if (id == THE_COLOR_PURPLE) {
      okToBlit = 1;
      timecount += (1 + timerclient_getSkipped()); // make sure we count skips. 
    }
    TimerClientI::timerclient_timerCallback(id);
  }

  virtual Dependent *timerclient_getDependencyPtr() {
    return this;
  }

private:
  MemBlock<ARGB32> purplebuf;
  int okToBlit;
  int timecount;
};

WACNAME::WACNAME() : WACPARENT(/**/"ExampleVidSource Component"/*EDITME*/) {
  registerService(new MediaConverterCreator<svc_purpleVid>);
}

WACNAME::~WACNAME() {
}

GUID WACNAME::getGUID() {
  return guid;
}
