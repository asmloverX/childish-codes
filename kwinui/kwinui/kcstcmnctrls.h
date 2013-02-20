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
 *  custum draw for win32 common contrls 
 *
 *  01/26/2007
 *  proguru
 */

//#define   _WIN32_WINNT           0x0400  
#define _CRT_SECURE_NO_WARNINGS

#ifndef __KXCMNCTRLS_H__
#define __KXCMNCTRLS_H__

#include "kwin.h"
#include "kdc.h"
#include "kgdiobj.h"
#include "ktypes.h"
#include "kcmnctrls.h"
#include "kmisc.h"
#include <cmath>
#include <cstdio>
#include <cassert>
 
namespace kwinui{

#define TBN_START		1000
#define TBN_LBUTTONDOWN	TBN_START
#define TBN_LBUTTONUP	TBN_START+1

class KXSlider : public KTrackBarBase<KXSlider>{
public:
	BEGIN_MSG_MAP
		MSG_HANDLER(WM_ERASEBKGND,OnEraseBkGnd)
		MSG_HANDLER(WM_LBUTTONDOWN,OnLButtonDown)
		MSG_HANDLER(WM_LBUTTONUP,OnLButtonUp)
		MSG_HANDLER(WM_MOUSEMOVE,OnMouseMove)
		MSG_HANDLER(WM_MOUSEWHEEL,OnMouseWheel)
		MSG_HANDLER(WM_KEYDOWN,OnKeyDown)
		MSG_HANDLER(WM_KEYUP,OnKeyUp)
		MSG_HANDLER(WM_PAINT,OnPaint)
		MSG_HANDLER(TBM_SETPOS,OnSetPos)
	END_MSG_MAP(KTrackBarBase<KXSlider>)

	KXSlider(){
		m_bDragging			=false;
		m_bDragChanged		=false;
		m_bHover			=false;
		m_nChFillHeight		=0;

		m_crTransparent		=RGB(255,255,255);
		m_crBack			=::GetSysColor(COLOR_BTNFACE);
		m_bmpChLeft			=0;
	 	m_bmpChRight		=0;
	 	m_bmpCh				=0;
	 	m_bmpChFill			=0;
	 	m_bmpThumbNormal	=0;
	 	m_bmpThumbHover		=0;
	 	m_bmpThumbPressed	=0;
	}
	bool SetBitmap(UINT nChannelLeft,UINT nChannelRight,UINT nChannel,UINT nChannelFill,UINT nThumbNormal,
			UINT nThumbHover=0,COLORREF crTransparent=RGB(255,255,255),COLORREF crBack=::GetSysColor(COLOR_BTNFACE),
			UINT nThumbPressed=0){

		BITMAP bm={0};
		m_bmpChLeft			=::LoadBitmap(m_hInst,MAKEINTRESOURCE(nChannelLeft));
		::GetObject((HGDIOBJ)m_bmpChLeft,sizeof(BITMAP),&bm);
		m_sizeChannelLeft	=KSize(bm.bmWidth,bm.bmHeight);
		m_rtChannelLeft		=KRect(0,0,bm.bmWidth,bm.bmHeight);
		m_rtChannelRight	=m_rtChannelLeft;

		m_bmpChRight	=::LoadBitmap(m_hInst,MAKEINTRESOURCE(nChannelRight));
		m_bmpCh			=::LoadBitmap(m_hInst,MAKEINTRESOURCE(nChannel));
		m_bmpChFill		=::LoadBitmap(m_hInst,MAKEINTRESOURCE(nChannelFill));
		::GetObject((HGDIOBJ)m_bmpChFill,sizeof(BITMAP),&bm);
		m_nChFillHeight	=bm.bmHeight;

		m_bmpThumbNormal	=::LoadBitmap(m_hInst,MAKEINTRESOURCE(nThumbNormal));
		::GetObject((HGDIOBJ)m_bmpThumbNormal,sizeof(BITMAP),&bm);
		m_sizeThumb			=KSize(bm.bmWidth,bm.bmHeight);
		m_rtThumb			=KRect(0,0,bm.bmWidth,bm.bmHeight);

		m_crTransparent		=crTransparent;
		m_crBack			=crBack;
		if(nThumbHover)
			m_bmpThumbHover	=::LoadBitmap(m_hInst,MAKEINTRESOURCE(nThumbHover));
		if(nThumbPressed)
			m_bmpThumbPressed=::LoadBitmap(m_hInst,MAKEINTRESOURCE(nThumbPressed));

		return true;

	}
	void Init(){
		SetRange(0,100,false);
		SetLineSize(1);
		SetPageSize(10);
		SetPos(0);

		KRect rt;
		GetClientRect(rt);
		
		KPoint pt(0,(rt.Height()-m_rtChannelLeft.Height())/2);
		m_rtChannelLeft.OffsetRect(pt);
		
		pt.Offset((rt.Width()-m_rtChannelLeft.Width()),0);
		m_rtChannelRight.OffsetRect(pt);
	}

	void PreSubclassWindow(){
		Init();	
	}

