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
#include "region.h"

#include "canvas.h"
#include "basewnd.h"

#include "../common/profiler.h"

#ifndef _NOSTUDIO
#include "../studio/api.h"
#endif

#define GETOSHANDLE(x) (const_cast<Region *>(x)->getOSHandle())

#define CBCLASS RegionI
START_DISPATCH;
  CB(REGION_GETOSHANDLE, getOSHandle);
  CB(REGION_CLONE, clone);
  VCB(REGION_DISPOSECLONE, disposeClone);
  CB(REGION_PTINREGION, ptInRegion);
  VCB(REGION_OFFSET, offset);
  VCB(REGION_GETBOX, getBox);
  VCB(REGION_SUBTRACTRGN, subtractRgn);
  VCB(REGION_SUBTRACTRECT, subtractRect);
  VCB(REGION_ADDRECT, addRect);
  VCB(REGION_ADD, add);
  VCB(REGION_AND, and);
  VCB(REGION_SETRECT, setRect);
  VCB(REGION_EMPTY, empty);
  CB(REGION_ISEMPTY, isEmpty);
  CB(REGION_EQUALS, equals);
  CB(REGION_ENCLOSED, enclosed);
  CB(REGION_INTERSECTRECT, intersectRect);
  CB(REGION_INTERSECTRGN, intersectRgn);
  CB(REGION_ISRECT, isRect);
  VCB(REGION_SCALE, scale);
  VCB(REGION_DEBUG, debug);
  CB(REGION_MAKEWNDREGION, makeWindowRegion);
  CB(REGION_GETNUMRECTS, getNumRects);
  CB(REGION_ENUMRECT, enumRect);
END_DISPATCH;
#undef CBCLASS

RegionI::RegionI() {
  hrgn = CreateRectRgn(0,0,0,0);
  init();
}

RegionI::RegionI(const RECT *r) {
  hrgn = CreateRectRgn(r->left,r->top,r->right,r->bottom);
  init();
  optimize();
}

RegionI::RegionI(int l, int t, int r, int b) {
  hrgn = CreateRectRgn(l,t,r,b);
  init();
  optimize();
}

RegionI::RegionI(OSREGION r) {
  OSREGION R = CreateRectRgn(0, 0, 0,0);
  CombineRgn(R, r, r, RGN_COPY);
  hrgn = R;
  init();
  optimize();
}

RegionI::RegionI(Canvas *c) {
  hrgn = CreateRectRgn(0,0,0,0);
  GetClipRgn(c->getHDC(), hrgn);
  init();
  optimize();
}

RegionI::~RegionI() {
  delete lastdebug;
  if (srv != NULL) srv->delRef(this);
  ASSERT(clonecount==0);
  if (srv == NULL && hrgn != NULL) DeleteObject(hrgn);
} 

void RegionI::init() {
  srv = NULL;
  clonecount = 0;
  lastdebug = NULL;
  optimized = 0;
}

Region *RegionI::clone() {
  Region *newregion = new RegionI(getOSHandle());
  clonecount++;
  return newregion;
}

void RegionI::disposeClone(Region *r) {
  RegionI *ri = static_cast<RegionI *>(r);
  delete ri; // todo: validate pointer before deleting
  clonecount--;
}

// returns a handle that SetWindowRgn understands (non portable). We should NOT delete this handle, windows will delete 
// it by itself upon setting a new region of destroying the window
OSREGION RegionI::makeWindowRegion() {
  deoptimize();
  OSREGION R = CreateRectRgn(0, 0, 0, 0);
  CombineRgn(R, hrgn, hrgn, RGN_COPY);
  optimize();
  return R;
}

