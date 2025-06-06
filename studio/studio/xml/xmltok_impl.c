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

/*
The contents of this file are subject to the Mozilla Public License
Version 1.1 (the "License"); you may not use this file except in
compliance with the License. You may obtain a copy of the License at
http://www.mozilla.org/MPL/

Software distributed under the License is distributed on an "AS IS"
basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
License for the specific language governing rights and limitations
under the License.

The Original Code is expat.

The Initial Developer of the Original Code is James Clark.
Portions created by James Clark are Copyright (C) 1998, 1999
James Clark. All Rights Reserved.

Contributor(s):

Alternatively, the contents of this file may be used under the terms
of the GNU General Public License (the "GPL"), in which case the
provisions of the GPL are applicable instead of those above.  If you
wish to allow use of your version of this file only under the terms of
the GPL and not to allow others to use your version of this file under
the MPL, indicate your decision by deleting the provisions above and
replace them with the notice and other provisions required by the
GPL. If you do not delete the provisions above, a recipient may use
your version of this file under either the MPL or the GPL.
*/

#ifndef IS_INVALID_CHAR
#define IS_INVALID_CHAR(enc, ptr, n) (0)
#endif

#define INVALID_LEAD_CASE(n, ptr, nextTokPtr) \
    case BT_LEAD ## n: \
      if (end - ptr < n) \
	return XML_TOK_PARTIAL_CHAR; \
      if (IS_INVALID_CHAR(enc, ptr, n)) { \
        *(nextTokPtr) = (ptr); \
        return XML_TOK_INVALID; \
      } \
      ptr += n; \
      break;

#define INVALID_CASES(ptr, nextTokPtr) \
  INVALID_LEAD_CASE(2, ptr, nextTokPtr) \
  INVALID_LEAD_CASE(3, ptr, nextTokPtr) \
  INVALID_LEAD_CASE(4, ptr, nextTokPtr) \
  case BT_NONXML: \
  case BT_MALFORM: \
  case BT_TRAIL: \
    *(nextTokPtr) = (ptr); \
    return XML_TOK_INVALID;

#define CHECK_NAME_CASE(n, enc, ptr, end, nextTokPtr) \
   case BT_LEAD ## n: \
     if (end - ptr < n) \
       return XML_TOK_PARTIAL_CHAR; \
     if (!IS_NAME_CHAR(enc, ptr, n)) { \
       *nextTokPtr = ptr; \
       return XML_TOK_INVALID; \
     } \
     ptr += n; \
     break;

#define CHECK_NAME_CASES(enc, ptr, end, nextTokPtr) \
  case BT_NONASCII: \
    if (!IS_NAME_CHAR_MINBPC(enc, ptr)) { \
      *nextTokPtr = ptr; \
      return XML_TOK_INVALID; \
    } \
  case BT_NMSTRT: \
  case BT_HEX: \
  case BT_DIGIT: \
  case BT_NAME: \
  case BT_MINUS: \
    ptr += MINBPC(enc); \
    break; \
  CHECK_NAME_CASE(2, enc, ptr, end, nextTokPtr) \
  CHECK_NAME_CASE(3, enc, ptr, end, nextTokPtr) \
  CHECK_NAME_CASE(4, enc, ptr, end, nextTokPtr)

#define CHECK_NMSTRT_CASE(n, enc, ptr, end, nextTokPtr) \
   case BT_LEAD ## n: \
     if (end - ptr < n) \
       return XML_TOK_PARTIAL_CHAR; \
     if (!IS_NMSTRT_CHAR(enc, ptr, n)) { \
       *nextTokPtr = ptr; \
       return XML_TOK_INVALID; \
     } \
     ptr += n; \
     break;

#define CHECK_NMSTRT_CASES(enc, ptr, end, nextTokPtr) \
  case BT_NONASCII: \
    if (!IS_NMSTRT_CHAR_MINBPC(enc, ptr)) { \
      *nextTokPtr = ptr; \
      return XML_TOK_INVALID; \
    } \
  case BT_NMSTRT: \
  case BT_HEX: \
    ptr += MINBPC(enc); \
    break; \
  CHECK_NMSTRT_CASE(2, enc, ptr, end, nextTokPtr) \
  CHECK_NMSTRT_CASE(3, enc, ptr, end, nextTokPtr) \
  CHECK_NMSTRT_CASE(4, enc, ptr, end, nextTokPtr)

#ifndef PREFIX
#define PREFIX(ident) ident
#endif

/* ptr points to character following "<!-" */

static
int PREFIX(scanComment)(const ENCODING *enc, const char *ptr, const char *end,
			const char **nextTokPtr)
{
  if (ptr != end) {
    if (!CHAR_MATCHES(enc, ptr, '-')) {
      *nextTokPtr = ptr;
      return XML_TOK_INVALID;
    }
    ptr += MINBPC(enc);
    while (ptr != end) {
      switch (BYTE_TYPE(enc, ptr)) {
      INVALID_CASES(ptr, nextTokPtr)
      case BT_MINUS:
	if ((ptr += MINBPC(enc)) == end)
	  return XML_TOK_PARTIAL;
	if (CHAR_MATCHES(enc, ptr, '-')) {
	  if ((ptr += MINBPC(enc)) == end)
	    return XML_TOK_PARTIAL;
	  if (!CHAR_MATCHES(enc, ptr, '>')) {
	    *nextTokPtr = ptr;
	    return XML_TOK_INVALID;
	  }
	  *nextTokPtr = ptr + MINBPC(enc);
	  return XML_TOK_COMMENT;
	}
	break;
      default:
	ptr += MINBPC(enc);
	break;
      }
    }
  }
  return XML_TOK_PARTIAL;
}

/* ptr points to character following "<!" */

static
int PREFIX(scanDecl)(const ENCODING *enc, const char *ptr, const char *end,
		     const char **nextTokPtr)
{
  if (ptr == end)
    return XML_TOK_PARTIAL;
  switch (BYTE_TYPE(enc, ptr)) {
  case BT_MINUS:
    return PREFIX(scanComment)(enc, ptr + MINBPC(enc), end, nextTokPtr);
  case BT_LSQB:
    *nextTokPtr = ptr + MINBPC(enc);
    return XML_TOK_COND_SECT_OPEN;
  case BT_NMSTRT:
  case BT_HEX:
    ptr += MINBPC(enc);
    break;
  default:
    *nextTokPtr = ptr;
    return XML_TOK_INVALID;
  }
  while (ptr != end) {
    switch (BYTE_TYPE(enc, ptr)) {
    case BT_PERCNT:
      if (ptr + MINBPC(enc) == end)
	return XML_TOK_PARTIAL;
      /* don't allow <!ENTITY% foo "whatever"> */
      switch (BYTE_TYPE(enc, ptr + MINBPC(enc))) {
      case BT_S: case BT_CR: case BT_LF: case BT_PERCNT:
	*nextTokPtr = ptr;
	return XML_TOK_INVALID;
      }
      /* fall through */
    case BT_S: case BT_CR: case BT_LF:
      *nextTokPtr = ptr;
      return XML_TOK_DECL_OPEN;
    case BT_NMSTRT:
    case BT_HEX:
      ptr += MINBPC(enc);
      break;
    default:
      *nextTokPtr = ptr;
      return XML_TOK_INVALID;
    }
  }
  return XML_TOK_PARTIAL;
}