	void PostMessageToParent(int nTBCode){
		::PostMessage(GetParent(),WM_HSCROLL,(WPARAM)((GetPos()<<16)|nTBCode),(LPARAM)(m_hWnd));
	}
	bool SetThumb(KPoint& pt){
		int nMin=GetRangeMin();
		int nMax=GetRangeMax();

		KRect rt;
		GetClientRect(rt);
		int nNewPos=0;

		if(pt.x>0x8000)
			nNewPos=nMin;
		else if(pt.x>=(rt.Width()-m_rtThumb.Width()/2))
			nNewPos=nMax;
		else if(pt.x<=m_rtThumb.Width()/2)
			nNewPos=nMin;
		else
			nNewPos=(pt.x-m_rtThumb.Width()/2)*(nMax-nMin)/(rt.Width()-m_rtThumb.Width());

		bool bChanged=(nNewPos!=GetPos());
		if(bChanged)
			SetPos(nNewPos);

		return bChanged;
	}
	LRESULT OnEraseBkGnd(UINT uMsg,WPARAM wParam,LPARAM lParam,bool& bHandled){
		return TRUE;
	}
	LRESULT OnLButtonDown(UINT uMsg,WPARAM wParam,LPARAM lParam, bool& bHandled){
		::SendMessage(GetParent(),WM_COMMAND,MAKELONG(::GetDlgCtrlID(*this),TBN_LBUTTONDOWN),(LPARAM)(HWND)(*this));
		KPoint point(LOWORD(lParam),HIWORD(lParam));

		if(!m_bDragging){
			m_bDragging=true;
			SetCapture();
			SetFocus();
			if(SetThumb(point)){
				PostMessageToParent(TB_THUMBTRACK);
				//dlog.Write("LButtonDown::TB_THUMBTRACK");
				m_bHover=true;
				RedrawWindow();
			}
		}else
			bHandled=false;
		return 0;
	}
	LRESULT OnLButtonUp(UINT uMsg,WPARAM wParam,LPARAM lParam,bool& bHandled){
		::SendMessage(GetParent(),WM_COMMAND,MAKELONG(::GetDlgCtrlID(*this),TBN_LBUTTONUP),(LPARAM)(HWND)(*this));
		KPoint point(LOWORD(lParam),HIWORD(lParam));

		if(m_bDragging){
			m_bDragging=false;
			::ReleaseCapture();
			PostMessageToParent(TB_THUMBPOSITION);
			//dlog.Write("LButtonUp::TB_THUMBPOSITION");
			/*
			if(SetThumb(point)){
				PostMessageToParent(TB_THUMBTRACK);	
				PostMessageToParent(TB_THUMBPOSITION);
				dlog.Write("LButtonUp::TB_THUMBPOSITION");
				RedrawWindow();
			}*/

		}else
			bHandled=false;

		return 0;
	}
	LRESULT OnMouseMove(UINT uMsg,WPARAM wParam,LPARAM lParam,bool& bHandled){
		KPoint point(LOWORD(lParam),HIWORD(lParam));

		int nMin=GetRangeMin();
		int nMax=GetRangeMax();
		int nPos=GetPos();

		KRect rtThumb=m_rtThumb;
		KRect rt;
		GetClientRect(rt);
		KPoint ptPos(((nPos*(rt.Width()-rtThumb.Width())/(nMax-nMin))+m_rtChannelLeft.left),
					(rt.Height()-rtThumb.Height())/2);
		rtThumb.Offset(ptPos);
		if(rtThumb.PtInRect(point))
			m_bHover=true;
		else
			m_bHover=false;

		if(m_bDragging){
			if(SetThumb(point)){
				PostMessageToParent(TB_THUMBTRACK);
				//dlog.Write("MouseMove::TB_THUMBTRACK");
			}
		}
		RedrawWindow();
		return 0;
	}
	LRESULT OnMouseWheel(UINT uMsg,WPARAM wParam,LPARAM lParam,bool&bHandled){
		return 0;
	}
	LRESULT OnKeyDown(UINT uMsg,WPARAM wParam,LPARAM lParam,bool& bHandled){
		int nChar=static_cast<int>(wParam);
		int nMin = GetRangeMin();
		int nMax = GetRangeMax();

		switch(nChar){
			case VK_LEFT:
			case VK_UP:
				{
					int nNewPos = GetPos()-GetLineSize();
					while(nNewPos < nMin) nNewPos=nMin; 
					SetPos(nNewPos);
					RedrawWindow();
					PostMessageToParent(TB_LINEUP);
				}
				break;
			
			case VK_RIGHT:
			case VK_DOWN:
				{
					int nNewPos = GetPos()+GetLineSize();
					while(nNewPos > nMax) nNewPos=nMax; 
					SetPos(nNewPos);
					RedrawWindow();
					PostMessageToParent(TB_LINEDOWN);
				}
				break;

			case VK_PRIOR:	//page up
				{
					int nNewPos = GetPos()+GetPageSize();
					while(nNewPos > nMax) nNewPos = nMax;
					SetPos(nNewPos);
					RedrawWindow();
					PostMessageToParent(TB_PAGEUP);
				}
				break;

			case VK_NEXT:	//page down
				{
					int nNewPos = GetPos()-GetPageSize();
					while(nNewPos < nMin) nNewPos=nMin; 
					SetPos(nNewPos);
					RedrawWindow();
					PostMessageToParent(TB_PAGEDOWN);
				}
				break;

			case VK_HOME:
			case VK_END:
				break;

			default:
				bHandled=false;
				break;
		}
		return 0;
	}
	LRESULT OnKeyUp(UINT uMsg,WPARAM wParam,LPARAM lParam,bool& bHandled){
		int nChar=static_cast<int>(wParam);
		switch(nChar){
			case VK_LEFT:
			case VK_UP:
			case VK_RIGHT:
			case VK_DOWN:
			case VK_PRIOR:
			case VK_NEXT:
				PostMessageToParent(TB_ENDTRACK);
				break;

			case VK_HOME:
			case VK_END:
				// Do nothing
				break;

			default:
				bHandled=false;
		}
		return 0;
	}
	LRESULT OnPaint(UINT uMsg,WPARAM wParam,LPARAM lParam,bool& bHandled){
		KPaintDC dc(*this);
		KMemDC pDC(dc);
		OnDraw(pDC);
		return 0;
	}
	void OnDraw(KDC* pDC){
		int nMin=GetRangeMin();
		int nMax=GetRangeMax();
		int nPos=GetPos();
		
		KRect rt;
		GetClientRect(rt);
		KRect rtThumb=m_rtThumb;

		KPoint ptPos(((nPos*(rt.Width()-rtThumb.Width())/(nMax-nMin))+m_rtChannelLeft.left),
				(rt.Height()-rtThumb.Height())/2);
		
		m_rtChannel.left	=m_rtChannelLeft.left+m_rtChannelLeft.Width();
		m_rtChannel.top		=m_rtChannelLeft.top;
		m_rtChannel.right	=rt.Width()-m_rtChannelLeft.Width();	
		m_rtChannel.bottom	=m_rtChannel.top+m_rtChannelLeft.Height();

		m_rtChannelFill.left	=m_rtChannelLeft.left+2;
		m_rtChannelFill.top		=m_rtChannel.top+(m_rtChannel.Height()-m_nChFillHeight)/2;
		m_rtChannelFill.right	=ptPos.x;
		m_rtChannelFill.bottom	=m_rtChannelFill.top+m_nChFillHeight;

		rtThumb.OffsetRect(ptPos);	
		
		pDC->FillSolidRect(rt,m_crBack);

		KDC MemDC;			
		HBITMAP hOldBmp;
		MemDC.CreateCompatibleDC(pDC->m_hDC);
		//
		hOldBmp=(HBITMAP)MemDC.SelectObject(m_bmpChLeft);
		pDC->BitBlt(m_rtChannelLeft.left,m_rtChannelLeft.top,m_rtChannelLeft.Width(),m_rtChannelLeft.Height(),
					MemDC,0,0,SRCCOPY);
		MemDC.SelectObject(m_bmpChRight);
		pDC->BitBlt(m_rtChannelRight.left,m_rtChannelRight.top,m_rtChannelRight.Width(),
					m_rtChannelRight.Height(),MemDC,0,0,SRCCOPY);

		BITMAP bm;
		::GetObject((HGDIOBJ)m_bmpCh,sizeof(BITMAP),&bm);
		MemDC.SelectObject(m_bmpCh);
		pDC->StretchBlt(m_rtChannel.left,m_rtChannel.top,m_rtChannel.Width(),m_rtChannel.Height(),
						MemDC,0,0,bm.bmWidth,bm.bmHeight,SRCCOPY);

		::GetObject((HGDIOBJ)m_bmpChFill,sizeof(BITMAP),&bm);
		MemDC.SelectObject(m_bmpChFill);
		pDC->StretchBlt(m_rtChannelFill.left,m_rtChannelFill.top,m_rtChannelFill.Width(),
						m_rtChannelFill.Height(),MemDC,0,0,bm.bmWidth,bm.bmHeight,SRCCOPY);

		if(m_bHover)
			MemDC.SelectObject(m_bmpThumbHover);
		else
			MemDC.SelectObject(m_bmpThumbNormal);
		//pDC->BitBlt(rtThumb.left,rtThumb.top,rtThumb.Width(),rtThumb.Height(),MemDC,0,0,SRCCOPY);
		pDC->TransparentBlt(rtThumb.left,rtThumb.top,rtThumb.Width(),rtThumb.Height(),MemDC,0,0,
				rtThumb.Width(),rtThumb.Height(),m_crTransparent);

		MemDC->SelectObject(hOldBmp);
	}
	LRESULT OnSetPos(UINT uMsg,WPARAM wParam,LPARAM lParam,bool& bHandled){
		KTrackBarBase<KXSlider>::ProcessWindowMessage(uMsg,wParam,lParam);
		RedrawWindow();
		return 0;
	}
	
private:
	bool 	m_bDragging;
	bool 	m_bDragChanged;
	bool	m_bHover;

