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
 * Custom Controls
 *
 * proguru
 * 02/20/2007
 */

#ifndef __KCSTCTRLS_H__
#define __KCSTCTRLS_H__
#include "kwin.h"
#include "kctrls.h"
#include "kcmnctrls.h"
#include "kdc.h"
#include "kgdiobj.h"
#include <cassert>
#include <cstdlib>
#include "kmisc.h"
 
namespace kwinui{

//DS---Draw State
#define BTN_DS_NORMAL			0
#define BTN_DS_HOVER			1
#define BTN_DS_PUSHED			2
#define BTN_DS_DISABLED			3
#define BTN_DS_FOCUS			4
//CS---Check State
#define BTN_CS_UNCHECKED		0
#define BTN_CS_CHECKED			1
#define BTN_CS_INDETERMINATE	2

#define BTN_PUSHBUTTON			0
#define BTN_CHECKBOX			1
#define BTN_RADIOBUTTON			2
#define BTN_GROUPBOX			3

class KXButton : public KButtonBase<KXButton>{
public:
	typedef KButtonBase<KXButton> __base;
	KXButton():m_bHover(false),m_nDrawState(0),m_nCheckState(0),m_hBitmap(0),m_clrTransparent(0),m_nPerBmpWidth(0){}

	BEGIN_MSG_MAP
		MSG_HANDLER(BM_SETSTATE,OnSetState)
		MSG_HANDLER(BM_SETCHECK,OnSetCheck)
		MSG_HANDLER(WM_EX_MOUSEENTER,OnMouseEnter)
		MSG_HANDLER(WM_EX_MOUSELEAVE,OnMouseLeave)
		MSG_HANDLER(WM_SETFOCUS,OnSetFocus)
		MSG_HANDLER(WM_KILLFOCUS,OnKillFocus)
		MSG_HANDLER(WM_SETTEXT,OnSetText)
		MSG_HANDLER(WM_ENABLE,OnEnable)
		MSG_HANDLER(WM_PAINT,OnPaint)
	END_MSG_MAP(__base)

