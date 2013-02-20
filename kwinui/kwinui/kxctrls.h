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
 * custom controls
 * 12/10/2006
 */

#ifndef __KXCTRLS_H__
#define __KXCTRLS_H__

#include "shellapi.h"
#include "kctrls.h"
#include "kgdiobj.h"
#include "ktypes.h"
#include "kutil.h"
#include "commctrl.h"
#include "kdef.h"
#include "kmisc.h"
#include "kdc.h"
#include <cstdlib>

#pragma comment(lib,"Comctl32.lib")
#pragma comment(lib,"Msimg32.lib")
#pragma comment(lib,"shell32.lib")
 
namespace kwinui{

#define SPASI_ANTAR_CELL		1
#define PEAK_DROP_SPEED			1
#define PEAK_HEIGHT				1
#define MAX_PEAK_LIVE_TIME		30	
#define PEAK_TOLERANT			2 // 2 blocks

class KSpectrum : public KStaticBase<KSpectrum>{
public:
	BEGIN_MSG_MAP
		MSG_HANDLER(WM_PAINT,OnPaint)
	END_MSG_MAP(KStaticBase<KSpectrum>)
	
	KSpectrum(COLORREF clrBk=RGB(74,171,74),COLORREF clrBlock=RGB(255,255,255),COLORREF clrPeak=RGB(255,255,255),
				int iArrayCnt=30, int iMaxPeak=20, KSize elementSize=KSize(4,1)){
		m_iArrayCnt 	=iArrayCnt;
		m_iMaxPeak 		=iMaxPeak;
		m_ElemenSize 	=elementSize;

		m_clrBk			=clrBk;
		m_clrBlock		=clrBlock;
		m_clrPeak		=clrPeak;
		m_pArray		=0;
		m_iptrOldPeak	=0;
	}
	~KSpectrum(){
		if(m_iptrOldPeak){
			std::free(m_iptrOldPeak);
			m_iptrOldPeak=0;
		}
	}
	void Init(){
		int iWidth;
		int iHeight;
		KRect rect;

		m_iptrOldPeak = (int*) std::malloc(sizeof(int)*m_iArrayCnt+2);
		memset(m_iptrOldPeak, 0x00, sizeof(int)*m_iArrayCnt+2);
		m_chptrPeakLiveTime = (char*) std::malloc(sizeof(char)*m_iArrayCnt+2);
		memset(m_chptrPeakLiveTime, MAX_PEAK_LIVE_TIME, sizeof(char)*m_iArrayCnt+2);

		//
		iWidth = (m_ElemenSize.cx + SPASI_ANTAR_CELL)*m_iArrayCnt;
		iHeight = (m_ElemenSize.cy + SPASI_ANTAR_CELL)*m_iMaxPeak;
		//iWidth += 3;
		iHeight += 4;
		
		GetClientRect(rect);

		rect.right = rect.left + iWidth;
		rect.bottom = rect.top + iHeight;
		SetWindowPos(NULL,0,0,rect.Width(),rect.Height(),SWP_NOMOVE);
		GetClientRect(m_rcClient);
	}
	
	void PreSubclassWindow(){
		Init();
	}
	
