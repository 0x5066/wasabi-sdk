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

#ifndef _DSP_H
#define _DSP_H

#include "../../studio/services/servicei.h"
#include "../../studio/services/svc_mediaconverter.h"

class DSP : public svc_mediaConverterI {
public:
  DSP();
  virtual ~DSP();

  //Service Name.
  static const char *getServiceName() { return "The 'I Do Nothing' DSP"; } //CHANGE ME!!

  //The format we accept.
  virtual int canConvertFrom(svc_fileReader *reader, const char *name, const char *chunktype) { 
    if(chunktype && !STRICMP(chunktype,"pcm")) return 1; // only accept PCM chunks.
    return 0;
  }

  //The format we are returning.
  virtual const char *getConverterTo() { return "PCM"; }

  //Where we do the magic things with our PCM data.
  virtual int processData(MediaInfo *infos, ChunkList *chunk_list, bool *killswitch);

  //Our latency.
  virtual int getLatency(void) { return 0; }

  //Media Information. Not used in the case of a DSP.
  virtual int getInfos(MediaInfo *infos) { return 0; };
};

#endif