	void SetBitmap(TCHAR* tszBmpFile,COLORREF clrTransparent=RGB(255,255,255)){
		assert(tszBmpFile);
		m_hBitmap=(HBITMAP)::LoadImage(0,tszBmpFile,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
		m_clrTransparent=clrTransparent;
		BITMAP bmp;
		::GetObject((HGDIOBJ)m_hBitmap,sizeof(BITMAP),&bmp);
		m_nTotalBmpWidth	=bmp.bmWidth;
		m_nTotalBmpHeight	=bmp.bmHeight;
	}
	void SetBitmap(int nResID,COLORREF clrTransparent=RGB(255,255,255)){
		assert(nResID);
		m_hBitmap=(HBITMAP)::LoadImage(m_hInst,MAKEINTRESOURCE(nResID),IMAGE_BITMAP,0,0,0);
		m_clrTransparent=clrTransparent;
		BITMAP bmp;
		::GetObject((HGDIOBJ)m_hBitmap,sizeof(BITMAP),&bmp);
		m_nTotalBmpWidth	=bmp.bmWidth;
		m_nTotalBmpHeight	=bmp.bmHeight;
	}

	void PreSubclassWindow(){
		LONG_PTR lStyles=GetStyle();
		m_nBtnType=BTN_PUSHBUTTON; //0-PushButton,1-CheckBox,2-RadioButton,3-GroupBox
		if((lStyles & BS_CHECKBOX)==BS_CHECKBOX || (lStyles & BS_AUTOCHECKBOX)==BS_AUTOCHECKBOX)
			m_nBtnType=BTN_CHECKBOX;
		else if((lStyles & BS_RADIOBUTTON)==BS_RADIOBUTTON || (lStyles & BS_AUTORADIOBUTTON)==BS_AUTORADIOBUTTON)
			m_nBtnType=BTN_RADIOBUTTON;

		if(m_nBtnType==BTN_PUSHBUTTON)
			m_nPerBmpWidth=m_nTotalBmpWidth/5;
		else if(m_nBtnType==BTN_CHECKBOX)
			m_nPerBmpWidth=m_nTotalBmpWidth/5/2;
		else if(m_nBtnType==BTN_RADIOBUTTON)	//BTN_RADIOBUTTON
			m_nPerBmpWidth=m_nTotalBmpWidth/5/2;
		
		RECT rectWindow;
		GetWindowRect(&rectWindow);	
		POINT ptLeftTop;
		ptLeftTop.x	=rectWindow.left;
		ptLeftTop.y	=rectWindow.top;
		::ScreenToClient(GetParent(),&ptLeftTop);
		::SetWindowPos(m_hWnd,0,ptLeftTop.x,ptLeftTop.y,m_nPerBmpWidth,m_nTotalBmpHeight,SWP_NOZORDER);
		
		KRgn region;
		region.BmpToRgn(m_hBitmap,m_clrTransparent,5);
		SetWindowRgn(region,true);
	}

	LRESULT OnSetState(UINT uMsg,WPARAM wParam,LPARAM lParam,bool& bHandled){
		if(wParam==TRUE)
			m_nDrawState=BTN_DS_PUSHED;
		else{
		   	if(m_bHover)
				m_nDrawState=BTN_DS_HOVER;
			else
				m_nDrawState=BTN_DS_NORMAL;
		}

		LONG_PTR lStyle=GetWindowLongPtr(GWL_STYLE);
		SetWindowLongPtr(GWL_STYLE,lStyle & ~WS_VISIBLE);
		LRESULT lRet=__base::ProcessWindowMessage(uMsg,wParam,lParam);
		SetWindowLongPtr(GWL_STYLE,lStyle);
		
		Invalidate();
		return lRet;
	}
	LRESULT OnSetCheck(UINT uMsg,WPARAM wParam,LPARAM lParam,bool& bHandled){
		if(wParam==BST_UNCHECKED)
			m_nCheckState=BTN_CS_UNCHECKED;
		else if(wParam==BST_CHECKED)
			m_nCheckState=BTN_CS_CHECKED;
		else if(wParam==BST_INDETERMINATE)
			m_nCheckState=BTN_CS_INDETERMINATE;

		LONG_PTR lStyle=GetWindowLongPtr(GWL_STYLE);
		SetWindowLongPtr(GWL_STYLE,lStyle & ~WS_VISIBLE);
		LRESULT lRet=__base::ProcessWindowMessage(uMsg,wParam,lParam);
		SetWindowLongPtr(GWL_STYLE,lStyle);

		Invalidate();
		return lRet; 
	}
	LRESULT OnMouseEnter(UINT uMsg,WPARAM wParam,LPARAM lParam,bool& bHandled){
		m_bHover=true;
		m_nDrawState=BTN_DS_HOVER;
		Invalidate();
		return 0;
	}
	LRESULT OnMouseLeave(UINT uMsg,WPARAM wParam,LPARAM lParam,bool& bHandled){
		m_bHover=false;
		if(::GetFocus()!=m_hWnd)
			m_nDrawState=BTN_DS_NORMAL;
		else
			m_nDrawState=BTN_DS_FOCUS;
		Invalidate();
		return 0;
	}
	LRESULT OnSetFocus(UINT uMsg,WPARAM wParam,LPARAM lParam,bool& bHandled){
		m_nDrawState=BTN_DS_FOCUS;

		LONG_PTR lStyle=GetWindowLongPtr(GWL_STYLE);
		SetWindowLongPtr(GWL_STYLE,lStyle & ~WS_VISIBLE);
		LRESULT lRet=__base::ProcessWindowMessage(uMsg,wParam,lParam);
		SetWindowLongPtr(GWL_STYLE,lStyle);
		
		Invalidate();
		return lRet;
	}
	LRESULT OnKillFocus(UINT uMsg,WPARAM wParam,LPARAM lParam,bool& bHandled){
		if(!m_bHover)
			m_nDrawState=BTN_DS_NORMAL;
		else
			m_nDrawState=BTN_DS_HOVER;

		LONG_PTR lStyle=GetWindowLongPtr(GWL_STYLE);
		SetWindowLongPtr(GWL_STYLE,lStyle & ~WS_VISIBLE);
		LRESULT lRet=__base::ProcessWindowMessage(uMsg,wParam,lParam);
		SetWindowLongPtr(GWL_STYLE,lStyle);

		Invalidate();
		return lRet;
	}
	LRESULT OnSetText(UINT uMsg,WPARAM wParam,LPARAM lParam,bool& bHandled){
		LONG_PTR lStyle=GetWindowLongPtr(GWL_STYLE);
		SetWindowLongPtr(GWL_STYLE,lStyle & ~WS_VISIBLE);
		LRESULT lRet=__base::ProcessWindowMessage(uMsg,wParam,lParam);
		SetWindowLongPtr(GWL_STYLE,lStyle);
		Invalidate();
		return lRet;
	}
	LRESULT OnEnable(UINT uMsg,WPARAM wParam,LPARAM lParam,bool& bHandled){
		m_nDrawState=wParam?BTN_DS_NORMAL:BTN_DS_DISABLED;
		Invalidate();
		bHandled=false;
		return 0;
	}
	LRESULT OnPaint(UINT uMsg,WPARAM wParam,LPARAM lParam,bool& bHandled){
		RECT rectClient;
		GetClientRect(&rectClient);
		KPaintDC dc(*this);

		if(!IsWindowEnabled())
			m_nDrawState=BTN_DS_DISABLED;

		int nDrawState=m_nDrawState;
		if(m_nBtnType==BTN_CHECKBOX)
			nDrawState=m_nDrawState*2+m_nCheckState;
		else if(m_nBtnType==BTN_RADIOBUTTON)
			nDrawState=m_nDrawState*2+m_nCheckState;

		HDC hMemDC=::CreateCompatibleDC(dc);
		HBITMAP hOldBmp=(HBITMAP)::SelectObject(hMemDC,m_hBitmap);

		HRGN hRgn=::CreateRectRgn(0, 0, 0, 0);
		GetWindowRgn(hRgn);
		dc.SelectClipRgn(hRgn);
		dc.BitBlt(0,0,RECT_W(rectClient),RECT_H(rectClient),hMemDC,nDrawState*m_nPerBmpWidth,0,SRCCOPY);
		dc.SelectClipRgn(NULL);

		::SelectObject(hMemDC,hOldBmp);
		::DeleteDC(hMemDC);
		return 0;	
	}
private:
	bool		m_bHover;
	int			m_nDrawState;
	int			m_nCheckState;
	HBITMAP		m_hBitmap;
	COLORREF 	m_clrTransparent;
	int			m_nTotalBmpWidth;
	int			m_nTotalBmpHeight;
	int 		m_nBtnType;
	int 		m_nPerBmpWidth;
};

class KUniButton : public KButtonBase<KUniButton>{
public:
	BEGIN_MSG_MAP 
		REFLECT_MSG_HANDLER(WM_DRAWITEM,OnDrawItem)	//reflected message
		MSG_HANDLER(WM_MOUSEMOVE,OnMouseMove)
		MSG_HANDLER(WM_LBUTTONDOWN,OnLButtonDown)
		MSG_HANDLER(WM_LBUTTONUP,OnLButtonUp)
	END_MSG_MAP(KButtonBase<KUniButton>)

