/* Copyright (C) 2006-2009 Proguru, changuoqiang[at]gmail[dot]com.
   http://blog.pcware.cn
   This file is part of KWinUI.

   The KWinUI is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The KWinUI is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the KWinUI; if not, write to the Free
   Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
   02111-1307 USA.  */

/*
 *
 *  04/17/2007
 */

#ifndef __KGDIOBJ_H__
#define __KGDIOBJ_H__

#include "windows.h"
#include "kdef.h"
#include <cassert>
 
namespace kwinui{

template <typename T>
class KGdiObj{
public:
	KGdiObj():m_hGdiObj(0){}
	KGdiObj(T t):m_hGdiObj(t){}

	~KGdiObj(){if(m_hGdiObj) DeleteObject();}

	KGdiObj& operator=(T t){m_hGdiObj=t; return *this;}
	KGdiObj& operator=(HGDIOBJ hGdiObj){m_hGdiObj=(T)hGdiObj; return *this;}
	operator T(){return m_hGdiObj;}
	operator HGDIOBJ(){return (HGDIOBJ)m_hGdiObj;}

	KGdiObj* operator->(){return this;}      
    	operator KGdiObj*(){return this;}

	bool Attach(T t){assert(!m_hGdiObj); assert(t); m_hGdiObj=t; return true;}
	T Detach(void){assert(m_hGdiObj); T hGdiObj=m_hGdiObj;m_hGdiObj=0;return hGdiObj;}
	bool DeleteObject(void){if(!m_hGdiObj) return false; return (0!=::DeleteObject((HGDIOBJ)Detach()));}

	//KBitmap relevant
	T CreateCompatibleBitmap(HDC hDC, int nWidth, int nHeight){
		return m_hGdiObj=::CreateCompatibleBitmap(hDC,nWidth,nHeight);
	}
	T LoadBitmap(TCHAR* tszResName,HINSTANCE hInst=::GetModuleHandle(NULL)){
		return m_hGdiObj=::LoadBitmap(hInst,tszResName);
	}
	T LoadBitmap(UINT nResID,HINSTANCE hInst=::GetModuleHandle(NULL)){
		return m_hGdiObj=::LoadBitmap(hInst,MAKEINTRESOURCE(nResID));
	}
	int GetBitmap(BITMAP* pBitmap){assert(m_hGdiObj); return ::GetObject((HGDIOBJ)m_hGdiObj,sizeof(BITMAP),pBitmap);}
	SIZE GetBitmapDimension(){
		SIZE size;
		BITMAP bm;
		GetBitmap(&bm);
		size.cx=bm.bmWidth;
		size.cy=bm.bmHeight;
		return size;
	}

	//KBrush relevant
	T CreateSolidBrush(COLORREF crColor){m_hGdiObj=::CreateSolidBrush(crColor); return m_hGdiObj;}

	//KPen relevant
	//KFont relevant
	T CreateFontIndirect(const LOGFONT* lpLogFont){
		return m_hGdiObj=::CreateFontIndirect(lpLogFont);
	}	
	T CreateFont(int nHeight, int nWidth, int nEscapement,
			int nOrientation, int nWeight, BYTE bItalic, BYTE bUnderline,
			BYTE cStrikeOut, BYTE nCharSet, BYTE nOutPrecision,
			BYTE nClipPrecision, BYTE nQuality, BYTE nPitchAndFamily,
			LPCTSTR lpszFacename){
		return m_hGdiObj=::CreateFont(nHeight,nWidth,nEscapement,nOrientation,nWeight,bItalic,bUnderline,
			cStrikeOut,nCharSet,nOutPrecision,nClipPrecision,nQuality,nPitchAndFamily,lpszFacename);
	}
	//BOOL CreatePointFont(int nPointSize, LPCTSTR lpszFaceName, CDC* pDC = NULL);
	//BOOL CreatePointFontIndirect(const LOGFONT* lpLogFont, CDC* pDC = NULL);

	int GetLogFont(LOGFONT* pLogFont){return ::GetObject((HGDIOBJ)m_hGdiObj,sizeof(LOGFONT),pLogFont);}

