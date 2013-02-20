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
 * HDC wrap
 * proguru
 *
 */

#ifndef __KDC_H__
#define __KDC_H__

#ifndef STRICT
	#define STRICT
#endif	//STRICT

#include "windows.h"
#include "kdef.h"
#include <cassert>
 
namespace kwinui{

class KDC{
public:
	KDC();
	KDC(HDC hDC);
	~KDC();

	inline KDC& operator=(HDC hDC){m_hDC=hDC; return *this;}
	inline operator HDC(){return m_hDC;};

   	inline KDC* operator->(){return this;}      
   	inline operator KDC*(){return this;}

	bool Attach(HDC hDC);   // Attach/Detach affects only the Output DC
	HDC Detach(void);

	HGDIOBJ SelectObject(HGDIOBJ hGdiObject);
	HGDIOBJ	GetCurrentObject(UINT uObjectType);
	HGDIOBJ SelectStockObject(int nIndex);
	bool DeleteDC();

	int SaveDC();
	bool RestoreDC(int nSavedDC);

	int SetBkMode(int nBkMode);
	COLORREF SetBkColor(COLORREF crColor);
	bool CreateCompatibleDC(HDC hDC);
	void FillSolidRect(const RECT* rect,COLORREF clrColor);
	void FillSolidRect(int x,int y,int cx,int cy,COLORREF clrColor);
	bool FillRect(const RECT* rect,HBRUSH hbr);
	bool BitBlt(int x,int y,int nWidth,int nHeight,HDC hSrcDC,int xSrc,int ySrc,DWORD dwRop);
	bool StretchBlt(int x,int y,int nWidth,int nHeight,HDC hDCSrc,int xSrc,int ySrc,int nWidthSrc,
					int nHeightSrc,DWORD dwRop);
	bool TransparentBlt(int x,int y,int nWidth,int nHeight,HDC hDCSrc,int xSrc,int ySrc,
						int nWidthSrc,int nHeightSrc,UINT crTransparent);
 
	bool Ellipse(int nLeft,int nTop,int nRight,int nBottom);
	bool SetPixelV(int x,int y,COLORREF crColor);

	int SelectClipRgn(HRGN hRgn);
	SIZE GetTextExtent(LPCTSTR szStr);
	bool DrawState(HBRUSH hbr,DRAWSTATEPROC lpOutputFunc,LPARAM lData,WPARAM wData,
					int x,int y,int cx,int cy,UINT fuFlags);

	bool TextOut(int x,int y,LPCTSTR szStr,int nCount);
	int GetClipBox(RECT* pRect);
	bool FillRgn(HRGN hRgn,HBRUSH hBrush);
	COLORREF SetTextColor(COLORREF crColor);
	void LPtoDP(RECT* pRect);
	void DPtoLP(RECT* pRect);

	int SetMapMode(int nMapMode);
	int GetMapMode();

	SIZE SetWindowExt(SIZE size);
	SIZE GetWindowExt();
	SIZE SetViewportExt(SIZE size);
	SIZE GetViewportExt();
	POINT SetWindowOrg(int x,int y);
	POINT SetWindowOrg(POINT point);
	POINT GetWindowOrg();
	COLORREF GetBkColor();
	bool IsPrinting();
	int DrawText(const TCHAR* ctszString,RECT* pRect,
	unsigned int uFormat=DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	bool DrawFocusRect(const RECT* pRect);
	bool Rectangle(RECT* pRect);

	bool MoveToEx(int x,int y,POINT* pOldPoint=0);
	bool LineTo(int x,int y);
public:
	HDC 	m_hDC;
	HDC 	m_hAttribDC;
	bool 	m_bPrinting;

};

class KPaintDC : public KDC{
public:
	KPaintDC(HWND hWnd);
	~KPaintDC();
public:
	HWND 		m_hWnd;
	PAINTSTRUCT	m_ps;
};

class KClientDC : public KDC{
public:
	KClientDC(HWND hWnd);
	~KClientDC();
public:
	HWND m_hWnd;
};

class KWindowDC : public KDC{
public:
	KWindowDC(HWND hWnd);
	~KWindowDC();
public:
	HWND m_hWnd;
};

class KMemDC : public KDC{
public:
	KMemDC(HDC hDC, const RECT* pRect=NULL);
	~KMemDC();
	
	// Allow usage as a pointer   
	inline KMemDC* operator->(){return this;}      

	// Allow usage as a pointer   
	inline operator KMemDC*(){return this;}

private:      
	HBITMAP	m_bitmap;       // Offscreen bitmap
	HBITMAP m_oldBitmap; 	// bitmap originally found in CMemDC
	HDC		m_hDCPaint;     // Saves CDC passed in constructor
	RECT    m_rect;         // Rectangle of drawing area.
};
} //namespace kwinui
#endif //__KDC_H__