	KUniButton(){
		m_nColor	=GetSysColor(COLOR_BTNFACE);
		m_sColor 	=m_nColor;
		m_hColor 	=m_nColor;
		m_dColor 	=m_nColor;
		m_nBorder 	=1;
		m_lfEscapement 	=0;
		m_pNormal 	=NULL;
		m_pSelected 	=NULL;
		m_pHover 	=NULL;
		m_pDisabled 	=NULL;
		m_bHover 	=false;
		m_bCapture 	=false;
		m_bMouseDown 	=false;
		m_bNeedBitmaps 	=true;
	}
	~KUniButton(){	// destructor
		SAFE_DEL_PTR(m_pNormal);
		SAFE_DEL_PTR(m_pSelected);
		SAFE_DEL_PTR(m_pHover);
		SAFE_DEL_PTR(m_pDisabled);
	}
	// constructor with default colors and border
	bool Create(LPCTSTR lpszCaption,DWORD dwStyle,const POINT& point,const HRGN hRgn,HWND hParentWnd,UINT nID){
		// store region in member variable
		m_hRgn=::CreateRectRgn(0, 0, 31, 31);
		KRect box(0, 0, 0, 0);
		if(m_hRgn) 
			::CombineRgn(m_hRgn,hRgn,0,RGN_COPY);

		// make sure that region bounding rect is located in (0, 0)
		::GetRgnBox(m_hRgn, &box);
		::OffsetRgn(m_hRgn, -box.left, -box.top);
		::GetRgnBox(m_hRgn, &box);

		// update position of region center for caption output
		m_ptCenterPoint=KPoint(box.left+box.Width()/2,box.top+box.Height()/2);
		box.OffsetRect(point);
		bool ret=(0!=KButtonBase<KUniButton>::Create(hParentWnd,lpszCaption,dwStyle,box,nID));

		SetWindowRgn(m_hRgn,true);
		return ret;
	}
	// constructor with hover & selected color specified
	bool Create(LPCTSTR lpszCaption,DWORD dwStyle,const POINT& point,
			const HRGN hRgn,HWND hParentWnd,UINT nID,COLORREF crColor){
		m_sColor=crColor;
		m_hColor=crColor;

		// call another constructor
		return Create(lpszCaption, dwStyle, point, hRgn, hParentWnd, nID);
	}	

