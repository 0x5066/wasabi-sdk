
Nullsoft Wasabi Component SDK Preview #2

Still a big pile of code, but getting better.

See the file sdklicense.txt for license.

The file layout is as follows.

attribs/
	code for the attribute system, which consists of basic variables
	that can be made public to other components and to the script system

cnv_rawpcm/
	sample converter that converts RAW audio data into PCM audio data

cnv_tonepcm/
	sample tone generator (in winamp3, use ctrl+l, tone://2600 to try it out)

common/
	code that is common between the studio.exe runtime and components

docs/
	some documentation

generic/
	a generic example component that does nothing but put up a window

services/
	header files for you to use to instantiate your own versions of
	services. Publishing services is how you extend the wasabi platform.

studio/
	header files for interfaces to studio.exe, most notably the API,
	which provides nearly all of the functionality
