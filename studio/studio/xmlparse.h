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

#ifndef _XMLPARSE_H
#define _XMLPARSE_H

class XMLParse {
private:
	void *parser;

public:
  XMLParse();
  virtual ~XMLParse();

	virtual void SetUserData(void *param);
	virtual void SetElementHandler(void (*start)(void *userData, const char *name, const char **atts),
																 void (*end)(void *userData, const char *name));
	virtual void SetCharacterDataHandler(void (*handler)(void *userData,const char *s, int len));
	virtual int Parse(const char *s, int len, int isFinal);
	virtual const char *ErrorString(int code);
	virtual int GetErrorCode();
	virtual int GetCurrentLineNumber();

};

#endif