	// complex parameters constructor
	// lpszCaption - window caption
	// dwStyle - window styles
	// point - position of the button on the parent window (in parent coordinates)
	// hRgn - handle to region which represents the button (in button client coordinates)
	// hParentWnd - handle to parent window
	// nID - control Id
	// nBorder - specifies width of the border in pixels for 3D highlight (allowed values are 1, 2)
	// nColor - normal color 
	// sColor - selected color
	// hColor - hover color
	// dColor - disabled color						
	bool Create(LPCTSTR lpszCaption, DWORD dwStyle, const POINT& point,const HRGN hRgn,HWND hParentWnd,
			UINT nID, UINT nBorder, COLORREF nColor, COLORREF sColor, COLORREF hColor, COLORREF dColor){
			// change default colors
		m_nBorder = nBorder;
		m_nColor = nColor;
		m_sColor = sColor;
		m_hColor = hColor;
		m_dColor = dColor;
		// call another constructor
		return Create(lpszCaption, dwStyle, point, hRgn, hParentWnd, nID);	
	}	
	
	// variation of above
	bool Create(LPCTSTR lpszCaption, DWORD dwStyle,const POINT& point,const HRGN hRgn,HWND hParentWnd,UINT nID,
		UINT nBorder,LONG lfEscapement,COLORREF nColor,COLORREF sColor,COLORREF hColor,COLORREF dColor){
		m_lfEscapement = lfEscapement;
		return Create(lpszCaption,dwStyle,point,hRgn,hParentWnd,nID,nBorder,nColor,sColor,hColor,dColor);
	}

	LRESULT DoDefault(/*HWND hWnd,*/UINT uMsg, WPARAM wParam, LPARAM lParam){ 
		if(uMsg==WM_LBUTTONDBLCLK)
			uMsg=WM_LBUTTONDOWN;
		return  KButtonBase<KUniButton>::DoDefault(/*hWnd,*/uMsg,wParam,lParam);
       	}
protected:
	LRESULT OnLButtonDown(UINT uMsg,WPARAM wParam,LPARAM lParam,bool& bHandled){
		// record that mouse is down
		m_bMouseDown = true;
		if (!m_bCapture) {
			SetCapture();
			m_bCapture = true;
		}	

		bHandled=false;
		return 0;
	}
	LRESULT OnLButtonUp(UINT uMsg,WPARAM wParam,LPARAM lParam,bool& bHandled){
		m_bMouseDown = false;	
		if (m_bCapture) {
			ReleaseCapture();
			m_bCapture = false;
		}
		KPoint point(LOWORD(lParam),HIWORD(lParam));
		CheckHover(point);

		bHandled=false;
		return 0;
	}
	LRESULT OnMouseMove(UINT uMsg,WPARAM wParam,LPARAM lParam,bool& bHandled){
		KPoint point(LOWORD(lParam),HIWORD(lParam));
		if (!m_bMouseDown)
			CheckHover(point);

		bHandled=false;
		return 0;	
	}

