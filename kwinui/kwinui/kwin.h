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
 * modify thunk code for assign hwnd from stack to member m_hWnd
 * add message map support 
 * add function ProcessWindowMessage for message map handle
 * add function PreRegisterClassEx give derived class a chance to adjust class info before create window
 * add class KControlBase for subclass and superclass support of controls
 * delete some redundant code
 * tweak some code 
 * by proguru (changuoqiang#gmail.com)
 * 11/24/2006 
 */

/*
 * add notify message reflect
 * 12/08/2006
 */

/*	
 *	This GUI FrameWork is named 'KWinUI'.
 */

#pragma warning(disable:4996)
#ifndef __KWIN_H__
#define __KWIN_H__

#define PRETRANSLATEMESSAGE

#include "kdef.h"
#include "kthunk.h"
#include <CommCtrl.h>
#include <tchar.h>
#include <cassert>
 
namespace kwinui{
//for notify message reflect
struct MSGREFLECTSTRUCT{
	UINT 	uMsg;
	WPARAM 	wParam;
	LPARAM 	lParam;
	bool 	bHandled;
};

#define WM_EX_IDLE			(WM_APP+10000)
#define WM_EX_MSGREFLECT	(WM_APP+10001)
#define WM_EX_MOUSEENTER	(WM_APP+10002)
#define WM_EX_MOUSELEAVE	(WM_APP+10003)
#define WM_EX_LANGCHANGE	(WM_APP+10004)

#define BEGIN_MSG_MAP 	\
	LRESULT ProcessWindowMessage(UINT uMsg,WPARAM wParam,LPARAM lParam){	\
	bool 	bHandled=true;	\
	LRESULT	lResult=0;

#define REFLECTED_COMMAND_HANDLER(id,code,func)	\
	if(uMsg==WM_EX_MSGREFLECT){	\
		MSGREFLECTSTRUCT*	pMrs = (MSGREFLECTSTRUCT*)wParam; \
		if(pMrs->uMsg==WM_COMMAND && (id==LOWORD(pMrs->wParam) && code==HIWORD(pMrs->wParam))){	\
			bool bHandledReflect=false;		\
			lResult=func(LOWORD(pMrs->wParam),HIWORD(pMrs->wParam),(HWND)pMrs->lParam,bHandledReflect);	\
			if(bHandledReflect){	\
				pMrs->bHandled=true;	\
				return lResult;}}}	

#define REFLECTED_COMMAND_ID_HANDLER(id,func)		\
	if(uMsg==WM_EX_MSGREFLECT){	\
		MSGREFLECTSTRUCT*	pMrs = (MSGREFLECTSTRUCT*)wParam; \
		if(pMrs->uMsg==WM_COMMAND && (id==LOWORD(pMrs->wParam))){	\
			bool bHandledReflect=false;		\
			lResult=func(LOWORD(pMrs->wParam),HIWORD(pMrs->wParam),(HWND)pMrs->lParam,bHandledReflect);	\
			if(bHandledReflect){	\
				pMrs->bHandled=true;	\
				return lResult;}}}

#define REFLECTED_COMMAND_CODE_HANDLER(code,func)		\
	if(uMsg==WM_EX_MSGREFLECT){	\
		MSGREFLECTSTRUCT*	pMrs = (MSGREFLECTSTRUCT*)wParam; \
		if(pMrs->uMsg==WM_COMMAND && (code==HIWORD(pMrs->wParam))){	\
			bool bHandledReflect=false;		\
			lResult=func(LOWORD(pMrs->wParam),HIWORD(pMrs->wParam),(HWND)pMrs->lParam,bHandledReflect);	\
			if(bHandledReflect){	\
				pMrs->bHandled=true;	\
				return lResult;}}}

#define REFLECTED_NOTIFY_HANDLER(id,notifycode,func)	\
	if(uMsg==WM_EX_MSGREFLECT){	\
		MSGREFLECTSTRUCT*	pMrs = (MSGREFLECTSTRUCT*)wParam; \
		if(pMrs->uMsg==WM_NOTIFY&&(id==((LPNMHDR)pMrs->lParam)->idFrom)&&(notifycode==((LPNMHDR)pMrs->lParam)->code)){	\
			bool bHandledReflect=false;		\
			lResult=func(((LPNMHDR)pMrs->lParam)->idFrom,(LPNMHDR)pMrs->lParam,bHandledReflect);	\
			if(bHandledReflect){	\
				pMrs->bHandled=true;	\
				return lResult;}}}

#define REFLECTED_NOTIFY_ID_HANDLER(id,func)	\
	if(uMsg==WM_EX_MSGREFLECT){	\
		MSGREFLECTSTRUCT*	pMrs = (MSGREFLECTSTRUCT*)wParam; \
		if(pMrs->uMsg==WM_NOTIFY && (id==((LPNMHDR)pMrs->lParam)->idFrom)){	\
			bool bHandledReflect=false;		\
			lResult=func(((LPNMHDR)pMrs->lParam)->idFrom,(LPNMHDR)pMrs->lParam,bHandledReflect);	\
			if(bHandledReflect)	\
				pMrs->bHandled=true;	\
				return lResult;}}}

#define REFLECTED_NOTIFY_CODE_HANDLER(notifycode,func)  \
	if(uMsg==WM_EX_MSGREFLECT){	\
		MSGREFLECTSTRUCT*	pMrs = (MSGREFLECTSTRUCT*)wParam; \
		if(pMrs->uMsg==WM_NOTIFY && notifycode==((LPNMHDR)pMrs->lParam)->code){  \
			bool bHandledReflect=false;		\
			lResult=func((int)((LPNMHDR)pMrs->lParam)->idFrom,(LPNMHDR)pMrs->lParam,bHandledReflect);  \
			if(bHandledReflect){  \
				pMrs->bHandled=true;	\
				return lResult;}}}

#define MSG_HANDLER(msg,func)	\
	if(uMsg==msg){	\
		lResult=func(msg,wParam,lParam,bHandled);	\
		if(bHandled)	\
			return lResult;}

#define REFLECT_MSG_HANDLER(msg,func)	\
	if(uMsg==WM_EX_MSGREFLECT){	\
		MSGREFLECTSTRUCT*	pMrs = (MSGREFLECTSTRUCT*)wParam; \
		if(pMrs->uMsg==msg){	\
			bool bHandledReflect=false;		\
			lResult=func(msg,pMrs->wParam,pMrs->lParam,bHandledReflect);	\
			if(bHandledReflect){  \
				pMrs->bHandled=true;	\
				return lResult;}}}

#define COMMAND_HANDLER(id,code,func)	\
	if(uMsg==WM_COMMAND && (id==LOWORD(wParam) && code==HIWORD(wParam))){	\
		lResult=func(LOWORD(wParam),HIWORD(wParam),(HWND)lParam,bHandled);\
		if(bHandled)	\
			return lResult;}	

#define COMMAND_ID_HANDLER(id,func)		\
	if(uMsg==WM_COMMAND && (id==LOWORD(wParam))){	\
		lResult=func(LOWORD(wParam),HIWORD(wParam),(HWND)lParam,bHandled);	\
		if(bHandled)	\
			return lResult;}

#define COMMAND_ID_RANGE_HANDLER(id_first,id_last,func)	\
	if(uMsg==WM_COMMAND && ((LOWORD(wParam))>=id_first || (LOWORD(wParam))<=id_last)){	\
		lResult=func(LOWORD(wParam),HIWORD(wParam),(HWND)lParam,bHandled);	\
		if(bHandled)	\
			return lResult;}

#define COMMAND_CODE_HANDLER(code,func)		\
	if(uMsg==WM_COMMAND && (code==HIWORD(wParam))){	\
		lResult=func(LOWORD(wParam),HIWORD(wParam),(HWND)lParam,bHandled);	\
		if(bHandled)	\
			return lResult;}

#define NOTIFY_HANDLER(id,notifycode,func)	\
	if(uMsg==WM_NOTIFY && (id==((LPNMHDR)lParam)->idFrom) && (notifycode==((LPNMHDR)lParam)->code)){	\
		lResult=func(((LPNMHDR)lParam)->idFrom,(LPNMHDR)lParam,bHandled);	\
		if(bHandled)	\
			return lResult;}

#define NOTIFY_ID_HANDLER(id,func)	\
	if(uMsg==WM_NOTIFY && (id==((LPNMHDR)lParam)->idFrom)){	\
		lResult=func(((LPNMHDR)lParam)->idFrom,(LPNMHDR)lParam,bHandled);	\
		if(bHandled)	\
			return lResult;}	

#define NOTIFY_CODE_HANDLER(notifycode,func)	\
	if(uMsg==WM_NOTIFY && (notifycode==((LPNMHDR)lParam)->code)){	\
		lResult=func((int)((LPNMHDR)lParam)->idFrom,(LPNMHDR)lParam,bHandled);	\
		if(bHandled)	\
			return lResult;}	

#define REFLECT_NOTIFICATIONS()	\
	lResult=ReflectNotifications(uMsg,wParam,lParam,bHandled); \
	if(bHandled)		\
		return lResult;

#define END_MSG_MAP(baseClass)	\
	return baseClass::ProcessWindowMessage(uMsg,wParam,lParam);}

class KWindow{
public:
	KWindow():m_hWnd(NULL),m_hInst(::GetModuleHandle(NULL)){}

    inline operator HWND(){return m_hWnd;}

 	bool Attach(HWND hWnd){		//no subclass,provide convenient operation for windows only
		assert(m_hWnd==NULL && hWnd!=NULL);
		m_hWnd=hWnd;
		return true;
	}		
	HWND Detach(){
		assert(m_hWnd);
		HWND hWnd=m_hWnd;
		m_hWnd=0;
		return hWnd;
	}

	//window helper function
	bool Enable(bool bEnable){return (0!=::EnableWindow(m_hWnd,(BOOL)bEnable));}
	bool Enable(void){return (0!=::IsWindowEnabled(m_hWnd));}
	bool EnableWindow(bool bEnable){return 0==::EnableWindow(m_hWnd,bEnable);}
	bool IsWindowEnabled(void){return (0!=::IsWindowEnabled(m_hWnd));}
	bool EnableDlgItem(int nCtrlID,bool bEnable){return 0!=::EnableWindow(GetDlgItem(nCtrlID),bEnable);}
	
	int GetWindowTextLength(void){return ::GetWindowTextLength(m_hWnd);}
	int GetWindowText(__out TCHAR* tszText,int nMax){
		assert(m_hWnd!=NULL);
		return ::GetWindowText(m_hWnd,tszText,nMax);
	}
	TCHAR* GetWindowText(__out TCHAR* tszText){
		assert(m_hWnd);
		if(::GetWindowText(m_hWnd,tszText,GetWindowTextLength()+1))
			return tszText;
		return 0;
	}
	TCHAR* GetWindowText(){	//caller response for release the resource returned
		assert(m_hWnd);
		int nTextLen=GetWindowTextLength()+1;
		if(nTextLen>1){
			TCHAR* tszText=new TCHAR[nTextLen];
			if(tszText && ::GetWindowText(m_hWnd,tszText,nTextLen))
				return tszText;
		}
		return 0;
	}
	int SetWindowText(__in TCHAR* tszText){
		assert(m_hWnd!=NULL);
		return ::SetWindowText(m_hWnd,tszText);
	}

	HFONT GetFont(void){return (HFONT)SendMessage(WM_GETFONT);}
	void SetFont(HFONT hFont){
		SendMessage(WM_SETFONT,(WPARAM)hFont,(LPARAM)TRUE);
	}
	bool ModifyStyle(int nRemove,int nAdd){
		return KModifyStyle(GWL_STYLE,nRemove,nAdd);
	}
	bool ModifyStyleEx(int nRemove,int nAdd){
		return KModifyStyle(GWL_EXSTYLE,nRemove,nAdd);
	}
	bool KModifyStyle(int nStyleOffset,int nRemove,int nAdd){
		assert(m_hWnd != NULL);
		LONG_PTR lStyle = ::GetWindowLongPtr(m_hWnd, nStyleOffset);
		LONG_PTR lNewStyle = (lStyle & ~nRemove)|nAdd;
		if (lStyle == lNewStyle)
			return true;

		return 0!=::SetWindowLongPtr(m_hWnd, nStyleOffset, lNewStyle);
	}
	int SetWindowRgn(HRGN hRgn,bool bRedraw=true){
		return ::SetWindowRgn(m_hWnd,hRgn,bRedraw?TRUE:FALSE);
	}
	HWND SetCapture(void){return ::SetCapture(m_hWnd);}

	bool ReleaseCapture(void){return (0!=::ReleaseCapture());}
	bool RedrawWindow(const RECT* pRectUpdate=NULL,HRGN hRgnUpdate=NULL,
		UINT flags=RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE){
		return (0!=::RedrawWindow(m_hWnd,pRectUpdate,hRgnUpdate,flags));
	}
	
	int GetWindowRgn(HRGN hRgn){return ::GetWindowRgn(m_hWnd,hRgn);}
	bool GetClientRect(RECT* pRect){return (0!=::GetClientRect(m_hWnd,pRect));} 
	bool GetWindowRect(RECT* pRect){return 0!=::GetWindowRect(m_hWnd,pRect);}

	HWND GetParent(void){return ::GetParent(m_hWnd);}

	HWND GetDlgItem(int nCtrlID){return ::GetDlgItem(m_hWnd,nCtrlID);}
	UINT GetDlgItemInt(int nIDDlgItem,bool* pTranslated,bool bSigned){
		BOOL bTranslated;
		UINT nRet=::GetDlgItemInt(m_hWnd,nIDDlgItem,&bTranslated,bSigned?TRUE:FALSE);
		*pTranslated=bTranslated?true:false;
		return nRet;
	}
	bool SetDlgItemInt(int nIDDlgItem,UINT uValue,bool bSigned=false){
		return 0!=::SetDlgItemInt(m_hWnd,nIDDlgItem,uValue,bSigned?TRUE:FALSE);
	}
	UINT GetDlgItemText(int nIDDlgItem,__out TCHAR* tszText,int nMaxCount){
		return ::GetDlgItemText(m_hWnd,nIDDlgItem,tszText,nMaxCount);
	}
	bool SetDlgItemText(int nIDDlgItem,TCHAR* tszText){return 0!=::SetDlgItemText(m_hWnd,nIDDlgItem,tszText);}
	int GetDlgCtrlID(){return ::GetDlgCtrlID(m_hWnd);}

	HDC GetDC(void){return ::GetDC(m_hWnd);}
	HDC GetWindowDC(void){return ::GetWindowDC(m_hWnd);}
	int ReleaseDC(HDC hDC){return ::ReleaseDC(m_hWnd,hDC);}
	
	bool InvalidateRect(CONST RECT* lpRect,bool bErase=true){return 0!=::InvalidateRect(m_hWnd,lpRect,bErase);}
	bool Invalidate(bool bErase=true){return 0!=::InvalidateRect(m_hWnd,NULL,bErase);}

	bool ClientToScreen(POINT* pPoint){return 0!=::ClientToScreen(m_hWnd,pPoint);}
	bool ClientToScreen(RECT* pRect){
		if(!::ClientToScreen(m_hWnd,(POINT*)pRect))
			return false;
		return 0!=::ClientToScreen(m_hWnd,((POINT*)pRect)+1);
	}
	bool ScreenToClient(POINT* pPoint){return 0!=::ScreenToClient(m_hWnd,pPoint);}
	bool ScreenToClient(RECT* pRect){
		if(!::ScreenToClient(m_hWnd,(POINT*)pRect))
			return false;
		return 0!=::ScreenToClient(m_hWnd,((POINT*)pRect)+1);
	}
	bool MoveWindow(int x,int y,int nWidth,int nHeight,bool bRepaint=true){
		return 0!=::MoveWindow(m_hWnd,x,y,nWidth,nHeight,bRepaint);}
	bool MoveWindow(RECT* pRect,bool bRepaint=true){
		return MoveWindow(pRect->left,pRect->top,pRect->right-pRect->left,pRect->bottom-pRect->top,bRepaint);
	}
	bool SetWindowPos(HWND hWndInsertAfter,int x,int y,int nWidth,int nHeight,UINT uFlags){
		return 0!=::SetWindowPos(m_hWnd,hWndInsertAfter,x,y,nWidth,nHeight,uFlags);
	}

	LONG_PTR GetWindowLongPtr(int nIndex){return ::GetWindowLongPtr(m_hWnd,nIndex);}
	LONG_PTR SetWindowLongPtr(int nIndex,LONG_PTR dwNewLong){return ::SetWindowLongPtr(m_hWnd,nIndex,dwNewLong);}

	LONG_PTR GetStyle(){return GetWindowLongPtr(GWL_STYLE);}
	LONG_PTR GetStyleEx(){return GetWindowLongPtr(GWL_EXSTYLE);}

	bool CentralizeWindow(){
		RECT rect;
		GetWindowRect(&rect);
		int width=rect.right-rect.left;
		int height=rect.bottom-rect.top;
		int xStart=(::GetSystemMetrics(SM_CXSCREEN)-width)/2;
		int yStart=(::GetSystemMetrics(SM_CYSCREEN)-height)/2;

		return MoveWindow(xStart,yStart,width,height,false);	
	}	
	bool ShowWindow(int nCmdShow){return 0!=::ShowWindow(m_hWnd,nCmdShow);}
	bool UpdateWindow(){return 0!=::UpdateWindow(m_hWnd);}
	HWND SetFocus(){return ::SetFocus(m_hWnd);}
	HWND ChildWindowFromPoint(POINT pt){return ::ChildWindowFromPoint(m_hWnd,pt);}

	bool PostMessage(UINT uMsg,WPARAM wParam=0,LPARAM lParam=0){return 0!=::PostMessage(m_hWnd,uMsg,wParam,lParam);}
	LRESULT SendMessage(UINT uMsg,WPARAM wParam=0,LPARAM lParam=0){return ::SendMessage(m_hWnd,uMsg,wParam,lParam);}
	LRESULT SendDlgItemMessage(int nIDDlgItem,UINT uMsg,WPARAM wParam=0,LPARAM lParam=0){
		return ::SendDlgItemMessage(m_hWnd,nIDDlgItem,uMsg,wParam,lParam);
	}
	LRESULT PostDlgItemMessage(int nIDDlgItem,UINT uMsg,WPARAM wParam=0,LPARAM lParam=0){
		return ::PostMessage(GetDlgItem(nIDDlgItem),uMsg,wParam,lParam);
	}

	int MessageBox(LPCTSTR lpText,LPCTSTR lpCaption,UINT uType=MB_OK){
		return ::MessageBox(m_hWnd,lpText,lpCaption,uType);
	}
	void ShowMessage(TCHAR* tszText){
		TCHAR* tszTitle=GetWindowText();
		if(tszTitle)
			MessageBox(tszText,tszTitle);
		else
			MessageBox(tszText,_T(""));
	}
	UINT_PTR SetTimer(UINT_PTR nIDEvent,UINT uElapse,TIMERPROC lpTimerFunc=NULL){
		return ::SetTimer(m_hWnd,nIDEvent,uElapse,lpTimerFunc);
	}
	bool KillTimer(UINT_PTR uIDEvent){return 0!=::KillTimer(m_hWnd,uIDEvent);}
	//
	bool SetLayeredWindowAttributes(COLORREF crKey=(COLORREF)0,BYTE bAlpha=128,DWORD dwFlags=LWA_ALPHA){
		return 0!=::SetLayeredWindowAttributes(m_hWnd,crKey,bAlpha,dwFlags);
	}
	bool AnimateWindow(DWORD dwTime,DWORD dwFlags){return 0!=::AnimateWindow(m_hWnd,dwTime,dwFlags);}
	bool IsZoomed(){return 0!=::IsZoomed(m_hWnd);}

	HWND GetWindow(UINT wCmd){return ::GetWindow(m_hWnd,wCmd);}
	HWND GetNextWindow(UINT wCmd=GW_HWNDNEXT){return GetWindow(wCmd);}

	//icon
	HICON SetIcon(HICON hIcon,bool bBigIcon=false){
		return (HICON)SendMessage(WM_SETICON,(WPARAM)(bBigIcon?ICON_BIG:ICON_SMALL),(LPARAM)hIcon); 
	}
	HICON GetIcon(bool bBigIcon=false){
		return (HICON)SendMessage(WM_GETICON,(WPARAM)(bBigIcon?ICON_BIG:ICON_SMALL));
	}
protected:
	HWND 		m_hWnd;		//must be first data member!!!!!!!!!!!!!!!!!!
	HINSTANCE	m_hInst;	//store the handle of instance that include this window/dialog,exe or dll
};

template <typename T,typename TBase=KWindow>
class  KWindowRoot : public TBase{
public:
	KWindowRoot():TBase(){
		T* pT=static_cast<T*>(this);
       	m_thunk.Init((INT_PTR)pT, pT->GetMessageProcPtr());
#ifdef	PRETRANSLATEMESSAGE
		m_hHookGetMsg=0;
		m_thunkGetMsgProc.Init((INT_PTR)pT,pT->GetHookProcPtr(&T::GetMsgProc)); //for PreTranslateMessage
#endif
    }

	/* 	KWin Message Flow Diagram:
	 *
	 * 	Derived Class 'KWndProc' ==> Base Class 'KWndProc' ==> Derived Class 'ProcessWindowMessage'
	 *
 	 *	==> Base Class 'ProcessWindowMessage' ==> Derived Class 'DoDefault' ==> Base Class 'DoDefault'(Optional)
	 */

	INT_PTR GetMessageProcPtr(){
		typedef LRESULT (KCALLBACK T::*KWndProc_t)(UINT,WPARAM,LPARAM);
		union{
			KWndProc_t 	wndproc;
			INT_PTR		dwProcAddr;			
		}u;
		u.wndproc=&T::KWndProc;
		return u.dwProcAddr;
	}

    LRESULT KCALLBACK KWndProc(UINT uMsg, WPARAM wParam, LPARAM lParam){
#ifdef PRETRANSLATEMESSAGE
		if(uMsg==WM_DESTROY)
			if(m_hHookGetMsg){
				::UnhookWindowsHookEx(m_hHookGetMsg);
				m_hHookGetMsg=0;
		}
#endif
		T* pT=static_cast<T*>(this);
		return pT->ProcessWindowMessage(uMsg,wParam,lParam);
	}

	LRESULT ProcessWindowMessage(UINT uMsg,WPARAM wParam,LPARAM lParam){
		T* pT = static_cast<T*> (this);
		return pT->DoDefault(uMsg,wParam,lParam); 
	}
	LRESULT DoDefault(UINT uMsg, WPARAM wParam, LPARAM lParam){return 0;}

	LRESULT ReflectNotifications(UINT uMsg, WPARAM wParam, LPARAM lParam,bool& bHandled){
		HWND hWndChild	=NULL;
		bHandled		=false;
		switch(uMsg){
			case WM_COMMAND:
				if(lParam!=NULL)	// not from a menu
					hWndChild=(HWND)lParam;
				break;
			case WM_NOTIFY:
				hWndChild=((LPNMHDR)lParam)->hwndFrom;
				break;
			case WM_PARENTNOTIFY:
				switch(LOWORD(wParam)){
					case WM_CREATE:
					case WM_DESTROY:
						hWndChild=(HWND)lParam;
						break;
					default:
						hWndChild=GetDlgItem(HIWORD(wParam));
						break;
				}
				break;
			case WM_DRAWITEM:
				{
					DRAWITEMSTRUCT* pdis=((LPDRAWITEMSTRUCT)lParam);
					if (pdis->CtlType!=ODT_MENU)	// not from a menu
						hWndChild=pdis->hwndItem;
					else							// Status bar control sends this message with type set to ODT_MENU
						if(::IsWindow(pdis->hwndItem))
							hWndChild=pdis->hwndItem;
				}
				break;
			case WM_MEASUREITEM:
				{
					MEASUREITEMSTRUCT* pmis=((LPMEASUREITEMSTRUCT)lParam);
					if(pmis->CtlType!=ODT_MENU)	// not from a menu
						hWndChild=GetDlgItem(pmis->CtlID);
				}
				break;
			case WM_COMPAREITEM:	// Sent only by combo or list box
				hWndChild=((LPCOMPAREITEMSTRUCT)lParam)->hwndItem;
				break;
			case WM_DELETEITEM:		// Sent only by combo or list box  
				hWndChild=((LPDELETEITEMSTRUCT)lParam)->hwndItem; 
				break;
			case WM_VKEYTOITEM:
			case WM_CHARTOITEM:
			case WM_HSCROLL:
			case WM_VSCROLL:
				hWndChild=(HWND)lParam;
				break;
			case WM_CTLCOLORBTN:
			case WM_CTLCOLORDLG:
			case WM_CTLCOLOREDIT:
			case WM_CTLCOLORLISTBOX:
			case WM_CTLCOLORMSGBOX:
			case WM_CTLCOLORSCROLLBAR:
			case WM_CTLCOLORSTATIC:
				hWndChild=(HWND)lParam;
				break;
			default:
				break;
		}
		
		if(!hWndChild)
			return -1;

		MSGREFLECTSTRUCT	mrs;
		mrs.uMsg			=uMsg;
		mrs.wParam			=wParam;
		mrs.lParam			=lParam;
		mrs.bHandled		=false;
		
		assert(::IsWindow(hWndChild));
		LRESULT lRet=::SendMessage(hWndChild,WM_EX_MSGREFLECT,(WPARAM)&mrs,0);
		if(mrs.bHandled)
			bHandled=true;
		return lRet;
	}

	typedef LRESULT (KCALLBACK T::*HookProc_t)(int,WPARAM,LPARAM);
	INT_PTR GetHookProcPtr(HookProc_t proc){		
		union{
			HookProc_t 	hookproc;
			INT_PTR		dwProcAddr;			
		}u;
		u.hookproc=proc;	
		return u.dwProcAddr;
	}

#ifdef	PRETRANSLATEMESSAGE
	bool PreTranslateMessage(MSG* pMsg){return false;}	

	LRESULT KCALLBACK GetMsgProc(int nCode,WPARAM wParam,LPARAM lParam){
		if(nCode>=0 && PM_REMOVE==wParam){
			LPMSG lpMsg=(LPMSG)lParam;
			T* pT=static_cast<T*>(this);

			//for non child window only
			if((::GetWindowLongPtr(*pT,GWL_STYLE) & WS_CHILD)!=WS_CHILD){			//not a child window
				if((lpMsg->message>=WM_KEYFIRST && lpMsg->message<=WM_KEYLAST)){
					if(::IsDialogMessage(*this,lpMsg)){
						lpMsg->message	=WM_NULL;
						lpMsg->wParam	=0;
						lpMsg->lParam	=0;
					}
				}
			}

			if(pT->PreTranslateMessage(lpMsg)){
				lpMsg->message	=WM_NULL;
				lpMsg->wParam	=0;
				lpMsg->lParam	=0;
			}
		}
		return ::CallNextHookEx(m_hHookGetMsg,nCode,wParam,lParam);
	}
#endif

protected:
	KWndProcThunk	m_thunk;
    inline INT_PTR 	GetThunkedProcPtr(){return (INT_PTR)m_thunk.GetThunkedCodePtr();}

#ifdef PRETRANSLATEMESSAGE
	HHOOK			m_hHookGetMsg;
	KGeneralThunk 	m_thunkGetMsgProc; 
#endif
};

//control window base
template <typename T,typename TBase=KWindow>
class KControlBase : public KWindowRoot<T,TBase>{
public:
	typedef KWindowRoot<T,TBase> __base;
	KControlBase():KWindowRoot<T,TBase>(),m_pfnOldWndProc(NULL),m_tszClsName(0),m_bMouseEnter(false){
       	m_thunkCBTProc.Init((INT_PTR)this, GetHookProcPtr(&T::CBTProc));
	}
	~KControlBase(){SAFE_DEL_PTR(m_tszClsName);}

	BEGIN_MSG_MAP
		MSG_HANDLER(WM_MOUSEMOVE,OnMouseMove)
		MSG_HANDLER(WM_MOUSELEAVE,OnMouseLeave)
	END_MSG_MAP(__base)
	
	LRESULT OnMouseMove(UINT uMsg,LPARAM lParam,WPARAM wParam,bool& bHandled){
		if(!m_bMouseEnter){
			TRACKMOUSEEVENT tme;
			tme.cbSize 		=sizeof(tme);
			tme.dwFlags 	=TME_LEAVE;
			tme.hwndTrack 	=m_hWnd;
			m_bMouseEnter 	=(0!=_TrackMouseEvent(&tme));
			PostMessage(WM_EX_MOUSEENTER,wParam,lParam);
		}
		bHandled=false;
		return 0;
	}
	LRESULT OnMouseLeave(UINT uMsg,LPARAM lParam,WPARAM wParam,bool& bHandled){
		m_bMouseEnter=false;
		PostMessage(WM_EX_MOUSELEAVE,wParam,lParam);
		bHandled=false;
		return 0;
	}

	void PreSubclassWindow(){}
	bool SubclassWindow(HWND hWnd){
		assert(::IsWindow(hWnd) && m_hWnd==0);

		m_hWnd=hWnd;

		T* pT=static_cast<T*>(this);
		pT->PreSubclassWindow();
		
		return _SubclassWindow(hWnd);
	} 
	bool _SubclassWindow(HWND hWnd){
		assert(IsWindow(hWnd));

		m_hWnd=hWnd;

#ifdef PRETRANSLATEMESSAGE
		if(!m_hHookGetMsg)
			m_hHookGetMsg=::SetWindowsHookEx(WH_GETMESSAGE,
				(HOOKPROC)m_thunkGetMsgProc.GetThunkedCodePtr(),NULL,::GetCurrentThreadId());
#endif
		if(m_pfnOldWndProc=(WNDPROC)::SetWindowLongPtr(hWnd,GWLP_WNDPROC,(LONG_PTR)GetThunkedProcPtr()))
			return true;

		return false;
	}
	bool SubclassDlgItem(int nCtrlID,HWND hParent){return SubclassWindow(::GetDlgItem(hParent,nCtrlID));}

	bool UnsubclassWindow(){
		bool bRet=true;
		if(m_pfnOldWndProc){
			bRet		=0!=::SetWindowLongPtr(m_hWnd,GWL_WNDPROC,(INT_PTR)m_pfnOldWndProc);
			m_hWnd		=0;
			m_pfnOldWndProc	=0;
		}
		return bRet;
	}

	bool Superclass(TCHAR* tszNewClassName,TCHAR* tszOldClassName,UINT style=-1,int cbClsExtra=-1,
		int cbWndExtra=-1,HICON hIcon=NULL,HCURSOR hCursor=NULL,HBRUSH hbrBackground=NULL,
		LPCTSTR lpszMenuName=NULL,HICON hIconSm=NULL,HINSTANCE hInst=NULL){

		assert(tszNewClassName!=NULL && tszOldClassName!=NULL);
   		assert(m_hWnd == NULL);
		
		WNDCLASSEX		wcxNew;
		wcxNew.cbSize	=sizeof(WNDCLASSEX);

		if(::GetClassInfoEx(hInst,wcx.lpszClassName,&wcxNew)){
			m_pfnOldWndProc=wcxNew.lpfnWndProc;

			wcxNew.lpszClassName=tszNewClassName;
			wcxNew.Instance=(HINSTANCE)GetModuleHandle(NULL);	//if in a DLL???
			wcxNew.lpfnWndProc=GetThunkedProcPtr();
			if(style!=-1)
				wcxNew.style=style;
			if(cbClsExtra!=-1)
				wcxNew.cbClsExtra=cbClsExtra;
			if(cbWndExtra!=-1)
				wcxNew.cbWndExtra=cbWndExtra;
			if(hIcon)
				wcxNew.hIcon=hIcon;
			if(hCursor)
				wcxNew.hCursor=hCursor;
			if(hbrBackground)
				wcxNew.hbrBackground=hbrBackground;
			if(lpszMenuName)
				wcxNew.lpszMenuName=lpszMenuName;
			if(hIconSm)
				wcxNew.hIconSm=hIconSm;

			if(::RegisterClassEx(&wcxNew))		//register new class
				return true;
		}
		return false;
	}
	bool PreCreateWindow(CREATESTRUCT& cs){
		if(!cs.lpszClass)
			return false;
		return true;
	}
 	HWND Create(LPTSTR tszClassName,HWND hWndParent,LPCTSTR ctszWndName,DWORD dwStyle,RECT& rc,INT_PTR nCtrlId,
							DWORD dwExStyle=0,HINSTANCE hInst=::GetModuleHandle(NULL),LPVOID lpParam=NULL){
   		assert(m_hWnd==NULL);
		m_hInst=hInst;
		
		CREATESTRUCT 		cs;
		cs.dwExStyle 		=dwExStyle;
		cs.lpszClass 		=tszClassName;
		cs.lpszName 		=ctszWndName;
		cs.style 			=dwStyle;
		cs.x 				=rc.left;
		cs.y 				=rc.top;
		cs.cx 				=(rc.left==CW_USEDEFAULT)?CW_USEDEFAULT:(rc.right-rc.left);
		cs.cy 				=(rc.top==CW_USEDEFAULT)?CW_USEDEFAULT:(rc.bottom-rc.top);
		cs.hwndParent 		=hWndParent;
		cs.hMenu 			=(HMENU)nCtrlId;
		cs.hInstance 		=hInst;
		cs.lpCreateParams 	=lpParam;

		m_hParent			=hWndParent;
		m_nCtrlID			=nCtrlId;
		m_tszClsName		=new TCHAR[::_tcslen(tszClassName)+1];
		::_tcscpy(m_tszClsName,tszClassName);
		
		T* pT=static_cast<T*>(this);
		if(!pT->PreCreateWindow(cs))
			return NULL;

		m_hHookCBT=::SetWindowsHookEx(WH_CBT,(HOOKPROC)m_thunkCBTProc.GetThunkedCodePtr(),NULL,::GetCurrentThreadId());
       	m_hWnd=::CreateWindowEx(cs.dwExStyle,cs.lpszClass,cs.lpszName,cs.style,cs.x,cs.y,cs.cx,cs.cy,
								cs.hwndParent,cs.hMenu,cs.hInstance, cs.lpCreateParams);
		::UnhookWindowsHookEx(m_hHookCBT);
		return m_hWnd;
   	}
	LRESULT KCALLBACK CBTProc(int nCode,WPARAM wParam,LPARAM lParam){
		if(nCode==HCBT_CREATEWND){
			LPCREATESTRUCT lpcs=((LPCBT_CREATEWND)lParam)->lpcs;
			if(INT_PTR(lpcs->lpszClass)>0xffff)
				if(::_tcscmp(m_tszClsName,lpcs->lpszClass)==0
					&& lpcs->hwndParent==m_hParent 
					&& lpcs->hMenu==(HMENU)m_nCtrlID)
				_SubclassWindow((HWND)wParam);
		}	
		return ::CallNextHookEx(m_hHookCBT,nCode,wParam,lParam);
	}

	LRESULT DoDefault(UINT uMsg, WPARAM wParam, LPARAM lParam){ 
		/*
		HWND hWnd		=m_hWnd;
		WNDPROC	OldWndProc	=m_pfnOldWndProc;
		if(uMsg==WM_NCDESTROY)
			UnsubclassWindow();  //detach 'this object' from 'window object' and restore window object
		*/
		return ::CallWindowProc(m_pfnOldWndProc/*OldWndProc*/,m_hWnd/*hWnd*/,uMsg,wParam,lParam);
   	}
	LRESULT KCALLBACK KWndProc(UINT uMsg, WPARAM wParam, LPARAM lParam){
		LRESULT lRet=KWindowRoot<T,TBase>::KWndProc(uMsg,wParam,lParam);
		if(uMsg==WM_CREATE){ 
			SetFont((HFONT)::GetStockObject(DEFAULT_GUI_FONT));
			T* pT=static_cast<T*>(this);
			pT->PreSubclassWindow();
		}
		return lRet;
	}
protected:
	WNDPROC 	m_pfnOldWndProc;
	HHOOK		m_hHookCBT;
	TCHAR*		m_tszClsName;
	HWND		m_hParent;
	INT_PTR 	m_nCtrlID;

	KGeneralThunk	m_thunkCBTProc;
	bool			m_bMouseEnter;
};

//transparent control base
template<typename T,typename TBase=KWindow>
class KTransparentControlBase : public KControlBase<T,TBase>{
public:
	typedef KControlBase<T,TBase> __base;
	
	KTransparentControlBase():m_hBmpBKOld(0),m_bFirstEreaseBk(true),m_hMemDCBk(0){}
	~KTransparentControlBase(){
		if(m_hMemDCBk){
			if(m_hBmpBKOld)
				::DeleteObject(::SelectObject(m_hMemDCBk,m_hBmpBKOld));
			::DeleteDC(m_hMemDCBk);
		}
	}
	BEGIN_MSG_MAP
		MSG_HANDLER(WM_ERASEBKGND,OnEraseBkGround)		
	END_MSG_MAP(__base)

	LRESULT OnEraseBkGround(UINT uMsg,WPARAM wParam,LPARAM lParam,bool& bHandled){
		HDC hDC=(HDC)wParam;
		RECT  		rect;
		GetWindowRect(&rect);
		ScreenToClient(&rect);

		if(m_bFirstEreaseBk){
			m_hMemDCBk	=::CreateCompatibleDC(hDC);
			m_hBmpBKOld	=(HBITMAP)::SelectObject(m_hMemDCBk,::CreateCompatibleBitmap(hDC,rect.right,rect.bottom));
			
			::BitBlt(m_hMemDCBk,0,0,rect.right,rect.bottom,hDC,0,0,SRCCOPY);
			m_bFirstEreaseBk=false;
		}else{
			::BitBlt(hDC,0,0,rect.right,rect.bottom,m_hMemDCBk,0,0,SRCCOPY);
		}
		return TRUE;
	}
private:
	HBITMAP		m_hBmpBKOld;
	bool		m_bFirstEreaseBk;
	HDC			m_hMemDCBk;
};

template <typename T,typename TBase=KWindow>
class KDialogBase : public KWindowRoot<T,TBase>{
public:
	typedef KWindowRoot<T,TBase> __base;
	KDialogBase():KWindowRoot<T,TBase>(),m_pDlgTemplate(0){}

	BEGIN_MSG_MAP
		MSG_HANDLER(WM_INITDIALOG,OnInitDialog)
		COMMAND_ID_HANDLER(IDCANCEL,OnCancel)
	END_MSG_MAP(__base)

	LRESULT OnInitDialog(UINT uMsg,WPARAM wParam,LPARAM lParam,bool& bHandled){
		CentralizeWindow();
		return TRUE;
	}
	LRESULT OnCancel(WORD wID,WORD wNotifyCode,HWND hWndCtrl,bool& bHandled){
		EndDialog(wID);	
		return 0;
	}

	LRESULT DoDefault(UINT uMsg, WPARAM wParam, LPARAM lParam){return FALSE;}
	LRESULT KCALLBACK KWndProc(UINT uMsg, WPARAM wParam, LPARAM lParam){
#ifdef PRETRANSLATEMESSAGE
		if(uMsg==WM_INITDIALOG)
			if(!m_hHookGetMsg)
				m_hHookGetMsg=::SetWindowsHookEx(WH_GETMESSAGE,
					(HOOKPROC)m_thunkGetMsgProc.GetThunkedCodePtr(),NULL,::GetCurrentThreadId());
#endif
		LRESULT lReturn=KWindowRoot<T,TBase>::KWndProc(uMsg,wParam,lParam);
		if(lReturn!=FALSE){		
		    switch(uMsg){
				case WM_COMPAREITEM         :
				case WM_VKEYTOITEM          :
				case WM_CHARTOITEM          :
				case WM_INITDIALOG          :
				case WM_QUERYDRAGICON       :
				case WM_CTLCOLORMSGBOX      :
				case WM_CTLCOLOREDIT        :
				case WM_CTLCOLORLISTBOX     :
				case WM_CTLCOLORBTN         :
				case WM_CTLCOLORDLG         :
				case WM_CTLCOLORSCROLLBAR   :
				case WM_CTLCOLORSTATIC      :
					break;
				default:
					::SetWindowLongPtr(m_hWnd,DWLP_MSGRESULT,(LONG_PTR)lReturn);	
					lReturn=TRUE;
					break;
			}
		}
		return lReturn;
	}
	void SubclassDialog(HWND hWndDlg){
		assert(::IsWindow(hWndDlg) && m_hWnd==0);
		::SetWindowLongPtr(hWndDlg,DWLP_DLGPROC,(LONG_PTR)GetThunkedProcPtr());
	}
	bool EndDialog(int nRetCode){return 0!=::EndDialog(m_hWnd,nRetCode);}
	void SetDlgMsgResult(LONG_PTR lResult){::SetWindowLongPtr(m_hWnd,DWLP_MSGRESULT,(LONG_PTR)lResult);}

	INT_PTR DoModal(HWND hParent=::GetActiveWindow(),HINSTANCE hInst=::GetModuleHandle(0),LPARAM lpInitParam=NULL,
					LPCTSTR tszTemplateName=MAKEINTRESOURCE(T::IDD)){
		assert(m_hWnd==NULL);
		m_hInst=hInst;
		return ::DialogBoxParam(hInst,tszTemplateName,hParent,(DLGPROC)GetThunkedProcPtr(),lpInitParam);
	}

	INT_PTR DoModalIndirect(HWND hParent=::GetActiveWindow(),HINSTANCE hInst=::GetModuleHandle(0),
							LPARAM lpInitParam=NULL,LPCDLGTEMPLATE pDlgTemplate=0){
		assert(m_hWnd==NULL);
		m_hInst=hInst;
		if(!pDlgTemplate)
			pDlgTemplate=m_pDlgTemplate;

      	return ::DialogBoxIndirectParam(hInst,pDlgTemplate,hParent,(DLGPROC)GetThunkedProcPtr(),lpInitParam);
	}

	HWND Create(HWND hParent=::GetActiveWindow(),HINSTANCE hInst=::GetModuleHandle(0),LPARAM lpInitParam=NULL,
					LPCTSTR tszTemplateName=MAKEINTRESOURCE(T::IDD)){
		assert(m_hWnd==NULL);
		m_hInst=hInst;
		return ::CreateDialogParam(hInst,tszTemplateName,hParent,(DLGPROC)GetThunkedProcPtr(),lpInitParam);
	}
	HWND CreateIndirect(HWND hParent=::GetActiveWindow(),HINSTANCE hInst=::GetModuleHandle(0),
						LPARAM lpInitParam=NULL,LPCDLGTEMPLATE pDlgTemplate=0){
		assert(m_hWnd==NULL);
		m_hInst=hInst;
		if(!pDlgTemplate)
			pDlgTemplate=m_pDlgTemplate;

	    return ::CreateDialogIndirectParam(hInst,pDlgTemplate,hParent,(DLGPROC)GetThunkedProcPtr(),lpInitParam);
	}
protected:
	LPCDLGTEMPLATE m_pDlgTemplate;
};

template <typename T,typename TBase=KWindow>
class KWindowBase : public KControlBase<T,TBase>{
public:
	KWindowBase(LPCTSTR lpszClassName=NULL):KControlBase<T,TBase>(){  
		if(lpszClassName){ 
        	m_lpszClassName = new TCHAR [::_tcslen(lpszClassName)+1];
        	::_tcscpy(m_lpszClassName,lpszClassName);
	   	}else{
			m_lpszClassName=new TCHAR[12];
			::_tcscpy(m_lpszClassName,_T("KWinGUI"));
	   	}
   	}

    ~KWindowBase(){SAFE_DEL_PTRS(m_lpszClassName);}
	void PreRegisterClassEx(WNDCLASSEX& wcx){
	}
   	bool RegisterClass(){
		WNDCLASSEX wcx={sizeof(WNDCLASSEX),};	       
		wcx.cbClsExtra			=0;
		wcx.cbWndExtra       	=0;
		wcx.hbrBackground    	=(HBRUSH)(COLOR_BTNFACE+1);
		wcx.hCursor          	=(HCURSOR)::LoadCursor(NULL,IDC_ARROW);
		wcx.hIcon            	=(HICON)::LoadIcon(NULL,IDI_APPLICATION);
		wcx.lpszClassName    	=m_lpszClassName;
		wcx.lpszMenuName     	=NULL;
		wcx.style            	=CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS;
		wcx.hInstance  	     	=m_hInst;
		wcx.hIconSm				=NULL;

		T* pT=static_cast<T*>(this);
		pT->PreRegisterClassEx(wcx);
		if(::_tcscmp(wcx.lpszClassName,m_lpszClassName)){
			if(m_lpszClassName)
				delete m_lpszClassName;
			m_lpszClassName=new TCHAR[::_tcslen(wcx.lpszClassName)+1];
			::_tcscpy(m_lpszClassName,wcx.lpszClassName);	
		}

		WNDCLASSEX trivialwcx={sizeof(WNDCLASSEX),};
		if(::GetClassInfoEx (wcx.hInstance,wcx.lpszClassName, &trivialwcx))   
			return false;

		wcx.lpfnWndProc=::DefWindowProc; 
		return (0!=::RegisterClassEx(&wcx));
	}

	HWND CreateOverlappedWindow(LPCTSTR lpszWndName,RECT* lprc=NULL,BOOL bShow=TRUE,HMENU hMenu=NULL,
							HINSTANCE hInst=::GetModuleHandle(NULL),LPVOID pParam=NULL,HWND hWndParent=NULL){
    	RECT rc={CW_USEDEFAULT,CW_USEDEFAULT,0,0};
    	if(lprc)
			::CopyRect(&rc, lprc);

   		return Create(hWndParent,lpszWndName,WS_OVERLAPPEDWINDOW|(bShow?WS_VISIBLE:0),rc,(INT_PTR)hMenu,0,hInst,pParam);
	}

	HWND Create(HWND hWndParent,LPCTSTR lpszWndName,DWORD dwStyle,RECT& rc,INT_PTR nCtrlId=0,
				DWORD dwExStyle=0,HINSTANCE hInst=::GetModuleHandle(NULL),LPVOID lpParam=NULL){
       	assert(m_hWnd==NULL);

		RegisterClass();
		m_hInst=hInst;
		return KControlBase<T>::Create(m_lpszClassName,hWndParent,lpszWndName,dwStyle,rc,nCtrlId,dwExStyle,hInst,lpParam);
	}
	LRESULT ProcessWindowMessage(UINT uMsg,WPARAM wParam,LPARAM lParam){
		//provide default handler of WM_NCDESTROY for non child window
   		if(uMsg==WM_NCDESTROY && ((::GetWindowLongPtr(m_hWnd,GWL_STYLE) & WS_CHILD)!=WS_CHILD)){
			::PostQuitMessage(0);
			return 0;
		}
		return KControlBase<T>::ProcessWindowMessage(uMsg,wParam,lParam);
	}	
private:
	LPTSTR m_lpszClassName;
};

} //namespace kwinui
#endif // __KWIN_H__