RegionI::RegionI(SkinBitmap *bitmap, RECT *r, int xoffset, int yoffset, BOOL inverted, int dothreshold, char threshold, int thinverse, int minalpha) {
  init();

  PR_ENTER("alphatoregionrect");

#ifndef _NOSTUDIO

  const char *id = bitmap->getBitmapName();

  if (xoffset == 0 && yoffset == 0 && r == NULL && !inverted && !dothreshold && minalpha == 1 && id != NULL && *id != 0) {
    srv = api->imgldr_requestSkinRegion(id);
    if (srv != NULL) {
      srv->addRef(this);
      hrgn = srv->getRegion()->getOSHandle();
    }
  }

  if (srv == NULL) {

#endif

    if (r)
      hrgn = alphaToRegionRect(bitmap, xoffset, yoffset, TRUE, r->left, r->top, r->right-r->left, r->bottom-r->top, inverted, dothreshold, threshold, thinverse, minalpha);
    else
      hrgn = alphaToRegionRect(bitmap, xoffset, yoffset, FALSE, 0, 0, 0, 0, inverted, dothreshold, threshold, thinverse, minalpha);

#ifndef _NOSTUDIO

    if (id != NULL && *id != 0) {
      if (xoffset == 0 && yoffset == 0 && r == NULL && !inverted && !dothreshold && minalpha == 1) {
        api->imgldr_cacheSkinRegion(id, this);
        srv = api->imgldr_requestSkinRegion(id);
        if (srv != NULL) {
          srv->addRef(this);
          DeleteObject(hrgn);
          hrgn = srv->getRegion()->getOSHandle();
        }
      }
    } 
  }
#endif

  optimize();

  PR_LEAVE();
}

OSREGION RegionI::alphaToRegionRect(SkinBitmap *bitmap, int xoffset, int yoffset, BOOL portion, int _x, int _y, int _w, int _h, BOOL inverted, int dothreshold, unsigned char threshold, int thinverse, int minalpha) {
  return alphaToRegionRect(bitmap->getBits(), bitmap->getX(), bitmap->getY(), bitmap->getWidth(), bitmap->getHeight(), bitmap->getFullWidth(), bitmap->getFullHeight(), xoffset, yoffset, portion, _x, _y, _w, _h, inverted, dothreshold, threshold, thinverse, minalpha);
}