	LRESULT OnDrawItem(UINT uMsg,WPARAM wParam,LPARAM lParam,bool& bHandled){
		LPDRAWITEMSTRUCT lpDrawItemStruct=(LPDRAWITEMSTRUCT)lParam;
		// prepare DC
		KDC dc(lpDrawItemStruct->hDC);
		KRect rect;
		GetClientRect(rect);

		// prepare bitmaps they need to be prepared
		if (m_bNeedBitmaps)
			PrepareStateBitmaps(&dc, &rect);

		// draw button to the screen
		DrawButton(&dc,&rect,lpDrawItemStruct->itemState);
		
		return TRUE;
	}
	void PrepareNormalState(KDC* pDC, KDC* pMemDC,KRect* pRect){	// prepare normal state button bitmap
		// prepare MYBS_NORMAL state bitmap
		SAFE_DEL_PTR(m_pNormal);
		m_pNormal = new KBitmap;
		PaintRgn(pDC,pMemDC,m_pNormal,m_nColor,pRect,true,false);
	}
	void PrepareSelectedState(KDC* pDC, KDC* pMemDC, KRect* pRect){	// prepare selectedstate button bitmap
		SAFE_DEL_PTR(m_pSelected);
		m_pSelected = new KBitmap;
		PaintRgn(pDC, pMemDC, m_pSelected, m_sColor, pRect, true, true);
	}
	void PrepareHoverState(KDC * pDC, KDC * pMemDC, KRect * pRect){		// prepare hover state button bitmap
		SAFE_DEL_PTR(m_pHover);
		m_pHover = new KBitmap;
		PaintRgn(pDC, pMemDC, m_pHover, m_hColor, pRect, true, false);
	}
	void PrepareDisabledState(KDC * pDC, KDC * pMemDC, KRect * pRect){	// prepare disabled state button bitmap
		SAFE_DEL_PTR(m_pDisabled);
		m_pDisabled = new KBitmap;
		PaintRgn(pDC, pMemDC, m_pDisabled, m_dColor, pRect, false, false);
	}
	void DrawButtonCaption(HDC hDC, KRect * pRect, bool bEnabled, bool bSunken){	// draws button caption 
		// select parent font
		int nOldMode =::SetBkMode(hDC, TRANSPARENT);
		//CString text;
		int nTextLen=GetWindowTextLength()+1;
		TCHAR* text=new TCHAR[nTextLen];
		//::memset(text,'\0',sizeof(TCHAR)*nTextLen);
		GetWindowText(text,nTextLen);
		
		//LOGFONT lf;
		//GetParent() -> GetFont() -> GetLogFont(&lf);
		//HFONT hFont = CreateFontIndirect(&lf);
		//HFONT hFont=(HFONT)::SendMessage(GetParent(),WM_GETFONT,0,0);
		//HFONT hOldFont=(HFONT)::SelectObject(hDC, hFont);

		// determine point where to output text
		TEXTMETRIC tm;
		::GetTextMetrics(hDC, &tm);
		KPoint p = KPoint(m_ptCenterPoint.x, m_ptCenterPoint.y + tm.tmHeight/ 2); 
		/////////////////////////////
		if (bSunken) 
			p.Offset(m_nBorder, m_nBorder); 
			
		// draw button caption depending upon button state
		if (bEnabled) {
			::SetTextColor(hDC, GetSysColor(COLOR_BTNTEXT));
			::SetTextAlign(hDC, TA_CENTER | TA_BOTTOM);
			::TextOut(hDC, p.x, p.y, text,nTextLen-1);
		}
		else {
			::SetTextColor(hDC, GetSysColor(COLOR_3DHILIGHT));
			::TextOut(hDC, p.x + 1, p.y + 1, text,nTextLen);
			::SetTextColor(hDC, GetSysColor(COLOR_3DSHADOW));
			::TextOut(hDC, p.x, p.y, text,nTextLen-1);
		}

		//::SelectObject(hDC, hOldFont);
		//DeleteObject(hFont);
		::SetBkMode(hDC, nOldMode);
	}
	// paint button 
	void PaintRgn(KDC* pDC,KDC* pMemDC,KBitmap* pBitmap,COLORREF color,KRect* pRect,bool bEnabled,bool bSunken){
		// create bitmap
		pBitmap->CreateCompatibleBitmap(*pDC,pRect->Width(),pRect->Height());
		HBITMAP OldBitmap = (HBITMAP)pMemDC->SelectObject(*pBitmap); 

		// prepare region
		HRGN hRgn = CreateRectRgn(0, 0, 0, 0);
		GetWindowRgn(hRgn);

		// fill rect a with transparent color and fill rgn
		//pMemDC->FillSolidRect(pRect, RGB(0, 0, 0));
		HBRUSH hBrush=::CreateSolidBrush(color);
		::FillRgn(*pMemDC,hRgn,hBrush);
		::DeleteObject(hBrush);

		// draw 3D border and text
		DrawButtonCaption(*pMemDC, pRect, bEnabled, bSunken);
		FrameRgn3D(*pMemDC, hRgn, bSunken);

		// clean up
		::DeleteObject(hRgn);
		pMemDC->SelectObject(OldBitmap); 
	}
	
private:
	UINT 		m_nBorder;// width of the border in pixels for 3D highlight
	LONG 		m_lfEscapement;	// orientation of the caption (in tenth of the degree as in LOGFONT)
	COLORREF 	m_nColor, m_sColor, m_hColor, m_dColor;//background colors: normal, selected, hover, disabled
	KBitmap* 	m_pNormal;		// bitmaps to hold button images
	KBitmap* 	m_pSelected;
	KBitmap* 	m_pHover;
	KBitmap* 	m_pDisabled;	
	POINT 		m_ptCenterPoint;			// button caption will be centered around this point
	bool 		m_bMouseDown;		// indicated that mouse is pressed down
	bool 		m_bHover;	// indicates if mouse is over the button
	bool 		m_bCapture;	// indicates that mouse is captured in the buton
	HRGN 		m_hRgn;		// region in screen coordinates
	bool 		m_bNeedBitmaps;	// flag idicates that state bitmaps must be rebuild
	
