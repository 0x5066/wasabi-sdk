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

#include "std.h"

#ifdef WIN32

#define WIN32_LEAN_AND_MEAN
#include <multimon.h>
#include <objbase.h>
#include <shellapi.h>	// for ShellExecute

#ifndef SPI_GETWHEELSCROLLLINES
#  define SPI_GETWHEELSCROLLLINES	104
#endif

#ifndef SPI_GETLISTBOXSMOOTHSCROLLING
#  define SPI_GETLISTBOXSMOOTHSCROLLING	0x1006
#endif

#endif	// WIN32

#include "../studio/assert.h"

#include "ptrlist.h"

#ifndef _NOSTUDIO
#include "pathparse.h"
#include "../studio/api.h"
#endif

#include "time.h"	// CUT if possible

#define USE_MALLOC
//#define COUNTUSAGE

#ifdef USE_MALLOC
#include <malloc.h>
#endif

#ifdef USE_MEM_CHECK
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

#ifdef COUNTUSAGE
static totalMem=0;
#endif

void *DO_MALLOC(int size EXTRA_INFO) {
  ASSERT(size > 0);
  if (size <= 0) return NULL;

#ifdef COUNTUSAGE
  totalMem+=size;
#endif

#ifndef USE_MEM_CHECK
 #ifndef USE_MALLOC
  return GlobalAlloc(GPTR, size);
 #else
  void *p = malloc(size);
  memset(p, 0, size);
  return p;
 #endif
#else
 #ifndef USE_MALLOC
  return GlobalAlloc(GPTR, size , file, line);
 #else
  void *p = _malloc_dbg(size, _NORMAL_BLOCK, file, line);
  memset(p, 0, size);
  return p;
 #endif
#endif
}
              
void *DO_CALLOC(int records, int recordsize EXTRA_INFO) {
  ASSERT(records > 0 && recordsize > 0);
  return DO_MALLOC(records * recordsize EXTRA_PARAMS);
}

void DO_FREE(void *ptr EXTRA_INFO) {
  if (ptr == NULL) return;

#ifdef COUNTUSAGE
 #ifndef USE_MALLOC
  totalMem-=GlobalSize(ptr);
 #else
  totalMem-=_msize(ptr);
 #endif
#endif

#ifndef USE_MEM_CHECK
 #ifndef USE_MALLOC
  GlobalFree(ptr);
 #else
  free(ptr);
 #endif
#else
 #ifndef USE_MALLOC
  GlobalFree(ptr, file, line); // FG> same as GlobalAlloc here, i'd like a way to translate my define inside the macro
 #else
  _free_dbg(ptr, _NORMAL_BLOCK);
 #endif
#endif
}

// Note: MEMCPY allows dest and src to overlap
void MEMCPY(void *dest, const void *src, int n) {
  if (n == 0) return;
  ASSERT(dest != NULL);
  ASSERT(src != NULL);
  ASSERT(n >= 0);
  MoveMemory(dest, src, n);
}

void *DO_MEMDUP(const void *src, int n EXTRA_INFO) {
  void *ret;
  ASSERT(n >= 0);
  if (src == NULL || n == 0) return NULL;
  ret = DO_MALLOC(n EXTRA_PARAMS);
  if (ret == NULL) return NULL;
  MEMCPY(ret, src, n);
  return ret;
}

#define FAKE_REALLOC

void *DO_REALLOC(void *ptr, int size EXTRA_INFO) {
  ASSERT(size >= 0);
  if (size == 0) {
#ifdef USE_MEM_CHECK
    DO_FREE(ptr, file, line);
#else
    DO_FREE(ptr);
#endif
    return NULL;
  }

#ifdef USE_MEM_CHECK
  if(ptr == NULL) return DO_MALLOC(size, file, line);
#else
  if(ptr == NULL) return DO_MALLOC(size);
#endif
  void *r=NULL; 

#ifdef COUNTUSAGE
 #ifndef USE_MALLOC
  totalMem-=GlobalSize(ptr);
 #else
  totalMem-=_msize(ptr);
 #endif
#endif

#ifndef USE_MEM_CHECK
 #ifndef USE_MALLOC
  r = GlobalReAlloc((HGLOBAL)ptr, size, GMEM_ZEROINIT);
 #else
  int oldsize = _msize(ptr);
  r = realloc(ptr, size);
 #endif // use_malloc
#else // mem_spy
 #ifndef USE_MALLOC
  r = GlobalReAlloc((HGLOBAL)ptr, size, file, line);
 #else
  int oldsize = _msize(ptr);
  r = _realloc_dbg(ptr, size, _NORMAL_BLOCK, file, line);
 #endif
#endif

  if (!r) { // realloc failed
    void *newblock = MALLOC(size);
#ifndef USE_MALLOC
    int s = GlobalSize(ptr);
#else
    int s = _msize(ptr);
#endif
    MEMCPY(newblock, ptr, MIN(s, size));
    FREE(ptr);
    r = newblock;
  } 

#ifdef COUNTUSAGE
 #ifndef USE_MALLOC
  totalMem+=size;
 #else
  totalMem+=size;
 #endif
#endif

  #ifdef USE_MALLOC
   if (size > oldsize)
    MEMZERO((char *)r+oldsize, size-oldsize);
  #endif
  return r;
}

