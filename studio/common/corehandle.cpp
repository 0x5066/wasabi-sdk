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

#include "rootcomp.h"
#include "metatags.h"
#include "../attribs/cfgitem.h"
#include "corehandle.h"
#include "../studio/api.h"
#include "../studio/buttons.h"
#include "../studio/services/svc_coreadmin.h"
#include "../bfc/svc_enum.h" // for castService

using namespace UserButton;

static svc_coreAdmin *da_coreadmin=NULL;
static int instancecount=0;

static void initDaCoreAdmin() {
  if(!da_coreadmin) {
    ASSERT(api != NULL);
    waServiceFactory *s=api->service_enumService(WaSvc::COREADMIN,0);
    ASSERTPR(s,"Core Admin non present!");
    da_coreadmin=castService<svc_coreAdmin>(s);
  }
}

CoreHandle::CoreHandle(CoreToken tok) {
  initDaCoreAdmin();
  instancecount++;
  createdcore=0;
  if(da_coreadmin->verifyToken(tok)) token=tok;
  else {
    token=da_coreadmin->createCore();
    createdcore=1;
  }
}

CoreHandle::CoreHandle(const char *name) {
  initDaCoreAdmin();
  instancecount++;
  createdcore=0;
  token=da_coreadmin->nameToToken(name);
  if(token==-1) {
    token=da_coreadmin->createCore(name);
    createdcore=1;
  }
}

CoreHandle::~CoreHandle() {
  if(createdcore) da_coreadmin->freeCoreByToken(token);
  instancecount--;
  if(!instancecount) {
    api->service_release(da_coreadmin);
    da_coreadmin=NULL;
  }
}

int CoreHandle::setNextFile(const char *playstring, const char *destination) {
  return da_coreadmin->setNextFile(token, playstring, destination);
}

void CoreHandle::prev() {
  userButton(PREV);
}

void CoreHandle::play() {
  userButton(PLAY);
}

void CoreHandle::pause() {
  userButton(PAUSE);
}

void CoreHandle::stop() {
  userButton(STOP);
}

void CoreHandle::next() {
  userButton(NEXT);
}

void CoreHandle::userButton(int button) {
  da_coreadmin->userButton(token, button);
}

const char *CoreHandle::getSupportedExtensions() {
  return da_coreadmin->getSupportedExtensions();
}

const char *CoreHandle::getExtSupportedExtensions() {
  return da_coreadmin->getExtSupportedExtensions();
}

const char *CoreHandle::getExtensionFamily(const char *extension) {
  return da_coreadmin->getExtensionFamily(extension);
}

void CoreHandle::registerExtension(const char *extensions, const char *extension_name) {
  da_coreadmin->registerExtension(extensions, extension_name);
}

void CoreHandle::unregisterExtension(const char *extensions) {
  da_coreadmin->unregisterExtension(extensions);
}

int CoreHandle::getStatus() {
  return da_coreadmin->getStatus(token);
}

const char *CoreHandle::getCurrent() {
  return da_coreadmin->getCurrent(token);
}

int CoreHandle::getNumTracks() {
  return da_coreadmin->getNumTracks(token);
}

int CoreHandle::getCurPlaybackNumber() {
  return da_coreadmin->getCurPlaybackNumber(token);
}

int CoreHandle::getPosition() {
  return da_coreadmin->getPosition(token);
}

int CoreHandle::getWritePosition() {
  return da_coreadmin->getWritePosition(token);
}

int CoreHandle::setPosition(int ms) {
  return da_coreadmin->setPosition(token,ms);
}

int CoreHandle::getLength() {
  return da_coreadmin->getLength(token);
}

int CoreHandle::getPluginData(const char *playstring, const char *name, char *data, int data_len, int data_type) {
  return da_coreadmin->getPluginData(playstring, name, data, data_len, data_type);
}

unsigned int CoreHandle::getVolume() {
  return da_coreadmin->getVolume(token);
}

void CoreHandle::setVolume(unsigned int vol) {
  da_coreadmin->setVolume(token, vol);
}

int CoreHandle::getPan() {
  return da_coreadmin->getPan(token);
}

void CoreHandle::setPan(int bal) {
  da_coreadmin->setPan(token, bal);
}

void CoreHandle::setMute(int mute) {
  da_coreadmin->setMute(token, mute);
}

int CoreHandle::getMute() {
  return da_coreadmin->getMute(token);
}

void CoreHandle::addCallback(CoreCallback *cb) {
  da_coreadmin->addCallback(token, cb);
}

void CoreHandle::delCallback(CoreCallback *cb) {
  da_coreadmin->delCallback(token, cb);
}

int CoreHandle::getVisData(void *dataptr, int sizedataptr) {
  return da_coreadmin->getVisData(token, dataptr, sizedataptr);
}

int CoreHandle::getLeftVuMeter() {
  return da_coreadmin->getLeftVuMeter(token);
}

int CoreHandle::getRightVuMeter() {
  return da_coreadmin->getRightVuMeter(token);
}
  
int CoreHandle::registerSequencer(ItemSequencer *seq) {
  return da_coreadmin->registerSequencer(token, seq);
}

int CoreHandle::deregisterSequencer(ItemSequencer *seq) {
  return da_coreadmin->deregisterSequencer(token, seq);
}

int CoreHandle::getEqStatus() {
  return da_coreadmin->getEqStatus(token);
}

void CoreHandle::setEqStatus(int enable) {
  da_coreadmin->setEqStatus(token, enable);
}

int CoreHandle::getEqPreamp() {
  return da_coreadmin->getEqPreamp(token);
}

void CoreHandle::setEqPreamp(int pre) {
  da_coreadmin->setEqPreamp(token, pre);
}

int CoreHandle::getEqBand(int band) {
  return da_coreadmin->getEqBand(token, band);
}

void CoreHandle::setEqBand(int band, int val) {
  da_coreadmin->setEqBand(token, band, val);
}

int CoreHandle::getEqAuto() {
  return da_coreadmin->getEqAuto(token);
}

void CoreHandle::setEqAuto(int enable) {
  da_coreadmin->setEqAuto(token, enable);
}

void CoreHandle::setPriority(int priority) {
  da_coreadmin->setPriority(token, priority);
}

int CoreHandle::getPriority() {
  return da_coreadmin->getPriority(token);
}
