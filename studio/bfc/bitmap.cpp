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


//#define NO_SIMPLEFASTMODE

#include "blending.h"

#include "bitmap.h"
#ifndef _NOSTUDIO
#include "../studio/api.h"
#endif
#include "std.h"
#include "canvas.h"


#define ERRORBMP "wasabi.bitmapnotfound"
#define HARDERRORBMP "wacs/xml/guiobjects/wasabi/window/error.png" 

// do not define NO_MMX in this file. :)

#ifndef NO_MMX
static unsigned int const SkinBitmap_mmx_revn2[2]={0x01000100,0x01000100};
static unsigned int const SkinBitmap_mmx_zero[2];
static unsigned int const SkinBitmap_mmx_one[2]={1,0};
#define HAS_MMX Blenders::MMX_AVAILABLE()
#else
#define HAS_MMX 0
#endif


#ifndef _NOSTUDIO
SkinBitmap::SkinBitmap(HINSTANCE hInstance, int id, const char *forcegroup) {
  bitmapname = "";
  subimage_w=-1;
  subimage_h=-1;
  x_offset=-1;
  y_offset=-1;
  fullimage_w=fullimage_h=0;
  has_alpha = 0;
  ASSERT(hInstance != NULL);
  ownbits=1;
  bits=api->imgldr_makeBmp(hInstance, id,&has_alpha,&fullimage_w,&fullimage_h, forcegroup);
  if (bits == NULL) {
    last_failed = 1;
    bits = api->imgldr_makeBmp(HARDERRORBMP, &has_alpha, &fullimage_w, &fullimage_h);
  } else {
    last_failed = 0;
  }
}
#endif

void SkinBitmap::bmpToBits(HBITMAP hbmp, HDC defaultDC)
{
  if (hbmp && !bits) 
  {
    BITMAPINFO srcbmi={0,};
    HDC hMemDC, hMemDC2;
    HBITMAP hprev,hprev2;
    HBITMAP hsrcdib;
    void *srcdib;
    BITMAP bm;
    int r = GetObject(hbmp, sizeof(BITMAP), &bm);
    ASSERT(r != 0);

    fullimage_w=bm.bmWidth;
    fullimage_h=ABS(bm.bmHeight);

    int bmw=getWidth();
    int bmh=getHeight();
    int xo=getX();
    int yo=getY();

    srcbmi.bmiHeader.biSize=sizeof(srcbmi.bmiHeader);
    srcbmi.bmiHeader.biWidth=bmw;
    srcbmi.bmiHeader.biHeight=-bmh;
    srcbmi.bmiHeader.biPlanes=1;
    srcbmi.bmiHeader.biBitCount=32;
    srcbmi.bmiHeader.biCompression=BI_RGB;
    hMemDC = CreateCompatibleDC(NULL);
    hsrcdib=CreateDIBSection(hMemDC,&srcbmi,DIB_RGB_COLORS,&srcdib,NULL,0);
    ASSERTPR(hsrcdib != 0, "CreateDIBSection() failed #6");
    if (defaultDC) 
      hMemDC2 = defaultDC;
    else {
      hMemDC2 = CreateCompatibleDC(NULL);
      hprev2 = (HBITMAP) SelectObject(hMemDC2, hbmp);
    }
    hprev = (HBITMAP) SelectObject(hMemDC, hsrcdib);
    BitBlt(hMemDC,0,0,bmw,bmh,hMemDC2,xo,yo,SRCCOPY);
    SelectObject(hMemDC, hprev);
    if (!defaultDC) {
      SelectObject(hMemDC2, hprev2);
      DeleteDC(hMemDC2);
    }
    DeleteDC(hMemDC);
    bits=(ARGB32*)MALLOC(bmw*bmh*4);
    if (getHeight()+getY() > bm.bmHeight || getWidth()+getX() > bm.bmWidth) {
      char txt[128];
      SPRINTF(txt, "Subbitmap coordinates outside master bitmap [%d,%d,%d,%d in 0,0,%d,%d]", getX(), getY(), getWidth(), getHeight(), bm.bmWidth, bm.bmHeight);
      ASSERTPR(0, txt);
    }
    MEMCPY(bits,srcdib,bmw*bmh*sizeof(ARGB32));
    DeleteObject(hsrcdib);
    x_offset=-1;
    y_offset=-1;
    subimage_w=-1;
    subimage_h=-1;
    fullimage_w=bmw;
    fullimage_h=bmh;
  }
}

