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

const ENCODING *NS(XmlGetUtf8InternalEncoding)(void)
{
  return &ns(internal_utf8_encoding).enc;
}

const ENCODING *NS(XmlGetUtf16InternalEncoding)(void)
{
#if XML_BYTE_ORDER == 12
  return &ns(internal_little2_encoding).enc;
#elif XML_BYTE_ORDER == 21
  return &ns(internal_big2_encoding).enc;
#else
  const short n = 1;
  return *(const char *)&n ? &ns(internal_little2_encoding).enc : &ns(internal_big2_encoding).enc;
#endif
}

static
const ENCODING *NS(encodings)[] = {
  &ns(latin1_encoding).enc,
  &ns(ascii_encoding).enc,
  &ns(utf8_encoding).enc,
  &ns(big2_encoding).enc,
  &ns(big2_encoding).enc,
  &ns(little2_encoding).enc,
  &ns(utf8_encoding).enc /* NO_ENC */
};

static
int NS(initScanProlog)(const ENCODING *enc, const char *ptr, const char *end,
		       const char **nextTokPtr)
{
  return initScan(NS(encodings), (const INIT_ENCODING *)enc, XML_PROLOG_STATE, ptr, end, nextTokPtr);
}

static
int NS(initScanContent)(const ENCODING *enc, const char *ptr, const char *end,
		       const char **nextTokPtr)
{
  return initScan(NS(encodings), (const INIT_ENCODING *)enc, XML_CONTENT_STATE, ptr, end, nextTokPtr);
}

int NS(XmlInitEncoding)(INIT_ENCODING *p, const ENCODING **encPtr, const char *name)
{
  int i = getEncodingIndex(name);
  if (i == UNKNOWN_ENC)
    return 0;
  SET_INIT_ENC_INDEX(p, i);
  p->initEnc.scanners[XML_PROLOG_STATE] = NS(initScanProlog);
  p->initEnc.scanners[XML_CONTENT_STATE] = NS(initScanContent);
  p->initEnc.updatePosition = initUpdatePosition;
  p->encPtr = encPtr;
  *encPtr = &(p->initEnc);
  return 1;
}

static
const ENCODING *NS(findEncoding)(const ENCODING *enc, const char *ptr, const char *end)
{
#define ENCODING_MAX 128
  char buf[ENCODING_MAX];
  char *p = buf;
  int i;
  XmlUtf8Convert(enc, &ptr, end, &p, p + ENCODING_MAX - 1);
  if (ptr != end)
    return 0;
  *p = 0;
  if (streqci(buf, "UTF-16") && enc->minBytesPerChar == 2)
    return enc;
  i = getEncodingIndex(buf);
  if (i == UNKNOWN_ENC)
    return 0;
  return NS(encodings)[i];
}

int NS(XmlParseXmlDecl)(int isGeneralTextEntity,
			const ENCODING *enc,
			const char *ptr,
			const char *end,
			const char **badPtr,
			const char **versionPtr,
			const char **encodingName,
			const ENCODING **encoding,
			int *standalone)
{
  return doParseXmlDecl(NS(findEncoding),
			isGeneralTextEntity,
			enc,
			ptr,
			end,
			badPtr,
			versionPtr,
			encodingName,
			encoding,
			standalone);
}