OSREGION RegionI::alphaToRegionRect(void *pbits32, int bmX, int bmY, int bmWidth, int bmHeight, int fullw, int fullh, int xoffset, int yoffset, BOOL portion, int _x, int _y, int _w, int _h, BOOL inverted, int dothreshold, unsigned char threshold, int thinverse, int minalpha) {
	OSREGION hRgn = NULL;
  if (!pbits32) return NULL;

	RGNDATA *pData;
	int y, x;

	// For better performances, we will use the ExtCreateRegion() function to create the
	// region. This function take a RGNDATA structure on entry. We will add rectangles by
	// amount of ALLOC_UNIT number in this structure.
  // JF> rects are 8 bytes, so this allocates just under 16kb of memory, no need to REALLOC
	#define MAXRECTS 2000
	pData = (RGNDATA *)MALLOC(sizeof(RGNDATAHEADER) + (sizeof(RECT) * MAXRECTS));
  if (!pData) return NULL;

	pData->rdh.dwSize = sizeof(RGNDATAHEADER);
	pData->rdh.iType = RDH_RECTANGLES;
	pData->rdh.nCount = pData->rdh.nRgnSize = 0;

  SetRect(&pData->rdh.rcBound, MAXLONG, MAXLONG, 0, 0);

  int x_end=(portion ? _w+_x : bmWidth);
  int y_end=(portion ? _h+_y : bmHeight);
  int x_start=(portion ? _x : 0);
  int y_start=(portion ? _y : 0);

  x_start += bmX;
  x_end += bmX;
  y_start += bmY;
  y_end += bmY;

  unsigned int iv=minalpha<<24;//inverted?0xff000000:0;

  int shiftx = xoffset - bmX;
  int shifty = yoffset - bmY;

	for (y = y_start; y < y_end; y++) {
		// Scan each bitmap pixel from left to right
    unsigned int *lineptr=((unsigned int *)pbits32) + fullw * y;
		for (x = x_start; x < x_end; x++) {
			// Search for a continuous range of "non transparent pixels"
			int x0 = x;
    	unsigned int *p = lineptr;
      if (dothreshold) {
        if (inverted) {
          if (thinverse) {
				    while (x < x_end) {
              unsigned int a=p[x];
					    if ((a&0xff000000) >= iv || 
                  (((((a & 0xFF) > threshold || ((a & 0xFF00) >> 8) > threshold || ((a & 0xFF0000) >> 16) > threshold)))))
								    break;
					    x++;
				    }
          }
          else {
				    while (x < x_end) {
              unsigned int a=p[x];
					    if ((a&0xff000000) >= iv || 
                  (((((a & 0xFF) < threshold || ((a & 0xFF00) >> 8) < threshold || ((a & 0xFF0000) >> 16) < threshold)))))
								    break;
					    x++;
				    }
          }
        } else {
          if (thinverse) {
				    while (x < x_end) {
              unsigned int a=p[x];
					    if ((a&0xff000000) < iv || 
                  (((((a & 0xFF) > threshold || ((a & 0xFF00) >> 8) > threshold || ((a & 0xFF0000) >> 16) > threshold)))))
								    break;
					    x++;
				    }
          }
          else {
				    while (x < x_end) {
              unsigned int a=p[x];
					    if ((a&0xff000000) < iv || 
                  (((((a & 0xFF) < threshold || ((a & 0xFF00) >> 8) < threshold || ((a & 0xFF0000) >> 16) < threshold)))))
								    break;
					    x++;
				    }
          }
        }
      }
      else {
        if (inverted) {
  			  while (x < x_end) {
		  		  if ((p[x] & 0xFF000000) >= iv) break;
            x++;
          }
        } else {
  			  while (x < x_end) {
		  		  if ((p[x] & 0xFF000000) < iv) break;
            x++;
          }
        }
      }

			if (x > x0) {
				SetRect(((RECT *)&pData->Buffer) + pData->rdh.nCount, x0 + shiftx, y + shifty, x + shiftx, y + 1 + shifty);

        pData->rdh.nCount++;

				if (x0 + shiftx < pData->rdh.rcBound.left) pData->rdh.rcBound.left = x0 + shiftx;
				if (y + shifty < pData->rdh.rcBound.top) pData->rdh.rcBound.top = y + shifty;
				if (x + shiftx > pData->rdh.rcBound.right) pData->rdh.rcBound.right = x + shiftx;
				if (y + 1 + shifty > pData->rdh.rcBound.bottom) pData->rdh.rcBound.bottom = y + 1 + shifty;

				// On Windows98, ExtCreateRegion() may fail if the number of rectangles is too
				// large (ie: > 4000). Therefore, we have to create the region by multiple steps.
				if (pData->rdh.nCount == MAXRECTS) {
					OSREGION h = ExtCreateRegion(NULL, sizeof(RGNDATAHEADER) + (sizeof(RECT) * pData->rdh.nCount), pData);
					if (hRgn) {
						CombineRgn(hRgn, hRgn, h, RGN_OR);
						DeleteObject(h);
					}
					else hRgn = h;
					pData->rdh.nCount = 0;
					SetRect(&pData->rdh.rcBound, MAXLONG, MAXLONG, 0, 0);
				}
			}
		}
	}

	// Create or extend the region with the remaining rectangles
	OSREGION h = ExtCreateRegion(NULL, sizeof(RGNDATAHEADER) + (sizeof(RECT) * pData->rdh.nCount), pData);
	if (hRgn)
	{
		CombineRgn(hRgn, hRgn, h, RGN_OR);
		DeleteObject(h);
	}
	else
		hRgn = h;

	// Clean up
	FREE(pData);

	return hRgn;
}

BOOL RegionI::ptInRegion(const POINT *pt) {
  if (optimized) return PtInRect(&optrect, *pt);
  ASSERT(hrgn != NULL);
  return PtInRegion(hrgn, pt->x, pt->y);
}

void RegionI::offset(int x, int y) {
  if (optimized) {
    optrect.left += x;
    optrect.top += y;
    optrect.right += x;
    optrect.bottom += y;
    return;
  }
  ASSERT(hrgn != NULL);
  if (srv) {
    hrgn = CreateRectRgn(0,0,0,0);
    add(srv->getRegion());
    srv->delRef(this);
    srv = NULL;
  }
  if (x == 0 && y == 0) return;
  OffsetRgn(hrgn, x, y);
}

void RegionI::getBox(RECT *r) {
  if (optimized) {
    *r = optrect;
    return;
  }
  ASSERT(r != NULL);
  GetRgnBox(hrgn, r);
}

OSREGION RegionI::getOSHandle() {
  deoptimize();
  ASSERT(hrgn != NULL);
  return hrgn;
}

void RegionI::subtractRect(const RECT *r) {
  RegionI *s = new RegionI(r);
  subtractRgn(s);
  delete s;
}

void RegionI::subtractRgn(const Region *reg) {
  if (srv) {
    hrgn = CreateRectRgn(0,0,0,0);
    add(srv->getRegion());
    srv->delRef(this);
    srv = NULL;
  }
  deoptimize();
  CombineRgn(hrgn, hrgn, GETOSHANDLE(reg), RGN_DIFF);
  optimize();
}