#ifndef _NOSTUDIO
SkinBitmap::SkinBitmap(const char *elementname, int _cached) {
  ASSERT(elementname!= NULL);

  bitmapname = elementname;
  BOOL isldd = TRUE;
  x_offset = -1;
  y_offset = -1;
  subimage_w = -1;
  subimage_h = -1;
  fullimage_w=fullimage_h=0;
  ownbits=1;
  bits = api->imgldr_requestSkinBitmap(elementname, &has_alpha, &x_offset, &y_offset, &subimage_w, &subimage_h, &fullimage_w, &fullimage_h,_cached);
  if (bits == NULL) bits = api->imgldr_requestSkinBitmap(ERRORBMP, &has_alpha, &x_offset, &y_offset, &subimage_w, &subimage_h, &fullimage_w, &fullimage_h,_cached);
  if (bits == NULL) bits = api->imgldr_makeBmp(HARDERRORBMP, &has_alpha, &fullimage_w, &fullimage_h);
  
  // check that coordinates are correct
  if(x_offset!=-1 && x_offset>fullimage_w) x_offset=fullimage_w-1;
  if(y_offset!=-1 && y_offset>fullimage_h) y_offset=fullimage_h-1;
  if(subimage_w!=-1 && (x_offset+subimage_w)>fullimage_w) subimage_w=fullimage_w-x_offset;
  if(subimage_h!=-1 && (y_offset+subimage_h)>fullimage_h) subimage_h=fullimage_h-y_offset;

  ASSERTPR(bits != NULL, elementname);
}
#endif

SkinBitmap::SkinBitmap(HBITMAP bitmap) {
  ASSERT(bitmap != NULL);
  subimage_w=-1;
  subimage_h=-1;
  x_offset=-1;
  y_offset=-1;
  bitmapname = "";
  fullimage_w=fullimage_h=0;
  has_alpha = 0;
  ownbits=1;
  bits = NULL;
  bmpToBits(bitmap,NULL);
}

SkinBitmap::SkinBitmap(HBITMAP bitmap, HDC dc, int _has_alpha, void *_bits) {
  subimage_w=-1;
  subimage_h=-1;
  x_offset=-1;
  y_offset=-1;
  bitmapname = "";
  fullimage_w=fullimage_h=0;
  ASSERT(bitmap != NULL);
  has_alpha = _has_alpha;
  bits = (ARGB32*)_bits;
  if (!_bits) 
  {
    ownbits=1;
    bmpToBits(bitmap,dc);
  }
  else
  {
    BITMAP bm;
    ownbits=0;
    int r = GetObject(bitmap, sizeof(BITMAP), &bm);
    ASSERT(r != 0);
    fullimage_w=bm.bmWidth;
    fullimage_h=ABS(bm.bmHeight);
  }
}


SkinBitmap::SkinBitmap(int w, int h, DWORD bgcolor) {
  subimage_w=-1;
  subimage_h=-1;
  x_offset=-1;
  y_offset=-1;
  fullimage_w=w;
  bitmapname = "";
  fullimage_h=h;

  bits=(ARGB32*)MALLOC(w*h*sizeof(ARGB32));

  DWORD *dw = (DWORD *)bits;
  int l=w*h;
  while (l--) *dw++=bgcolor;
  has_alpha = TRUE;
  ownbits=2; // 2 specifies should be FREE()'d
}

SkinBitmap::~SkinBitmap() {
  if (bits) {
    if (ownbits==2) FREE(bits);
#ifndef _NOSTUDIO
    else if (ownbits) api->imgldr_releaseSkinBitmap(bits);
#endif
  }
  bits=NULL;
}

void SkinBitmap::blit(CanvasBase *canvas, int x, int y) {
  RECT src, dst;
  src.left=0;
  src.top=0;
  src.bottom=getHeight();
  src.right=getWidth();
  dst.left=x;
  dst.right=x+getWidth();
  dst.top=y;
  dst.bottom=y+getHeight();
  blitToRect(canvas,&src,&dst,255);
}

void SkinBitmap::blitRectToTile(CanvasBase *canvas, RECT *dest, RECT *src, int xoffs, int yoffs, int alpha) {
  int startx,starty;

  int w,h;

  w = src->right-src->left;
  h = src->bottom-src->top;

  RECT c;
  if (GetClipBox(canvas->getHDC(), &c) == NULLREGION)
		c = *dest;

	starty = c.top-((c.top - dest->top) % h)- yoffs;
	startx = c.left-((c.left - dest->left) % w) - xoffs;

	for (int j=starty;j<c.bottom;j+=h)
		for (int i=startx;i<c.right;i+=w) {
      int xp=i;
      int yp=j;
      int xo=0;
      int yo=0;
      int _w=getWidth();
      int _h=getHeight();
      if (xp < c.left) {
        xo=c.left-xp;
        _w+=xo;
        xp=c.left;
      }
      if (yp < c.top) {
        yo=c.top-yp;
        _h+=yo;
        yp=c.top;
      }
      if (xp + _w >= c.right) _w=c.right-xp;
      if (yp + _h >= c.bottom) _h=c.bottom-yp;
      RECT _s={xo, yo, xo+_w, yo+_h};
      RECT _d={xp, yp, xp+_w, yp+_h};
      blitToRect(canvas, &_s, &_d, alpha);
    }

}