	void DisplayArray(double* pArray){
		m_pArray=pArray;
		RedrawWindow();
	}
	LRESULT OnPaint(UINT uMsg,WPARAM wParam,LPARAM lParam,bool& bHandled){
		KPaintDC dc(*this);
		KMemDC pDC(dc,&m_rcClient);

		if(m_pArray)
			OnDraw(pDC);
		else
			pDC->FillSolidRect(&m_rcClient,m_clrBk);
		return 0;
	}
	void OnDraw(KDC* pMemDC){
		int i;
		int iPeak;
		int iPeakLoop;
		int iVertBlockCnt = 0;
		int iHorzBlockCnt = 0;
		int iMaxValue = 0;
		int iVBlock = 0;
		double dTemp;
		COLORREF clrTemp;

		for(i = 0; i < m_iArrayCnt; i++)
			if( (int)m_pArray[i] > iMaxValue ) 
				iMaxValue = (int)m_pArray[i];
		//
		if( iMaxValue == 0 ) iMaxValue = 1;

		// Clear Screen
		pMemDC->FillSolidRect(&m_rcClient, m_clrBk );

		//
		clrTemp = m_clrBlock;
		m_clrBlock = m_clrPeak; // Set the color of the peaks
#pragma warning(disable:4244)
		for(i = 0; i < m_iArrayCnt; i++)
		{
			dTemp = (double) (m_pArray[i]/ (double) iMaxValue);
			iPeak = (m_iMaxPeak - PEAK_TOLERANT) * dTemp;

			// If the old peak still higher than current
			if( iPeak <= m_iptrOldPeak[i] )
			{
				PeakON(m_iptrOldPeak[i], i,pMemDC);
				if( m_iptrOldPeak[i] > 0) // Make sure it won't get to be negative
				{
					// How long we must retain the old peak
					if( m_chptrPeakLiveTime[i] > 0)
					{
						m_chptrPeakLiveTime[i]--;
					}
					else
					{
						// Timeout! Now drop the peak
						m_iptrOldPeak[i] -= PEAK_DROP_SPEED;
					}
				}
			}
			else
			{
				// Wow, current peak is higher than the old one
				m_iptrOldPeak[i] = iPeak;
				m_chptrPeakLiveTime[i] = MAX_PEAK_LIVE_TIME; // Long live the king
			}

		}

		//
		m_clrBlock = clrTemp;
		for(i = 0; i < m_iArrayCnt; i++)
		{
			dTemp = (double) ((double) m_pArray[i]/ (double) iMaxValue);
			iPeak = (m_iMaxPeak - PEAK_TOLERANT) * dTemp;
			for(iPeakLoop = 0; iPeakLoop < iPeak; iPeakLoop++)
			{
				// Put something like:
				//
				// if( iPeak >= m_iMaxPeak*(2/3) ) m_clrBlock = RGB(....)
				// if( iPeak >= m_iMaxPeak*(1/3) ) m_clrBlock = RGB(....)
				// if( iPeak < m_iMaxPeak*(1/3) ) m_clrBlock = RGB(....)
				//
				// to make it more colorfull
				//
				GridON(iPeakLoop,i,pMemDC);
			}
		}
#pragma warning(default:4244)
	}
private:
	COLORREF	m_clrPeak;
	COLORREF 	m_clrBlock;
	COLORREF 	m_clrBk;
	KRect 		m_rcClient;

	int* 	m_iptrOldPeak;
	char* 	m_chptrPeakLiveTime;
	int 	m_iMaxPeak;
	double*	m_pArray;
	int 	m_iArrayCnt;
	KSize 	m_ElemenSize;

	void GridON(int iThePEak, int iArrayIdx,KDC* pDC){
		int iExactX;
		int iExactY;

		iExactY = (iThePEak)*(m_ElemenSize.cy + SPASI_ANTAR_CELL);
		iExactX = (iArrayIdx)*(m_ElemenSize.cx + SPASI_ANTAR_CELL);

		iExactY = m_rcClient.Height() - iExactY;
		pDC->FillSolidRect(iExactX, iExactY, m_ElemenSize.cx, m_ElemenSize.cy, m_clrBlock );
	}

	void PeakON(int iThePEak, int iArrayIdx,KDC* pDC){
		int iExactX;
		int iExactY;

		iExactY = (iThePEak)*(PEAK_HEIGHT + SPASI_ANTAR_CELL);
		iExactX = (iArrayIdx)*(m_ElemenSize.cx + SPASI_ANTAR_CELL);

		iExactY = m_rcClient.Height() - iExactY;
		pDC->FillSolidRect(iExactX, iExactY, m_ElemenSize.cx, m_ElemenSize.cy, m_clrBlock );
	}
};

class KHyperLink : public KTransparentStaticBase<KHyperLink>{
public:
	typedef KTransparentStaticBase<KHyperLink> __base;

	BEGIN_MSG_MAP
		MSG_HANDLER(WM_MOUSEMOVE,OnMouseMove)
		MSG_HANDLER(WM_SETCURSOR,OnSetCursor)
		MSG_HANDLER(WM_LBUTTONDOWN,OnLButtonDown)
		MSG_HANDLER(WM_LBUTTONUP,OnLButtonUp)
		MSG_HANDLER(WM_SETFOCUS,OnSetFocus)
		MSG_HANDLER(WM_KILLFOCUS,OnKillFocus)
		MSG_HANDLER(WM_KEYUP,OnKeyUp)
		MSG_HANDLER(WM_CAPTURECHANGED,OnCaptureChanged)
		MSG_HANDLER(WM_PAINT,OnPaint)
	END_MSG_MAP(__base)
	
