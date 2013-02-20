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
 * misc win controls warp for KWinUI
 * by kujohn 
 * 11/27/2006
 */

#ifndef __KCTRLS_H__
#define __KCTRLS_H__

#include "kwin.h"
#include "kdc.h"
#include "property.h"
#include <cassert>
 
namespace kwinui{

template<typename T,typename TBase=KWindow>
class KStaticBase : public KControlBase<T,TBase>{
public:
	typedef KControlBase<T,TBase> __base;
	HWND Create(HWND hWndParent,LPCTSTR lptszWndName,DWORD dwStyle,RECT& rc,INT_PTR nCtrlID,
			DWORD dwExStyle=0,HINSTANCE hInst=(HINSTANCE)GetModuleHandle(NULL),LPVOID lpParam=NULL){
		assert(m_hWnd==NULL);
		return __base::Create(WC_STATIC,hWndParent,lptszWndName,dwStyle,rc,nCtrlID,dwExStyle,hInst,lpParam);
	}
};

class KStatic : public KStaticBase<KStatic>{
};


template<typename T,typename TBase=KWindow>
class KTransparentStaticBase : public KTransparentControlBase<T,TBase>{
public:
	typedef KTransparentControlBase<T,TBase> __base;
	HWND Create(HWND hWndParent,LPCTSTR lptszWndName,DWORD dwStyle,RECT& rc,INT_PTR nCtrlID,
			DWORD dwExStyle=0,HINSTANCE hInst=(HINSTANCE)GetModuleHandle(NULL),LPVOID lpParam=NULL){
		assert(m_hWnd==NULL);
		return __base::Create(WC_STATIC,hWndParent,lptszWndName,dwStyle,rc,nCtrlID,dwExStyle,hInst,lpParam);
	}
};

class KColorTextBox : public KTransparentStaticBase<KColorTextBox>{
public:
	typedef KTransparentStaticBase<KColorTextBox> __base;
	KColorTextBox(COLORREF crText=RGB(0,0,0),int nPoint=8,TCHAR* tszFaceName=0){
		m_crText=crText;	
		m_nPoint=nPoint;
		if(tszFaceName){
			m_tszFaceName=new TCHAR[::_tcslen(tszFaceName)+1];
			::_tcscpy(m_tszFaceName,tszFaceName);
		}else{
			m_tszFaceName=new TCHAR[7];
			::_tcscpy(m_tszFaceName,_T("Tahoma"));
		}
	}
	~KColorTextBox(){
		SAFE_DEL_PTRS(m_tszFaceName);
		SAFE_DEL_OBJ(m_hFont);
	}
	BEGIN_MSG_MAP
		MSG_HANDLER(WM_PAINT,OnPaint)
	END_MSG_MAP(__base)
	void PreSubclassWindow(){
		HDC hDCTmp=::GetDC(*this);
		m_hFont=::CreateFont(-MulDiv(m_nPoint,::GetDeviceCaps(hDCTmp,LOGPIXELSY),72),
			0,0,0,FW_DONTCARE,FALSE,FALSE,FALSE,DEFAULT_CHARSET,OUT_TT_ONLY_PRECIS,
			CLIP_DEFAULT_PRECIS,PROOF_QUALITY,DEFAULT_PITCH|FF_ROMAN,m_tszFaceName);
		::ReleaseDC(*this,hDCTmp);	
		SetFont(m_hFont);
	}
	LRESULT OnPaint(UINT uMsg,WPARAM wParam,LPARAM lParam,bool& bHandled){
		KPaintDC dc(*this);
		dc.SetBkMode(TRANSPARENT);
		dc.SetTextColor(m_crText);

		RECT rectClient;
		GetClientRect(&rectClient);
		TCHAR* tszCaption=GetWindowText();
		dc.DrawText(tszCaption,&rectClient,DT_CENTER|DT_SINGLELINE|DT_VCENTER);
		SAFE_DEL_PTRS(tszCaption);		
		return 0;
	}
private:
	HFONT		m_hFont;
	COLORREF	m_crText;
	int 		m_nPoint;
	TCHAR*		m_tszFaceName;
};

template<typename T,typename TBase=KWindow,int nButtonStyle=BS_PUSHBUTTON>
class KButtonBase : public KControlBase<T,TBase>{
public:
	typedef KControlBase<T,TBase> __base;
	typedef KButtonBase<T,TBase,nButtonStyle> __self;
	KButtonBase():Check(this,&__self::setCheck,&__self::getCheck){
	}

