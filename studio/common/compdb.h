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

#ifndef __COMP_DB_H
#define __COMP_DB_H

#define DB_DENY  0
#define DB_ALLOW 1

#define DB_READ             0
#define DB_WRITE            1
#define DB_DELETE           2
#define DB_GETSCANNER       3
#define DB_DROPINDEX        4

#define DB_ERROR            0
#define DB_SUCCESS          1
#define DB_NOSUCHFIELD      2
#define DB_RECORDNOTFOUND   3
#define DB_UNKNOWNDATATYPE  4
#define DB_DATATYPEMISMATCH 5
#define DB_OPERATIONDENIED  6
#define DB_INVALIDGUID      7
#define DB_EMPTYFIELD       8

#define DB_ENDOFENUM        0
#define DB_FOUND            TRUE
#define DB_NOTFOUND         FALSE

#endif
