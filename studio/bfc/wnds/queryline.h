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

#ifndef _QUERYLINE_H
#define _QUERYLINE_H

#include "../nakedobject.h"
#include "../db/subqueryserver.h"

#define QUERYLINE_PARENT NakedObject

/**
  Class 

  @short 
  @author Nullsoft
  @ver 1.0
  @see 
*/
class QueryLine : public QUERYLINE_PARENT, public SubQueryServerI {
public:
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  QueryLine(const char *query=NULL);
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual ~QueryLine() { }

  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual void setQuery(const char *query);
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  int setAuto(int bv);

protected:
  int autoquery;

private:
  String querytext, autofield;
};

#endif