	enum UnderLineOptions{ulHover=-1,ulNone=0,ulAlways=1};
	
	KHyperLink(COLORREF clrNormal=RGB(0,0,255),COLORREF clrHover=RGB(255,0,0),COLORREF clrVisited=RGB(128,0,128))
			:m_clrNormal(clrNormal),m_clrHover(clrHover),m_clrVisited(clrVisited),m_tszLinkURL(0),m_tszLinkText(0),
			m_hFontStd(0),m_hFontUnderline(0),m_nUnderline(ulHover),
			m_bVisited(false),m_bMouseOn(false){
		m_hCurHand=::LoadCursor(NULL,IDC_HAND);
	}
	~KHyperLink(){
		SAFE_DEL_PTRS(m_tszLinkText);
		SAFE_DEL_PTRS(m_tszLinkURL);
		SAFE_DEL_OBJ(m_hFontStd);
		SAFE_DEL_OBJ(m_hFontUnderline);
	}
	void SetLinkURL(TCHAR* tszURL){
		assert(tszURL!=0);
		m_tszLinkURL=new TCHAR[::_tcslen(tszURL)+1];
		::_tcscpy(m_tszLinkURL,tszURL);
	}
	void SetUnderline(int nUnderline){m_nUnderline=nUnderline;}
	void PreSubclassWindow(){
		ModifyStyle(0,SS_NOTIFY);
		//FONT
		LOGFONT	lf;
		HFONT hFont=(HFONT)SendMessage(WM_GETFONT);
		if(!hFont)
			hFont=(HFONT)::GetStockObject(DEFAULT_GUI_FONT);

		::GetObject(hFont,sizeof(lf),&lf);
		m_hFontStd=::CreateFontIndirect(&lf);
		lf.lfUnderline=TRUE;
		m_hFontUnderline=::CreateFontIndirect(&lf);

		int nWindowText=GetWindowTextLength();
		if(!nWindowText){	//if window have not a text
			assert(m_tszLinkURL);
			m_tszLinkText=new TCHAR[::_tcslen(m_tszLinkURL)+1];
			::_tcscpy(m_tszLinkText,m_tszLinkURL);
		}else{
			m_tszLinkText=new TCHAR[nWindowText+1];
			GetWindowText(m_tszLinkText,nWindowText+1);
			if(!m_tszLinkURL)
				SetLinkURL(m_tszLinkText);
		}

		SIZE szTextExt;
		HDC hDC=::GetDC(*this);
		HFONT hFontOld=(HFONT)::SelectObject(hDC,m_hFontStd);
		::GetTextExtentPoint32(hDC,m_tszLinkText,(int)::_tcslen(m_tszLinkText),&szTextExt);
		::SelectObject(hDC,hFontOld);
		::ReleaseDC(*this,hDC);

		szTextExt.cx+=::GetSystemMetrics(SM_CXEDGE)*2;
		szTextExt.cy+=::GetSystemMetrics(SM_CYEDGE)*2;

		SetWindowPos(NULL,0,0,szTextExt.cx,szTextExt.cy,SWP_NOMOVE);		
	}
	LRESULT OnMouseMove(UINT uMsg,WPARAM wParam,LPARAM lParam,bool& bHandled){
		if(!m_bMouseOn){		//enter
			SetCapture();
			m_bMouseOn=true;
			Invalidate();
		}else{					//leave
			POINT pt;
			GetCursorPos(&pt);
			RECT rect;
			GetWindowRect(&rect);
			if(!::PtInRect(&rect,pt)){
				m_bMouseOn=false;
				::ReleaseCapture();
				Invalidate();
			}
		}
		return 0;
	}
	LRESULT OnSetCursor(UINT uMsg,WPARAM wParam,LPARAM lParam,bool& bHandled){
		::SetCursor(m_hCurHand);
		return TRUE;
	}
	LRESULT OnLButtonDown(UINT uMsg,WPARAM wParam,LPARAM lParam,bool& bHandled){
		::SetFocus(*this);
		Invalidate();
		bHandled=false;
		return 0;
	}
	LRESULT OnLButtonUp(UINT uMsg,WPARAM wParam,LPARAM lParam,bool& bHandled){
		if(!m_bVisited)
			m_bVisited=true;
		::ShellExecute(NULL,_T("open"),m_tszLinkURL,NULL,NULL,SW_SHOWNORMAL);	
		return 0;
	}
	LRESULT OnSetFocus(UINT uMsg,WPARAM wParam,LPARAM lParam,bool& bHandled){
		Invalidate();
		return 0;
	}
	LRESULT OnKillFocus(UINT uMsg,WPARAM wParam,LPARAM lParam,bool& bHandled){
		Invalidate();
		return 0;
	}
	LRESULT OnKeyUp(UINT uMsg,WPARAM wParam,LPARAM lParam,bool& bHandled){
		if(wParam==VK_SPACE)
			SendMessage(WM_LBUTTONUP);
		return 0;
	}
	LRESULT OnCaptureChanged(UINT uMsg,WPARAM wParam,LPARAM lParam,bool& bHandled){
		m_bMouseOn=false;
		bHandled=false;
		return 0;
	}
	LRESULT OnPaint(UINT uMsg,WPARAM wParam,LPARAM lParam,bool& bHandled){
		KPaintDC dc(*this);

		COLORREF clrText=m_clrNormal;
		if(m_bVisited)
			clrText=m_clrVisited;
		else if(m_bMouseOn)
			clrText=m_clrHover;

		HFONT hFont=m_hFontStd;
		if(m_nUnderline==ulAlways)
			hFont=m_hFontUnderline;
		else if(m_nUnderline==ulHover)
			if(m_bMouseOn)
				hFont=m_hFontUnderline;

		int nSavedDC=dc.SaveDC();
		dc.SelectObject(hFont);
		dc.SetTextColor(clrText);
		dc.SetBkMode(TRANSPARENT);

		RECT rect;
		GetClientRect(&rect);
		dc.DrawText(m_tszLinkText,&rect);

		if(::GetFocus()==*this)
			dc.DrawFocusRect(&rect);

		dc.RestoreDC(nSavedDC);
		return 0;
	}
private:
	COLORREF	m_clrNormal;
	COLORREF	m_clrHover;
	COLORREF	m_clrVisited;
	