void SkinBitmap::blitTile(CanvasBase *canvas, RECT *dest, int xoffs, int yoffs, int alpha) {
  RECT r={0,0,getWidth(),getHeight()};
  blitRectToTile(canvas, dest, &r, xoffs, yoffs, alpha);
}

#pragma warning(push) 
#pragma warning(disable : 4799) 

void SkinBitmap::blitToRect(CanvasBase *canvas, RECT *src, RECT *dst, int alpha) { // only dst(top,left) are used
  if (alpha <= 0) return;
  if (alpha > 255) alpha = 255;

  HDC hdc = canvas->getHDC();
  if (hdc == NULL) return;
  void *dib=canvas->getBits();
  HBITMAP hdib,hprevdibdcbm;
  HDC hdibdc=NULL;
  int cwidth,cheight;

  RECT destrect=*dst;
  destrect.bottom=destrect.top+(src->bottom-src->top);
  destrect.right=destrect.left+(src->right-src->left);

  RECT c;
  int ctype=GetClipBox(hdc, &c);

  if (c.top > destrect.top) destrect.top=c.top;
  if (c.left > destrect.left) destrect.left=c.left;
  if (c.bottom < destrect.bottom) destrect.bottom=c.bottom;
  if (c.right < destrect.right) destrect.right=c.right;

#ifdef NO_SIMPLEFASTMODE
  dib=NULL;
#endif

  if (destrect.right <= destrect.left || destrect.bottom <= destrect.top) return;
  int xs,yp,xe,ye;

  if (!dib || canvas->getDim(NULL,&cheight,&cwidth) || !cwidth || cheight < 1 || ctype == COMPLEXREGION)
  {
    cwidth=destrect.right-destrect.left;
    cheight=destrect.bottom-destrect.top;
    BITMAPINFO bmidib={0,};
    bmidib.bmiHeader.biSize=sizeof(bmidib.bmiHeader);
    bmidib.bmiHeader.biWidth=cwidth;
    bmidib.bmiHeader.biHeight=-cheight;
    bmidib.bmiHeader.biPlanes=1;
    bmidib.bmiHeader.biBitCount=32;
    bmidib.bmiHeader.biCompression=BI_RGB;
    hdib=CreateDIBSection(hdc,&bmidib,DIB_RGB_COLORS,&dib,NULL,0);
    ASSERTPR(hdib != 0, "CreateDIBSection() failed #3");
    hdibdc = CreateCompatibleDC(hdc);
    hprevdibdcbm = (HBITMAP)SelectObject(hdibdc, hdib);
    if (has_alpha || alpha < 255) BitBlt(hdibdc,0,0,cwidth,cheight,hdc,destrect.left,destrect.top,SRCCOPY);

    xs=0;
    yp=0;
    xe=cwidth;
    ye=cheight;
  }
  else 
  {
    xs=destrect.left;
    xe=destrect.right;
    yp=destrect.top;
    ye=destrect.bottom;

    cwidth/=4;
  }
  int xpo=(dst->left-destrect.left+xs)-(getX()+src->left);
  int ypo=(dst->top-destrect.top+yp)-(getY()+src->top);

  if (yp < 0) yp=0;
  if (xs < 0) xs=0;

  if (yp<getY()+ypo) yp=ypo+getY();
  if (xs<getX()+xpo) xs=xpo+getX();

  if (xe > getWidth()+getX()+xpo) xe=getWidth()+getX()+xpo;
  if (ye > getHeight()+getY()+ypo) ye=getHeight()+getY()+ypo;

  // blend bitmap to dib

  if (xs<xe) for (; yp < ye; yp ++) {
    int xp=xe-xs;
    unsigned int *dest=((unsigned int*)dib) + cwidth*yp + xs;
    unsigned int *src=((unsigned int*)bits) + (yp-ypo)*fullimage_w + (xs-xpo);
    
    if (!has_alpha && alpha==255) // simple copy
    {
      MEMCPY(dest,src,xp*4);
    }   
    else if (!has_alpha) { // no alpha channel info, but just a simple blend
      if (!HAS_MMX)
        while (xp--) *dest++ = Blenders::BLEND_ADJ1(*src++, *dest, alpha);
      #ifndef NO_MMX
        else
        {
          if (xp>1) __asm
          {
            movd mm3, [alpha]
            mov ecx, xp

            movq mm4, [SkinBitmap_mmx_revn2]
            packuswb mm3, mm3 // 0000HHVV

            paddusw mm3, [SkinBitmap_mmx_one]
            mov edi, dest

            punpcklwd mm3, mm3 // HHVVHHVV
            mov esi, src
        
            punpckldq mm3, mm3 // HHVVHHVV HHVVHHVV
            shr ecx, 1

            psubw mm4, mm3

            align 16
            _blitAlpha_Loop1:
  
              movd mm0, [edi]

              movd mm1, [esi]
              punpcklbw mm0, [SkinBitmap_mmx_zero]
        
              movd mm7, [edi+4]
              punpcklbw mm1, [SkinBitmap_mmx_zero]

              pmullw mm0, mm4
              pmullw mm1, mm3

              movd mm6, [esi+4]
              punpcklbw mm7, [SkinBitmap_mmx_zero]
            
              punpcklbw mm6, [SkinBitmap_mmx_zero]

              pmullw mm7, mm4      
              pmullw mm6, mm3

              paddw mm0, mm1

              psrlw mm0, 8

              packuswb mm0, mm0
              add esi, 8   

              movd [edi], mm0
              paddw mm7, mm6
           
              psrlw mm7, 8
    
              packuswb mm7, mm7

              movd [edi+4], mm7

              add edi, 8

            dec ecx
            jnz _blitAlpha_Loop1
            mov src, esi
            mov dest, edi

          }
          if (xp & 1) *dest++ = Blenders::BLEND_ADJ1_MMX(*src++, *dest, alpha);
        } // mmx available
      #endif // !NO_MMX
    }
    else if (alpha == 255) { // no global alpha, just alpha channel
      if (!HAS_MMX)
        while (xp--) *dest++ = Blenders::BLEND_ADJ2(*dest, *src++);
      #ifndef NO_MMX
        else
        {
          if (xp > 1) __asm
          {
            mov ecx, xp
            shr ecx, 1
            mov edi, dest
            mov esi, src
            align 16
            _blitAlpha_Loop2:

            movd mm3, [esi]
            movd mm5, [esi+4]

            movq mm2, [SkinBitmap_mmx_revn2]
            psrld mm3, 24

            movq mm4, [SkinBitmap_mmx_revn2]
            psrld mm5, 24
          
            movd mm0, [edi]
            packuswb mm3, mm3 // 0000HHVV

            movd mm1, [esi]
            packuswb mm5, mm5 // 0000HHVV
          
            movd mm6, [esi+4]
            paddusw mm3, [SkinBitmap_mmx_one]

            punpcklwd mm3, mm3 // HHVVHHVV
            paddusw mm5, [SkinBitmap_mmx_one]

            movd mm7, [edi+4]
            punpcklwd mm5, mm5 // HHVVHHVV
        
            punpckldq mm3, mm3 // HHVVHHVV HHVVHHVV
            punpckldq mm5, mm5 // HHVVHHVV HHVVHHVV

            punpcklbw mm6, [SkinBitmap_mmx_zero]
            psubw mm4, mm5

            punpcklbw mm0, [SkinBitmap_mmx_zero]
            psubw mm2, mm3

            punpcklbw mm7, [SkinBitmap_mmx_zero]
            pmullw mm0, mm2
          
            pmullw mm7, mm4      
            punpcklbw mm1, [SkinBitmap_mmx_zero]

            psubw mm2, mm3

            psrlw mm0, 8
            psrlw mm7, 8
            paddw mm0, mm1
   
            paddw mm7, mm6
            packuswb mm0, mm0

            movd [edi], mm0
            packuswb mm7, mm7
                 
            movd [edi+4], mm7

            add esi, 8
            add edi, 8

            dec ecx
            jnz _blitAlpha_Loop2
            mov src, esi
            mov dest, edi
          }
          if (xp&1) *dest++ = Blenders::BLEND_ADJ2_MMX(*dest, *src++);
        } // HAS_MMX
      #endif // ifndef NO_MMX
    }
    else { // both
      if (!HAS_MMX)
        while (xp--) *dest++ = Blenders::BLEND_ADJ3(*dest, *src++, alpha);
      #ifndef NO_MMX
        else
        {
          if (xp > 1) __asm
          {
            movd mm5, [alpha]
            mov ecx, xp

            packuswb mm5, mm5 
            shr ecx, 1

            paddusw mm5, [SkinBitmap_mmx_one]

            punpcklwd mm5, mm5        
            mov edi, dest

            punpckldq mm5, mm5
            mov esi, src

            align 16
            _blitAlpha_Loop3:

            movd mm3, [esi] // VVVVVVVV
            movd mm4, [esi+4] // VVVVVVVV

            movd mm0, [edi]    
            psrld mm3, 24

            movd mm1, [esi]
            psrld mm4, 24

            paddusw mm3, [SkinBitmap_mmx_one]
            paddusw mm4, [SkinBitmap_mmx_one]

            movd mm7, [edi+4]    
            punpcklwd mm3, mm3

            movd mm6, [esi+4]
            punpcklwd mm4, mm4

            punpckldq mm3, mm3
            punpckldq mm4, mm4
          
            pmullw mm3, mm5
            pmullw mm4, mm5

            punpcklbw mm7, [SkinBitmap_mmx_zero]
            punpcklbw mm6, [SkinBitmap_mmx_zero]

            movq mm2, [SkinBitmap_mmx_revn2]
            psrlw mm3, 8

            psrlw mm4, 8  

            punpcklbw mm0, [SkinBitmap_mmx_zero]
            punpcklbw mm1, [SkinBitmap_mmx_zero]

            psubw mm2, mm3
            pmullw mm0, mm2      

            pmullw mm1, mm5
            add esi, 8

            movq mm2, [SkinBitmap_mmx_revn2]
            pmullw mm6, mm5
    
            paddusw mm0, mm1
            psubw mm2, mm4

            pmullw mm7, mm2      
            psrlw mm0, 8   

            packuswb mm0, mm0
            paddusw mm7, mm6

            movd [edi], mm0
            psrlw mm7, 8   
                 
            packuswb mm7, mm7

            movd [edi+4], mm7

            add edi, 8

            dec ecx
            jnz _blitAlpha_Loop3
            mov src, esi
            mov dest, edi
          }
          if (xp&1) *dest++ = Blenders::BLEND_ADJ3_MMX(*dest, *src++, alpha);
        } // HAS_MMX
      #endif // ifndef NO_MMX
    }
  }
#ifndef NO_MMX
  Blenders::BLEND_MMX_END();
#endif
  // write bits back to dib.

  if (hdibdc) {
    BitBlt(hdc,destrect.left,destrect.top,destrect.right-destrect.left,destrect.bottom-destrect.top,hdibdc,0,0,SRCCOPY);
    SelectObject(hdibdc, hprevdibdcbm);
    DeleteObject(hdib);
    DeleteDC(hdibdc);
  }
}