static
int PREFIX(checkPiTarget)(const ENCODING *enc, const char *ptr, const char *end, int *tokPtr)
{
  int upper = 0;
  *tokPtr = XML_TOK_PI;
  if (end - ptr != MINBPC(enc)*3)
    return 1;
  switch (BYTE_TO_ASCII(enc, ptr)) {
  case 'x':
    break;
  case 'X':
    upper = 1;
    break;
  default:
    return 1;
  }
  ptr += MINBPC(enc);
  switch (BYTE_TO_ASCII(enc, ptr)) {
  case 'm':
    break;
  case 'M':
    upper = 1;
    break;
  default:
    return 1;
  }
  ptr += MINBPC(enc);
  switch (BYTE_TO_ASCII(enc, ptr)) {
  case 'l':
    break;
  case 'L':
    upper = 1;
    break;
  default:
    return 1;
  }
  if (upper)
    return 0;
  *tokPtr = XML_TOK_XML_DECL;
  return 1;
}

/* ptr points to character following "<?" */

static
int PREFIX(scanPi)(const ENCODING *enc, const char *ptr, const char *end,
		   const char **nextTokPtr)
{
  int tok;
  const char *target = ptr;
  if (ptr == end)
    return XML_TOK_PARTIAL;
  switch (BYTE_TYPE(enc, ptr)) {
  CHECK_NMSTRT_CASES(enc, ptr, end, nextTokPtr)
  default:
    *nextTokPtr = ptr;
    return XML_TOK_INVALID;
  }
  while (ptr != end) {
    switch (BYTE_TYPE(enc, ptr)) {
    CHECK_NAME_CASES(enc, ptr, end, nextTokPtr)
    case BT_S: case BT_CR: case BT_LF:
      if (!PREFIX(checkPiTarget)(enc, target, ptr, &tok)) {
	*nextTokPtr = ptr;
	return XML_TOK_INVALID;
      }
      ptr += MINBPC(enc);
      while (ptr != end) {
        switch (BYTE_TYPE(enc, ptr)) {
        INVALID_CASES(ptr, nextTokPtr)
	case BT_QUEST:
	  ptr += MINBPC(enc);
	  if (ptr == end)
	    return XML_TOK_PARTIAL;
	  if (CHAR_MATCHES(enc, ptr, '>')) {
	    *nextTokPtr = ptr + MINBPC(enc);
	    return tok;
	  }
	  break;
	default:
	  ptr += MINBPC(enc);
	  break;
	}
      }
      return XML_TOK_PARTIAL;
    case BT_QUEST:
      if (!PREFIX(checkPiTarget)(enc, target, ptr, &tok)) {
	*nextTokPtr = ptr;
	return XML_TOK_INVALID;
      }
      ptr += MINBPC(enc);
      if (ptr == end)
	return XML_TOK_PARTIAL;
      if (CHAR_MATCHES(enc, ptr, '>')) {
	*nextTokPtr = ptr + MINBPC(enc);
	return tok;
      }
      /* fall through */
    default:
      *nextTokPtr = ptr;
      return XML_TOK_INVALID;
    }
  }
  return XML_TOK_PARTIAL;
}


static
int PREFIX(scanCdataSection)(const ENCODING *enc, const char *ptr, const char *end,
			     const char **nextTokPtr)
{
  int i;
  /* CDATA[ */
  if (end - ptr < 6 * MINBPC(enc))
    return XML_TOK_PARTIAL;
  for (i = 0; i < 6; i++, ptr += MINBPC(enc)) {
    if (!CHAR_MATCHES(enc, ptr, "CDATA["[i])) {
      *nextTokPtr = ptr;
      return XML_TOK_INVALID;
    }
  }
  *nextTokPtr = ptr;
  return XML_TOK_CDATA_SECT_OPEN;
}

static
int PREFIX(cdataSectionTok)(const ENCODING *enc, const char *ptr, const char *end,
			    const char **nextTokPtr)
{
  if (ptr == end)
    return XML_TOK_NONE;
  if (MINBPC(enc) > 1) {
    size_t n = end - ptr;
    if (n & (MINBPC(enc) - 1)) {
      n &= ~(MINBPC(enc) - 1);
      if (n == 0)
	return XML_TOK_PARTIAL;
      end = ptr + n;
    }
  }
  switch (BYTE_TYPE(enc, ptr)) {
  case BT_RSQB:
    ptr += MINBPC(enc);
    if (ptr == end)
      return XML_TOK_PARTIAL;
    if (!CHAR_MATCHES(enc, ptr, ']'))
      break;
    ptr += MINBPC(enc);
    if (ptr == end)
      return XML_TOK_PARTIAL;
    if (!CHAR_MATCHES(enc, ptr, '>')) {
      ptr -= MINBPC(enc);
      break;
    }
    *nextTokPtr = ptr + MINBPC(enc);
    return XML_TOK_CDATA_SECT_CLOSE;
  case BT_CR:
    ptr += MINBPC(enc);
    if (ptr == end)
      return XML_TOK_PARTIAL;
    if (BYTE_TYPE(enc, ptr) == BT_LF)
      ptr += MINBPC(enc);
    *nextTokPtr = ptr;
    return XML_TOK_DATA_NEWLINE;
  case BT_LF:
    *nextTokPtr = ptr + MINBPC(enc);
    return XML_TOK_DATA_NEWLINE;
  INVALID_CASES(ptr, nextTokPtr)
  default:
    ptr += MINBPC(enc);
    break;
  }
  while (ptr != end) {
    switch (BYTE_TYPE(enc, ptr)) {
#define LEAD_CASE(n) \
    case BT_LEAD ## n: \
      if (end - ptr < n || IS_INVALID_CHAR(enc, ptr, n)) { \
	*nextTokPtr = ptr; \
	return XML_TOK_DATA_CHARS; \
      } \
      ptr += n; \
      break;
    LEAD_CASE(2) LEAD_CASE(3) LEAD_CASE(4)
#undef LEAD_CASE
    case BT_NONXML:
    case BT_MALFORM:
    case BT_TRAIL:
    case BT_CR:
    case BT_LF:
    case BT_RSQB:
      *nextTokPtr = ptr;
      return XML_TOK_DATA_CHARS;
    default:
      ptr += MINBPC(enc);
      break;
    }
  }
  *nextTokPtr = ptr;
  return XML_TOK_DATA_CHARS;
}

/* ptr points to character following "</" */

static
int PREFIX(scanEndTag)(const ENCODING *enc, const char *ptr, const char *end,
		       const char **nextTokPtr)
{
  if (ptr == end)
    return XML_TOK_PARTIAL;
  switch (BYTE_TYPE(enc, ptr)) {
  CHECK_NMSTRT_CASES(enc, ptr, end, nextTokPtr)
  default:
    *nextTokPtr = ptr;
    return XML_TOK_INVALID;
  }
  while (ptr != end) {
    switch (BYTE_TYPE(enc, ptr)) {
    CHECK_NAME_CASES(enc, ptr, end, nextTokPtr)
    case BT_S: case BT_CR: case BT_LF:
      for (ptr += MINBPC(enc); ptr != end; ptr += MINBPC(enc)) {
	switch (BYTE_TYPE(enc, ptr)) {
	case BT_S: case BT_CR: case BT_LF:
	  break;
	case BT_GT:
	  *nextTokPtr = ptr + MINBPC(enc);
          return XML_TOK_END_TAG;
	default:
	  *nextTokPtr = ptr;
	  return XML_TOK_INVALID;
	}
      }
      return XML_TOK_PARTIAL;
#ifdef XML_NS
    case BT_COLON:
      /* no need to check qname syntax here, since end-tag must match exactly */
      ptr += MINBPC(enc);
      break;
#endif
    case BT_GT:
      *nextTokPtr = ptr + MINBPC(enc);
      return XML_TOK_END_TAG;
    default:
      *nextTokPtr = ptr;
      return XML_TOK_INVALID;
    }
  }
  return XML_TOK_PARTIAL;
}