	//KRgn relevant
	int GetRegionData(int nCount,RGNDATA* pRgnData){return ::GetRegionData(m_hGdiObj,nCount,pRgnData);}
	T ExtCreateRegion(int nCount,RGNDATA* pRgnData){
		return m_hGdiObj=::ExtCreateRegion(NULL,nCount,pRgnData);
	}
	T CreateEllipticRgnIndirect(RECT* pRect){
		return m_hGdiObj=::CreateEllipticRgnIndirect(pRect);
	}
	
	T CreateEllipticRgn(int l,int t,int r,int b){
		return m_hGdiObj=::CreateEllipticRgn(l,t,r,b);
	}
	T BmpToRgn(HBITMAP hBitmap,COLORREF cTransparentColor/*=0*/,COLORREF cTolerance/*=0x101010*/){
		HRGN hRgn=NULL;
		if(hBitmap){
			// Create a memory DC inside which we will scan the bitmap content
			HDC hMemDC=::CreateCompatibleDC(NULL);
			if(hMemDC){
				// Get bitmap size
				BITMAP bm;
				::GetObject(hBitmap,sizeof(bm),&bm);

				// Create a 32 bits depth bitmap and select it into the memory DC 
				BITMAPINFOHEADER RGB32BITSBITMAPINFO={	
						sizeof(BITMAPINFOHEADER),// biSize 
						bm.bmWidth,		// biWidth; 
						bm.bmHeight,		// biHeight; 
						1,			// biPlanes; 
						32,			// biBitCount 
						BI_RGB,			// biCompression; 
						0,			// biSizeImage; 
						0,			// biXPelsPerMeter; 
						0,			// biYPelsPerMeter; 
						0,			// biClrUsed; 
						0			// biClrImportant; 
				};
				VOID * pbits32; 
				HBITMAP hbm32=::CreateDIBSection(hMemDC,(BITMAPINFO *)&RGB32BITSBITMAPINFO,
						DIB_RGB_COLORS, &pbits32, NULL, 0);
				if(hbm32){
					HBITMAP holdBmp=(HBITMAP)::SelectObject(hMemDC, hbm32);

					// Create a DC just to copy the bitmap into the memory DC
					HDC hDC=::CreateCompatibleDC(hMemDC);
					if(hDC){
					//Get how many bytes per row we have for the bitmap bits(rounded up to 32 bits)
						BITMAP bm32;
						::GetObject(hbm32, sizeof(bm32),&bm32);
						while(bm32.bmWidthBytes % 4)
							bm32.bmWidthBytes++;

						// Copy the bitmap into the memory DC
						HBITMAP holdBmp= (HBITMAP)::SelectObject(hDC, hBitmap);
						::BitBlt(hMemDC,0,0,bm.bmWidth,bm.bmHeight,hDC,0,0,SRCCOPY);

				//For better performances,we will use the ExtCreateRegion() function to create the
				// region. This function take a RGNDATA structure on entry. We will add rectangles by
						// amount of ALLOC_UNIT number in this structure.
						#define ALLOC_UNIT	100
						DWORD maxRects = ALLOC_UNIT;
						HANDLE hData=::GlobalAlloc(GMEM_MOVEABLE,
								sizeof(RGNDATAHEADER)+(sizeof(RECT) * maxRects));
						RGNDATA *pData = (RGNDATA *)::GlobalLock(hData);
						pData->rdh.dwSize = sizeof(RGNDATAHEADER);
						pData->rdh.iType = RDH_RECTANGLES;
						pData->rdh.nCount = pData->rdh.nRgnSize = 0;
						SetRect(&pData->rdh.rcBound, MAXLONG, MAXLONG, 0, 0);

						// Keep on hand highest and lowest values for the "transparent" pixels
						BYTE lr = GetRValue(cTransparentColor);
						BYTE lg = GetGValue(cTransparentColor);
						BYTE lb = GetBValue(cTransparentColor);
						BYTE hr = min(0xff, lr + GetRValue(cTolerance));
						BYTE hg = min(0xff, lg + GetGValue(cTolerance));
						BYTE hb = min(0xff, lb + GetBValue(cTolerance));

					//Scan each bitmap row from bottom to top (the bitmap is inverted vertically)
						BYTE *p32 = (BYTE *)bm32.bmBits+(bm32.bmHeight-1) * bm32.bmWidthBytes;
						for (int y = 0; y < bm.bmHeight; y++){
							// Scan each bitmap pixel from left to right
							for (int x = 0; x < bm.bmWidth; x++){
							//Search for a continuous range of "non transparent pixels"
								int x0 = x;
								LONG *p = (LONG *)p32 + x;
								while (x < bm.bmWidth){
									BYTE b = GetRValue(*p);
									if (b >= lr && b <= hr){
										b = GetGValue(*p);
										if (b >= lg && b <= hg){
											b = GetBValue(*p);
											if (b >= lb && b <= hb)
											//This pixel is "transparent"
												break;
										}
									}
									p++;
									x++;
								}

								if (x > x0){
						// Add the pixels (x0, y) to (x, y+1) as a new rectangle in the region
									if (pData->rdh.nCount >= maxRects){
										::GlobalUnlock(hData);
										maxRects += ALLOC_UNIT;
										hData=::GlobalReAlloc(hData,sizeof(RGNDATAHEADER) + (sizeof(RECT) * maxRects), GMEM_MOVEABLE);
										pData = (RGNDATA *)::GlobalLock(hData);
									}
									RECT *pr = (RECT *)&pData->Buffer;
									SetRect(&pr[pData->rdh.nCount], x0, y, x, y+1);
									if (x0 < pData->rdh.rcBound.left)
										pData->rdh.rcBound.left = x0;
									if (y < pData->rdh.rcBound.top)
										pData->rdh.rcBound.top = y;
									if (x > pData->rdh.rcBound.right)
										pData->rdh.rcBound.right = x;
									if (y+1 > pData->rdh.rcBound.bottom)
										pData->rdh.rcBound.bottom = y+1;
									pData->rdh.nCount++;

					// On Windows98, ExtCreateRegion() may fail if the number of rectangles is too
					// large (ie: > 4000). Therefore, we have to create the region by multiple steps.
									if (pData->rdh.nCount == 2000){
										HRGN h =::ExtCreateRegion(NULL, sizeof(RGNDATAHEADER) + (sizeof(RECT) * maxRects), pData);
										if (hRgn){
											::CombineRgn(hRgn,hRgn,h,RGN_OR);
											::DeleteObject(h);
										}
										else
											hRgn = h;
										pData->rdh.nCount = 0;
										::SetRect(&pData->rdh.rcBound,
												MAXLONG,MAXLONG, 0, 0);
									}
								}
							}

							// Go to next row (remember, the bitmap is inverted vertically)
							p32 -= bm32.bmWidthBytes;
						}

						// Create or extend the region with the remaining rectangles
						HRGN h=::ExtCreateRegion(NULL,sizeof(RGNDATAHEADER)+(sizeof(RECT)*maxRects), pData);
						if (hRgn){
							::CombineRgn(hRgn, hRgn, h, RGN_OR);
							::DeleteObject(h);
						}
						else
							hRgn = h;

						// Clean up
						::GlobalFree(hData);
						::SelectObject(hDC, holdBmp);
						::DeleteDC(hDC);
					}
					::DeleteObject(SelectObject(hMemDC, holdBmp));
				}
				::DeleteDC(hMemDC);
			}	
		}
		return m_hGdiObj=hRgn;
	}