void RegionI::and(const Region *reg) {
  if (srv) {
    hrgn = CreateRectRgn(0,0,0,0);
    add(srv->getRegion());
    srv->delRef(this);
    srv = NULL;
  }
  deoptimize();
  CombineRgn(hrgn, hrgn, GETOSHANDLE(reg), RGN_AND);
  optimize();
}

void RegionI::addRect(const RECT *r) {
  RegionI *a = new RegionI(r);
  add(a);
  delete a;
}

void RegionI::add(const Region *reg) {
  if (srv) {
    hrgn = CreateRectRgn(0,0,0,0);
    add(srv->getRegion());
    srv->delRef(this);
    srv = NULL;
  }
  deoptimize();
  ASSERT(hrgn != NULL);
  ASSERT(reg != NULL);
  CombineRgn(hrgn, hrgn, GETOSHANDLE(reg), RGN_OR);
  optimize();
}

int RegionI::isEmpty() {
  RECT r;
  getBox(&r);
  if (r.left == r.right || r.bottom == r.top) return 1;
  return 0;
}

int RegionI::enclosed(const Region *r, Region *outside) {
  deoptimize();
  OSREGION del=NULL;
  if (!outside) 
    del = CreateRectRgn(0,0,0,0);
  int rs = CombineRgn(outside ? outside->getOSHandle() : del, hrgn, GETOSHANDLE(r), RGN_DIFF);
  if (del != NULL) DeleteObject(del);
  optimize();
  return rs == NULLREGION;
}

#define IntersectRgn(hrgnResult, hrgnA, hrgnB) CombineRgn(hrgnResult, hrgnA, hrgnB, RGN_AND)

int RegionI::intersectRgn(const Region *r, Region *intersection) {
  deoptimize();
  OSREGION del = NULL;
  if (intersection == NULL)
    del = CreateRectRgn(0,0,0,0);
  int rs = IntersectRgn(intersection ? intersection->getOSHandle() : del, hrgn, GETOSHANDLE(r));
  if (del != NULL) DeleteObject(del);
  optimize();
  return (rs != NULLREGION && rs != ERROR);
}

int RegionI::intersectRect(const RECT *r, Region *intersection) {
  RegionI *rect = new RegionI(r);
  int res = intersectRgn(rect, intersection);
  delete rect;
  return res;
}

void RegionI::empty() {
  if (srv) {
    hrgn = CreateRectRgn(0,0,0,0);
    ASSERT(hrgn != NULL);
    srv->delRef(this);
    srv = NULL;
    optimize();
    return;
  }
  deoptimize();
  ASSERT(hrgn != NULL);
  DeleteObject(hrgn);
  hrgn = CreateRectRgn(0,0,0,0);
  ASSERT(hrgn != NULL);
  optimize();
}

void RegionI::setRect(const RECT *r) {
  if (srv) {
    hrgn = CreateRectRgnIndirect(r);
    srv->delRef(this);
    srv = NULL;
    optimize();
    return;
  }
  deoptimize();
  ASSERT(hrgn != NULL);
  SetRectRgn(hrgn, r->left, r->top, r->right, r->bottom);
  optimize();
}

int RegionI::equals(const Region *r) {
  ASSERT(r);
  Region *cl = const_cast<Region*>(r)->clone();
  cl->subtractRgn(this);
  int ret = cl->isEmpty();
  const_cast<Region*>(r)->disposeClone(cl);
  cl = clone();
  cl->subtractRgn(r);
  ret &= cl->isEmpty();
  disposeClone(cl);
  return ret;
}

int RegionI::isRect() {
  if (optimized) return 1;
  RECT r;
  getBox(&r);
  RegionI *n = new RegionI(&r);
  ASSERT(n != NULL);
  if (equals(n)) {
    delete n;
    return 1;
  }
  delete n;
  return 0;
}