	int		m_nChFillHeight;
	KSize	m_sizeChannelLeft;	//also right
	KSize	m_sizeThumb;
	//KPoint	m_ptPos;

	KRect	m_rtChannelLeft;
	KRect	m_rtChannelRight;
	KRect	m_rtChannel;
	KRect	m_rtChannelFill;
	KRect	m_rtThumb;

	COLORREF m_crTransparent;
	COLORREF m_crBack;
	HBITMAP	m_bmpChLeft;
	HBITMAP m_bmpChRight;
	HBITMAP m_bmpCh;
	HBITMAP m_bmpChFill;
	HBITMAP m_bmpThumbNormal;
	HBITMAP m_bmpThumbHover;
	HBITMAP m_bmpThumbPressed;
};

#define pi 3.141592653589793238462643383279
//roundslider
class KRoundSlider : public KTrackBarBase<KRoundSlider>{
public:
	BEGIN_MSG_MAP
		MSG_HANDLER(WM_SIZE,OnSize)
		MSG_HANDLER(WM_ERASEBKGND,OnEraseBkgnd)
		MSG_HANDLER(WM_PAINT,OnPaint)
		MSG_HANDLER(WM_LBUTTONDOWN,OnLButtonDown)
		MSG_HANDLER(WM_LBUTTONUP,OnLButtonUp)
		MSG_HANDLER(WM_MOUSEMOVE,OnMouseMove)
		MSG_HANDLER(WM_KEYDOWN,OnKeyDown)
		MSG_HANDLER(WM_KEYUP,OnKeyUp)
		MSG_HANDLER(WM_MOUSEWHEEL,OnMouseWheel)
	END_MSG_MAP(KTrackBarBase<KRoundSlider>)
	