/* ptr points to character following "&#X" */

static
int PREFIX(scanHexCharRef)(const ENCODING *enc, const char *ptr, const char *end,
			   const char **nextTokPtr)
{
  if (ptr != end) {
    switch (BYTE_TYPE(enc, ptr)) {
    case BT_DIGIT:
    case BT_HEX:
      break;
    default:
      *nextTokPtr = ptr;
      return XML_TOK_INVALID;
    }
    for (ptr += MINBPC(enc); ptr != end; ptr += MINBPC(enc)) {
      switch (BYTE_TYPE(enc, ptr)) {
      case BT_DIGIT:
      case BT_HEX:
	break;
      case BT_SEMI:
	*nextTokPtr = ptr + MINBPC(enc);
	return XML_TOK_CHAR_REF;
      default:
	*nextTokPtr = ptr;
	return XML_TOK_INVALID;
      }
    }
  }
  return XML_TOK_PARTIAL;
}

/* ptr points to character following "&#" */

static
int PREFIX(scanCharRef)(const ENCODING *enc, const char *ptr, const char *end,
			const char **nextTokPtr)
{
  if (ptr != end) {
    if (CHAR_MATCHES(enc, ptr, 'x'))
      return PREFIX(scanHexCharRef)(enc, ptr + MINBPC(enc), end, nextTokPtr);
    switch (BYTE_TYPE(enc, ptr)) {
    case BT_DIGIT:
      break;
    default:
      *nextTokPtr = ptr;
      return XML_TOK_INVALID;
    }
    for (ptr += MINBPC(enc); ptr != end; ptr += MINBPC(enc)) {
      switch (BYTE_TYPE(enc, ptr)) {
      case BT_DIGIT:
	break;
      case BT_SEMI:
	*nextTokPtr = ptr + MINBPC(enc);
	return XML_TOK_CHAR_REF;
      default:
	*nextTokPtr = ptr;
	return XML_TOK_INVALID;
      }
    }
  }
  return XML_TOK_PARTIAL;
}

/* ptr points to character following "&" */

static
int PREFIX(scanRef)(const ENCODING *enc, const char *ptr, const char *end,
		    const char **nextTokPtr)
{
  if (ptr == end)
    return XML_TOK_PARTIAL;
  switch (BYTE_TYPE(enc, ptr)) {
  CHECK_NMSTRT_CASES(enc, ptr, end, nextTokPtr)
  case BT_NUM:
    return PREFIX(scanCharRef)(enc, ptr + MINBPC(enc), end, nextTokPtr);
  default:
    *nextTokPtr = ptr;
    return XML_TOK_INVALID;
  }
  while (ptr != end) {
    switch (BYTE_TYPE(enc, ptr)) {
    CHECK_NAME_CASES(enc, ptr, end, nextTokPtr)
    case BT_SEMI:
      *nextTokPtr = ptr + MINBPC(enc);
      return XML_TOK_ENTITY_REF;
    default:
      *nextTokPtr = ptr;
      return XML_TOK_INVALID;
    }
  }
  return XML_TOK_PARTIAL;
}

/* ptr points to character following first character of attribute name */

static
int PREFIX(scanAtts)(const ENCODING *enc, const char *ptr, const char *end,
		     const char **nextTokPtr)
{
#ifdef XML_NS
  int hadColon = 0;
#endif
  while (ptr != end) {
    switch (BYTE_TYPE(enc, ptr)) {
    CHECK_NAME_CASES(enc, ptr, end, nextTokPtr)
#ifdef XML_NS
    case BT_COLON:
      if (hadColon) {
	*nextTokPtr = ptr;
	return XML_TOK_INVALID;
      }
      hadColon = 1;
      ptr += MINBPC(enc);
      if (ptr == end)
	return XML_TOK_PARTIAL;
      switch (BYTE_TYPE(enc, ptr)) {
      CHECK_NMSTRT_CASES(enc, ptr, end, nextTokPtr)
      default:
	*nextTokPtr = ptr;
	return XML_TOK_INVALID;
      }
      break;
#endif
    case BT_S: case BT_CR: case BT_LF:
      for (;;) {
	int t;

	ptr += MINBPC(enc);
	if (ptr == end)
	  return XML_TOK_PARTIAL;
	t = BYTE_TYPE(enc, ptr);
	if (t == BT_EQUALS)
	  break;
	switch (t) {
	case BT_S:
	case BT_LF:
	case BT_CR:
	  break;
	default:
	  *nextTokPtr = ptr;
	  return XML_TOK_INVALID;
	}
      }
    /* fall through */
    case BT_EQUALS:
      {
	int open;
#ifdef XML_NS
	hadColon = 0;
#endif
	for (;;) {
	  
	  ptr += MINBPC(enc);
	  if (ptr == end)
	    return XML_TOK_PARTIAL;
	  open = BYTE_TYPE(enc, ptr);
	  if (open == BT_QUOT || open == BT_APOS)
	    break;
	  switch (open) {
	  case BT_S:
	  case BT_LF:
	  case BT_CR:
	    break;
	  default:
	    *nextTokPtr = ptr;
	    return XML_TOK_INVALID;
	  }
	}
	ptr += MINBPC(enc);
	/* in attribute value */
	for (;;) {
	  int t;
	  if (ptr == end)
	    return XML_TOK_PARTIAL;
	  t = BYTE_TYPE(enc, ptr);
	  if (t == open)
	    break;
	  switch (t) {
	  INVALID_CASES(ptr, nextTokPtr)
	  case BT_AMP:
	    {
	      int tok = PREFIX(scanRef)(enc, ptr + MINBPC(enc), end, &ptr);
	      if (tok <= 0) {
		if (tok == XML_TOK_INVALID)
		  *nextTokPtr = ptr;
		return tok;
	      }
	      break;
	    }
	  case BT_LT:
	    *nextTokPtr = ptr;
	    return XML_TOK_INVALID;
	  default:
	    ptr += MINBPC(enc);
	    break;
	  }
	}
	ptr += MINBPC(enc);
	if (ptr == end)
	  return XML_TOK_PARTIAL;
	switch (BYTE_TYPE(enc, ptr)) {
	case BT_S:
	case BT_CR:
	case BT_LF:
	  break;
	case BT_SOL:
	  goto sol;
	case BT_GT:
	  goto gt;
	default:
	  *nextTokPtr = ptr;
	  return XML_TOK_INVALID;
	}
	/* ptr points to closing quote */
	for (;;) {
	  ptr += MINBPC(enc);
	  if (ptr == end)
	    return XML_TOK_PARTIAL;
	  switch (BYTE_TYPE(enc, ptr)) {
	  CHECK_NMSTRT_CASES(enc, ptr, end, nextTokPtr)
	  case BT_S: case BT_CR: case BT_LF:
	    continue;
	  case BT_GT:
          gt:
	    *nextTokPtr = ptr + MINBPC(enc);
	    return XML_TOK_START_TAG_WITH_ATTS;
	  case BT_SOL:
          sol:
	    ptr += MINBPC(enc);
	    if (ptr == end)
	      return XML_TOK_PARTIAL;
	    if (!CHAR_MATCHES(enc, ptr, '>')) {
	      *nextTokPtr = ptr;
	      return XML_TOK_INVALID;
	    }
	    *nextTokPtr = ptr + MINBPC(enc);
	    return XML_TOK_EMPTY_ELEMENT_WITH_ATTS;
	  default:
	    *nextTokPtr = ptr;
	    return XML_TOK_INVALID;
	  }
	  break;
	}
	break;
      }
    default:
      *nextTokPtr = ptr;
      return XML_TOK_INVALID;
    }
  }
  return XML_TOK_PARTIAL;
}