	HWND Create(HWND hWndParent,LPCTSTR lptszWndName,DWORD dwStyle,RECT& rc,INT_PTR nCtrlID,DWORD dwExStyle=0,
				HINSTANCE hInst=(HINSTANCE)GetModuleHandle(NULL),LPVOID lpParam=NULL){
		assert(m_hWnd==NULL);
		return __base::Create(WC_BUTTON,hWndParent,lptszWndName,dwStyle|nButtonStyle,rc,nCtrlID,dwExStyle,hInst,lpParam);
	}
public:
	__Property<__self,bool,PROPERTY_READ_WRITE>	Check;
private:
	void setCheck(bool bCheck){
		assert(m_hWnd);
		SendMessage(BM_SETCHECK,bCheck?BST_CHECKED:BST_UNCHECKED);
	}
	bool getCheck(){
		assert(m_hWnd);
		return BST_CHECKED==SendMessage(BM_GETCHECK);
	}
};

class KButton : public KButtonBase<KButton>{
};

class KPushButton : public KButtonBase<KPushButton>{
};

//radio button controls
class KAutoRadioButton : public KButtonBase<KAutoRadioButton,KWindow,BS_AUTORADIOBUTTON>{
};	
class KRadioButton : public KButtonBase<KRadioButton,KWindow,BS_RADIOBUTTON>{
};	

class KAutoCheckBox : public KButtonBase<KAutoCheckBox,KWindow,BS_AUTOCHECKBOX>{
};
class KCheckBox : public KButtonBase<KCheckBox,KWindow,BS_CHECKBOX>{
};

template<typename T,typename TBase=KWindow>
class KGroupBoxBase : public KControlBase<T,TBase>{
public:
	typedef KGroupBoxBase<T,TBase> __self;
	typedef KControlBase<T,TBase> __base;
	typedef bool (__self::*GROUPCONTROLACTIONFUNC)(HWND hWnd,LPARAM lParam);

	KGroupBoxBase():m_bAllowOverlap(false),m_bUseTabOrder(false),m_bIgnoreControls(false){}

	HWND Create(HWND hWndParent,LPCTSTR lptszWndName,DWORD dwStyle,RECT& rc,INT_PTR nCtrlID,DWORD dwExStyle=0,
				HINSTANCE hInst=(HINSTANCE)GetModuleHandle(NULL),LPVOID lpParam=NULL){
		assert(m_hWnd==NULL)
		return __base::Create(WC_BUTTON,hWndParent,lptszWndName,dwStyle|BS_GROUPBOX,rc,nCtrlID,dwExStyle,hInst,lpParam);
	}

	BEGIN_MSG_MAP
		MSG_HANDLER(WM_SHOWWINDOW,OnShowWindow)
		MSG_HANDLER(WM_ENABLE,OnEnable)
		MSG_HANDLER(WM_WINDOWPOSCHANGING,OnWindowPosChanging)
	END_MSG_MAP(__base)

	void SetUseTabOrder(bool bUseTabOrder=true){m_bUseTabOrder=bUseTabOrder;}
	void SetAllowOverlap(bool bAllowOverlap=true){m_bAllowOverlap=bAllowOverlap;}
	void IgnoreControls(bool bIgnore=true){m_bIgnoreControls=bIgnore; }