	void DrawButton(KDC* pDC, KRect* pRect,UINT state){	// draws button to the screen
		// create memory DC
		KDC* pMemDC = new KDC;
		pMemDC->CreateCompatibleDC(*pDC);
		KBitmap bmOldBitmap;

		// get region
		HRGN hRgn =::CreateRectRgn(0, 0, 0, 0);
		GetWindowRgn(hRgn);

		// select bitmap to paint depending upon button state
		if (state & ODS_DISABLED)
			bmOldBitmap=pMemDC->SelectObject(*m_pDisabled);
		else {
			if (state & ODS_SELECTED)
				bmOldBitmap = pMemDC->SelectObject(*m_pSelected);
			else {
				if (m_bHover)
					bmOldBitmap=pMemDC->SelectObject(*m_pHover);
				else 
					bmOldBitmap=pMemDC->SelectObject(*m_pNormal);
			}	
		}

		// paint using region for clipping
		::SelectClipRgn(*pDC, hRgn);
		pDC->BitBlt(0, 0, pRect->Width(), pRect->Height(),*pMemDC,0,0,SRCCOPY);
		::SelectClipRgn(*pDC, NULL);

		// clean up
		::DeleteObject(hRgn);
		pMemDC->SelectObject(bmOldBitmap);
		delete pMemDC;
	}
	void PrepareStateBitmaps(KDC* pDC, KRect* pRect){	// prepares bitmaps for button states
		// prepare memory DC
		KDC* pMemDC=new KDC;
		pMemDC->CreateCompatibleDC(*pDC);

		// prepare bitmaps for all button states and for the mask
		PrepareNormalState(pDC, pMemDC, pRect);
		PrepareSelectedState(pDC, pMemDC, pRect);
		PrepareHoverState(pDC, pMemDC, pRect);
		PrepareDisabledState(pDC, pMemDC, pRect);

		// clean up
		delete pMemDC; 
		m_bNeedBitmaps = false;
	}
	bool HitTest(POINT& point){	// determines if point is inside the button region
		bool result = false;

		// Obtain handle to window region.
		HRGN hRgn =::CreateRectRgn(0, 0, 0, 0);
		GetWindowRgn(hRgn);
		KRect rgnRect;
		::GetRgnBox(hRgn, &rgnRect);

		// First check if point is in region bounding rect.
		// Then check if point is in the region in adition to being in the bouding rect.
		result=::PtInRect(&rgnRect,point) && ::PtInRegion(hRgn, point.x, point.y);

		// Clean up and exit.
		DeleteObject(hRgn);
		return result;
	}
	void FrameRgn3D(HDC hDC, const HRGN hRgn, bool bSunken){	// frames region to show 3D shadows
		// we need two differenr regions to keep base region and border region
		HBRUSH hBrush;
		HRGN hBaseRgn = CreateRectRgn(0, 0, 0, 0);
		//colors of inner and outer shadow for top-left and right-bottom corners
		COLORREF ltOuter,ltInner,rbOuter,rbInner;

		// decide on color scheme
		if (!bSunken) {
			ltOuter = GetSysColor(COLOR_3DLIGHT);
			ltInner = GetSysColor(COLOR_3DHILIGHT);
			rbOuter = GetSysColor(COLOR_3DDKSHADOW);
			rbInner = GetSysColor(COLOR_3DSHADOW);
		}else{
			rbInner = GetSysColor(COLOR_3DLIGHT);
			rbOuter = GetSysColor(COLOR_3DHILIGHT);
			ltInner = GetSysColor(COLOR_3DDKSHADOW);
			ltOuter = GetSysColor(COLOR_3DSHADOW);
		}

		// offset highlight and shadow regions
		// substract them from the base region 
		switch (m_nBorder){
			case 2:
				::CombineRgn(hBaseRgn, hRgn, 0, RGN_COPY);
				::OffsetRgn(hBaseRgn, 2, 2);
				::CombineRgn(hBaseRgn, hRgn, hBaseRgn, RGN_DIFF);
				hBrush =::CreateSolidBrush(ltInner);
				::FillRgn(hDC, hBaseRgn, hBrush);
				::DeleteObject(hBrush);

				::CombineRgn(hBaseRgn, hRgn, 0, RGN_COPY);
				::OffsetRgn(hBaseRgn, -2, -2);
				::CombineRgn(hBaseRgn, hRgn, hBaseRgn, RGN_DIFF);
				hBrush=::CreateSolidBrush(rbInner);
				::FillRgn(hDC, hBaseRgn, hBrush);
				::DeleteObject(hBrush);

				::CombineRgn(hBaseRgn, hRgn, 0, RGN_COPY);
				::OffsetRgn(hBaseRgn, 1, 1);
				::CombineRgn(hBaseRgn, hRgn, hBaseRgn, RGN_DIFF);
				hBrush =::CreateSolidBrush(ltOuter);
				::FillRgn(hDC, hBaseRgn, hBrush);
				::DeleteObject(hBrush);

				::CombineRgn(hBaseRgn, hRgn, 0, RGN_COPY);
				::OffsetRgn(hBaseRgn, -1, -1);
				::CombineRgn(hBaseRgn, hRgn, hBaseRgn, RGN_DIFF);
				hBrush =::CreateSolidBrush(rbOuter);
				::FillRgn(hDC, hBaseRgn, hBrush);
				::DeleteObject(hBrush);
			break;
			default:
				::CombineRgn(hBaseRgn, hRgn, 0, RGN_COPY);
				::OffsetRgn(hBaseRgn, 1, 1);
				::CombineRgn(hBaseRgn, hRgn, hBaseRgn, RGN_DIFF);
				hBrush=::CreateSolidBrush(ltInner);
				::FillRgn(hDC, hBaseRgn, hBrush);
				::DeleteObject(hBrush);

				::CombineRgn(hBaseRgn, hRgn, 0, RGN_COPY);
				::OffsetRgn(hBaseRgn, -1, -1);
				::CombineRgn(hBaseRgn, hRgn, hBaseRgn, RGN_DIFF);
				hBrush=::CreateSolidBrush(rbOuter);
				::FillRgn(hDC, hBaseRgn, hBrush);
				::DeleteObject(hBrush);
			break;
		}
		
		// clean up regions
		::DeleteObject(hBaseRgn);
	}
	void CheckHover(POINT& point){
		if (HitTest(point)) {
			if (!m_bCapture) {
				SetCapture();
				m_bCapture = true;
			}
			if (!m_bHover) {
				m_bHover = true;
				RedrawWindow();
			}
		}else{
			if (m_bCapture) {
				ReleaseCapture();
				m_bCapture = false;
			}
			m_bHover = false;
			RedrawWindow();
		}
	}
};

class KRoundButton : public KButtonBase<KRoundButton>{
public:
	BEGIN_MSG_MAP
		REFLECT_MSG_HANDLER(WM_DRAWITEM,OnDrawItem)
	END_MSG_MAP(KButtonBase<KRoundButton>)

