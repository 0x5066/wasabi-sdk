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

#include "xml/xmlparse.h"
#include "xml/xmltok.h"
#include "xmlreader.h"
#include "../bfc/util/pathparse.h"
#include "../common/wildcharsenum.h"
#include "api.h"
#include "services/svc_xmlprov.h"

void XmlReaderParamsI::addItem(const char *parm, const char *value) {
  parms_struct *p= new parms_struct;
  p->parm = parm;
  p->value = value;
  parms_list.addItem(p);
}

int XmlReaderParamsI::getNbItems() {
  return parms_list.getNumItems();
}

const char *XmlReaderParamsI::getItemName(int i) {
  if(i>getNbItems()) return "";
  return parms_list[i]->parm;
}

const char *XmlReaderParamsI::getItemValue(int i) {
  if(i>getNbItems()) return "";
  return parms_list[i]->value;
}

const char *XmlReaderParamsI::getItemValue2(const char *name) {
  for(int i=0;i<getNbItems();i++)
    if(STRCASEEQL(parms_list[i]->parm, name))
      return parms_list[i]->value;
  return "";
}

const char *XmlReaderParamsI::enumItemValues(const char *name, int nb) {
  int f=0;
  for(int i=0;i<getNbItems();i++)
    if(STRCASEEQL(parms_list[i]->parm, name))
      if(f==nb)
        return parms_list[i]->value;
      else f++;
  return NULL;
}

int XmlReaderParamsI::getItemValueInt(const char *name, int def) {
  for(int i=0;i<getNbItems();i++)
    if(STRCASEEQL(parms_list[i]->parm, name))
      return atoi(parms_list[i]->value);
  return def;
}

#define CBCLASS XmlReaderParamsI
START_DISPATCH;
    CB(GETITEMNAME, getItemName);
    CB(GETITEMVALUE, getItemValue);
    CB(GETITEMVALUE2, getItemValue2);
    CB(GETITEMVALUEINT, getItemValueInt);
    CB(GETNBITEMS, getNbItems);
    VCB(ADDITEM, addItem);
END_DISPATCH;

void XmlReader::registerCallback(const char *matchstr, XmlReaderCallback *callback) {
  cb_struct s;
  s.matchstr=new String(matchstr);
  s.type=TYPE_CLASS_CALLBACK;
  s.callback=callback;
  callback_list.insertItem(s, 0);	// mig: add to FRONT of list... so we don't step on hierarchical toes.
}

void XmlReader::registerCallback(const char *matchstr, void (* callback)(int, const char *, const char *, XmlReaderParams *)) {
  cb_struct s;
  s.matchstr=new String(matchstr);
  s.type=TYPE_STATIC_CALLBACK;
  s.callback=(XmlReaderCallback *)callback;
  callback_list.insertItem(s, 0);	// mig: add to FRONT of list... so we don't step on hierarchical toes.
}

void XmlReader::unregisterCallback(void *callback) {
  for(int i=0;i<callback_list.getNumItems();i++)
    if(callback_list[i].callback==callback) {
      delete callback_list[i].matchstr;
      callback_list.delByPos(i);
      i--;
    }
}

int XmlReader::loadFile(const char *filename, const char *incpath, void *only_this_callback, const char **params) {
  FILE *fp=NULL;
  svc_xmlProvider *svc = NULL;
  String fn;

  if (!STRNINCMP(filename, "buf:")) {
    return doLoadFile(NULL, NULL, filename, incpath, only_this_callback, params);
  } else {
    fn = filename;
    if (incpath) fn.prepend(incpath);

    WildcharsEnumerator e(fn);
  
    if (e.getNumFiles() > 0) { // we're including multiple files

      for (int i=0;i<e.getNumFiles();i++) {
        fp=fopen(e.enumFile(i),"rb");
        if (!fp) return 0;

        PathParser pp(e.enumFile(i));
        doLoadFile(fp, svc, pp.getLastString(), incpath, only_this_callback);
      }

      return 1;
    
    } else {

      fp=fopen(fn,"rb");
      if (!fp)   // file failed to open, try xmlproviders
        svc = XmlProviderEnum(filename).getNext();

      return doLoadFile(fp, svc, filename, incpath, only_this_callback, params);
    }
  }
  // some compilers are stupid!
  return 0;
}