#pragma warning(pop) 

void SkinBitmap::stretch(CanvasBase *canvas, int x, int y, int w, int h) {
  RECT src, dst;
  src.left=0;
  src.top=0;
  src.right=getWidth();
  src.bottom=getHeight();
  dst.left=x;
  dst.right=x+w;
  dst.top=y;
  dst.bottom=y+h;
  stretchToRectAlpha(canvas,&src,&dst,255);
}

void SkinBitmap::stretchToRect(CanvasBase *canvas, RECT *r) {
  stretch(canvas, r->left, r->top, r->right - r->left, r->bottom - r->top);
}

void SkinBitmap::stretchRectToRect(CanvasBase *canvas, RECT *src, RECT *dst) {
  stretchToRectAlpha(canvas,src,dst,255);
}


void SkinBitmap::stretchToRectAlpha(CanvasBase *canvas, RECT *r, int alpha) {
  RECT re;
  re.left=0; re.top=0;
  re.right=getWidth(); re.bottom=getHeight();
  stretchToRectAlpha(canvas,&re,r,alpha);
}

void SkinBitmap::blitAlpha(CanvasBase *canvas, int x, int y, int alpha)
{
  RECT dst,src;
  dst.left=x;
  dst.top=y;
  src.left=0;
  src.top=0;
  src.bottom=getHeight();
  src.right=getWidth();
  blitToRect(canvas,&src,&dst,alpha);
}
#pragma warning(push) 
#pragma warning(disable : 4799) 