void MEMZERO(void *dest, int nbytes) {
  ZeroMemory(dest, nbytes);
}

void MEMSET(void *dest, char c, int n) {
  ASSERT(n >= 0);
  FillMemory(dest, n, c);
}

int MEMCMP( const void *buf1, const void *buf2, int count ) {
  return memcmp(buf1,buf2,count);
}

#if 0
int TOUPPER(int c) {
  if (c >= 'a' && c <= 'z') c = (c - 'a') + 'A';
  return c;
}

int TOLOWER(int c) {
  if (c >= 'A' && c <= 'Z') c = (c - 'A') + 'a';
  return c;
}

int ISDIGIT(int c) {
  return (c >= '0' && c <= '9');
}

int ISALPHA(int c) {
  return (c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z');
}
#endif

char *DO_STRDUP(const char *ptr EXTRA_INFO) {
  char *ret;
  if (ptr == NULL) return NULL;
  ret = (char *)DO_MALLOC(STRLEN(ptr)+1 EXTRA_PARAMS);
  if (ret != NULL) {
    STRCPY(ret, ptr);
  }
  return ret;
}

int STRLEN(const char *str) {
  if (!str) return 0;
#ifdef WIN32
  return lstrlen(str);
#else
  int r = 0;
  while (*str++ != 0) r++;
  return r;
#endif
}

int STRCMP(const char *str1, const char *str2) {
  ASSERT(str1!=NULL);
  ASSERT(str2!=NULL);
#ifdef WIN32
#if 0
  if (!str1 && !str2) return 0;
  if (!str1 && str2) return 1;
  if (!str2) return -1;
#endif
  return lstrcmp(str1, str2);
#else
#error implement me
#endif
}

int STRICMP(const char *str1, const char *str2) {
  ASSERT(str1!=NULL);
  ASSERT(str2!=NULL);
#if 0
  if (!str1 && !str2) return 0;
  if (!str1 && str2) return 1;
  if (!str2) return -1;
#endif
  return lstrcmpi(str1, str2);
}

int STRNCMP(const char *str1, const char *str2, int l) {
  ASSERT(str1!=NULL);
  ASSERT(str2!=NULL);
#ifdef WIN32
#if 0
  if (!str1 && !str2) return 0;
  if (!str1 && str2) return 1;
  if (!str2) return -1;
#endif
  return strncmp(str1, str2, l);
#else
#error implement me
#endif
}

int STRNNCMP(const char *str1, const char *str2) {
  ASSERT(str1!=NULL);
  ASSERT(str2!=NULL);
  return STRNCMP(str1, str2, STRLEN(str2));
}

//FG> sorry brennan, this need to not be a const :)
void STRTOUPPER(char *p) {
  while (*p) {
    *p = TOUPPER(*p);
    p++;
  }
}

void STRTOLOWER(char *p) {
  while (*p) {
    *p = TOLOWER(*p);
    p++;
  }
}

int STRNICMP(const char *str1, const char *str2, int l) {
  ASSERT(str1!=NULL);
  ASSERT(str2!=NULL);

  ASSERT(l>=0);
  while (TOUPPER(*str1) == TOUPPER(*str2) && *str1 != 0 && *str2 != 0 && l--)
    str1++, str2++;
  if (l == 0) return 0;
  return (*str2 - *str1);
}

int STRNINCMP(const char *str1, const char *str2) {
  ASSERT(str1!=NULL);
  ASSERT(str2!=NULL);
  return STRNICMP(str1, str2, STRLEN(str2));
}

int STRNCASEEQL(const char *str1, const char *str2, int l) {
  ASSERT(str1!=NULL);
  ASSERT(str2!=NULL);
  return STRNICMP(str1, str2, l) == 0;
}

int STREQL(const char *str1, const char *str2) {
  ASSERT(str1!=NULL);
  ASSERT(str2!=NULL);
#ifdef WIN32
  return (lstrcmp(str1, str2)==0);
#else
  return (strcmp(str1, str2) == 0);
#endif
}

int STRCASEEQL(const char *str1, const char *str2) {
  ASSERT(str1!=NULL);
  ASSERT(str2!=NULL);
#ifdef WIN32
  return lstrcmpi(str1, str2)==0;
#else
  while (TOUPPER(*str1) == TOUPPER(*str2) && *str1 != 0 && *str2 != 0)
    str1++, str2++;
  return *str1 == *str2;
#endif
}

int STRISNUM(const char *p) {
  while (*p) {
    if (*p < '0' || *p > '9') return 0;
    p++;
  }
  return 1;
}

char TOUPPERANDSLASH(char a) {
  a = TOUPPER(a);
  if (a=='\\') a='/';
  return a;	
}

int PATHEQL(const char *str1, const char *str2) {
//NONPORTABLE
  if (str1 == NULL) {
    if (str2 == NULL) return TRUE;
    return FALSE;
  }
  while (TOUPPERANDSLASH(*str1) == TOUPPERANDSLASH(*str2) && *str1 != 0 && *str2 != 0) str1++, str2++;
  return *str1 == *str2;
}

char *STRSTR(const char *str1, const char *str2) {
  ASSERT(str1!=NULL);
  ASSERT(str2!=NULL);
  return strstr(str1, str2);
}

char *STRCASESTR(const char *str1, const char *str2) {
  ASSERT(str1!=NULL);
  ASSERT(str2!=NULL);
  char *p;
  int len = STRLEN(str2);
  for (p = (char *)str1; *p; p++) {
    if (STRNCASEEQL(p, str2, len)) return p;
  }
  return NULL;
}

int STRCASERPL(char *s, const char *w, const char *r, int maxsize) {

  int rl = STRLEN(r);
  int wl = STRLEN(w);
  int gain = rl - wl;
  int i=0;
  char *o = s;

  while (1) {
    char *f = STRCASESTR(o, w);
    if (!f) break;
    int pos = s - f;
    if (pos + wl + gain >= maxsize) break;
    if (gain < 0) {
      MEMCPY(f+wl+gain, f+wl, maxsize - pos - wl);
      s[maxsize-1] = 0;
    }
    else if (gain > 0) {
      MEMCPY(f+wl+gain, f+wl, maxsize - pos - wl - gain);
      s[maxsize-1] = 0;
    }
    i++;    
    MEMCPY(f, r, rl);
    o = f+rl;
  }
  return i;
}

void STRCPY(char *dest, const char *src) {
  ASSERT(dest != NULL);
  ASSERT(src != NULL);
#ifdef WIN32
  lstrcpy(dest, src);
#else
  strcpy(dest, src);
#endif
}

void STRNCPY(char *dest, const char *src, int maxchar) {
  ASSERT(dest != NULL);
  ASSERT(src != NULL);
  strncpy(dest, src, maxchar);
}

char *STRCHR(const char *str, int c) {
  ASSERT(str != NULL);
  const char *p = str;
  while (*p) {
    if (*p == c) return (char *)p;
    p++;
  }
  return NULL;
}

char *STRRCHR(const char *str, int c) {
  ASSERT(str != NULL);
  const char *p = str+STRLEN(str)-1;
  while (*p && p >= str) {
    if (*p == c) return (char *)p;
    p--;
  }
  return NULL;
}

void STRCAT(char *dest, const char *append) {
  ASSERT(dest != NULL);
  ASSERT(append != NULL);
#ifdef WIN32
  lstrcat(dest, append);
#else
  strcat(dest, append);
#endif
}

unsigned long STRTOUL(const char *s, char **p, int rx) {
  ASSERT(s != NULL);
  ASSERT(p != NULL);
  return strtoul(s,p,rx);
}

#ifdef NS_TRANSCENDENTALS
#		ifdef NS_REAL_DEFINED
#		include <math.h>
		Real SIN( Real a ) {
			return sin( a );
		}
		Real COS( Real a ) {
			return cos( a );
		}
#		endif//NS_REAL_DEFINED
#endif//NS_TRANSCENDENTALS

static Std blep;
static int rand_inited;

static double divider=0.;

Std::Std() {
  LARGE_INTEGER ll;
  QueryPerformanceFrequency(&ll);
  divider = (double)ll.QuadPart;
}

void Std::getMousePos(POINT *p) {
  ASSERT(p != NULL);
  GetCursorPos(p);
}

void Std::getMousePos(int *x, int *y) {
  POINT p;
  getMousePos(&p);
  if (x != NULL) *x = p.x;
  if (y != NULL) *y = p.y;
}

void Std::getMousePos(long *x, long *y) {
  getMousePos((int *)x, (int *)y);
}

void Std::setMousePos(POINT *p) {
  ASSERT(p != NULL);
  SetCursorPos(p->x, p->y);
}

void Std::setMousePos(int x, int y) {
  POINT p={x,y};
  setMousePos(&p);
}

void Std::getViewport(RECT *r, POINT *p, int full) {
  getViewport(r, p, NULL, NULL, full);
}

void Std::getViewport(RECT *r, RECT *sr, int full) {
  getViewport(r, NULL, sr, NULL, full);
}

void Std::getViewport(RECT *r, HWND wnd, int full) {
  getViewport(r, NULL, NULL, wnd, full);
}

void Std::getViewport(RECT *r, POINT *p, RECT *sr, HWND wnd, int full) {
  ASSERT(r != NULL);
	if (p || sr || wnd) {
	  HINSTANCE h=LoadLibrary("user32.dll");
	  if (h) {
			HMONITOR (WINAPI *Mfp)(POINT pt, DWORD dwFlags) = (HMONITOR (WINAPI *)(POINT,DWORD)) GetProcAddress(h,"MonitorFromPoint");
      HMONITOR (WINAPI *Mfr)(LPCRECT lpcr, DWORD dwFlags) = (HMONITOR (WINAPI *)(LPCRECT, DWORD)) GetProcAddress(h, "MonitorFromRect");
      HMONITOR (WINAPI *Mfw)(HWND wnd, DWORD dwFlags)=(HMONITOR (WINAPI *)(HWND, DWORD)) GetProcAddress(h, "MonitorFromWindow");
      BOOL (WINAPI *Gmi)(HMONITOR mon, LPMONITORINFO lpmi) = (BOOL (WINAPI *)(HMONITOR,LPMONITORINFO)) GetProcAddress(h,"GetMonitorInfoA");    
			if (Mfp && Mfr && Mfw && Gmi) {
			  HMONITOR hm;
			  if (p)
				  hm=Mfp(*p,MONITOR_DEFAULTTONULL);
				else if (sr)
				  hm=Mfr(sr,MONITOR_DEFAULTTONULL);
				else if (wnd)
				  hm=Mfw(wnd,MONITOR_DEFAULTTONULL);
        if (hm) {
          MONITORINFOEX mi;
          MEMZERO(&mi,sizeof(mi));
          mi.cbSize=sizeof(mi);

          if (Gmi(hm,&mi)) {
            if(!full) *r=mi.rcWork;
            else *r=mi.rcMonitor;
            FreeLibrary(h);
            return;
          }          
        }
			}
			FreeLibrary(h);
		}
	}
  SystemParametersInfo(SPI_GETWORKAREA,0,r,0);
}

int Std::pointInRect(RECT *r, POINT &p) {
  if (p.x < r->left ||
      p.x >= r->right ||
      p.y < r->top ||
      p.y >= r->bottom) return 0;
  return 1;
}

int Std::random() {
  if (!rand_inited++)
    srand(time(NULL));
  return rand();
}

void Std::usleep(int ms) {
  Sleep(ms);
}

stdtimeval Std::getTimeStamp() {
  return time(NULL);
}

stdtimevalms Std::getTimeStampMS() {
  LARGE_INTEGER ll;
  if (!QueryPerformanceCounter(&ll)) return GetTickCount(); // jf
  stdtimevalms ret = (stdtimevalms)ll.QuadPart;
  return ret /= divider;
}

DWORD Std::getTickCount() {
  return GetTickCount();
}

void Std::tolowerString(char *str) {
  while (*str++) *str = TOLOWER(*str);
}

void Std::ensureVisible(RECT *r) {
  RECT sr;
  POINT p={(r->right+r->left)/2,(r->bottom+r->top)/2};
  Std::getViewport(&sr,&p);
  int w = r->right-r->left;
  int h = r->bottom-r->top;
  if (r->bottom > sr.bottom) {
    r->bottom = sr.bottom;
    r->top = r->bottom-h;
  }
  if (r->right > sr.right) {
    r->right = sr.right;
    r->left = r->right-w;
  }
  if (r->left < sr.left) {
    r->left = sr.left;
    r->right = r->left+w;
  }
  if (r->top < sr.top) {
    r->top = sr.top;
    r->bottom = r->top+h;
  }
}

int Std::getScreenWidth() {
  RECT r;
	SystemParametersInfo(SPI_GETWORKAREA,0,&r,0);
  return r.right-r.left;
}

int Std::getScreenHeight() {
  RECT r;
	SystemParametersInfo(SPI_GETWORKAREA,0,&r,0);
  return r.bottom-r.top;
}

int Std::messageBox(char *txt, char *title, int flags) {
return MessageBox(NULL, txt, title, flags);
}

int Std::getDoubleClickDelay() {
  return GetDoubleClickTime();
}

#undef GetSystemMetrics //FG> DUH!
int Std::getDoubleClickX() {
  return GetSystemMetrics(SM_CYDOUBLECLK);
}

int Std::getDoubleClickY() {
  return GetSystemMetrics(SM_CXDOUBLECLK);
}

int Std::osparam_getScrollLines() {
  int ret = 3;
  SystemParametersInfo(SPI_GETWHEELSCROLLLINES, 0, &ret, 0);
  return ret;
}

int Std::osparam_getSmoothScroll() {
  int ret = 1;
  SystemParametersInfo(SPI_GETLISTBOXSMOOTHSCROLLING, 0, &ret, 0);
  return ret;
}

int Std::dirChar() {
#ifdef WIN32
  return '\\';
#else
  return '/';
#endif
}

const char *Std::dirCharStr() {
#ifdef WIN32
  return "\\";
#else
  return "/";
#endif
};

const char *Std::matchAllFiles() {
#ifdef WIN32
  return "*.*";
#else
  return "*";
#endif
}

const char *Std::dotDir() {
  return ".";
}

const char *Std::dotDotDir() {
  return "..";
}

const char *Std::scanstr_back(const char *str, const char *toscan, const char *defval) {
	const char *s=str+STRLEN(str)-1;
	if (STRLEN(str) < 1) return defval;
	if (STRLEN(toscan) < 1) return defval;
	while (1) {
		const char *t=toscan;
		while (*t)
			if (*t++ == *s) return s;
		t=CharPrev(str,s);
		if (t==s) return defval;
		s=t;
	}
}

const char *Std::extension(const char *fn) {
  const char *s = scanstr_back(fn,".\\/",fn-1);
  if (s < fn) return "";
  if (*s == '\\' || *s == '/') return "";
  return (s+1);
}

const char *Std::filename(const char *fn) {
  const char *s=scanstr_back(fn,"\\/",0);
  if(!s) return fn;
  return (s+1);
}

BOOL Std::isMatchPattern(const char *p) {
  while (*p) {
    switch (*p++) {
      case '?':
      case '*':
      case '[':
      case '\\':
        return TRUE;
    }
  }
  return FALSE;
}

BOOL Std::isValidMatchPattern(const char *p, int *error_type) {
  /* init error_type */
  *error_type = PATTERN_VALID;
  /* loop through pattern to EOS */
  while (*p) {
    /* determine pattern type */
    switch (*p) {
      /* check literal escape, it cannot be at end of pattern */
      case '\\':
			  if (!*++p) {
          *error_type = PATTERN_ESC;
          return FALSE;
        }
        p++;
        break;
      /* the [..] construct must be well formed */
      case '[':
        p++;
        /* if the next character is ']' then bad pattern */
        if (*p == ']') {
          *error_type = PATTERN_EMPTY;
          return FALSE;
        }
        /* if end of pattern here then bad pattern */
        if (!*p) {
          *error_type = PATTERN_CLOSE;
          return FALSE;
        }
        /* loop to end of [..] construct */
        while (*p != ']') {
          /* check for literal escape */
          if (*p == '\\') {
            p++;
            /* if end of pattern here then bad pattern */
            if (!*p++) {
              *error_type = PATTERN_ESC;
              return FALSE;
            }
          } else p++;
        /* if end of pattern here then bad pattern */
        if (!*p) {
          *error_type = PATTERN_CLOSE;
          return FALSE;
        }
        /* if this a range */
        if (*p == '-') {
          /* we must have an end of range */
					if (!*++p || *p == ']') {
            *error_type = PATTERN_RANGE;
            return FALSE;
          } else {
            /* check for literal escape */
            if (*p == '\\') p++;
            /* if end of pattern here then bad pattern */
            if (!*p++) {
              *error_type = PATTERN_ESC;
              return FALSE;
            }
          }
        }
      }
      break;
      /* all other characters are valid pattern elements */
      case '*':
      case '?':
      default:
        p++; /* "normal" character */
        break;
    }
  }
  return TRUE;
}

int Std::matche(register const char *p, register const char *t) {
  register char range_start, range_end;  /* start and end in range */

  BOOLEAN invert;             /* is this [..] or [!..] */
  BOOLEAN member_match;       /* have I matched the [..] construct? */
  BOOLEAN loop;               /* should I terminate? */

  for ( ; *p; p++, t++) {
    /* if this is the end of the text then this is the end of the match */
    if (!*t) {
      return ( *p == '*' && *++p == '\0' ) ? MATCH_VALID : MATCH_ABORT;
    }
    /* determine and react to pattern type */
    switch (*p) {
      case '?': /* single any character match */
        break;
			case '*': /* multiple any character match */
        return matche_after_star (p, t);

      /* [..] construct, single member/exclusion character match */
      case '[': {
        /* move to beginning of range */
        p++;
        /* check if this is a member match or exclusion match */
        invert = FALSE;
        if (*p == '!' || *p == '^') {
          invert = TRUE;
          p++;
        }
        /* if closing bracket here or at range start then we have a malformed pattern */
        if (*p == ']') return MATCH_PATTERN;

        member_match = FALSE;
        loop = TRUE;
        while (loop) {
          /* if end of construct then loop is done */
          if (*p == ']') {
            loop = FALSE;
            continue;
          }
          /* matching a '!', '^', '-', '\' or a ']' */
          if (*p == '\\') range_start = range_end = *++p;
          else  range_start = range_end = *p;
          /* if end of pattern then bad pattern (Missing ']') */
          if (!*p) return MATCH_PATTERN;
          /* check for range bar */
          if (*++p == '-') {
            /* get the range end */
            range_end = *++p;
            /* if end of pattern or construct then bad pattern */
            if (range_end == '\0' || range_end == ']') return MATCH_PATTERN;
					  /* special character range end */
            if (range_end == '\\') {
              range_end = *++p;
              /* if end of text then we have a bad pattern */
              if (!range_end) return MATCH_PATTERN;
            }
            /* move just beyond this range */
            p++;
          }
          /* if the text character is in range then match found.
             make sure the range letters have the proper
             relationship to one another before comparison */
          if (range_start < range_end) {
            if (*t >= range_start && *t <= range_end) {
              member_match = TRUE;
              loop = FALSE;
            }
          } else {
            if (*t >= range_end && *t <= range_start) {
              member_match = TRUE;
              loop = FALSE;
            }
          }
        }
        /* if there was a match in an exclusion set then no match */
        /* if there was no match in a member set then no match */
        if ((invert && member_match) || !(invert || member_match)) return MATCH_RANGE;
        /* if this is not an exclusion then skip the rest of the [...] construct that already matched. */
        if (member_match) {
          while (*p != ']') {
            /* bad pattern (Missing ']') */
            if (!*p) return MATCH_PATTERN;
            /* skip exact match */
            if (*p == '\\') {
              p++;
						  /* if end of text then we have a bad pattern */
              if (!*p) return MATCH_PATTERN;
            }
            /* move to next pattern char */
            p++;
					}
				}
        break;
      }
      case '\\':  /* next character is quoted and must match exactly */
        /* move pattern pointer to quoted char and fall through */
        p++;
        /* if end of text then we have a bad pattern */
        if (!*p) return MATCH_PATTERN;
        /* must match this character exactly */
      default:
        if (toupper(*p) != toupper(*t)) return MATCH_LITERAL;
    }
  }
	/* if end of text not reached then the pattern fails */
  if (*t) return MATCH_END;
  else return MATCH_VALID;
}

int Std::matche_after_star(register const char *p, register const char *t) {
  register int match = 0;
  register nextp;
  /* pass over existing ? and * in pattern */
  while ( *p == '?' || *p == '*' ) {
    /* take one char for each ? and + */
    if (*p == '?') {
      /* if end of text then no match */
      if (!*t++) return MATCH_ABORT;
    }
    /* move to next char in pattern */
    p++;
  }
  /* if end of pattern we have matched regardless of text left */
  if (!*p) return MATCH_VALID;
  /* get the next character to match which must be a literal or '[' */
  nextp = *p;
  if (nextp == '\\') {
    nextp = p[1];
    /* if end of text then we have a bad pattern */
    if (!nextp) return MATCH_PATTERN;
  }
  /* Continue until we run out of text or definite result seen */
  do {
    /* a precondition for matching is that the next character
       in the pattern match the next character in the text or that
       the next pattern char is the beginning of a range.  Increment
       text pointer as we go here */
    if (tolower(nextp) == tolower(*t) || nextp == '[') match = matche(p, t);
    /* if the end of text is reached then no match */
    if (!*t++) match = MATCH_ABORT;
  } while ( match != MATCH_VALID && match != MATCH_ABORT && match != MATCH_PATTERN);
  /* return result */
  return match;
}

BOOL Std::match(const char *p, const char *t) {
  int error_type;
  error_type = matche(p,t);
  return (error_type == MATCH_VALID ) ? TRUE : FALSE;
}

#ifndef _NOSTUDIO

#undef fopen
#undef fclose
#undef fseek
#undef ftell
#undef fread
#undef fwrite
#undef fgets
#undef fprintf
static PtrList<void> fileReaders;
FILE *FOPEN(const char *filename,const char *mode)
{
  if (!STRNINCMP(filename, "file:")) filename += STRLEN("file:");
  FILE *ret=fopen(filename,mode);
  if (ret) return ret;

  // File not found... try to open it with the file readers
  // but before that, resolve ".." in path so zip can
  // find it

  const char *rFilename = filename;
  char str[WA_MAX_PATH]="";

  if (STRSTR(filename, "..")) {
    PathParser pp(filename);
    for (int i=0;i<pp.getNumStrings();i++) {
      if (STREQL(pp.enumString(i), "..")) {
        PathParser pp2(str);
if (pp2.getNumStrings() <= 0) return NULL;
        ASSERTPR(pp2.getNumStrings() > 0, "we don't handle this right, and I'm not sure how to fix it because I'm not sure what the code should do with a leading .. --BU");
        int l = STRLEN(pp2.enumString(pp2.getNumStrings()-1));
        str[STRLEN(str)-l-1]=0;
        continue;
      }
      if (STREQL(pp.enumString(i), ".")) 
        continue;
      STRCAT(str, pp.enumString(i));
      STRCAT(str, "/");
    }
    str[STRLEN(str)-1]=0;
    rFilename = str;
  }

  if (api && (ret=(FILE *)api->fileOpen(rFilename,mode))) {
    fileReaders.addItem((void *)ret);
    return ret;
  }

	// File still not found ...
#ifdef _DEBUG
	char tmp[512];
	wsprintf(tmp,"Warning: can't open %s\n",filename);
	OutputDebugString(tmp);
#endif
  return 0;
}

int FCLOSE(FILE *stream) {
  if (fileReaders.searchItem((void *)stream) != -1) {
    fileReaders.removeItem((void *)stream);
    api->fileClose((void *)stream);
    return 0;
  } 
  return fclose(stream);
}

int FSEEK(FILE *stream, long offset, int origin) {
  if (fileReaders.searchItem((void *)stream)!=-1)
    return api->fileSeek(offset, origin,(void *)stream);
  return fseek(stream,offset,origin);
}

long FTELL(FILE *stream) {
  if (fileReaders.searchItem((void *)stream)!=-1)
    return api->fileTell((void *)stream);
  return ftell(stream);
}

size_t FREAD(void *buffer, size_t size, size_t count, FILE *stream) {
  if (fileReaders.searchItem((void *)stream)!=-1)
    return api->fileRead(buffer,size*count,(void *)stream);
  return fread(buffer,size,count,stream);
}

size_t FWRITE(const void *buffer, size_t size, size_t count, FILE *stream) {
  if (fileReaders.searchItem((void *)stream) != -1)
    return api->fileWrite(buffer,size*count,(void *)stream);
  return fwrite(buffer,size,count,stream);
}

int FGETSIZE(FILE *stream) {
  if (fileReaders.searchItem((void *)stream)!=-1)
    return api->fileGetFileSize((void *)stream);
  int p = ftell(stream);
  fseek(stream, 0, SEEK_END);
  int s = ftell(stream);
  fseek(stream, p, SEEK_SET);
  return s;
}

char *FGETS(char *string, int n, FILE *stream) {
  if (fileReaders.searchItem((void *)stream) != -1) {
    char c;
    char *p=string;
    for(int i=0;i<(n-1);i++) {
      if(!api->fileRead(&c,1,stream)) {
        if(!i) return NULL;
        break;
      }
      if(c==0x0d) continue;
      if(c==0x0a) break;
      *p++=c;
    }
    *p=0;
    return string;
  }
  return fgets(string,n,stream);
}

int FPRINTF(FILE *stream, const char *format , ...) {
  va_list args;
  int ret;
  va_start (args, format);	
  ret=vfprintf(stream,format,args);
  va_end (args);
  return ret;
}

const char *TMPNAM2(char *string, int val) {
  char tempPath[WA_MAX_PATH];
  static char tempName[WA_MAX_PATH];
  GetTempPath(WA_MAX_PATH, tempPath);
  if (tempPath[STRLEN(tempPath)-1] != '\\')
    STRCAT(tempPath, "\\");
  GetTempFileName(tempPath,"DTC",val,tempName);
  if(string) {
    STRCPY(string,tempName);
    return string;
  } else
    return tempName;
}

const char *TMPNAM(char *string) {
  return TMPNAM2(string, 0);
}

int FEXISTS(const char *filename) {
  return api->fileExists(filename);
}

int UNLINK(const char *filename) {
  return api->fileRemove(filename);
}

int MOVEFILE(const char *filename, const char *destfilename) {
  return api->fileMove(filename, destfilename);
}

int Std::getCurDir(char *str, int maxlen) {
#ifdef WIN32
  return GetCurrentDirectory(maxlen, str);
#else
#error port me!
#endif
}

int Std::setCurDir(const char *str) {
#ifdef WIN32
  return SetCurrentDirectory(str);
#else
#error port me!
#endif
}

void Std::GUIDprint(GUID &guid, char *str) {
#ifdef WIN32
  OLECHAR oguid[256]; // NONPORTABLE
  StringFromGUID2(guid, oguid, 256);
  wsprintf(str, "%$", oguid);
#else
#error port me!
#endif
}

THREADID Std::getCurrentThreadId() {
#ifdef WIN32
  return GetCurrentThreadId();
#else
#error port me!
#endif
}

void Std::getTempPath(int bufsize, char *dst) {
#ifdef WIN32
  GetTempPath(bufsize,dst);
#else
#error port me!
#endif
}

int Std::createDirectory(const char *dirname) {
#ifdef WIN32
  if(!CreateDirectory(dirname,NULL)) {
    // create all the path
    PathParser pp(dirname);
    int l=pp.getNumStrings();
    for(int i=2;i<=l;i++) {
      String dir;
      for(int j=0;j<i;j++) {
        dir+=StringPrintf("%s\\",pp.enumString(j));
        CreateDirectory(dir,NULL);
      }
    }
  }
  return 1;
#else
#error port me!
#endif
}

int Std::getFileInfos(const char *filename, fileInfoStruct *infos) {
#ifdef WIN32
  // GetFileAttributesEx doesn't exist in win95 or nt3-4 ...
/*  WIN32_FILE_ATTRIBUTE_DATA fi;
  if(!GetFileAttributesEx(filename, GetFileExInfoStandard, &fi)) return 0;
  infos->fileSizeHigh=fi.nFileSizeHigh;
  infos->fileSizeLow=fi.nFileSizeLow;
  infos->lastWriteTimeHigh=fi.ftLastWriteTime.dwHighDateTime;
  infos->lastWriteTimeLow=fi.ftLastWriteTime.dwLowDateTime;*/
  HANDLE h;
  WIN32_FIND_DATA fd;
  if((h=FindFirstFile(filename, &fd))==INVALID_HANDLE_VALUE) return 0;
  infos->fileSizeHigh=fd.nFileSizeHigh;
  infos->fileSizeLow=fd.nFileSizeLow;
  infos->lastWriteTimeHigh=fd.ftLastWriteTime.dwHighDateTime;
  infos->lastWriteTimeLow=fd.ftLastWriteTime.dwLowDateTime;
  FindClose(h);
  return 1;
#else
#error port me!
#endif
}

void Std::removeDirectory(const char *buf, int recurse) {
#ifdef WIN32
  if (recurse) {
    HANDLE h;
    WIN32_FIND_DATA fd;
    char path[WA_MAX_PATH];
    STRCPY(path,buf);
    char *p=path;
    while (*p) p++;
    STRCPY(p,"\\*.*");
    h = FindFirstFile(path,&fd);
    if (h != INVALID_HANDLE_VALUE) {
      do {
        if (fd.cFileName[0] != '.' ||
            (fd.cFileName[1] != '.' && fd.cFileName[1])) {
          STRCPY(p+1,fd.cFileName);
          if (fd.dwFileAttributes & FILE_ATTRIBUTE_READONLY) 
            SetFileAttributes(path,fd.dwFileAttributes^FILE_ATTRIBUTE_READONLY);
          if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) removeDirectory(path,recurse);
          else DeleteFile(path);
        }
      } while (FindNextFile(h,&fd));
      FindClose(h);
    }
    p[0]=0; // fix buffer
  }
  RemoveDirectory(buf);
#else
#error port me!
#endif
}

int Std::fileExists(const char *filename) {
#if 0
  //return (access(filename, 0) == 0); // gives bad linker warnings and forces io.h in
  FILE *f = fopen(filename, "rb");
  int r = (f != NULL);
  fclose(f);
  return r;
#endif
  return api->fileExists(filename);
}

void Std::shellExec(const char *cmd) {
#ifdef WIN32
  ShellExecute(NULL,"open",cmd,NULL,".",0);
#else
#error port me
#endif
}

#endif
