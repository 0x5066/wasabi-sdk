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

#include "dsp.h"
#include "../../attribs/attribs.h"

extern _bool dsp_enabled;

/**
  Nothing to do in the constructor.
**/
DSP::DSP() {
}

/**
  Nothing to do in the destructor.
**/
DSP::~DSP() {
}

/**
  Replace my lame volume conrol routine with whatever
  manipulation you want to do on the PCM data.
**/
DSP::processData(MediaInfo *infos, ChunkList *chunk_list, bool *killswitch) {
  if(!dsp_enabled) return 1;

  Chunk* chunk = chunk_list->getChunk("PCM");

  if(!chunk) return 1;
  
  unsigned int c_size = chunk->getSize();  //in bytes.
  void * d = chunk->getData();
  short * z = (short *)d;   //signed short for pcm data.

  /**
    I know i don't actually use these, but it's to show you how to read that type of info
  **/
  int srate = chunk->getInfo("srate");  //sample rate
  int bps = chunk->getInfo("bps");  //bits per sample
  int nch = chunk->getInfo("nch");  //number of channels

  for(unsigned int i = 0; i < (c_size/sizeof(short)); i++) {
    z[i] = z[i]/2;  //Cut the volume by half.
  }
  
  /**
    Please note that creating NEW infos for a chunk is ONLY
    required IF you change them.

    If they don't change, just don't send any.
  **/

  ChunkInfosI *ci = new ChunkInfosI();

  ci->addInfo("srate", srate); 
  ci->addInfo("bps", bps);
  ci->addInfo("nch", nch);

  chunk_list->setChunk("PCM", z, chunk->getSize(), ci);
  
  return 1;
}
