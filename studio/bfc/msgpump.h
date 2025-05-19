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

#ifndef _MSGPUMP_H
#define _MSGPUMP_H

#include "ptrlist.h"
#include "timerclient.h"
#include "../common/critsec.h"

class PumpMessage {
public:
  PumpMessage(int _type = 0) { type = _type; };
  virtual ~PumpMessage();

  void setType(int _type);
  int getType();
  
  void setValue(const char * _value, int _len);
  void *getMem() { return value.getMemory(); };
  int getSize() { return value.getSize(); };

private:
  int type;
  MemBlock<char> value;
};    


//-------------------------------------------------------------------
#define MESSAGEPUMP_PARENT  TimerClientDI

class MessagePump : public MESSAGEPUMP_PARENT, public CriticalSection {
public:
  MessagePump(int interval = 50);
  virtual ~MessagePump();
 
  virtual void pumpCallback(PumpMessage * msg)=0;

  void pushMessage(PumpMessage *msg);

protected:
  void setInterval(int interval);

  virtual void timerclient_timerCallback(int id) { onPumpCallback(); };
  void onPumpCallback();
    
private:
  PtrList<PumpMessage> message_list;

};

#endif