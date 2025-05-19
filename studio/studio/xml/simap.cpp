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

// s2imap.cpp
#include "simap.h"
SIMap::~SIMap()
{
	int i;

	for ( i = oriNumNodes; i < numNodes; i++)
	{
		delete ary[i].s;
		ary[i].s=NULL;
	}
	
	delete ary;
	ary=NULL;
}

SIMap::SIMap(int numnodes, SInode *s2i)
{	
	ary = new SInode[numnodes]; 
	memcpy (ary, s2i, sizeof(SInode)*numnodes);
	numNodes = numnodes;
	oriNumNodes = numnodes;
}

void SIMap::AddNode(SInode *s2i)
{	
	SInode *ary2;	
	if (s2i== NULL) return ;

	numNodes++;
	ary2 = new SInode[numNodes];
	memcpy(ary2, ary, sizeof(SInode)*(numNodes-1));
	delete ary;
	ary = ary2;
	memcpy(&(ary[numNodes-1]),s2i, sizeof(SInode));
}

char *SIMap::String(int n)
{
	int i;

	if (ary) 
	 for (i = 0; i < numNodes; i++)
	 {
		 if (ary[i].i == n) return ary[i].s;
	 }
	return NULL;
}

void SIMap::Remove(int n)
{
	int i;

	if (ary) 
	{
		for (i = 0; i < numNodes; i++)
		{
			if (ary[i].i == n) break;
		}

		if(i>=numNodes) return;

		numNodes--;

		SInode* ary2 = new SInode[numNodes];
		memcpy(ary2, ary, sizeof(SInode)*(i));
		memcpy(ary2+i, ary+i+1, sizeof(SInode)*(numNodes-i));

		delete ary;
		ary = ary2;
	}
}

int SIMap::IntCanBeNew(char *str)
{
	int	ret;
	SInode sn;

	ret = Int(str);
	if (ret == 0)
	{
		if (nextInt < numNodes) nextInt=numNodes;
		ret = nextInt;
		sn.i = nextInt;
		sn.s = new char[strlen(str)+1];
		strcpy(sn.s, str);

		AddNode(&sn);
		nextInt++;
	}
	return Int(str);
}
int SIMap::Int(char *str)
{
	int i;

	if (ary)
		for(i=0; i<numNodes; i++)
		{
			if (!stricmp(ary[i].s, str)) return ary[i].i;
		}
	return 0;
}