	KRoundSlider(){
		m_strText=new TCHAR[4];
		_tcscpy(m_strText,_T("%ld"));

		m_nKnobRadius = 12;
		m_nZero = 0;
		m_bInverted = false;
		m_bDragging = false;
		m_bDrawRadioButton = false;
		m_bDragByKnobOnly = false;

		::GetObject((HFONT)GetStockObject(DEFAULT_GUI_FONT), sizeof(m_lf), &m_lf);
		m_font.CreateFontIndirect(&m_lf);
		m_crText = ::GetSysColor(COLOR_WINDOWTEXT);

		m_hKnobBrush = ::CreateSolidBrush(::GetSysColor(COLOR_3DFACE));
		m_hDialBrush = ::CreateSolidBrush(::GetSysColor(COLOR_3DFACE));
	}
	~KRoundSlider(){SAFE_DEL_PTRS(m_strText);}
	
	void Init(){
		KRect rc;
		GetClientRect(rc);

		// Resize the window to make it square
		rc.bottom = rc.right = min(rc.bottom, rc.right);

		// Get the vital statistics of the window
		m_ptCenter = rc.CenterPoint();
		m_nRadius = rc.bottom/2-(m_nKnobRadius+1);

		// Set the window region so mouse clicks only activate the round section 
		// of the slider
		m_rgn.DeleteObject(); 
		SetWindowRgn(NULL, FALSE);
		m_rgn.CreateEllipticRgnIndirect(rc);
		SetWindowRgn(m_rgn, TRUE);
	}
	void PreSubclassWindow(){
		KTrackBarBase<KRoundSlider>::PreSubclassWindow();

		SetRange(0, 359, false);
		SetLineSize(1);
		SetPageSize(10);

		Init();
	}