	KRoundButton(){m_bDrawDashedFocusCircle=true;}
	~KRoundButton(){m_rgn.DeleteObject();}

	LRESULT DoDefault(UINT uMsg, WPARAM wParam, LPARAM lParam){ 
		if(uMsg==WM_LBUTTONDBLCLK)
			uMsg=WM_LBUTTONDOWN;
		return  KButtonBase<KRoundButton>::DoDefault(uMsg,wParam,lParam);
    }

	void PreSubclassWindow(){
		KButtonBase<KRoundButton>::PreSubclassWindow();

		ModifyStyle(0, BS_OWNERDRAW);

		KRect rect;
		GetClientRect(&rect);

		// Resize the window to make it square
		rect.bottom = rect.right = min(rect.bottom,rect.right);

		// Get the vital statistics of the window
		m_ptCentre = rect.CenterPoint();
		m_nRadius  = rect.bottom/2-1;

		// Set the window region so mouse clicks only activate the round section 
		// of the button
		m_rgn.DeleteObject(); 
		SetWindowRgn(NULL, FALSE);
		m_rgn.CreateEllipticRgnIndirect(rect);
		SetWindowRgn(m_rgn, TRUE);
	}
	LRESULT OnDrawItem(UINT uMsg,WPARAM wParam,LPARAM lParam,bool& bHandled){
		
		LPDRAWITEMSTRUCT lpDrawItemStruct=(LPDRAWITEMSTRUCT)lParam;
		KDC* pDC=new KDC(lpDrawItemStruct->hDC);
		KRect rect = lpDrawItemStruct->rcItem;
		UINT state = lpDrawItemStruct->itemState;
		LONG_PTR nStyle = GetStyle();
		int nRadius = m_nRadius;

		int nSavedDC = pDC->SaveDC();

		pDC->SelectStockObject(NULL_BRUSH);
		pDC->FillSolidRect(&rect, ::GetSysColor(COLOR_BTNFACE));

		// Draw the focus circle around the button
		if ((state & ODS_FOCUS) && m_bDrawDashedFocusCircle)
			DrawCircle(*pDC, m_ptCentre, nRadius--, RGB(0,0,0));

		// Draw the raised/sunken edges of the button (unless flat)
		if (nStyle & BS_FLAT) {
			DrawCircle(*pDC, m_ptCentre, nRadius--, RGB(0,0,0));
			DrawCircle(*pDC, m_ptCentre, nRadius--, ::GetSysColor(COLOR_3DHIGHLIGHT));
		} else {
			if ((state & ODS_SELECTED))	{
				DrawCircle(*pDC, m_ptCentre, nRadius--, 
						   ::GetSysColor(COLOR_3DDKSHADOW), ::GetSysColor(COLOR_3DHIGHLIGHT));
				DrawCircle(*pDC, m_ptCentre, nRadius--, 
						   ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DLIGHT));
			} else {
				DrawCircle(*pDC, m_ptCentre, nRadius--, 
						   ::GetSysColor(COLOR_3DHIGHLIGHT), ::GetSysColor(COLOR_3DDKSHADOW));
				DrawCircle(*pDC, m_ptCentre, nRadius--, 
						   ::GetSysColor(COLOR_3DLIGHT), ::GetSysColor(COLOR_3DSHADOW));
			}
		}
		