/* ptr points to character following "<" */

static
int PREFIX(scanLt)(const ENCODING *enc, const char *ptr, const char *end,
		   const char **nextTokPtr)
{
#ifdef XML_NS
  int hadColon;
#endif
  if (ptr == end)
    return XML_TOK_PARTIAL;
  switch (BYTE_TYPE(enc, ptr)) {
  CHECK_NMSTRT_CASES(enc, ptr, end, nextTokPtr)
  case BT_EXCL:
    if ((ptr += MINBPC(enc)) == end)
      return XML_TOK_PARTIAL;
    switch (BYTE_TYPE(enc, ptr)) {
    case BT_MINUS:
      return PREFIX(scanComment)(enc, ptr + MINBPC(enc), end, nextTokPtr);
    case BT_LSQB:
      return PREFIX(scanCdataSection)(enc, ptr + MINBPC(enc), end, nextTokPtr);
    }
    *nextTokPtr = ptr;
    return XML_TOK_INVALID;
  case BT_QUEST:
    return PREFIX(scanPi)(enc, ptr + MINBPC(enc), end, nextTokPtr);
  case BT_SOL:
    return PREFIX(scanEndTag)(enc, ptr + MINBPC(enc), end, nextTokPtr);
  default:
    *nextTokPtr = ptr;
    return XML_TOK_INVALID;
  }
#ifdef XML_NS
  hadColon = 0;
#endif
  /* we have a start-tag */
  while (ptr != end) {
    switch (BYTE_TYPE(enc, ptr)) {
    CHECK_NAME_CASES(enc, ptr, end, nextTokPtr)
#ifdef XML_NS
    case BT_COLON:
      if (hadColon) {
	*nextTokPtr = ptr;
	return XML_TOK_INVALID;
      }
      hadColon = 1;
      ptr += MINBPC(enc);
      if (ptr == end)
	return XML_TOK_PARTIAL;
      switch (BYTE_TYPE(enc, ptr)) {
      CHECK_NMSTRT_CASES(enc, ptr, end, nextTokPtr)
      default:
        *nextTokPtr = ptr;
        return XML_TOK_INVALID;
      }
      break;
#endif
    case BT_S: case BT_CR: case BT_LF:
      {
        ptr += MINBPC(enc);
	while (ptr != end) {
	  switch (BYTE_TYPE(enc, ptr)) {
	  CHECK_NMSTRT_CASES(enc, ptr, end, nextTokPtr)
	  case BT_GT:
	    goto gt;
	  case BT_SOL:
	    goto sol;
	  case BT_S: case BT_CR: case BT_LF:
	    ptr += MINBPC(enc);
	    continue;
	  default:
	    *nextTokPtr = ptr;
	    return XML_TOK_INVALID;
	  }
	  return PREFIX(scanAtts)(enc, ptr, end, nextTokPtr);
	}
	return XML_TOK_PARTIAL;
      }
    case BT_GT:
    gt:
      *nextTokPtr = ptr + MINBPC(enc);
      return XML_TOK_START_TAG_NO_ATTS;
    case BT_SOL:
    sol:
      ptr += MINBPC(enc);
      if (ptr == end)
	return XML_TOK_PARTIAL;
      if (!CHAR_MATCHES(enc, ptr, '>')) {
	*nextTokPtr = ptr;
	return XML_TOK_INVALID;
      }
      *nextTokPtr = ptr + MINBPC(enc);
      return XML_TOK_EMPTY_ELEMENT_NO_ATTS;
    default:
      *nextTokPtr = ptr;
      return XML_TOK_INVALID;
    }
  }
  return XML_TOK_PARTIAL;
}

static
int PREFIX(contentTok)(const ENCODING *enc, const char *ptr, const char *end,
		       const char **nextTokPtr)
{
  if (ptr == end)
    return XML_TOK_NONE;
  if (MINBPC(enc) > 1) {
    size_t n = end - ptr;
    if (n & (MINBPC(enc) - 1)) {
      n &= ~(MINBPC(enc) - 1);
      if (n == 0)
	return XML_TOK_PARTIAL;
      end = ptr + n;
    }
  }
  switch (BYTE_TYPE(enc, ptr)) {
  case BT_LT:
    return PREFIX(scanLt)(enc, ptr + MINBPC(enc), end, nextTokPtr);
  case BT_AMP:
    return PREFIX(scanRef)(enc, ptr + MINBPC(enc), end, nextTokPtr);
  case BT_CR:
    ptr += MINBPC(enc);
    if (ptr == end)
      return XML_TOK_TRAILING_CR;
    if (BYTE_TYPE(enc, ptr) == BT_LF)
      ptr += MINBPC(enc);
    *nextTokPtr = ptr;
    return XML_TOK_DATA_NEWLINE;
  case BT_LF:
    *nextTokPtr = ptr + MINBPC(enc);
    return XML_TOK_DATA_NEWLINE;
  case BT_RSQB:
    ptr += MINBPC(enc);
    if (ptr == end)
      return XML_TOK_TRAILING_RSQB;
    if (!CHAR_MATCHES(enc, ptr, ']'))
      break;
    ptr += MINBPC(enc);
    if (ptr == end)
      return XML_TOK_TRAILING_RSQB;
    if (!CHAR_MATCHES(enc, ptr, '>')) {
      ptr -= MINBPC(enc);
      break;
    }
    *nextTokPtr = ptr;
    return XML_TOK_INVALID;
  INVALID_CASES(ptr, nextTokPtr)
  default:
    ptr += MINBPC(enc);
    break;
  }
  while (ptr != end) {
    switch (BYTE_TYPE(enc, ptr)) {
#define LEAD_CASE(n) \
    case BT_LEAD ## n: \
      if (end - ptr < n || IS_INVALID_CHAR(enc, ptr, n)) { \
	*nextTokPtr = ptr; \
	return XML_TOK_DATA_CHARS; \
      } \
      ptr += n; \
      break;
    LEAD_CASE(2) LEAD_CASE(3) LEAD_CASE(4)
#undef LEAD_CASE
    case BT_RSQB:
      if (ptr + MINBPC(enc) != end) {
	 if (!CHAR_MATCHES(enc, ptr + MINBPC(enc), ']')) {
	   ptr += MINBPC(enc);
	   break;
	 }
	 if (ptr + 2*MINBPC(enc) != end) {
	   if (!CHAR_MATCHES(enc, ptr + 2*MINBPC(enc), '>')) {
	     ptr += MINBPC(enc);
	     break;
	   }
	   *nextTokPtr = ptr + 2*MINBPC(enc);
	   return XML_TOK_INVALID;
	 }
      }
      /* fall through */
    case BT_AMP:
    case BT_LT:
    case BT_NONXML:
    case BT_MALFORM:
    case BT_TRAIL:
    case BT_CR:
    case BT_LF:
      *nextTokPtr = ptr;
      return XML_TOK_DATA_CHARS;
    default:
      ptr += MINBPC(enc);
      break;
    }
  }
  *nextTokPtr = ptr;
  return XML_TOK_DATA_CHARS;
}