	LRESULT OnSize(UINT uMsg,WPARAM wParam,LPARAM lParam,bool& bHandled){
		Init();
		return 0;
	}
	LRESULT OnEraseBkgnd(UINT uMsg,WPARAM wParam,LPARAM lParam,bool& bHandled){
		return TRUE;
	}
	LRESULT OnPaint(UINT uMsg,WPARAM wParam,LPARAM lParam,bool& bHandled){
		int nMin = GetRangeMin();
		int nMax = GetRangeMax()+1;

		bool bDisabled = !IsWindowEnabled();

		KPaintDC dc(*this); // device context for painting
		KMemDC pDC(dc);

		int nRadius = m_nRadius;

		// Draw (clear) the background
		KRect rc;
		GetClientRect(rc);
		pDC->SelectStockObject(NULL_BRUSH);
		pDC->SelectStockObject(NULL_PEN);
		pDC->FillSolidRect(rc, ::GetSysColor(COLOR_BTNFACE));

		// Draw the sliders channel
		if(!m_bDrawRadioButton){
		DrawCircle(pDC,m_ptCenter,nRadius--,::GetSysColor(COLOR_3DDKSHADOW),::GetSysColor(COLOR_3DHIGHLIGHT));
		DrawCircle(pDC,m_ptCenter,nRadius,::GetSysColor(COLOR_3DSHADOW),::GetSysColor(COLOR_3DLIGHT));
		}

		int nPos = (((GetPos()-nMin)*360/(nMax-nMin)) + m_nZero + 360) % 360;
		if(m_bInverted) nPos = 360-nPos;

		double dPos = ((double)(nPos))*pi/180.0;	//convert to radian
		KPoint ptKnobCenter;

		if(m_bDrawRadioButton){
			ptKnobCenter=KPoint((int)(m_ptCenter.x+(nRadius-m_nKnobRadius)*std::sin(dPos)),
					(int)(m_ptCenter.y-(nRadius-m_nKnobRadius) * std::cos(dPos)));
		}
		else{
			ptKnobCenter=KPoint((int)(m_ptCenter.x+(nRadius)*std::sin(dPos)),
					(int)(m_ptCenter.y-(nRadius)*std::cos(dPos)));
		}
		m_ptKnobCenter = ptKnobCenter;

		if(m_bDrawRadioButton){nRadius += 2;}
		else{nRadius -= 2;}

		if(!bDisabled){
			HBRUSH pOldBrush = (HBRUSH)pDC->SelectObject(m_hDialBrush);
			pDC->Ellipse(m_ptCenter.x-nRadius+1,m_ptCenter.y-nRadius+1,
					m_ptCenter.x+nRadius+1,m_ptCenter.y+nRadius+1);
			pDC->SelectObject(pOldBrush);
		}
		DrawCircle(pDC,m_ptCenter,nRadius--,::GetSysColor(COLOR_3DHIGHLIGHT), ::GetSysColor(COLOR_3DDKSHADOW));
		DrawCircle(pDC, m_ptCenter, nRadius--, ::GetSysColor(COLOR_3DLIGHT), ::GetSysColor(COLOR_3DSHADOW));

		// Draw the knob
		int nKnobRadius = m_nKnobRadius;

		if(m_bDrawRadioButton){
			nKnobRadius *= 4;
			nKnobRadius /= 5;
		}

		KRect rcKnob(ptKnobCenter.x - nKnobRadius + 2, ptKnobCenter.y - nKnobRadius + 2,
			       ptKnobCenter.x + nKnobRadius, ptKnobCenter.y + nKnobRadius);

		KRgn rgnKnob;
		rgnKnob.CreateEllipticRgnIndirect(rcKnob);
		if(bDisabled){
			pDC->FillRgn(rgnKnob, ::GetSysColorBrush(COLOR_BTNFACE));
		}else{
			pDC->FillRgn(rgnKnob,m_hKnobBrush);
		}
		//rgnKnob.DeleteObject();

		if(m_bDrawRadioButton||m_bDragging){
			DrawCircle(pDC,ptKnobCenter,--nKnobRadius,::GetSysColor(COLOR_3DDKSHADOW),
					::GetSysColor(COLOR_3DHIGHLIGHT));
			DrawCircle(pDC, ptKnobCenter, --nKnobRadius, ::GetSysColor(COLOR_3DSHADOW),
				        ::GetSysColor(COLOR_3DLIGHT));
		}else{
			DrawCircle(pDC, ptKnobCenter, --nKnobRadius, ::GetSysColor(COLOR_3DHIGHLIGHT),
				       ::GetSysColor(COLOR_3DDKSHADOW));
			DrawCircle(pDC, ptKnobCenter, --nKnobRadius, ::GetSysColor(COLOR_3DLIGHT),
				       ::GetSysColor(COLOR_3DSHADOW));
		}

		// Draw the focus circle on the inside of the knob
		if(!m_bDrawRadioButton && (::GetFocus()==*this)){
			DrawCircle(pDC, ptKnobCenter, nKnobRadius-2, RGB(0, 0, 0), true);
		}

		// Draw the text
		// CString strFormattedText = OnFormatText();
		TCHAR* strText=new TCHAR[_tcslen(m_strText)+16];
		_stprintf(strText,m_strText,GetPos());
		int nStrLen=(int)_tcslen(strText);
		if(nStrLen){
			KFont pOldFont = (HFONT)pDC->SelectObject(m_font);

			KSize szExtent = pDC->GetTextExtent(strText);
			KPoint ptText = KPoint(m_ptCenter.x - szExtent.cx/2, m_ptCenter.y - szExtent.cy/2);
			COLORREF nOldTextColor = pDC->SetTextColor(m_crText);

			pDC->SetBkMode(TRANSPARENT);
			if(bDisabled){
				pDC->DrawState((HBRUSH)NULL,0,(LPARAM)strText,(WPARAM)nStrLen,
						ptText.x,ptText.y,szExtent.cx,szExtent.cy,DST_TEXT|DSS_DISABLED);
			}
			else{
				pDC->TextOut(ptText.x, ptText.y, strText,nStrLen);
			}

			// Clean up
			pDC->SelectObject(pOldFont);
			pDC->SetTextColor(nOldTextColor);
		}
		delete[] strText;
		return 0;
	}
	LRESULT OnLButtonDown(UINT uMsg,WPARAM wParam,LPARAM lParam,bool& bHandled){
		KPoint point(LOWORD(lParam),HIWORD(lParam));

		if(m_bDragByKnobOnly){
			KPoint pt = m_ptKnobCenter - point;
			if((pt.x*pt.x + pt.y*pt.y) > m_nKnobRadius*m_nKnobRadius) return 0; // Ignore it...
		}

		if(!m_bDragging){
			m_bDragging = true;
			//m_bDragChanged = false;
			SetCapture();
			SetFocus();
			if(SetKnob(point)){
				//m_bDragChanged = true;
				PostMessageToParent(TB_THUMBTRACK);
			}
			RedrawWindow();
			//}
		}else
			bHandled=false;
		return 0;
	}
	LRESULT OnLButtonUp(UINT uMsg,WPARAM wParam,LPARAM lParam,bool& bHandled){
		KPoint point(LOWORD(lParam),HIWORD(lParam));

		if(m_bDragging){
			m_bDragging = false;
			::ReleaseCapture();
			if(SetKnob(point)){
				PostMessageToParent(TB_THUMBTRACK);
				//m_bDragChanged = true;
			//}
			//if(m_bDragChanged){
				PostMessageToParent(TB_THUMBPOSITION);
				//m_bDragChanged = false;
			}
			RedrawWindow();
			//}
		}
		else
			bHandled=false;
		return 0;
	}