template <class C>
class Stretcher {
public:
  static void _stretchToRectAlpha(SkinBitmap *bitmap, int ys, int ye, int xe, int xs, int xstart, int yv, void *dib, int cwidth, int dxv, int dyv, int alpha) {
    int bitmap_x = bitmap->getX();
    int bitmap_y = bitmap->getY();
    int bmpheight = bitmap->getHeight();
    int fullimage_w = bitmap->getFullWidth();
    void *bits = bitmap->getBits();
    int xp=xe-xs;
    for (int yp = ys; yp < ye; yp ++) {
      int t=yv>>16;
      if (t < 0) t=0;
      if (t >= bmpheight) t=bmpheight-1;
      int *psrc=((int*)bits) + (t+bitmap_y)*fullimage_w + bitmap_x;
      int *dest=((int*)dib) + cwidth*yp + xs;     

      C::stretch(xp, psrc, dest, xstart, dxv, alpha);

      yv+=dyv;
    }
  }
};

// no alpha, just stretch
class Stretch {
public:
  static void stretch(int xp, int *psrc, int *dest, int xv, int dxv, int alpha) {
    while (xp--) { //JFtodo: assembly optimize - these first two modes aren't used that much anyway
      *dest++ = psrc[xv>>16];
      xv+=dxv;
    }
  }
};