int XmlReader::doLoadFile(FILE *fp, svc_xmlProvider *svc, const char *filename, const char *incpath, void *only_this_callback, const char **params) {
  
  // Set us up for being able to handle buffers, too.
  int buf_pos = 0;
  const char *buffer = NULL;
  if (!fp && !svc) {
    if (!STRNINCMP(filename, "buf:")) {
      buffer = filename + strlen("buf:");
    } else {
      return 0;
    }
  }


  void *parser=XML_ParserCreate(0);
  XML_SetUserData(parser, only_this_callback);
	XML_SetElementHandler(parser, parserCallbackOnStartElement, parserCallbackOnEndElement);
  XML_SetCharacterDataHandler(parser, parserCallbackOnCharacterData);

  String *incr=new String(incpath);

  int includeshit=0;
  if(!include_stack.isempty()) {
    includeshit=1;
    XML_Parse(parser, "<includeshit>", 13, 0);
  }

  include_stack.push(incr);

	char buf[16384];
  const char *pbuf = buf;
  int done;
	do {
		unsigned long len;
    if (buffer) {
      int buflen = strlen(buffer + buf_pos); // remaining buffer.
      MEMCPY(buf, buffer + buf_pos, min(buflen,sizeof(buf)));
      if (buflen < sizeof(buf)) {
        len = buflen;
        done = 1;
      } else {
        len = sizeof(buf);
        buf_pos += sizeof(buf);
      }
    } else if (fp) {
      len=fread(buf,1,sizeof(buf),fp);
      done = len < sizeof(buf);
		} else {
      XmlReaderParamsI *p=new XmlReaderParamsI();
      if(params)
        for(int i=0;params[i];i+=2)
          p->addItem(params[i], api->varmgr_translate(params[i+1]));
      pbuf = svc->getXmlData(filename, incpath, p);
      delete p;
      len = STRLEN(pbuf);
      done = 1;
    }
    if (pbuf && len > 0) {
      if (!XML_Parse(parser, pbuf, len, includeshit?0:done)) {
        int disperr=1;
        if(only_this_callback) {
          for(int i=0;i<callback_list.getNumItems();i++) { 
            cb_struct *c=&callback_list[i];
            if(c->callback==only_this_callback && c->type==TYPE_CLASS_CALLBACK) {
              if(!c->callback->xmlReaderDisplayErrors()) disperr=0;
              break;
            }
          }
        }
        StringPrintf txt("error parsing xml layer (%s)",filename);
        StringPrintf err("%s at line %d\n",
			    XML_ErrorString(XML_GetErrorCode(parser)),
			    XML_GetCurrentLineNumber(parser));
		    if(disperr) MessageBox(NULL, err, txt, MB_OK);
        else api->console_outputString(1,txt);
        currentpos="";
			  break;
      }
		}
	} while (!done);

  if(includeshit) XML_Parse(parser, "</includeshit>", 14, 1);

  XML_ParserFree(parser);
  if (fp) fclose(fp);

  include_stack.pop(&incr);
  delete incr;

  if (svc) {
    SvcEnum::release(svc); 
    svc = NULL;
  }

  return 1;
}

const char *XmlReader::peekParam(const char *txt, const char **parms) {
  for (int i=0;parms[i];i+=2)
    if (STRCASEEQL(parms[i],txt)) return parms[i+1];
  return NULL;
}