	LRESULT OnMouseMove(UINT uMsg,WPARAM wParam,LPARAM lParam,bool& bHandled){
		KPoint point(LOWORD(lParam),HIWORD(lParam));
		if(m_bDragging){
			if(SetKnob(point)){
				//m_bDragChanged = true;
				PostMessageToParent(TB_THUMBTRACK);
				RedrawWindow();
			}
		}else
			bHandled=false;
		return 0;
	}
	LRESULT OnKeyDown(UINT uMsg,WPARAM wParam,LPARAM lParam,bool& bHandled){
		int nChar=static_cast<int>(wParam);
		int nMin = GetRangeMin();
		int nMax = GetRangeMax()+1;

		switch(nChar){
			case VK_LEFT:
			case VK_UP:
				{
					int nNewPos = GetPos()-GetLineSize();
					while(nNewPos < nMin) nNewPos += (nMax - nMin);
					SetPos(nNewPos);
					RedrawWindow();
					PostMessageToParent(TB_LINEUP);
				}
				break;
			
			case VK_RIGHT:
			case VK_DOWN:
				{
					int nNewPos = GetPos()+GetLineSize();
					while(nNewPos >= nMax) nNewPos -= (nMax - nMin);
					SetPos(nNewPos);
					RedrawWindow();
					PostMessageToParent(TB_LINEDOWN);
				}
				break;

			case VK_PRIOR:	//page up
				{
					int nNewPos = GetPos()-GetPageSize();
					while(nNewPos < nMin) nNewPos += (nMax - nMin);
					SetPos(nNewPos);
					RedrawWindow();
					PostMessageToParent(TB_PAGEUP);
				}
				break;

			case VK_NEXT:	//page down
				{
					int nNewPos = GetPos()+GetPageSize();
					while(nNewPos >= nMax) nNewPos -= (nMax - nMin);
					SetPos(nNewPos);
					RedrawWindow();
					PostMessageToParent(TB_PAGEDOWN);
				}
				break;

			case VK_HOME:
			case VK_END:
				// Do nothing (ignore keystroke)
				break;

			default:
				bHandled=false;
				break;
		}
		return 0;
	}
	LRESULT OnKeyUp(UINT uMsg,WPARAM wParam,LPARAM lParam,bool& bHandled){
		int nChar=static_cast<int>(wParam);
		switch(nChar){
			case VK_LEFT:
			case VK_UP:
			case VK_RIGHT:
			case VK_DOWN:
			case VK_PRIOR:
			case VK_NEXT:
				PostMessageToParent(TB_ENDTRACK);
				break;

			case VK_HOME:
			case VK_END:
				// Do nothing
				break;

			default:
				bHandled=false;
		}
		return 0;
	}
	LRESULT OnMouseWheel(UINT uMsg,WPARAM wParam,LPARAM lParam,bool& bHandled){
		KPoint pt(LOWORD(lParam),HIWORD(lParam));
		LRESULT lResult=0;

		ScreenToClient(&pt);
		if(ChildWindowFromPoint(pt)==*this){
			lResult=KTrackBarBase<KRoundSlider>::ProcessWindowMessage(WM_MOUSEWHEEL,wParam,lParam);
			RedrawWindow();
		}
		return lResult;
	}

	bool SetKnob(KPoint& pt){
		int nMin = GetRangeMin();
		int nMax = GetRangeMax()+1;

		KSize szDelta = pt - m_ptCenter;
		if(m_bInverted) szDelta.cx *= -1;

		double dNewPos = 0.0;

		if(szDelta.cx != 0){
			dNewPos = 90.0 - std::atan(-(double)szDelta.cy / (double)szDelta.cx) * 180.0 / pi;
		}

		if(((szDelta.cx == 0) && (szDelta.cy >= 0)) || (szDelta.cx < 0)){
			dNewPos += 180.0;
		}

		dNewPos -= m_nZero;

		while(dNewPos < 0.0) dNewPos += 360.0;
		while(dNewPos >= 360.0) dNewPos -= 360.0;

		 int nNewPos = nMin + (int)(dNewPos*(nMax-nMin)/360.0);
		 bool bChanged = (nNewPos != GetPos());

		if(bChanged){SetPos(nNewPos);}

		return bChanged;
	}
	void PostMessageToParent( int nTBCode) {
		
		::PostMessage(GetParent(),WM_HSCROLL, (WPARAM)((GetPos()<<16) | nTBCode), (LPARAM)(m_hWnd));
		
	}
	void SetText(TCHAR* strNewText){SAFE_DEL_PTRS(m_strText);m_strText = strNewText;}

	void SetKnobRadius( int nNewKnobRadius){
		assert(nNewKnobRadius > 4);

		m_nKnobRadius = nNewKnobRadius;
	}

	int GetKnobRadius() {return m_nKnobRadius;}

	void SetDragByKnobOnly( bool bDragByKnobOnly){m_bDragByKnobOnly = bDragByKnobOnly;}

	bool GetDragByKnobOnly() {return m_bDragByKnobOnly;}

	void SetZero( int nZero){
		assert(nZero >= 0 && nZero < 360);
		m_nZero = nZero;
	}

	int GetZero() {return m_nZero;}

	void SetInverted( bool bNewInverted=true){m_bInverted = bNewInverted;}

	bool GetInverted() {return m_bInverted;}

	void SetRadioButtonStyle( bool bNewRadioButtonStyle=true){m_bDrawRadioButton = bNewRadioButtonStyle;}

	bool GetRadioButtonStyle() {return m_bDrawRadioButton;}

	void ReructFont(){
		m_font.DeleteObject();
		m_font.CreateFontIndirect(&m_lf);
	}

	void SetDialColor( COLORREF crBkgnd){
		if(m_hDialBrush) ::DeleteObject(m_hDialBrush);
		m_hDialBrush = ::CreateSolidBrush(crBkgnd);
	}

	void SetKnobColor( COLORREF crBkgnd){
		if(m_hKnobBrush) ::DeleteObject(m_hKnobBrush);
		m_hKnobBrush = ::CreateSolidBrush(crBkgnd);
	}

	void SetTextColor( COLORREF crText){m_crText = crText;}

	void SetFontBold( bool bSet=true){
		m_lf.lfWeight = bSet;
		ReructFont();
	}

	void SetFontUnderline( bool bSet=true){
		m_lf.lfUnderline = bSet;
		ReructFont();
	}

	void SetFontItalic( bool bSet=true){
		m_lf.lfItalic = bSet;
		ReructFont();
	}

