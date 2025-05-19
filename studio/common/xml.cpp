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


#include "../bfc/std.h"
#include "xml.h"
#include "utf8.h"
#include "../bfc/string.h"

#define XMLVERSION "1.0"

XMLWrite::XMLWrite(const char *filename, const char *doctype) {
  Init(fopen(filename, "wt"), doctype);
}

XMLWrite::XMLWrite(FILE *file, const char *doctype) {
  Init(file,doctype);
}

void XMLWrite::Init(FILE *file, const char *doctype) {
  fp = file;
  ASSERT(fp != NULL);   // sheet, need exceptions here
  indent = 0;
  fprintf(fp, "<?xml version=\"" XMLVERSION "\" encoding='UTF-8' standalone=\"yes\"?>\n");
  pushCategory(doctype);
}

XMLWrite::~XMLWrite() {
  popCategory();
  fflush(fp);
  fclose(fp);
  ASSERT(titles.peek() == 0);
}

void XMLWrite::comment(const char *comment) {
  fprintf(fp, "<!-- %s -->\n", comment);
}

void XMLWrite::pushCategory(const char *title) {
  fprintf(fp, "%*s<%s>\n", indent*2, " ", title);
  indent++;
  titles.push(STRDUP((char *)title));
}

void XMLWrite::pushCategoryAttrib(const char *title, int nodata) {
  fprintf(fp, "%*s<%s", indent*2, " ", title);
  indent++;
  titles.push(nodata ? NULL : STRDUP((char *)title));
}

void XMLWrite::writeCategoryAttrib(const char *title, const int val) {
  fprintf(fp, " %s=\"%d\"", title, val);
}

void XMLWrite::writeCategoryAttrib(const char *title, const char *str) {
  fprintf(fp, " %s=\"", title);
  efprintf(fp, "%s", str);
  fprintf(fp, "\"");
}

void XMLWrite::closeCategoryAttrib() {
  if (titles.top() == NULL) fprintf(fp, " /");
  fprintf(fp, ">\n");
}

void XMLWrite::writeAttrib(const char *title, const char *text) {
  fprintf(fp, "%*s<%s>", indent*2, " ", title);
  efprintf(fp, "%s", text);
  fprintf(fp, "</%s>\n", title);
}

void XMLWrite::writeAttrib(const char *title, int val) {
  fprintf(fp, "%*s<%s>%d</%s>\n", indent*2, " ", title, val, title);
}

int XMLWrite::popCategory() {
  indent--;
  char *title;
  int r = titles.pop(&title);
  if (!r) return 0;
  if (title != NULL) {
    fprintf(fp, "%*s</%s>\n", indent*2, " ", title);
    FREE(title);
  }
  return titles.peek();
}

void XMLWrite::efprintf(FILE *fp, char *format, ...) {
  va_list v;
// http://www.w3.org/TR/REC-xml#syntax
  unsigned char buf[WA_MAX_PATH];
  va_start(v, format);
  int n = vsprintf((char *)buf, format, v);
  va_end(v);
  for (int i = 0; i < n; i++) {
    unsigned int c = buf[i];
    switch (c) {
      case '<': fputs("&lt;", fp); break;
      case '>': fputs("&gt;", fp); break;
      case '&': fputs("&amp;", fp); break;
      case '\"': fputs("&quot;", fp); break;
      case '\'': fputs("&apos;", fp); break;
      default:
        if (c > 127)
          fprintf(fp, "&#x%02x;", c);
        else
          fputc(c, fp);
      break;
    }
  }
}

static void unescape(char *str, char *esc, char replace) {
  ASSERT(str != NULL);
  if (*str == 0) return;	// ignore empty strings
  char *c;
  ASSERT(esc != NULL);
  int slen = STRLEN(esc);
  ASSERT(slen > 0);
  while ((c = STRSTR(str, esc)) != NULL) {
    *c = replace;
    if (c[slen-1]) STRCPY(c+1, c+slen);
    else c[1] = 0;
  }
}

void xmlUnescape(char *str) {
  unescape(str, "&lt;", '<');
  unescape(str, "&gt;", '>');
  unescape(str, "&amp;", '&');
  unescape(str, "&quot;", '\"');
  unescape(str, "&apos;", '\'');
  String dup = str;
  UTF8_to_ASCII(dup, str);
}