void RegionI::scale(double sx, double sy, BOOL round) {
  if (srv) {
    hrgn = CreateRectRgn(0,0,0,0);
    add(srv->getRegion());
    srv->delRef(this);
    srv = NULL;
  }
  deoptimize();
  ASSERT(hrgn != NULL);
  DWORD size=0;
  RECT box;
  getBox(&box);
  size = GetRegionData(hrgn, size, NULL); 
  if (!size) return;
  DWORD res;
  RGNDATA *data = (RGNDATA *)MALLOC(size);
  RECT *r = (RECT *)data->Buffer;

  res = GetRegionData(hrgn, size, (RGNDATA *)data);
  double adj = round?0.99999:0.0;
  int iadj = round?1:0;

  if (data->rdh.nCount == 1) {
    RECT nr = box;
    nr.left = (int)((double)nr.left * sx -iadj);
    nr.top = (int)((double)nr.top * sy -iadj);
    nr.right = (int)((double)nr.right * sx + adj);
    nr.bottom = (int)((double)nr.bottom * sy + adj);
    setRect(&nr);
    FREE(data);
    return;
  }

  for (int i=0;i<(int)data->rdh.nCount;i++) {
    r[i].left = (int)((double)r[i].left * sx -iadj);
    r[i].top = (int)((double)r[i].top * sy -iadj);
    r[i].right = (int)((double)r[i].right * sx + adj);
    r[i].bottom = (int)((double)r[i].bottom * sy + adj);
  }

  OSREGION nhrgn = ExtCreateRegion(NULL, size, data);
  if (!nhrgn) {
	 nhrgn = CreateRectRgn(0,0,0,0);
  }
  FREE(data);
  DeleteObject(hrgn);
  hrgn = nhrgn;
  optimize();
}

void RegionI::debug(int async) {
  if (!async) {
    SysCanvas c;
    RECT r;
    getBox(&r);
//    c.fillRect(&r, 0);
    InvertRgn(c.getHDC(), getOSHandle());
    Sleep(200);
    InvertRgn(c.getHDC(), getOSHandle());
  } else {
    SysCanvas c;
    RECT r;
    getBox(&r);
//    c.fillRect(&r, 0);
    if (lastdebug)
      InvertRgn(c.getHDC(), lastdebug->getOSHandle());
    delete lastdebug;
    lastdebug = new RegionI();
    lastdebug->add(this);
    InvertRgn(c.getHDC(), getOSHandle());
  }
}

// later we can cache this data or something if needed
int RegionI::getNumRects() {
  if (optimized) return 1;
  int bytes_needed = GetRegionData(hrgn, 0, NULL) + sizeof(RGNDATA);
  MemBlock<unsigned char> data(bytes_needed);
  GetRegionData(hrgn, bytes_needed, (LPRGNDATA)data.getMemory());
  RGNDATA *rgndata = reinterpret_cast<RGNDATA *>(data.getMemory());
  return rgndata->rdh.nCount;
}

int RegionI::enumRect(int n, RECT *r) {
  if (optimized) {
    if (n == 0) {
      if (r != NULL) *r = optrect;
      return 1;
    }
    return 0;
  }
  if (n < 0) return 0;
  int bytes_needed = GetRegionData(hrgn, 0, NULL) + sizeof(RGNDATA);
  MemBlock<unsigned char> data(bytes_needed);
  GetRegionData(hrgn, bytes_needed, (LPRGNDATA)data.getMemory());
  RGNDATA *rgndata = reinterpret_cast<RGNDATA *>(data.getMemory());
  int nrects = rgndata->rdh.nCount;
  if (n >= nrects) return 0;
  RECT *rectlist = reinterpret_cast<RECT*>(rgndata->Buffer);
  *r = rectlist[n];
  return 1;
}

void RegionI::optimize() {
  if (optimized) return;
  ASSERT(hrgn != NULL);
  if (srv != NULL) return; // region is cached and shared, do not optimize
  getBox(&optrect);
  OSREGION gr = CreateRectRgnIndirect(&optrect);
  OSREGION res = CreateRectRgn(0,0,0,0);
  CombineRgn(res, gr, hrgn, RGN_DIFF);
  DeleteObject(gr);
  RECT br;
  GetRgnBox(res, &br);
  DeleteObject(res);
  if (br.left == br.right || br.bottom == br.top) {
    optimized = 1;
    DeleteObject(hrgn);
    hrgn = NULL;
  }
}

void RegionI::deoptimize() {
  if (!optimized) return;
  ASSERT(hrgn == NULL);
  hrgn = CreateRectRgnIndirect(&optrect);
  ASSERT(hrgn != NULL);
  optimized = 0;
}


#define CBCLASS RegionServerI
START_DISPATCH;
  VCB(REGIONSERVER_ADDREF, addRef);
  VCB(REGIONSERVER_DELREF, delRef);
  CB(REGIONSERVER_GETREGION, getRegion);
END_DISPATCH;