/* ptr points to character following "%" */

static
int PREFIX(scanPercent)(const ENCODING *enc, const char *ptr, const char *end,
			const char **nextTokPtr)
{
  if (ptr == end)
    return XML_TOK_PARTIAL;
  switch (BYTE_TYPE(enc, ptr)) {
  CHECK_NMSTRT_CASES(enc, ptr, end, nextTokPtr)
  case BT_S: case BT_LF: case BT_CR: case BT_PERCNT:
    *nextTokPtr = ptr;
    return XML_TOK_PERCENT;
  default:
    *nextTokPtr = ptr;
    return XML_TOK_INVALID;
  }
  while (ptr != end) {
    switch (BYTE_TYPE(enc, ptr)) {
    CHECK_NAME_CASES(enc, ptr, end, nextTokPtr)
    case BT_SEMI:
      *nextTokPtr = ptr + MINBPC(enc);
      return XML_TOK_PARAM_ENTITY_REF;
    default:
      *nextTokPtr = ptr;
      return XML_TOK_INVALID;
    }
  }
  return XML_TOK_PARTIAL;
}

static
int PREFIX(scanPoundName)(const ENCODING *enc, const char *ptr, const char *end,
			  const char **nextTokPtr)
{
  if (ptr == end)
    return XML_TOK_PARTIAL;
  switch (BYTE_TYPE(enc, ptr)) {
  CHECK_NMSTRT_CASES(enc, ptr, end, nextTokPtr)
  default:
    *nextTokPtr = ptr;
    return XML_TOK_INVALID;
  }
  while (ptr != end) {
    switch (BYTE_TYPE(enc, ptr)) {
    CHECK_NAME_CASES(enc, ptr, end, nextTokPtr)
    case BT_CR: case BT_LF: case BT_S:
    case BT_RPAR: case BT_GT: case BT_PERCNT: case BT_VERBAR:
      *nextTokPtr = ptr;
      return XML_TOK_POUND_NAME;
    default:
      *nextTokPtr = ptr;
      return XML_TOK_INVALID;
    }
  }
  return -XML_TOK_POUND_NAME;
}

static
int PREFIX(scanLit)(int open, const ENCODING *enc,
		    const char *ptr, const char *end,
		    const char **nextTokPtr)
{
  while (ptr != end) {
    int t = BYTE_TYPE(enc, ptr);
    switch (t) {
    INVALID_CASES(ptr, nextTokPtr)
    case BT_QUOT:
    case BT_APOS:
      ptr += MINBPC(enc);
      if (t != open)
	break;
      if (ptr == end)
	return -XML_TOK_LITERAL;
      *nextTokPtr = ptr;
      switch (BYTE_TYPE(enc, ptr)) {
      case BT_S: case BT_CR: case BT_LF:
      case BT_GT: case BT_PERCNT: case BT_LSQB:
	return XML_TOK_LITERAL;
      default:
	return XML_TOK_INVALID;
      }
    default:
      ptr += MINBPC(enc);
      break;
    }
  }
  return XML_TOK_PARTIAL;
}