	LRESULT OnShowWindow(UINT uMsg,WPARAM wParam,LPARAM lParam,bool& bHandled){
  		if(!m_bIgnoreControls)
    		DoGroupControlAction(&__self::GroupControlActionFunc_Show,wParam);
		bHandled=false;
		return 0;
	}

	LRESULT OnEnable(UINT uMsg,WPARAM wParam,LPARAM lParam,bool& bHandled){
  		if (!m_bIgnoreControls)
    		DoGroupControlAction(&__self::GroupControlActionFunc_Enable, wParam);
		Invalidate();
		return 0;
	}

	LRESULT OnWindowPosChanging(UINT uMsg,WPARAM wParam,LPARAM lParam,bool& bHandled){
		bHandled=false;
  		if (m_bIgnoreControls)
    		return 0;
		WINDOWPOS* lpwndpos=(WINDOWPOS*)(lParam);
		if(lpwndpos->flags & SWP_NOMOVE)
			return 0;

		// see if we've moved x or y, and 
		// move group controls with group box
		RECT rcGroup;
		GetWindowRect(&rcGroup);
		::ScreenToClient(GetParent(),(POINT*)&rcGroup);
		::ScreenToClient(GetParent(),((POINT*)&rcGroup)+1);

		int nDeltaX = lpwndpos->x - rcGroup.left;
		int nDeltaY = lpwndpos->y - rcGroup.top;

		if(nDeltaX == 0 && nDeltaY == 0)
			return 0;

		DoGroupControlAction(&__self::GroupControlActionFunc_Move, MAKELPARAM(nDeltaX, nDeltaY));
		Invalidate();
		return 0;
	}
	bool IsInGroup(HWND hWnd, bool& bOverlapped){
		bool bIsInGroup=false;
		if (!hWnd)
			return false;
		RECT rcGroup, rcCtrl;
		GetWindowRect(&rcGroup);
		::GetWindowRect(hWnd,&rcCtrl);
		
		POINT ptCtrlTopLeft,ptCtrlBottomRight;
		ptCtrlTopLeft.x		=rcCtrl.left,	ptCtrlTopLeft.y		=rcCtrl.top;
		ptCtrlBottomRight.x	=rcCtrl.right,	ptCtrlBottomRight.y	=rcCtrl.bottom;

		if (::PtInRect(&rcGroup,ptCtrlTopLeft) && ::PtInRect(&rcGroup,ptCtrlBottomRight))
			bIsInGroup = true;
		if (!bIsInGroup && (::PtInRect(&rcGroup,ptCtrlTopLeft) || ::PtInRect(&rcGroup,ptCtrlBottomRight)))
			bOverlapped = bIsInGroup = true;
		return bIsInGroup;
	}
	bool DoGroupControlAction(GROUPCONTROLACTIONFUNC pfnGCAF, LPARAM lParam=0){
		if(!pfnGCAF)
    		return false;

 		if(m_bIgnoreControls)
			return false;

	  	// don't do anything if we're not a groupbox
	  	if (!((GetStyle() & 0xF) & BS_GROUPBOX))
			return false;

	  	// go through siblings, and see if they lie within the boundary
	  	// of this group control
	  	HWND hWndCtrl=0;
	  	if(m_bUseTabOrder)
			hWndCtrl=::GetNextWindow(hWndCtrl,GW_HWNDNEXT);
	  	else
			hWndCtrl=::GetWindow(GetParent(),GW_CHILD);
		while(hWndCtrl){
			if(hWndCtrl!=m_hWnd){
				bool bOverlapped=false;
			  	bool bIsInGroup=IsInGroup(hWndCtrl, bOverlapped);
			  	if(bIsInGroup && (m_bAllowOverlap || !bOverlapped)){
					if (!(this->*pfnGCAF)(hWndCtrl, lParam))
			  			return false;
			  	}
			  	else if(!bOverlapped && m_bUseTabOrder)  // found out side of group, so ditch out
					break;
			}
			hWndCtrl=::GetNextWindow(hWndCtrl,GW_HWNDNEXT);
		}
		return true;
	}
	bool GroupControlActionFunc_Show(HWND hWndCtrl, LPARAM lParam){
		if (!hWndCtrl)
			return true;
		bool bShow = 0!=lParam;
		::ShowWindow(hWndCtrl,bShow ? SW_SHOW : SW_HIDE);
		return true;
	}
	bool GroupControlActionFunc_Enable(HWND hWndCtrl, LPARAM lParam){
  		if(!hWndCtrl)
    		return true;
  		bool bEnable =0!=lParam;
  		::EnableWindow(hWndCtrl,bEnable);
		::InvalidateRect(hWndCtrl,NULL,TRUE);
  		return true;
	}
	bool GroupControlActionFunc_Move(HWND hWndCtrl, LPARAM lParam){
		if (!hWndCtrl)
			return true;
		short nDeltaX = LOWORD(lParam);
		short nDeltaY = HIWORD(lParam);
		RECT rc;
		::GetWindowRect(hWndCtrl,&rc);
		rc.left+=nDeltaX,	rc.top+=nDeltaY;
		::ScreenToClient(::GetParent(hWndCtrl),(POINT*)&rc);
		::ScreenToClient(::GetParent(hWndCtrl),((POINT*)&rc)+1);
		::MoveWindow(hWndCtrl,rc.left,rc.top,rc.right-rc.left,rc.bottom-rc.top,TRUE);
		return true;
	}
private:
	bool m_bAllowOverlap;
	bool m_bUseTabOrder;
	bool m_bIgnoreControls;
};
class KGroupBox : public KGroupBoxBase<KGroupBox>{
};	

//ComboBox controls
template<typename T,typename TBase=KWindow>
class KComboBoxBase : public KControlBase<T,TBase>{
public:
	typedef KControlBase<T,TBase> __base;
	HWND Create(HWND hWndParent,LPCTSTR lptszWndName,DWORD dwStyle,RECT& rc,INT_PTR nCtrlID,DWORD dwExStyle=0,
				HINSTANCE hInst=(HINSTANCE)GetModuleHandle(NULL),LPVOID lpParam=NULL){
		assert(m_hWnd==NULL)
		return __base::Create(WC_COMBOBOX,hWndParent,lptszWndName,dwStyle,rc,nCtrlID,dwExStyle,hInst,lpParam);	
	}
	int AddString(TCHAR* tszString){
		assert(m_hWnd!=NULL && tszString!=NULL);
		return (int)::SendMessage(m_hWnd,CB_ADDSTRING,0,(LPARAM)tszString);
	}
	int SelectString(TCHAR* tszString){
		assert(m_hWnd!=NULL && tszString!=NULL);
		return (int)::SendMessage(m_hWnd,CB_SELECTSTRING,(WPARAM)-1,(LPARAM)tszString);
	}
};
class KComboBox : public KComboBoxBase<KComboBox>{
};

//ListBox control
template <typename T,typename TBase=KWindow> 
class KListBoxBase : public KControlBase<T,TBase>{
public:
	typedef KControlBase<T,TBase> __base;
	HWND Create(HWND hWndParent,LPCTSTR lptszWndName,DWORD dwStyle,RECT& rc,INT_PTR nCtrlID,DWORD dwExStyle=0,
				HINSTANCE hInst=(HINSTANCE)GetModuleHandle(NULL),LPVOID lpParam=NULL){
		assert(m_hWnd==NULL)
		return __base::Create(WC_LISTBOX,hWndParent,lptszWndName,dwStyle,rc,nCtrlID,dwExStyle,hInst,lpParam);	
	}
	int AddString(TCHAR* tszString){
		assert(m_hWnd!=NULL && tszString!=NULL);
		return (int)::SendMessage(m_hWnd,LB_ADDSTRING,0,(LPARAM)tszString);
	}
	int SelectString(TCHAR* tszString){
		assert(m_hWnd!=NULL && tszString!=NULL);
		return (int)::SendMessage(m_hWnd,LB_SELECTSTRING,(WPARAM)-1,(LPARAM)tszString);
	}
	bool SetItemData(int nIndex,int nData){
		LRESULT lResult=::SendMessage(m_hWnd,LB_SETITEMDATA,(WPARAM)nIndex,(LPARAM)nData);
		if(lResult & LB_ERR)
			return false;
		return true;
	}
	int GetItemData(int nIndex){
		return (int)::SendMessage(m_hWnd,LB_GETITEMDATA,(WPARAM)nIndex,0);
	}
	bool SetCurSel(int nIndex){
		LRESULT lResult=::SendMessage(m_hWnd,LB_SETCURSEL,(WPARAM)nIndex,0);
		if(lResult & LB_ERR)
			return false;
		return true;
	}
	int GetCurSel(void){
		return (int)::SendMessage(m_hWnd,LB_GETCURSEL,0,0);
	}
	void ResetContent(void){
		::SendMessage(m_hWnd,LB_RESETCONTENT,0,0);
	}
	void SetHorizontalExtent(int nPixels){
		::SendMessage(m_hWnd,LB_SETHORIZONTALEXTENT,(WPARAM)nPixels,0);
	}
	int GetCount(void){
		return (int)::SendMessage(m_hWnd,LB_GETCOUNT,0,0);
	}
	int GetTextLen(int nIndex){
		return (int)::SendMessage(m_hWnd,LB_GETTEXTLEN,(WPARAM)nIndex,0);
	}
	int GetText(int nIndex,TCHAR* tszTextBuf){
		return (int)::SendMessage(m_hWnd,LB_GETTEXT,(WPARAM)nIndex,(LPARAM)tszTextBuf);		
	}
	void SetHScroll(void){
		int	nTextLen=0;
		SIZE	size;
		int	dx=0;
		HDC	hDC	=GetDC(m_hWnd);
		HFONT	hFont	=GetFont();
		HFONT	hOldFont=(HFONT)SelectObject(hDC,hFont);
		TCHAR*  tszBuf=0;
		TEXTMETRIC   tm;
		GetTextMetrics(hDC,&tm);
		for(int i=0;i<GetCount();i++){
			nTextLen=GetTextLen(i);
			tszBuf=new TCHAR[nTextLen+1];
			tszBuf[nTextLen]=_T('\0');
			GetText(i,tszBuf);
			GetTextExtentPoint32(hDC,tszBuf,nTextLen,&size);
			size.cx+=tm.tmAveCharWidth;
			if(size.cx > dx)
				dx=size.cx;

			delete tszBuf;
		}
		SelectObject(hDC,hOldFont);
		ReleaseDC(m_hWnd,hDC);
		SetHorizontalExtent(dx);
	}
};
class KListBox : public KListBoxBase<KListBox>{
};

template<typename T,typename TBase=KWindow>
class KEditBase : public KControlBase<T,TBase>{
public:
	typedef KControlBase<T,TBase> __base;
	HWND Create(HWND hWndParent,LPCTSTR lptszWndName,DWORD dwStyle,RECT& rc,INT_PTR nCtrlID,
				DWORD dwExStyle=0,HINSTANCE hInst=(HINSTANCE)GetModuleHandle(NULL),LPVOID lpParam=NULL){
		assert(m_hWnd==NULL);
		return __base::Create(WC_EDIT,hWndParent,lptszWndName,dwStyle,rc,nCtrlID,dwExStyle,hInst,lpParam);
	}
};

class KEdit : public KEditBase<KEdit>{
};
} //namespace kwinui
#endif //__KCTRLS_H__
