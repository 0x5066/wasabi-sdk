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

// stringmap.h
#ifndef simaph
#define simaph
#include "windows.h"
typedef struct tag_sinode
{
	int i;
	char *s;
} SInode;

class SIMap
{
	SInode *ary;
	int	numNodes;
	int nextInt;
	int	oriNumNodes;
public:
	SIMap() {ary = NULL; oriNumNodes = numNodes = 0; nextInt=0;}
	SIMap(int numnodes, SInode *s2i); 
	void AddNode(SInode *s2i);
	~SIMap();
	char	*String(int n);
	int		Int(char *str);
	int		IntCanBeNew(char *);
	void	Remove(int n);
};




#endif