	void SetFontSize( int nSize){
		m_lf.lfHeight = -nSize;
		ReructFont();
	}

	void SetFontName( TCHAR* strFont){
		_tcscpy(m_lf.lfFaceName, strFont);
		ReructFont();
	}
protected:
	KPoint		m_ptCenter;
	KPoint		m_ptKnobCenter;
	int		m_nRadius;
	KRgn		m_rgn;

	// 0-359, 0 = Top, 90 = Right, 180 = Bottom, 270 = Left
	int		m_nZero;
	bool		m_bInverted;

	TCHAR*		m_strText;
	int		m_nKnobRadius;

	bool		m_bDragging;
	bool		m_bDragByKnobOnly;
	//bool		m_bDragChanged;
	bool		m_bDrawRadioButton;

	LOGFONT		m_lf;
	KFont		m_font;
	COLORREF	m_crText;
	HBRUSH		m_hKnobBrush;
	HBRUSH		m_hDialBrush;
};

class KGradientProgressBar : public KProgressBarBase<KGradientProgressBar>{
public:
	typedef KProgressBarBase<KGradientProgressBar> __base;
	KGradientProgressBar()
		:m_clrStart(::GetSysColor(COLOR_3DFACE)),m_clrEnd(RGB(128,128,128)),m_clrBk(::GetSysColor(COLOR_3DFACE)),
		m_bForward(true),m_nGradientBarWidth(0),m_nHead(0),m_nTail(0),m_nStepInc(10),m_bFirstRound(true){}
	void Init(){
		m_bFirstRound=true;
		m_bForward=true;
		m_nHead=0;
		m_nTail=0;
	}
	void Clear(){
		m_nHead=0;
		m_nTail=0;
		HDC hDC=::GetDC(m_hWnd);
		RECT rectClient;
		GetClientRect(&rectClient);
		if(rectClient.right!=m_nClientWidth)
			::SetRect(&rectClient,1,1,rectClient.right-1,rectClient.bottom-1);
			
		HBRUSH brush=::CreateSolidBrush(m_clrBk);
		::FillRect(hDC,&rectClient,brush);
		::DeleteObject(brush);
		::ReleaseDC(m_hWnd,hDC);
	}
	void SetStartColor(COLORREF color){m_clrStart=color;}
	void SetEndColor(COLORREF color){m_clrEnd=color;}
	void SetBkColor(COLORREF color){m_clrBk=color;}

	BEGIN_MSG_MAP
		MSG_HANDLER(WM_SIZE,OnSize)
		MSG_HANDLER(PBM_SETSTEP,OnSetStep)
		MSG_HANDLER(PBM_STEPIT,OnStepIt)
		MSG_HANDLER(WM_PAINT,OnPaint)
	END_MSG_MAP(__base)