// no alpha channel, just a global alpha val
class StretchGlobal {
public:
  static void stretch(int xp, int *psrc, int *dest, int xv, int dxv, int alpha) {
    while (xp--) { //JFTODO: make MMX optimized version
      *dest++ = Blenders::BLEND_ADJ1(psrc[xv>>16], *dest, alpha);
      xv+=dxv;
    }
  }
};

// alpha channel, no global alpha val
class StretchChannel {
public:
  static void stretch(int xp, int *psrc, int *dest, int xv, int dxv, int alpha) {
    while (xp--) {
      *dest++ = Blenders::BLEND_ADJ2(*dest, psrc[xv>>16]);
      xv+=dxv;
    }
  }
};

class StretchGlobalChannel {
public:
  static void stretch(int xp, int *psrc, int *dest, int xv, int dxv, int alpha) {
    while (xp--) {
      *dest++ = Blenders::BLEND_ADJ3(*dest, psrc[xv>>16], alpha);
      xv+=dxv;
    }
  }
};


#ifndef NO_MMX

// no alpha channel, just a global alpha val
class StretchGlobalMMX {
public:
  static void stretch(int xp, int *psrc, int *dest, int xv, int dxv, int alpha) {
    while (xp--) { //JFTODO: make MMX optimized version
      *dest++ = Blenders::BLEND_ADJ1_MMX(psrc[xv>>16], *dest, alpha);
      xv+=dxv;
    }
  }
};


// alpha channel, no global alpha val
class StretchChannelMMX {
public:
  static void stretch(int xp, int *psrc, int *dest, int xv, int dxv, int alpha) {
    if (xp>1) __asm
    {
      mov ecx, xp
      mov edi, dest

      shr ecx, 1
      mov esi, psrc

      mov edx, xv
      mov ebx, dxv

      align 16
    _stretchAlpha_Loop2:

      mov eax, edx
      movd mm0, [edi]

      movq mm4, [SkinBitmap_mmx_revn2]
      shr eax, 16

      movq mm2, [SkinBitmap_mmx_revn2]
      punpcklbw mm0, [SkinBitmap_mmx_zero]

      movd mm3, [esi+eax*4]
      movd mm1, [esi+eax*4]
      
      lea eax, [edx+ebx]
      shr eax, 16

      movd mm7, [edi+4]
      psrld mm3, 24

      packuswb mm3, mm3 // 0000HHVV
      movd mm5, [esi+eax*4]

      movd mm6, [esi+eax*4]
      psrld mm5, 24          

      paddusw mm3, [SkinBitmap_mmx_one]
      punpcklbw mm6, [SkinBitmap_mmx_zero]

      packuswb mm5, mm5 // 0000HHVV
      lea edx, [edx+ebx*2]
        
      paddusw mm5, [SkinBitmap_mmx_one]          
      punpcklwd mm3, mm3 // HHVVHHVV

      punpcklwd mm5, mm5 // HHVVHHVV
      add edi, 8
      
      punpckldq mm3, mm3 // HHVVHHVV HHVVHHVV

      punpckldq mm5, mm5 // HHVVHHVV HHVVHHVV

      psubw mm4, mm5

      psubw mm2, mm3

      punpcklbw mm7, [SkinBitmap_mmx_zero]
      pmullw mm0, mm2
        
      pmullw mm7, mm4      
      punpcklbw mm1, [SkinBitmap_mmx_zero]

      psubw mm2, mm3

      psrlw mm0, 8
      psrlw mm7, 8
      paddw mm0, mm1
 
      paddw mm7, mm6
      packuswb mm0, mm0

      movd [edi-8], mm0
      packuswb mm7, mm7
                 
      movd [edi-4], mm7

      dec ecx
      jnz _stretchAlpha_Loop2
      mov dest, edi
      mov xv, edx
    }

    if (xp&1) *dest++ = Blenders::BLEND_ADJ2_MMX(*dest, psrc[xv>>16]);
  }
};