	T BmpToRgn(HBITMAP hBitmap, HBITMAP hMaskBitmap){
		COLORREF	crColor=0;
		BITMAP		bm={0};
		//memset(&bm,sizeof(bm),'\0');
		HRGN		hRgn=0,hRgnTmp=0;
		HDC			hDC=0;
		HBITMAP		hOldBitmap=0;
		
		::GetObject((HGDIOBJ)hBitmap,sizeof(BITMAP),&bm);	
		hDC=::CreateCompatibleDC(NULL);
		hOldBitmap=(HBITMAP)::SelectObject(hDC,hMaskBitmap);

		hRgn=::CreateRectRgn(0,0,bm.bmWidth,bm.bmHeight);
		int x,y;
		for(x=0; x<=bm.bmWidth; x++){
			for(y=0; y<=bm.bmHeight; y++){
				crColor=::GetPixel(hDC,x,y);
				if(crColor==0){
					hRgnTmp=::CreateRectRgn(x,y,x+1,y+1);
					::CombineRgn(hRgn,hRgn,hRgnTmp,RGN_XOR);
					::DeleteObject(hRgnTmp);	
				}
			}
		}
		if(hOldBitmap) ::SelectObject(hDC,hOldBitmap);
		::DeleteDC(hDC);

		return m_hGdiObj=hRgn;
	}
	T BmpToRgn(HBITMAP hBitmap,COLORREF crKeyColor/*=0*/,int nCountBmp=0){
		COLORREF	crColor=0;
		BITMAP		bm={0};
		HRGN		hRgn=0,hRgnTmp=0;
		HDC			hDC=0;
		HBITMAP		hOldBitmap=0;
		int 		nWidthScan;
		
		::GetObject((HGDIOBJ)hBitmap,sizeof(BITMAP),&bm);	
		hDC=::CreateCompatibleDC(NULL);
		hOldBitmap=(HBITMAP)::SelectObject(hDC,hBitmap);
		if(nCountBmp)
			nWidthScan=bm.bmWidth/nCountBmp;

		hRgn=::CreateRectRgn(0,0,bm.bmWidth,bm.bmHeight);
		for(int x=0; x<=nWidthScan; x++){
			for(int y=0; y<=bm.bmHeight; y++){
				crColor=::GetPixel(hDC,x,y);
				if(crColor==crKeyColor){
					hRgnTmp=::CreateRectRgn(x,y,x+1,y+1);
					::CombineRgn(hRgn,hRgn,hRgnTmp,RGN_XOR);
					::DeleteObject(hRgnTmp);	
				}
			}
		}
		::SelectObject(hDC,hOldBitmap);
		::DeleteDC(hDC);

		return m_hGdiObj=hRgn;
	}
	T BmpToRgnEdge(HBITMAP hBitmap,COLORREF crEdgeColor,int nCountBmp=0){
		COLORREF	crColor=0;
		BITMAP		bm={0};
		HDC			hDC=0;
		HBITMAP		hOldBitmap=0;
		int			x,y;

		POINT*		pt;
		int 		nPtCount=0;
		int			nWidthScan=0;
		
		::GetObject((HGDIOBJ)hBitmap,sizeof(BITMAP),&bm);
		pt=new POINT[bm.bmHeight*2];
		if(nCountBmp)
			nWidthScan=bm.bmWidth/nCountBmp;
		hDC=::CreateCompatibleDC(NULL);
		hOldBitmap=(HBITMAP)::SelectObject(hDC,hBitmap);

		for(y=bm.bmHeight;y>=0;y--){
			for(x=0;x<=nWidthScan;x++){
				crColor=::GetPixel(hDC,x,y);
				if(crColor==CLR_INVALID || crColor==crEdgeColor)
					continue;

				pt[nPtCount].x=x;
				pt[nPtCount++].y=y;
				break;
			}
		}
		pt[0].x+=1;
		pt[0].y+=1;
		for(y=0;y<bm.bmHeight;y++){
			for(x=nWidthScan;x>=0;x--){
				crColor=::GetPixel(hDC,x,y);
				if(crColor==CLR_INVALID || crColor==crEdgeColor)
						continue;

				pt[nPtCount].x=x+1;
				pt[nPtCount++].y=y;
				break;
			}
		}
		HRGN hRgn=::CreatePolygonRgn(pt,nPtCount,ALTERNATE);

		::SelectObject(hDC,hOldBitmap);
		::DeleteDC(hDC);
		SAFE_DEL_PTRS(pt);

		return m_hGdiObj=hRgn;
	}
private:
	T  m_hGdiObj;
};

typedef KGdiObj<HBITMAP>	KBitmap;
typedef KGdiObj<HBRUSH> 	KBrush;
typedef KGdiObj<HFONT> 		KFont;
typedef KGdiObj<HPEN> 		KPen;
typedef KGdiObj<HRGN> 		KRgn;
} //namespace kwinui
#endif //__KGDIOBJ_H__
