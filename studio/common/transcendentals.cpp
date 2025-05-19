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

#include "transcendentals.h"

#include "std.h"

int		Transcendental::currentTableBitdepth = 0;

// All the tables.
Real *	Transcendental::sinTable = (Real *)NULL;


//
//	Constructors and Destructors
Transcendental::Transcendental() {
	initializeBitdepth( DEFAULT_TABLE_BITDEPTH );
}
 
Transcendental::~Transcendental() {
	//
	// Delete all the allocated tables.
	deleteTable( sinTable );

}

void		Transcendental::initializeBitdepth( const int newBitdepth ) {
	//
	//	For now, we'll call "SetBitdepth" -- soon we will save the default as a header table.
	setBitdepth( newBitdepth );
}

void		Transcendental::setBitdepth( const int newBitdepth ) {
	if ( newBitdepth >= 1 && newBitdepth <= 32 && newBitdepth != currentTableBitdepth ) {
		// Set the variable
		currentTableBitdepth = newBitdepth;

		// And regenerate all the tables
		makeTable( sinTable );
	}
}

int			Transcendental::getBitdepth( void ) {
	return currentTableBitdepth;
}



Real		Transcendental::mCRTSin( const Real radians ) {
	return SIN( radians );
}

Real		Transcendental::mCRTCos( const Real radians ) {
	return COS( radians );
}

void		Transcendental::makeTable( Real *&theTable ) {
	deleteTable( theTable );

	if ( sinTable == NULL )
	{
		int numTableEntries = 1 << currentTableBitdepth; 
		sinTable = new Real[ numTableEntries ];
		if ( sinTable != NULL )
		{
			int i;
			Real radians = 0,step = ((Real)1)/((Real)numTableEntries);
			for ( i = 0; i < numTableEntries; i++, radians += step )
			{
				sinTable[i] = mCRTSin( radians );
			}
		}

	}
}

void		Transcendental::deleteTable( Real *&theTable ) {
}


