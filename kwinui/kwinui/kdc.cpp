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

#include "kdc.h"
#include "tchar.h"
#pragma comment(lib,"Msimg32.lib")
 
namespace kwinui{
//KDC
KDC::KDC():m_hDC(0),m_hAttribDC(0),m_bPrinting(false){}
KDC::KDC(HDC hDC):m_hDC(hDC),m_hAttribDC(0),m_bPrinting(false){}

KDC::~KDC(){if(m_hDC) DeleteDC();}

bool KDC::Attach(HDC hDC){   // Attach/Detach affects only the Output DC
	assert(m_hDC == NULL);      // only attach once, detach on destroy
	if (!hDC)
		return false;

	m_hDC = hDC;
	return true;
}
HDC KDC::Detach(void){ assert(m_hDC);HDC hDC=m_hDC; m_hDC=0; return hDC;}

HGDIOBJ KDC::SelectObject(HGDIOBJ hGdiObject){return ::SelectObject(m_hDC,hGdiObject);}
HGDIOBJ	KDC::GetCurrentObject(UINT uObjectType){return ::GetCurrentObject(m_hDC,uObjectType);}

HGDIOBJ KDC::SelectStockObject(int nIndex){
	HGDIOBJ hGdiObj=::GetStockObject(nIndex);
	SelectObject(hGdiObj);
	return hGdiObj;
}
bool KDC::DeleteDC(){return 0!=::DeleteDC(Detach());}

int KDC::SaveDC(){return ::SaveDC(m_hDC);}
bool KDC::RestoreDC(int nSavedDC){return 0!=::RestoreDC(m_hDC,nSavedDC);}

int KDC::SetBkMode(int nBkMode){return ::SetBkMode(m_hDC,nBkMode);}
COLORREF KDC::SetBkColor(COLORREF crColor){return ::SetBkColor(m_hDC,crColor);}
bool KDC::CreateCompatibleDC(HDC hDC){return Attach(::CreateCompatibleDC(hDC));}
void KDC::FillSolidRect(const RECT* rect,COLORREF clrColor){
	HBRUSH hBrush=::CreateSolidBrush(clrColor);
	::FillRect(m_hDC,rect,hBrush);
	::DeleteObject(hBrush);		
}
void KDC::FillSolidRect(int x,int y,int cx,int cy,COLORREF clrColor){
	RECT rect;
	rect.left	=x;
	rect.top	=y;
	rect.right	=x+cx;
	rect.bottom	=y+cy;
	FillSolidRect(&rect,clrColor);
}
bool KDC::FillRect(const RECT* rect,HBRUSH hbr){return 0!=::FillRect(m_hDC,rect,hbr);}
bool KDC::BitBlt(int x,int y,int nWidth,int nHeight,HDC hSrcDC,int xSrc,int ySrc,DWORD dwRop){
	return 0!=::BitBlt(m_hDC,x,y,nWidth,nHeight,hSrcDC,xSrc,ySrc,dwRop);
}
bool KDC::StretchBlt(int x,int y,int nWidth,int nHeight,HDC hDCSrc,int xSrc,int ySrc,int nWidthSrc,
						int nHeightSrc,DWORD dwRop){
	return 0!=::StretchBlt(m_hDC,x,y,nWidth,nHeight,hDCSrc,xSrc,ySrc,nWidthSrc,nHeightSrc,dwRop);
}
bool KDC::TransparentBlt(int x,int y,int nWidth,int nHeight,HDC hDCSrc,int xSrc,int ySrc,
						int nWidthSrc,int nHeightSrc,UINT crTransparent){
	return 0!=::TransparentBlt(m_hDC,x,y,nWidth,nHeight,hDCSrc,xSrc,ySrc,nWidthSrc,nHeightSrc,crTransparent);
}

bool KDC::Ellipse(int nLeft,int nTop,int nRight,int nBottom){
	return 0!=::Ellipse(m_hDC,nLeft,nTop,nRight,nBottom);
}
bool KDC::SetPixelV(int x,int y,COLORREF crColor){return 0!=::SetPixelV(m_hDC,x,y,crColor);}

int KDC::SelectClipRgn(HRGN hRgn){return ::SelectClipRgn(m_hDC,hRgn);}
SIZE KDC::GetTextExtent(LPCTSTR szStr){
	SIZE size;
	::GetTextExtentPoint32(m_hDC,szStr,(int)_tcslen(szStr),&size);
	return size;
}
bool KDC::DrawState(HBRUSH hbr,DRAWSTATEPROC lpOutputFunc,LPARAM lData, WPARAM wData,
					int x,int y,int cx,int cy,UINT fuFlags){
	return 0!=::DrawState(m_hDC,hbr,lpOutputFunc,lData,wData,x,y,cx,cy,fuFlags);
}

bool KDC::TextOut(int x,int y,LPCTSTR szStr,int nCount){return 0!=::TextOut(m_hDC,x,y,szStr,nCount);}
int KDC::GetClipBox(RECT* pRect){return ::GetClipBox(m_hDC,pRect);}
bool KDC::FillRgn(HRGN hRgn,HBRUSH hBrush){return 0!=::FillRgn(m_hDC,hRgn,hBrush);}
COLORREF KDC::SetTextColor(COLORREF crColor){return ::SetTextColor(m_hDC,crColor);}
void KDC::LPtoDP(RECT* pRect){::LPtoDP(m_hDC,(POINT*)pRect,2);}
void KDC::DPtoLP(RECT* pRect){::DPtoLP(m_hDC,(POINT*)pRect,2);}

int KDC::SetMapMode(int nMapMode){return ::SetMapMode(m_hDC,nMapMode);}
int KDC::GetMapMode(){return ::GetMapMode(m_hDC);}

SIZE KDC::SetWindowExt(SIZE size){
	SIZE tmpSize;
	::SetWindowExtEx(m_hDC,size.cx,size.cy,&tmpSize);
	return tmpSize;
}
SIZE KDC::GetWindowExt(){
	SIZE tmpSize;
	::GetWindowExtEx(m_hDC,&tmpSize);
	return tmpSize;
}
SIZE KDC::SetViewportExt(SIZE size){
	SIZE tmpSize;
	::SetViewportExtEx(m_hDC,size.cx,size.cy,&tmpSize);
	return tmpSize;
}
SIZE KDC::GetViewportExt(){
	SIZE tmpSize;
	::GetViewportExtEx(m_hDC,&tmpSize);
	return tmpSize;
}
POINT KDC::SetWindowOrg(int x,int y){
	POINT pt;
	::SetWindowOrgEx(m_hDC,x,y,&pt);
	return pt;
}
POINT KDC::SetWindowOrg(POINT point){
	POINT pt;
	::SetWindowOrgEx(m_hDC,point.x,point.y,&pt);
	return pt;
}
POINT KDC::GetWindowOrg(){
	POINT pt;
	::GetWindowOrgEx(m_hDC,&pt);
	return pt;
}
COLORREF KDC::GetBkColor(){return ::GetBkColor(m_hDC);}
bool KDC::IsPrinting(){return false;}	//now return false always
int KDC::DrawText(const TCHAR* ctszString,RECT* pRect,unsigned int uFormat){
	return ::DrawText(m_hDC,ctszString,-1,pRect,uFormat);
}
bool KDC::DrawFocusRect(const RECT* pRect){return 0!=::DrawFocusRect(m_hDC,pRect);}
bool KDC::Rectangle(RECT* pRect){return 0!=::Rectangle(m_hDC,pRect->left,pRect->top,pRect->right,pRect->bottom);}

bool KDC::MoveToEx(int x,int y,POINT* pOldPoint){return 0!=::MoveToEx(m_hDC,x,y,pOldPoint); }
bool KDC::LineTo(int x,int y){return 0!=::LineTo(m_hDC,x,y);}
//KPaintDC
KPaintDC::KPaintDC(HWND hWnd){
	assert(::IsWindow(hWnd));
	m_hWnd=hWnd;
	Attach(::BeginPaint(m_hWnd,&m_ps));
}
KPaintDC::~KPaintDC(){
	::EndPaint(m_hWnd,&m_ps);
	Detach();
}
//KClientDC
KClientDC::KClientDC(HWND hWnd){
	assert(::IsWindow(hWnd));
	m_hWnd=hWnd;	
	Attach(::GetDC(m_hWnd));
}
KClientDC::~KClientDC(){
	assert(m_hDC);
	::ReleaseDC(m_hWnd,Detach());
}
//KWindowDC
KWindowDC::KWindowDC(HWND hWnd){
	assert(::IsWindow(hWnd));
	m_hWnd=hWnd;
	Attach(::GetWindowDC(m_hWnd));
}
KWindowDC::~KWindowDC(){
	assert(m_hDC);
	::ReleaseDC(m_hWnd,Detach());
}
//KMemDC
KMemDC::KMemDC(HDC hDC, const RECT* pRect):KDC(){
	assert(hDC!=NULL);
	// Some initialization
	m_hDCPaint=hDC;

	// Get the rectangle to draw
	if(!pRect)
		::GetClipBox(hDC,&m_rect);
	else 
		m_rect=*pRect;

	// Create a Memory DC
	CreateCompatibleDC(hDC);
	::LPtoDP(hDC,(POINT*)(&m_rect),2);

	m_bitmap=::CreateCompatibleBitmap(hDC, m_rect.right-m_rect.left,m_rect.bottom-m_rect.top);
	m_oldBitmap=(HBITMAP)SelectObject(m_bitmap);

	SetMapMode(::GetMapMode(hDC));

	SIZE size;
	::GetWindowExtEx(hDC,&size);	
	SetWindowExt(size);
	::GetViewportExtEx(hDC,&size);
	SetViewportExt(size);

	::DPtoLP(hDC,(POINT*)(&m_rect),2);
	SetWindowOrg(m_rect.left, m_rect.top);

	// Fill background
	FillSolidRect(&m_rect, ::GetBkColor(hDC));
}

KMemDC::~KMemDC(){         
	// Copy the offscreen bitmap onto the screen.
	::BitBlt(m_hDCPaint,m_rect.left, m_rect.top,m_rect.right-m_rect.left,m_rect.bottom-m_rect.top,
		     *this, m_rect.left, m_rect.top, SRCCOPY);           
    
	//Swap back the original bitmap.
	SelectObject(m_oldBitmap);
	::DeleteObject(m_bitmap);
} 
} //namespace kwinui