	void PreSubclassWindow(){
		RECT rectWindow;
		GetWindowRect(&rectWindow);
		m_nClientWidth		=rectWindow.right-rectWindow.left-2;
		m_nGradientBarWidth	=m_nClientWidth/3;	//one third of client width	
	}
	LRESULT OnSize(UINT uMsg,WPARAM wParam,LPARAM lParam,bool& bHandled){
		PreSubclassWindow();
		bHandled=false;
		return 0;
	}
	LRESULT OnSetStep(UINT uMsg,WPARAM wParam,LPARAM lParam,bool& bHandled){
		m_nStepInc=(int)wParam;
		bHandled=false;
		return 0;
	}
	LRESULT OnStepIt(UINT uMsg,WPARAM wParam,LPARAM lParam,bool& bHandled){
		int nLow=0,nUpper=0;
		GetRange(nLow,nUpper);
		int nHeadIncPerStep=m_nStepInc*m_nClientWidth/(nUpper-nLow);
		if((m_nHead>m_nClientWidth) || m_nHead<0)
				m_bForward=!m_bForward;

		if(m_bForward){
			m_nHead+=nHeadIncPerStep;
			if(m_nHead>=m_nGradientBarWidth){
				m_bFirstRound=false;
				m_nTail=m_nHead-m_nGradientBarWidth;
			}
			else{
				if(m_bFirstRound)
					m_nTail=0;
				else
					m_nTail=m_nGradientBarWidth-m_nHead;
			}
		}else{
			m_nHead-=nHeadIncPerStep;
			if((m_nClientWidth-m_nHead)>=m_nGradientBarWidth)
				m_nTail=m_nHead+m_nGradientBarWidth;
			else
				m_nTail=2*m_nClientWidth-m_nHead-m_nGradientBarWidth;
		}
		bHandled=false;
		return 0;
	}
	LRESULT OnPaint(UINT uMsg,WPARAM wParam,LPARAM lParam,bool& bHandled){
		KPaintDC dc(*this);
		RECT rectClient;
		GetClientRect(&rectClient);
		RECT rectWindow;
		GetWindowRect(&rectWindow);
		
		if(rectClient.right==(rectWindow.right-rectWindow.left)){
			HPEN hPen=::CreatePen(PS_SOLID,1,::GetSysColor(COLOR_3DSHADOW));
			HPEN hOldPen=(HPEN)dc.SelectObject(hPen);
			::MoveToEx(dc,0,rectClient.bottom,0);
			::LineTo(dc,0,0);
			::LineTo(dc,rectClient.right-1,0);
			::DeleteObject(hPen);
			hPen=::CreatePen(PS_SOLID,1,::GetSysColor(COLOR_3DHILIGHT));
			dc.SelectObject(hPen);
			::LineTo(dc,rectClient.right-1,rectClient.bottom-1);
			::LineTo(dc,0,rectClient.bottom-1);
			::DeleteObject(hPen);
			::SetRect(&rectClient,1,1,rectClient.right-1,rectClient.bottom-1);
		}
		
		KMemDC memdc(dc,&rectClient);

		HBRUSH brush=::CreateSolidBrush(m_clrBk);
		::FillRect(memdc,&rectClient,brush);
		::DeleteObject(brush);

		DrawGradient(memdc,&rectClient,m_bForward,m_nHead,m_nTail);
		
		return 0;
	}
private:
	void DrawGradient(HDC hDC,RECT* pRectClient,bool bForward,int nHead,int nTail){
		if(!nHead && !nTail)
			return;

		RECT		rectFill;
		HBRUSH		hBrush=0;

		int nRedDiff	=(GetRValue(m_clrEnd)-GetRValue(m_clrStart));
		int nGreenDiff 	=(GetGValue(m_clrEnd)-GetGValue(m_clrStart));
		int nBlueDiff 	=(GetBValue(m_clrEnd)-GetBValue(m_clrStart));
		
		float fRedStep 	=nRedDiff/(float)m_nGradientBarWidth;
		float fGreenStep=nGreenDiff/(float)m_nGradientBarWidth;
		float fBlueStep =nBlueDiff/(float)m_nGradientBarWidth;
	
		int nRedStart 	=GetRValue(m_clrStart);
		int nGreenStart	=GetGValue(m_clrStart);
		int nBlueStart 	=GetBValue(m_clrStart);
		int nRedEnd 	=GetRValue(m_clrEnd);
		int nGreenEnd	=GetGValue(m_clrEnd);
		int nBlueEnd 	=GetBValue(m_clrEnd);

		if(bForward){
			if((nHead-nTail)==m_nGradientBarWidth){
				for(int nBand=nTail; nBand<nHead; nBand++){
					::SetRect(&rectFill,nBand,pRectClient->top,nBand+1,pRectClient->bottom);
					hBrush=::CreateSolidBrush(RGB(nRedStart+fRedStep*(nBand-nTail),nGreenStart+fGreenStep*(nBand-nTail),
								nBlueStart+fBlueStep*(nBand-nTail)));
					::FillRect(hDC,&rectFill,hBrush);
					::DeleteObject(hBrush);
				}
			}else{
				if(nHead<nTail){
					for(int nBand=nTail;nBand>nHead;nBand--){
						::SetRect(&rectFill,nBand-1,pRectClient->top,nBand,pRectClient->bottom);
						hBrush=::CreateSolidBrush(RGB(nRedStart+fRedStep*(nTail-nBand),
									nGreenStart+fGreenStep*(nTail-nBand),nBlueStart+fBlueStep*(nTail-nBand)));
						::FillRect(hDC,&rectFill,hBrush);
						::DeleteObject(hBrush);
					}
				}
				for(int nBand=nHead;nBand>0;nBand--){
					::SetRect(&rectFill,nBand-1,pRectClient->top,nBand,pRectClient->bottom);
					hBrush=::CreateSolidBrush(RGB(nRedEnd-fRedStep*(nHead-nBand),nGreenEnd-fGreenStep*(nHead-nBand),
							nBlueEnd-fBlueStep*(nHead-nBand)));
					::FillRect(hDC,&rectFill,hBrush);
					::DeleteObject(hBrush);
				}
			}
		}else{			//backward
			if((nTail-nHead)==m_nGradientBarWidth){
				for(int nBand=nHead;nBand<nTail;nBand++){
					::SetRect(&rectFill,nBand,pRectClient->top,nBand+1,pRectClient->bottom);
					hBrush=::CreateSolidBrush(RGB(nRedEnd-fRedStep*(nBand-nHead),nGreenEnd-fGreenStep*(nBand-nHead),
							nBlueEnd-fBlueStep*(nBand-nHead)));
					::FillRect(hDC,&rectFill,hBrush);
					::DeleteObject(hBrush);
				}
			}else{
				if(nHead>nTail){
					for(int nBand=nTail; nBand<nHead; nBand++){
						::SetRect(&rectFill,nBand,pRectClient->top,nBand+1,pRectClient->bottom);
						hBrush=::CreateSolidBrush(RGB(nRedStart+fRedStep*(nBand-nTail),
									nGreenStart+fGreenStep*(nBand-nTail),nBlueStart+fBlueStep*(nBand-nTail)));
						::FillRect(hDC,&rectFill,hBrush);
						::DeleteObject(hBrush);
					}
				}
				for(int nBand=nHead; nBand<pRectClient->right; nBand++){
					::SetRect(&rectFill,nBand,pRectClient->top,nBand+1,pRectClient->bottom);
					hBrush=::CreateSolidBrush(RGB(nRedEnd-fRedStep*(nBand-nHead),nGreenEnd-fGreenStep*(nBand-nHead),
							nBlueEnd-fBlueStep*(nBand-nHead)));
					::FillRect(hDC,&rectFill,hBrush);
					::DeleteObject(hBrush);
				}
			}
		}
	}
private:	
	COLORREF	m_clrStart,m_clrEnd,m_clrBk;
	bool		m_bForward;						//true---forward,false---backward
	int			m_nHead,m_nTail;
	int 		m_nGradientBarWidth;
	int 		m_nClientWidth;
	int 		m_nStepInc;
	bool		m_bFirstRound;
};

} //namespace kwinui
#endif //__KXCMNCTRLS_H__
