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

#include "msgpump.h"

PumpMessage::~PumpMessage() {
}

void PumpMessage::setType(int _type) {
  type = _type;
}

int PumpMessage::getType() {
  return type;
}

void PumpMessage::setValue(const char * _value, int _len) {
  value.setSize(_len);
  value.setMemory(_value, _len, 0);
}

//-------------------------------------------------------------------
static const int MESSAGEPUMP_TIMER_ID = 0x7777;

MessagePump::MessagePump(int interval) {
  timerclient_setTimer(MESSAGEPUMP_TIMER_ID, interval);
}  

MessagePump::~MessagePump() {
}

void MessagePump::pushMessage(PumpMessage * msg) {
  enter();
  message_list.addItem(msg);
  leave();
}

void MessagePump::setInterval(int interval) {
  timerclient_setTimer(MESSAGEPUMP_TIMER_ID, interval);
}

void MessagePump::onPumpCallback() {
  PumpMessage * msg = NULL;
  
  enter();
  if(( msg = message_list.getFirst()) != NULL) {
    pumpCallback(msg);
    message_list.deleteItem(msg);
  }
  leave();
}