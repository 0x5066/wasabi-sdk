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

#ifndef	   _TRANSCENDENTALS_H
#define	   _TRANSCENDENTALS_H

//
//	This object provides static inlines to the CRT and 
//	Fast Table Based Sin, Cos, etc... functions.  Add any
//	needed functions at your leisure.
typedef double Real;
const Real kPI = 3.1415926535;

class Transcendental {

	//
	// Enumerations	& Constants
	protected:
	public:	
		// Ungrouped Constant Values
		enum {
			DEFAULT_TABLE_BITDEPTH = 8,
			//
			NUMBER_OF_CONSTANTS
		} Constants;

	//
	// Data
	protected:
		static int		currentTableBitdepth;

		static Real *	sinTable;
	public:	

	//
	// Constructors & Destructors
	protected:
	public:	
		Transcendental();
		~Transcendental();

	//
	// Published Static Methods
	protected:
		static void		makeTable( Real *&theTable );
		static void		deleteTable( Real *&theTable );

	public:	
		//	Changing Bitdepth
		static void		initializeBitdepth( const int newBitdepth );
		static void		setBitdepth( const int newBitdepth );
		static int		getBitdepth( void );

		//	CRT Methods
		static Real		mCRTSin( const Real radians );
		static Real		mCRTCos( const Real radians );
};





#endif	// _TRANSCENDENTALS_H