	bool		m_bVisited;

	TCHAR*		m_tszLinkText;
	TCHAR*		m_tszLinkURL;

	HFONT		m_hFontStd;
	HFONT		m_hFontUnderline;
	int			m_nUnderline;	//underline options
	HCURSOR		m_hCurHand;
	bool		m_bMouseOn;
};

class KBmpLink : public KStaticBase<KBmpLink>{
public:
	typedef KStaticBase<KBmpLink> __base;
	
	KBmpLink():m_tszLinkURL(0),m_hCurHand(::LoadCursor(NULL,IDC_HAND)){}

	BEGIN_MSG_MAP
		MSG_HANDLER(WM_LBUTTONUP,OnLButtonUp)
		MSG_HANDLER(WM_SETCURSOR,OnSetCursor)
	END_MSG_MAP(__base)

	void SetLinkURL(TCHAR* tszURL){
		assert(tszURL!=0);
		SAFE_DEL_PTRS(m_tszLinkURL);
		m_tszLinkURL=new TCHAR[::_tcslen(tszURL)+1];
		::_tcscpy(m_tszLinkURL,tszURL);
	}	

	void PreSubclassWindow(){
		ModifyStyle(0,SS_NOTIFY);
	}
	LRESULT OnLButtonUp(UINT uMsg,WPARAM wParam,LPARAM lParam,bool& bHandled){
		::ShellExecute(NULL,_T("open"),m_tszLinkURL,NULL,NULL,SW_SHOWNORMAL);	
		return 0;
	}
	LRESULT OnSetCursor(UINT uMsg,WPARAM wParam,LPARAM lParam,bool& bHandled){
		::SetCursor(m_hCurHand);
		return TRUE;
	}
private:
	TCHAR*	m_tszLinkURL;
	HCURSOR	m_hCurHand;
};
} //namespace kwinui
#endif //__KXCTRLS_H__
