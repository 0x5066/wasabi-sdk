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

#ifndef _METATAGS_H
#define _METATAGS_H

// as of winamp 3.0:

// stuff found in/on file itself
#define MT_ARTIST	"Artist"
#define MT_ALBUM	"Album"
#define MT_TITLE	"Title"
#define MT_GENRE	"Genre"
#define MT_YEAR		"Year"
#define MT_COMMENT	"Comment"
#define MT_LENGTH	"Length"	// int type
#define MT_SIZE		"Size"		// int type
#define MT_TRACK	"Track"
#define MT_BITRATE	"Bitrate"
#define MT_SAMPLERATE	"Srate"
#define MT_NUMCHANNELS	"Nch"
#define MT_BITSPERSAMPLE "Bps"
#define MT_MODIFIED	"Modified"	// MT_TIMESTAMP

// aliases: don't actually hit db
#define MT_PLAYSTRING	"Playstring"
#define MT_INFO		"Info"	// pretty-printed file info

// stored by winamp
//CUT #define MT_GUID		"ComponentGUID"	//owning component GUID
#define MT_NAME		"Name"	// pretty-printed name
#define MT_ADDEDON	"AddedOn"	// MT_TIMESTAMP when added to DB
#define MT_LASTPLAYED	"Last played"
#define MT_NAME2	"Name2" // secondary name // TODO: kill this?
#define MT_DISABLED	"Disabled"	// if TRUE, don't playback

enum {
  MDT_ERROR = -1,
  MDT_NONE = 0,
  MDT_STRINGZ,	// ascii zero-terminated string
  MDT_INT,	// 4-byte integer
  MDT_TIME,	// 4-byte in milliseconds
  MDT_TIMESTAMP,// 4-byte in seconds since epoch, yay 2030 bug! same as time_t
  MDT_FLOAT,	// 4-byte single precision floating-point
  MDT_DOUBLE,	// 8-byte double precision floating-point
  MDT_BINARY,
  MDT_BOOLEAN,	// 4-byte, 0 or 1
  MDT_GUID,	// 8-byte GUID

  NUM_METADATATYPES
};

#endif