class StretchGlobalChannelMMX {
public:
  static void stretch(int xp, int *psrc, int *dest, int xv, int dxv, int alpha) {
    if (xp>1) __asm
    {
      movd mm5, [alpha]
      mov ecx, xp

      packuswb mm5, mm5 
      shr ecx, 1

      paddusw mm5, [SkinBitmap_mmx_one]

      punpcklwd mm5, mm5        
      mov edi, dest

      punpckldq mm5, mm5
      mov esi, psrc

      mov edx, xv
      mov ebx, dxv

      align 16
    _stretchAlpha_Loop3:
      movd mm0, [edi]    
      mov eax, edx

      movd mm7, [edi+4]    
      shr eax, 16

      movd mm1, [esi+eax*4]
      movd mm3, [esi+eax*4] // VVVVVVVV

      lea eax, [edx+ebx]
      psrld mm3, 24

      paddusw mm3, [SkinBitmap_mmx_one]

      punpcklwd mm3, mm3
      shr eax, 16

      punpckldq mm3, mm3

      pmullw mm3, mm5

      movd mm4, [esi+eax*4] // VVVVVVVV
      movd mm6, [esi+eax*4]

      movq mm2, [SkinBitmap_mmx_revn2]
      psrld mm4, 24

      paddusw mm4, [SkinBitmap_mmx_one]
      punpcklbw mm7, [SkinBitmap_mmx_zero]

      punpcklwd mm4, mm4
      lea edx, [edx+ebx*2]

      punpckldq mm4, mm4
      add edi, 8

      punpcklbw mm6, [SkinBitmap_mmx_zero]         
      pmullw mm4, mm5

      psrlw mm3, 8

      punpcklbw mm0, [SkinBitmap_mmx_zero]

      punpcklbw mm1, [SkinBitmap_mmx_zero]
      psubw mm2, mm3

      pmullw mm0, mm2      
      pmullw mm1, mm5

      pmullw mm6, mm5
      psrlw mm4, 8  

      movq mm2, [SkinBitmap_mmx_revn2]    
      paddusw mm0, mm1
      psubw mm2, mm4

      pmullw mm7, mm2      
      psrlw mm0, 8   

      packuswb mm0, mm0
      paddusw mm7, mm6

      movd [edi-8], mm0
      psrlw mm7, 8   
             
      packuswb mm7, mm7

      movd [edi-4], mm7

      dec ecx
      jnz _stretchAlpha_Loop3
      mov xv, edx
      mov dest, edi
    }

    if (xp&1) *dest++ = Blenders::BLEND_ADJ3_MMX(*dest, psrc[xv>>16], alpha);
  }
};
#endif


class __Stretch : public Stretcher<Stretch> {};
class __StretchGlobal : public Stretcher<StretchGlobal> {};
class __StretchChannel : public Stretcher<StretchChannel> {};
class __StretchGlobalChannel : public Stretcher<StretchGlobalChannel> {};

#ifndef NO_MMX
class __StretchGlobalMMX : public Stretcher<StretchGlobalMMX> {};
class __StretchChannelMMX : public Stretcher<StretchChannelMMX> {};
class __StretchGlobalChannelMMX : public Stretcher<StretchGlobalChannelMMX> {};
#endif

#pragma warning(pop) 