static
int PREFIX(prologTok)(const ENCODING *enc, const char *ptr, const char *end,
		      const char **nextTokPtr)
{
  int tok;
  if (ptr == end)
    return XML_TOK_NONE;
  if (MINBPC(enc) > 1) {
    size_t n = end - ptr;
    if (n & (MINBPC(enc) - 1)) {
      n &= ~(MINBPC(enc) - 1);
      if (n == 0)
	return XML_TOK_PARTIAL;
      end = ptr + n;
    }
  }
  switch (BYTE_TYPE(enc, ptr)) {
  case BT_QUOT:
    return PREFIX(scanLit)(BT_QUOT, enc, ptr + MINBPC(enc), end, nextTokPtr);
  case BT_APOS:
    return PREFIX(scanLit)(BT_APOS, enc, ptr + MINBPC(enc), end, nextTokPtr);
  case BT_LT:
    {
      ptr += MINBPC(enc);
      if (ptr == end)
	return XML_TOK_PARTIAL;
      switch (BYTE_TYPE(enc, ptr)) {
      case BT_EXCL:
	return PREFIX(scanDecl)(enc, ptr + MINBPC(enc), end, nextTokPtr);
      case BT_QUEST:
	return PREFIX(scanPi)(enc, ptr + MINBPC(enc), end, nextTokPtr);
      case BT_NMSTRT:
      case BT_HEX:
      case BT_NONASCII:
      case BT_LEAD2:
      case BT_LEAD3:
      case BT_LEAD4:
	*nextTokPtr = ptr - MINBPC(enc);
	return XML_TOK_INSTANCE_START;
      }
      *nextTokPtr = ptr;
      return XML_TOK_INVALID;
    }
  case BT_CR:
    if (ptr + MINBPC(enc) == end)
      return -XML_TOK_PROLOG_S;
    /* fall through */
  case BT_S: case BT_LF:
    for (;;) {
      ptr += MINBPC(enc);
      if (ptr == end)
	break;
      switch (BYTE_TYPE(enc, ptr)) {
      case BT_S: case BT_LF:
	break;
      case BT_CR:
	/* don't split CR/LF pair */
	if (ptr + MINBPC(enc) != end)
	  break;
	/* fall through */
      default:
	*nextTokPtr = ptr;
	return XML_TOK_PROLOG_S;
      }
    }
    *nextTokPtr = ptr;
    return XML_TOK_PROLOG_S;
  case BT_PERCNT:
    return PREFIX(scanPercent)(enc, ptr + MINBPC(enc), end, nextTokPtr);
  case BT_COMMA:
    *nextTokPtr = ptr + MINBPC(enc);
    return XML_TOK_COMMA;
  case BT_LSQB:
    *nextTokPtr = ptr + MINBPC(enc);
    return XML_TOK_OPEN_BRACKET;
  case BT_RSQB:
    ptr += MINBPC(enc);
    if (ptr == end)
      return -XML_TOK_CLOSE_BRACKET;
    if (CHAR_MATCHES(enc, ptr, ']')) {
      if (ptr + MINBPC(enc) == end)
	return XML_TOK_PARTIAL;
      if (CHAR_MATCHES(enc, ptr + MINBPC(enc), '>')) {
	*nextTokPtr = ptr + 2*MINBPC(enc);
	return XML_TOK_COND_SECT_CLOSE;
      }
    }
    *nextTokPtr = ptr;
    return XML_TOK_CLOSE_BRACKET;
  case BT_LPAR:
    *nextTokPtr = ptr + MINBPC(enc);
    return XML_TOK_OPEN_PAREN;
  case BT_RPAR:
    ptr += MINBPC(enc);
    if (ptr == end)
      return -XML_TOK_CLOSE_PAREN;
    switch (BYTE_TYPE(enc, ptr)) {
    case BT_AST:
      *nextTokPtr = ptr + MINBPC(enc);
      return XML_TOK_CLOSE_PAREN_ASTERISK;
    case BT_QUEST:
      *nextTokPtr = ptr + MINBPC(enc);
      return XML_TOK_CLOSE_PAREN_QUESTION;
    case BT_PLUS:
      *nextTokPtr = ptr + MINBPC(enc);
      return XML_TOK_CLOSE_PAREN_PLUS;
    case BT_CR: case BT_LF: case BT_S:
    case BT_GT: case BT_COMMA: case BT_VERBAR:
    case BT_RPAR:
      *nextTokPtr = ptr;
      return XML_TOK_CLOSE_PAREN;
    }
    *nextTokPtr = ptr;
    return XML_TOK_INVALID;
  case BT_VERBAR:
    *nextTokPtr = ptr + MINBPC(enc);
    return XML_TOK_OR;
  case BT_GT:
    *nextTokPtr = ptr + MINBPC(enc);
    return XML_TOK_DECL_CLOSE;
  case BT_NUM:
    return PREFIX(scanPoundName)(enc, ptr + MINBPC(enc), end, nextTokPtr);
#define LEAD_CASE(n) \
  case BT_LEAD ## n: \
    if (end - ptr < n) \
      return XML_TOK_PARTIAL_CHAR; \
    if (IS_NMSTRT_CHAR(enc, ptr, n)) { \
      ptr += n; \
      tok = XML_TOK_NAME; \
      break; \
    } \
    if (IS_NAME_CHAR(enc, ptr, n)) { \
      ptr += n; \
      tok = XML_TOK_NMTOKEN; \
      break; \
    } \
    *nextTokPtr = ptr; \
    return XML_TOK_INVALID;
    LEAD_CASE(2) LEAD_CASE(3) LEAD_CASE(4)
#undef LEAD_CASE
  case BT_NMSTRT:
  case BT_HEX:
    tok = XML_TOK_NAME;
    ptr += MINBPC(enc);
    break;
  case BT_DIGIT:
  case BT_NAME:
  case BT_MINUS:
#ifdef XML_NS
  case BT_COLON:
#endif
    tok = XML_TOK_NMTOKEN;
    ptr += MINBPC(enc);
    break;
  case BT_NONASCII:
    if (IS_NMSTRT_CHAR_MINBPC(enc, ptr)) {
      ptr += MINBPC(enc);
      tok = XML_TOK_NAME;
      break;
    }
    if (IS_NAME_CHAR_MINBPC(enc, ptr)) {
      ptr += MINBPC(enc);
      tok = XML_TOK_NMTOKEN;
      break;
    }
    /* fall through */
  default:
    *nextTokPtr = ptr;
    return XML_TOK_INVALID;
  }
  while (ptr != end) {
    switch (BYTE_TYPE(enc, ptr)) {
    CHECK_NAME_CASES(enc, ptr, end, nextTokPtr)
    case BT_GT: case BT_RPAR: case BT_COMMA:
    case BT_VERBAR: case BT_LSQB: case BT_PERCNT:
    case BT_S: case BT_CR: case BT_LF:
      *nextTokPtr = ptr;
      return tok;
#ifdef XML_NS
    case BT_COLON:
      ptr += MINBPC(enc);
      switch (tok) {
      case XML_TOK_NAME:
	if (ptr == end)
	  return XML_TOK_PARTIAL;
	tok = XML_TOK_PREFIXED_NAME;
	switch (BYTE_TYPE(enc, ptr)) {
	CHECK_NAME_CASES(enc, ptr, end, nextTokPtr)
	default:
	  tok = XML_TOK_NMTOKEN;
	  break;
	}
	break;
      case XML_TOK_PREFIXED_NAME:
	tok = XML_TOK_NMTOKEN;
	break;
      }
      break;
#endif
    case BT_PLUS:
      if (tok == XML_TOK_NMTOKEN)  {
	*nextTokPtr = ptr;
	return XML_TOK_INVALID;
      }
      *nextTokPtr = ptr + MINBPC(enc);
      return XML_TOK_NAME_PLUS;
    case BT_AST:
      if (tok == XML_TOK_NMTOKEN)  {
	*nextTokPtr = ptr;
	return XML_TOK_INVALID;
      }
      *nextTokPtr = ptr + MINBPC(enc);
      return XML_TOK_NAME_ASTERISK;
    case BT_QUEST:
      if (tok == XML_TOK_NMTOKEN)  {
	*nextTokPtr = ptr;
	return XML_TOK_INVALID;
      }
      *nextTokPtr = ptr + MINBPC(enc);
      return XML_TOK_NAME_QUESTION;
    default:
      *nextTokPtr = ptr;
      return XML_TOK_INVALID;
    }
  }
  return -tok;
}

static
int PREFIX(attributeValueTok)(const ENCODING *enc, const char *ptr, const char *end,
			      const char **nextTokPtr)
{
  const char *start;
  if (ptr == end)
    return XML_TOK_NONE;
  start = ptr;
  while (ptr != end) {
    switch (BYTE_TYPE(enc, ptr)) {
#define LEAD_CASE(n) \
    case BT_LEAD ## n: ptr += n; break;
    LEAD_CASE(2) LEAD_CASE(3) LEAD_CASE(4)
#undef LEAD_CASE
    case BT_AMP:
      if (ptr == start)
	return PREFIX(scanRef)(enc, ptr + MINBPC(enc), end, nextTokPtr);
      *nextTokPtr = ptr;
      return XML_TOK_DATA_CHARS;
    case BT_LT:
      /* this is for inside entity references */
      *nextTokPtr = ptr;
      return XML_TOK_INVALID;
    case BT_LF:
      if (ptr == start) {
	*nextTokPtr = ptr + MINBPC(enc);
	return XML_TOK_DATA_NEWLINE;
      }
      *nextTokPtr = ptr;
      return XML_TOK_DATA_CHARS;
    case BT_CR:
      if (ptr == start) {
	ptr += MINBPC(enc);
	if (ptr == end)
	  return XML_TOK_TRAILING_CR;
	if (BYTE_TYPE(enc, ptr) == BT_LF)
	  ptr += MINBPC(enc);
	*nextTokPtr = ptr;
	return XML_TOK_DATA_NEWLINE;
      }
      *nextTokPtr = ptr;
      return XML_TOK_DATA_CHARS;
    case BT_S:
      if (ptr == start) {
	*nextTokPtr = ptr + MINBPC(enc);
	return XML_TOK_ATTRIBUTE_VALUE_S;
      }
      *nextTokPtr = ptr;
      return XML_TOK_DATA_CHARS;
    default:
      ptr += MINBPC(enc);
      break;
    }
  }
  *nextTokPtr = ptr;
  return XML_TOK_DATA_CHARS;
}