		// draw the text if there is any
		int nTextLen=GetWindowTextLength();
		TCHAR*	strText=new TCHAR[nTextLen+1];
		::memset(strText,'\0',nTextLen+1);
		if(GetWindowText(strText,nTextLen+1)){
			KRgn rgn;
			rgn.CreateEllipticRgn(m_ptCentre.x-nRadius, m_ptCentre.y-nRadius, 
								  m_ptCentre.x+nRadius, m_ptCentre.y+nRadius);
			pDC->SelectClipRgn(rgn);

			KSize Extent=pDC->GetTextExtent(strText);
			KPoint pt = KPoint( m_ptCentre.x - Extent.cx/2, m_ptCentre.x - Extent.cy/2 );

			if (state & ODS_SELECTED) pt.Offset(1,1);

			pDC->SetBkMode(TRANSPARENT);

			if (state & ODS_DISABLED)
				pDC->DrawState((HBRUSH)NULL,0,(LPARAM)strText,(WPARAM)nTextLen,
						pt.x,pt.y,Extent.cx,Extent.cy,DST_TEXT|DSS_DISABLED);
			else
				pDC->TextOut(pt.x, pt.y,strText,nTextLen);

			pDC->SelectClipRgn(NULL);
			rgn.DeleteObject();
		}
		delete[] strText;
		// Draw the focus circle on the inside of the button
		
		if ((state & ODS_FOCUS) && m_bDrawDashedFocusCircle)
			DrawCircle(*pDC, m_ptCentre, nRadius-2, RGB(0,0,0), true);

		pDC->RestoreDC(nSavedDC);

		return TRUE; 
	}
private:
	KRgn   m_rgn;
	KPoint m_ptCentre;
	int    m_nRadius;
	bool   m_bDrawDashedFocusCircle;
};

class KColorStatic : public KStaticBase<KColorStatic>{
public:
	typedef KStaticBase<KColorStatic> __base;
	BEGIN_MSG_MAP
		REFLECT_MSG_HANDLER(WM_CTLCOLORSTATIC,OnCtrlColor)
	END_MSG_MAP(__base)

	KColorStatic(COLORREF crBk=::GetSysColor(COLOR_BTNFACE),COLORREF crText=RGB(0,0,0)):m_crBk(crBk),m_crText(crText){
		m_hBrushBk=::CreateSolidBrush(m_crBk);
	}
	~KColorStatic(){::DeleteObject(m_hBrushBk);}

	LRESULT OnCtrlColor(UINT uMsg,WPARAM wParam,LPARAM lParam,bool& bHandled){
		::SetBkMode((HDC)wParam,TRANSPARENT);
		::SetTextColor((HDC)wParam,m_crText);
		bHandled=true;
		return (LRESULT)m_hBrushBk;
	}
private:
	COLORREF 	m_crBk;
	COLORREF 	m_crText;
	HBRUSH		m_hBrushBk;		
};

class KNumEdit : public KEditBase<KNumEdit>{
public:
	typedef KEditBase<KNumEdit> __base;
		
};
} //namespace kwinui
#endif //__KCSTCTRLS_H__