void SkinBitmap::stretchToRectAlpha(CanvasBase *canvas, RECT *_src, RECT *_dst, int alpha)
{
  if (alpha <= 0) return;
  if (alpha > 255) alpha = 255;

  RECT src=*_src;
  RECT dst=*_dst;

  if ((src.right-src.left) == (dst.right-dst.left) &&
      (src.bottom-src.top) == (dst.bottom-dst.top))
  {
    blitToRect(canvas,_src,_dst,alpha);
    return;
  }

  //FG> this is a hack, we should support subpixels instead
  if (src.left == src.right) {
    if (src.right < getWidth()) 
      src.right++;
    else
      src.left--;
  } 
  if (src.top== src.bottom) {
    if (src.bottom < getHeight()) 
      src.bottom++;
    else
      src.top--;
  } 

  if (src.left >= src.right || src.top >= src.bottom) return;
  if (dst.left >= dst.right || dst.top >= dst.bottom) return;

  void *dib=canvas->getBits();
  HDC hdc=canvas->getHDC();
  HBITMAP hdib,hprevdibdcbm;
  HDC hdibdc=NULL;
  int cwidth, cheight;

  int dyv=((src.bottom-src.top)<<16)/(dst.bottom-dst.top);
  int dxv=((src.right-src.left)<<16)/(dst.right-dst.left);  
  int yv=(src.top<<16);
  int xstart=(src.left<<16);

  RECT c;
  int ctype=GetClipBox(hdc, &c);
  if (c.top > dst.top) 
  {
    yv+=(c.top-dst.top)*dyv;
    dst.top=c.top;
  }
  if (c.left > dst.left)
  {
    xstart+=(c.left-dst.left)*dxv;
    dst.left=c.left;
  }
  if (c.bottom < dst.bottom) dst.bottom=c.bottom;
  if (c.right < dst.right) dst.right=c.right;

  if (dst.right <= dst.left || dst.bottom <= dst.top) return;

  int xs,xe,ys,ye;

#ifdef NO_SIMPLEFASTMODE
  dib=NULL;
#endif
  if (!dib || canvas->getDim(NULL,&cheight,&cwidth) || !cwidth || cheight < 1 || ctype == COMPLEXREGION)
  {
    // create dib
    BITMAPINFO bmidib={0,};
    bmidib.bmiHeader.biSize=sizeof(bmidib.bmiHeader);
    bmidib.bmiHeader.biWidth=dst.right-dst.left;
    bmidib.bmiHeader.biHeight=dst.top-dst.bottom; // this should be negative
    bmidib.bmiHeader.biPlanes=1;
    bmidib.bmiHeader.biBitCount=32;
    bmidib.bmiHeader.biCompression=BI_RGB;
    hdib=CreateDIBSection(hdc,&bmidib,DIB_RGB_COLORS,&dib,NULL,0);
    ASSERTPR(hdib != 0, "CreateDIBSection() failed #5");
    hdibdc = CreateCompatibleDC(hdc);
    hprevdibdcbm = (HBITMAP)SelectObject(hdibdc, hdib);
    if (has_alpha || alpha < 255) BitBlt(hdibdc,0,0,dst.right-dst.left,dst.bottom-dst.top,hdc,dst.left,dst.top,SRCCOPY);
    cwidth=dst.right-dst.left;
    cheight=dst.bottom-dst.top;
    xs=0;
    ys=0;
    xe=dst.right-dst.left;
    ye=dst.bottom-dst.top;
  }
  else 
  {
    xs=dst.left;
    xe=dst.right;
    ys=dst.top;
    ye=dst.bottom;
    cwidth/=4;
  }

  // stretch and blend bitmap to dib

  if (xstart < 0) xstart=0;

  if (xs<xe) {
    if (!has_alpha) {	// doesn't have alpha channel
      if (alpha == 255) {	// no global alpha
        __Stretch::_stretchToRectAlpha(this, ys, ye, xe, xs, xstart, yv, dib, cwidth, dxv, dyv, alpha);
      } else {	// has global alpha
#ifndef NO_MMX
        if (HAS_MMX) {
          __StretchGlobalMMX::_stretchToRectAlpha(this, ys, ye, xe, xs, xstart, yv, dib, cwidth, dxv, dyv, alpha);
        } else
#endif
        {
          __StretchGlobal::_stretchToRectAlpha(this, ys, ye, xe, xs, xstart, yv, dib, cwidth, dxv, dyv, alpha);
        }
      }
    } else {	// has alpha channel
      // FUCKO: JF> BRENNAN FIX THESE BITCHES :)
      if (alpha == 255) {	// no global alpha
#ifndef NO_MMX
        if (HAS_MMX) {
          __StretchChannelMMX::_stretchToRectAlpha(this, ys, ye, xe, xs, xstart, yv, dib, cwidth, dxv, dyv, alpha);
        } else 
#endif
        {
          __StretchChannel::_stretchToRectAlpha(this, ys, ye, xe, xs, xstart, yv, dib, cwidth, dxv, dyv, alpha);
        }
      } else {	// has global alpha
#ifndef NO_MMX
        if (HAS_MMX) {
          __StretchGlobalChannelMMX::_stretchToRectAlpha(this, ys, ye, xe, xs, xstart, yv, dib, cwidth, dxv, dyv, alpha);
        } else 
#endif
        {
          __StretchGlobalChannel::_stretchToRectAlpha(this, ys, ye, xe, xs, xstart, yv, dib, cwidth, dxv, dyv, alpha);
        }
      }
    }
  }

#ifndef NO_MMX
  Blenders::BLEND_MMX_END();
#endif
  // write bits back to dib.

  if (hdibdc) {
    BitBlt(hdc,dst.left,dst.top,dst.right-dst.left,dst.bottom-dst.top,hdibdc,0,0,SRCCOPY);
    SelectObject(hdibdc, hprevdibdcbm);
    DeleteObject(hdib);
    DeleteDC(hdibdc);
  }
}

COLORREF SkinBitmap::getPixel(int x, int y) {
  ASSERT(bits != NULL);
  if (x < 0 || y < 0 || x >= getFullWidth()-getX() || y>= getFullHeight()-getY()) return (COLORREF)0;
  return (COLORREF)(((int*)bits)[x+getX()+(y+getY())*getFullWidth()]);
}

void *SkinBitmap::getBits() {
  return bits;
}

int SkinBitmap::isInvalid() {
  return last_failed;
}

void SkinBitmap::setHasAlpha(int ha) { 
  has_alpha=ha; 
}

const char *SkinBitmap::getBitmapName() {
  return bitmapname;
}