static
int PREFIX(entityValueTok)(const ENCODING *enc, const char *ptr, const char *end,
			   const char **nextTokPtr)
{
  const char *start;
  if (ptr == end)
    return XML_TOK_NONE;
  start = ptr;
  while (ptr != end) {
    switch (BYTE_TYPE(enc, ptr)) {
#define LEAD_CASE(n) \
    case BT_LEAD ## n: ptr += n; break;
    LEAD_CASE(2) LEAD_CASE(3) LEAD_CASE(4)
#undef LEAD_CASE
    case BT_AMP:
      if (ptr == start)
	return PREFIX(scanRef)(enc, ptr + MINBPC(enc), end, nextTokPtr);
      *nextTokPtr = ptr;
      return XML_TOK_DATA_CHARS;
    case BT_PERCNT:
      if (ptr == start)
	return PREFIX(scanPercent)(enc, ptr + MINBPC(enc), end, nextTokPtr);
      *nextTokPtr = ptr;
      return XML_TOK_DATA_CHARS;
    case BT_LF:
      if (ptr == start) {
	*nextTokPtr = ptr + MINBPC(enc);
	return XML_TOK_DATA_NEWLINE;
      }
      *nextTokPtr = ptr;
      return XML_TOK_DATA_CHARS;
    case BT_CR:
      if (ptr == start) {
	ptr += MINBPC(enc);
	if (ptr == end)
	  return XML_TOK_TRAILING_CR;
	if (BYTE_TYPE(enc, ptr) == BT_LF)
	  ptr += MINBPC(enc);
	*nextTokPtr = ptr;
	return XML_TOK_DATA_NEWLINE;
      }
      *nextTokPtr = ptr;
      return XML_TOK_DATA_CHARS;
    default:
      ptr += MINBPC(enc);
      break;
    }
  }
  *nextTokPtr = ptr;
  return XML_TOK_DATA_CHARS;
}

#ifdef XML_DTD

static
int PREFIX(ignoreSectionTok)(const ENCODING *enc, const char *ptr, const char *end,
			     const char **nextTokPtr)
{
  int level = 0;
  if (MINBPC(enc) > 1) {
    size_t n = end - ptr;
    if (n & (MINBPC(enc) - 1)) {
      n &= ~(MINBPC(enc) - 1);
      end = ptr + n;
    }
  }
  while (ptr != end) {
    switch (BYTE_TYPE(enc, ptr)) {
    INVALID_CASES(ptr, nextTokPtr)
    case BT_LT:
      if ((ptr += MINBPC(enc)) == end)
	return XML_TOK_PARTIAL;
      if (CHAR_MATCHES(enc, ptr, '!')) {
	if ((ptr += MINBPC(enc)) == end)
	  return XML_TOK_PARTIAL;
	if (CHAR_MATCHES(enc, ptr, '[')) {
	  ++level;
	  ptr += MINBPC(enc);
	}
      }
      break;
    case BT_RSQB:
      if ((ptr += MINBPC(enc)) == end)
	return XML_TOK_PARTIAL;
      if (CHAR_MATCHES(enc, ptr, ']')) {
	if ((ptr += MINBPC(enc)) == end)
	  return XML_TOK_PARTIAL;
	if (CHAR_MATCHES(enc, ptr, '>')) {
	  ptr += MINBPC(enc);
	  if (level == 0) {
	    *nextTokPtr = ptr;
	    return XML_TOK_IGNORE_SECT;
	  }
	  --level;
	}
      }
      break;
    default:
      ptr += MINBPC(enc);
      break;
    }
  }
  return XML_TOK_PARTIAL;
}

#endif /* XML_DTD */

static
int PREFIX(isPublicId)(const ENCODING *enc, const char *ptr, const char *end,
		       const char **badPtr)
{
  ptr += MINBPC(enc);
  end -= MINBPC(enc);
  for (; ptr != end; ptr += MINBPC(enc)) {
    switch (BYTE_TYPE(enc, ptr)) {
    case BT_DIGIT:
    case BT_HEX:
    case BT_MINUS:
    case BT_APOS:
    case BT_LPAR:
    case BT_RPAR:
    case BT_PLUS:
    case BT_COMMA:
    case BT_SOL:
    case BT_EQUALS:
    case BT_QUEST:
    case BT_CR:
    case BT_LF:
    case BT_SEMI:
    case BT_EXCL:
    case BT_AST:
    case BT_PERCNT:
    case BT_NUM:
#ifdef XML_NS
    case BT_COLON:
#endif
      break;
    case BT_S:
      if (CHAR_MATCHES(enc, ptr, '\t')) {
	*badPtr = ptr;
	return 0;
      }
      break;
    case BT_NAME:
    case BT_NMSTRT:
      if (!(BYTE_TO_ASCII(enc, ptr) & ~0x7f))
	break;
    default:
      switch (BYTE_TO_ASCII(enc, ptr)) {
      case 0x24: /* $ */
      case 0x40: /* @ */
	break;
      default:
	*badPtr = ptr;
	return 0;
      }
      break;
    }
  }
  return 1;
}

/* This must only be called for a well-formed start-tag or empty element tag.
Returns the number of attributes.  Pointers to the first attsMax attributes 
are stored in atts. */

static
int PREFIX(getAtts)(const ENCODING *enc, const char *ptr,
		    int attsMax, ATTRIBUTE *atts)
{
  enum { other, inName, inValue } state = inName;
  int nAtts = 0;
  int open = 0; /* defined when state == inValue;
		   initialization just to shut up compilers */

  for (ptr += MINBPC(enc);; ptr += MINBPC(enc)) {
    switch (BYTE_TYPE(enc, ptr)) {
#define START_NAME \
      if (state == other) { \
	if (nAtts < attsMax) { \
	  atts[nAtts].name = ptr; \
	  atts[nAtts].normalized = 1; \
	} \
	state = inName; \
      }
#define LEAD_CASE(n) \
    case BT_LEAD ## n: START_NAME ptr += (n - MINBPC(enc)); break;
    LEAD_CASE(2) LEAD_CASE(3) LEAD_CASE(4)
#undef LEAD_CASE
    case BT_NONASCII:
    case BT_NMSTRT:
    case BT_HEX:
      START_NAME
      break;
#undef START_NAME
    case BT_QUOT:
      if (state != inValue) {
	if (nAtts < attsMax)
	  atts[nAtts].valuePtr = ptr + MINBPC(enc);
        state = inValue;
        open = BT_QUOT;
      }
      else if (open == BT_QUOT) {
        state = other;
	if (nAtts < attsMax)
	  atts[nAtts].valueEnd = ptr;
	nAtts++;
      }
      break;
    case BT_APOS:
      if (state != inValue) {
	if (nAtts < attsMax)
	  atts[nAtts].valuePtr = ptr + MINBPC(enc);
        state = inValue;
        open = BT_APOS;
      }
      else if (open == BT_APOS) {
        state = other;
	if (nAtts < attsMax)
	  atts[nAtts].valueEnd = ptr;
	nAtts++;
      }
      break;
    case BT_AMP:
      if (nAtts < attsMax)
	atts[nAtts].normalized = 0;
      break;
    case BT_S:
      if (state == inName)
        state = other;
      else if (state == inValue
	       && nAtts < attsMax
	       && atts[nAtts].normalized
	       && (ptr == atts[nAtts].valuePtr
		   || BYTE_TO_ASCII(enc, ptr) != ' '
		   || BYTE_TO_ASCII(enc, ptr + MINBPC(enc)) == ' '
	           || BYTE_TYPE(enc, ptr + MINBPC(enc)) == open))
	atts[nAtts].normalized = 0;
      break;
    case BT_CR: case BT_LF:
      /* This case ensures that the first attribute name is counted
         Apart from that we could just change state on the quote. */
      if (state == inName)
        state = other;
      else if (state == inValue && nAtts < attsMax)
	atts[nAtts].normalized = 0;
      break;
    case BT_GT:
    case BT_SOL:
      if (state != inValue)
	return nAtts;
      break;
    default:
      break;
    }
  }
  /* not reached */
}