void XmlReader::parserCallbackOnStartElement(void *userData, const char *name, const char **parms) {
  if (STRCASEEQL(name, "includeshit")) return;
  if (STRCASEEQL(name, "include")) {
    const char *p = api->varmgr_translate(peekParam("file", parms));
    if (p) {
      if(userData) {
        // check for include files thing
        XmlReaderCallback *cb=(XmlReaderCallback *)userData;
        if(!cb->xmlReaderIncludeFiles()) return;
      }
#if 0
  	  char includepath[WA_MAX_PATH];
  	  char file[256];
      PathParser pp(p);
  	  int fl = STRLEN(pp.enumString(pp.getNumStrings()-1));
  	  STRCPY(file, p+STRLEN(p)-fl);
      if (*p=='/' || *p=='\\' || (*p != 0 && *(p+1)==':')) {
        STRNCPY(includepath, p, STRLEN(p)-fl);
        includepath[STRLEN(p)-fl]=0;
      } else {
         RecycleString *incr=include_stack.top();
        STRCPY(includepath, incr->getValue());
        int l = STRLEN(includepath);
        STRNCPY(includepath+l, p, STRLEN(p)-fl);
        includepath[l+STRLEN(p)-fl]=0;
      }
#endif
// grrr... who fucking wrote this shit with a 256 char limit :P
      const char *file = Std::filename(p);
      int fnlen = STRLEN(file);
      String includepath = p;
      includepath.trunc(-fnlen);
 
      // if nonrelative path, use top of stack
      if (!Std::isRootPath(p))
        includepath.prepend(include_stack.top()->getValue());

      loadFile(file, includepath, userData, parms);
      return;
    }
  }

#if 0
  if(currentpos.isempty()) currentpos=name;
  else {
    char temp[512];
    SPRINTF(temp,"%s/%s",currentpos,name);
    currentpos=temp;
  }
#else
  if (!currentpos.isempty())
    currentpos.cat("/");
  currentpos.cat(name);
#endif

  XmlReaderParamsI *p=new XmlReaderParamsI();
  for(int i=0;parms[i];i+=2)
    p->addItem(parms[i], api->varmgr_translate(parms[i+1]));

  for(i=0;i<callback_list.getNumItems();i++) {
    if(!userData || userData==callback_list[i].callback)
      if(Std::match((char *)callback_list[i].matchstr->getValue(), (char *)currentpos.getValue())) {
        switch(callback_list[i].type) {
          case TYPE_CLASS_CALLBACK: 
            callback_list[i].callback->xmlReaderOnStartElementCallback(currentpos.getValue(), name, p);
            break;
          case TYPE_STATIC_CALLBACK:
            ((void (*)(int, const char*, const char *, XmlReaderParams *))callback_list[i].callback)(1, currentpos.getValue(), name, p);
            break;
        }
        if(userData) break;
      }
  }

  delete(p);
  cont_char="";
}

void XmlReader::parserCallbackOnEndElement(void *userData, const char *nameC) {
  if (STRCASEEQL(nameC, "includeshit")) return;
  if (STRCASEEQL(nameC, "include")) return;

  const char *s=currentpos.getValue();

  for(int i=0;i<callback_list.getNumItems();i++) {
    if(!userData || userData==callback_list[i].callback)
      if(Std::match((char *)callback_list[i].matchstr->getValue(), (char *)s)) {
        switch(callback_list[i].type) {
          case TYPE_CLASS_CALLBACK: 
            if(!cont_char.isempty()) callback_list[i].callback->xmlReaderOnCharacterDataCallback(s, nameC, cont_char.getValue());
            callback_list[i].callback->xmlReaderOnEndElementCallback(s, nameC);
            break;
          case TYPE_STATIC_CALLBACK:
            ((void (*)(int, const char *, const char *, XmlReaderParams *))callback_list[i].callback)(0, s, nameC, NULL);
            break;
        }
        if(userData) break;
      }
  }

  char *p=(char *)Std::scanstr_back(s, "/", 0);
  if(!p) currentpos="";
  else {
    *p=0;
  }

  cont_char="";
}

void XmlReader::parserCallbackOnCharacterData(void *userData, const char *str, int len) {
#if 0
  char tmp[WA_MAX_PATH];
  if(!cont_char.isempty()) STRCPY(tmp, cont_char.getValue());
  else tmp[0]=0;
  int ncopy = MIN<int>((sizeof(tmp) - 1) - STRLEN(tmp), len);
  if (ncopy <= 0) return;

  char buf[WA_MAX_PATH];
  STRNCPY(buf, str, ncopy);
  buf[ncopy] = 0;

  STRCAT(tmp, buf);
  cont_char=tmp;
#else
  cont_char.cat(str);
#endif
}

const char *XmlReader::getIncludePath() {
  return include_stack.top()->getValue();
}

TList<XmlReader::cb_struct> XmlReader::callback_list;
String XmlReader::currentpos;
String XmlReader::cont_char;
Stack<String *> XmlReader::include_stack;