static
int PREFIX(charRefNumber)(const ENCODING *enc, const char *ptr)
{
  int result = 0;
  /* skip &# */
  ptr += 2*MINBPC(enc);
  if (CHAR_MATCHES(enc, ptr, 'x')) {
    for (ptr += MINBPC(enc); !CHAR_MATCHES(enc, ptr, ';'); ptr += MINBPC(enc)) {
      int c = BYTE_TO_ASCII(enc, ptr);
      switch (c) {
      case '0': case '1': case '2': case '3': case '4':
      case '5': case '6': case '7': case '8': case '9':
	result <<= 4;
	result |= (c - '0');
	break;
      case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':
	result <<= 4;
	result += 10 + (c - 'A');
	break;
      case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
	result <<= 4;
	result += 10 + (c - 'a');
	break;
      }
      if (result >= 0x110000)
	return -1;
    }
  }
  else {
    for (; !CHAR_MATCHES(enc, ptr, ';'); ptr += MINBPC(enc)) {
      int c = BYTE_TO_ASCII(enc, ptr);
      result *= 10;
      result += (c - '0');
      if (result >= 0x110000)
	return -1;
    }
  }
  return checkCharRefNumber(result);
}

static
int PREFIX(predefinedEntityName)(const ENCODING *enc, const char *ptr, const char *end)
{
  switch ((end - ptr)/MINBPC(enc)) {
  case 2:
    if (CHAR_MATCHES(enc, ptr + MINBPC(enc), 't')) {
      switch (BYTE_TO_ASCII(enc, ptr)) {
      case 'l':
	return '<';
      case 'g':
	return '>';
      }
    }
    break;
  case 3:
    if (CHAR_MATCHES(enc, ptr, 'a')) {
      ptr += MINBPC(enc);
      if (CHAR_MATCHES(enc, ptr, 'm')) {
	ptr += MINBPC(enc);
	if (CHAR_MATCHES(enc, ptr, 'p'))
	  return '&';
      }
    }
    break;
  case 4:
    switch (BYTE_TO_ASCII(enc, ptr)) {
    case 'q':
      ptr += MINBPC(enc);
      if (CHAR_MATCHES(enc, ptr, 'u')) {
	ptr += MINBPC(enc);
	if (CHAR_MATCHES(enc, ptr, 'o')) {
	  ptr += MINBPC(enc);
  	  if (CHAR_MATCHES(enc, ptr, 't'))
	    return '"';
	}
      }
      break;
    case 'a':
      ptr += MINBPC(enc);
      if (CHAR_MATCHES(enc, ptr, 'p')) {
	ptr += MINBPC(enc);
	if (CHAR_MATCHES(enc, ptr, 'o')) {
	  ptr += MINBPC(enc);
  	  if (CHAR_MATCHES(enc, ptr, 's'))
	    return '\'';
	}
      }
      break;
    }
  }
  return 0;
}

static
int PREFIX(sameName)(const ENCODING *enc, const char *ptr1, const char *ptr2)
{
  for (;;) {
    switch (BYTE_TYPE(enc, ptr1)) {
#define LEAD_CASE(n) \
    case BT_LEAD ## n: \
      if (*ptr1++ != *ptr2++) \
	return 0;
    LEAD_CASE(4) LEAD_CASE(3) LEAD_CASE(2)
#undef LEAD_CASE
      /* fall through */
      if (*ptr1++ != *ptr2++)
	return 0;
      break;
    case BT_NONASCII:
    case BT_NMSTRT:
#ifdef XML_NS
    case BT_COLON:
#endif
    case BT_HEX:
    case BT_DIGIT:
    case BT_NAME:
    case BT_MINUS:
      if (*ptr2++ != *ptr1++)
	return 0;
      if (MINBPC(enc) > 1) {
	if (*ptr2++ != *ptr1++)
	  return 0;
	if (MINBPC(enc) > 2) {
	  if (*ptr2++ != *ptr1++)
	    return 0;
          if (MINBPC(enc) > 3) {
	    if (*ptr2++ != *ptr1++)
      	      return 0;
	  }
	}
      }
      break;
    default:
      if (MINBPC(enc) == 1 && *ptr1 == *ptr2)
	return 1;
      switch (BYTE_TYPE(enc, ptr2)) {
      case BT_LEAD2:
      case BT_LEAD3:
      case BT_LEAD4:
      case BT_NONASCII:
      case BT_NMSTRT:
#ifdef XML_NS
      case BT_COLON:
#endif
      case BT_HEX:
      case BT_DIGIT:
      case BT_NAME:
      case BT_MINUS:
	return 0;
      default:
	return 1;
      }
    }
  }
  /* not reached */
}

static
int PREFIX(nameMatchesAscii)(const ENCODING *enc, const char *ptr1,
			     const char *end1, const char *ptr2)
{
  for (; *ptr2; ptr1 += MINBPC(enc), ptr2++) {
    if (ptr1 == end1)
      return 0;
    if (!CHAR_MATCHES(enc, ptr1, *ptr2))
      return 0;
  }
  return ptr1 == end1;
}

static
int PREFIX(nameLength)(const ENCODING *enc, const char *ptr)
{
  const char *start = ptr;
  for (;;) {
    switch (BYTE_TYPE(enc, ptr)) {
#define LEAD_CASE(n) \
    case BT_LEAD ## n: ptr += n; break;
    LEAD_CASE(2) LEAD_CASE(3) LEAD_CASE(4)
#undef LEAD_CASE
    case BT_NONASCII:
    case BT_NMSTRT:
#ifdef XML_NS
    case BT_COLON:
#endif
    case BT_HEX:
    case BT_DIGIT:
    case BT_NAME:
    case BT_MINUS:
      ptr += MINBPC(enc);
      break;
    default:
      return ptr - start;
    }
  }
}

static
const char *PREFIX(skipS)(const ENCODING *enc, const char *ptr)
{
  for (;;) {
    switch (BYTE_TYPE(enc, ptr)) {
    case BT_LF:
    case BT_CR:
    case BT_S:
      ptr += MINBPC(enc);
      break;
    default:
      return ptr;
    }
  }
}

static
void PREFIX(updatePosition)(const ENCODING *enc,
			    const char *ptr,
			    const char *end,
			    POSITION *pos)
{
  while (ptr != end) {
    switch (BYTE_TYPE(enc, ptr)) {
#define LEAD_CASE(n) \
    case BT_LEAD ## n: \
      ptr += n; \
      break;
    LEAD_CASE(2) LEAD_CASE(3) LEAD_CASE(4)
#undef LEAD_CASE
    case BT_LF:
      pos->columnNumber = (unsigned)-1;
      pos->lineNumber++;
      ptr += MINBPC(enc);
      break;
    case BT_CR:
      pos->lineNumber++;
      ptr += MINBPC(enc);
      if (ptr != end && BYTE_TYPE(enc, ptr) == BT_LF)
	ptr += MINBPC(enc);
      pos->columnNumber = (unsigned)-1;
      break;
    default:
      ptr += MINBPC(enc);
      break;
    }
    pos->columnNumber++;
  }
}

#undef DO_LEAD_CASE
#undef MULTIBYTE_CASES
#undef INVALID_CASES
#undef CHECK_NAME_CASE
#undef CHECK_NAME_CASES
#undef CHECK_NMSTRT_CASE
#undef CHECK_NMSTRT_CASES
