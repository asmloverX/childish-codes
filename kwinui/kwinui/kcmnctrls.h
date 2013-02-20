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
 *  win32 common contrls wrap
 *
 *  01/23/2007
 *  proguru
 */

#ifndef __KCMNCTRLS_H__
#define __KCMNCTRLS_H__

#include "kwin.h"
#include "ktypes.h"
#include "commctrl.h"
#include <cassert>
 
namespace kwinui{

//trackbar
template<typename T,typename TBase=KWindow>
class KTrackBarBase : public KControlBase<T,TBase>{
public:
	typedef KControlBase<T,TBase> __base;
	HWND Create(HWND hWndParent,LPCTSTR tszWndName,DWORD dwStyle,RECT& rc,INT_PTR nCtrlID,DWORD dwExStyle=0,
				HINSTANCE hInst=(HINSTANCE)GetModuleHandle(NULL),LPVOID lpParam=NULL){
		assert(m_hWnd==NULL);
		return __base::Create(TRACKBAR_CLASS,hWndParent,tszWndName,dwStyle,rc,nCtrlID,dwExStyle,hInst,lpParam);
	}
	//Attributes
	int GetLineSize(){return (int)::SendMessage(m_hWnd,TBM_GETLINESIZE,0,0);}
	int SetLineSize(int nSize){assert(nSize>0); return (int)::SendMessage(m_hWnd,TBM_SETLINESIZE,0,(LPARAM)nSize);}
	int GetPageSize()const{return (int)::SendMessage(m_hWnd,TBM_GETPAGESIZE,0,0);}
	int SetPageSize(int nSize){assert(nSize>0); return (int)::SendMessage(m_hWnd,TBM_SETPAGESIZE,0,(LPARAM)nSize);}
	int GetRangeMax()const{return (int)::SendMessage(m_hWnd,TBM_GETRANGEMAX,0,0);}
	int GetRangeMin()const{return (int)::SendMessage(m_hWnd,TBM_GETRANGEMIN,0,0);}
	void GetRange(int& nMin, int& nMax)const{nMin=GetRangeMin();nMax=GetRangeMax();}
	void SetRangeMin(int nMin, bool bRedraw=true){::SendMessage(m_hWnd,TBM_SETRANGEMIN,(WPARAM)bRedraw,(LPARAM)nMin);}
	void SetRangeMax(int nMax, bool bRedraw=true){::SendMessage(m_hWnd,TBM_SETRANGEMAX,(WPARAM)bRedraw,(LPARAM)nMax);}
	void SetRange(int nMin, int nMax, bool bRedraw=true){SetRangeMin(nMin,bRedraw); SetRangeMax(nMax,bRedraw);}
	void GetSelection(int& nMin, int& nMax)const{
		nMin=int(::SendMessage(m_hWnd,TBM_GETSELSTART,0,0L));
		nMax=int(::SendMessage(m_hWnd,TBM_GETSELEND,0,0L));
	}
	void SetSelection(int nMin, int nMax){
		::SendMessage(m_hWnd,TBM_SETSELSTART,0,(LPARAM)nMin);
		::SendMessage(m_hWnd,TBM_SETSELEND,0,(LPARAM)nMax);
	}
	void GetChannelRect(LPRECT lprc)const{assert(lprc);::SendMessage(m_hWnd,TBM_GETCHANNELRECT,0,(LPARAM)lprc);}
	void GetThumbRect(LPRECT lprc)const{assert(lprc);::SendMessage(m_hWnd,TBM_GETTHUMBRECT,0,(LPARAM)lprc);}
	int GetPos()const{return (int)::SendMessage(m_hWnd,TBM_GETPOS,0,0);}
	void SetPos(int nPos,bool bRedraw=true){::SendMessage(m_hWnd,TBM_SETPOS,(WPARAM)bRedraw,(LPARAM)nPos);}

	int GetNumTics()const{return (int)::SendMessage(m_hWnd,TBM_GETNUMTICS,0,0);}
	DWORD* GetTicArray()const{return (DWORD*)::SendMessage(m_hWnd,TBM_GETPTICS,0,0);}
	int GetTic(int nTic)const{return (int)::SendMessage(m_hWnd,TBM_GETTIC,(WPARAM)nTic,0);}
	int GetTicPos(int nTic)const{return (int)::SendMessage(m_hWnd,TBM_GETTICPOS,(WPARAM)nTic,0);}
	BOOL SetTic(int nTic){::SendMessage(m_hWnd,TBM_SETTIC,0,(LPARAM)nTic);}
	void SetTicFreq(int nFreq){::SendMessage(m_hWnd,TBM_SETTICFREQ,(WPARAM)nFreq,0);}
	HWND GetBuddy(bool fLocation=true)const{return ::SendMessage(m_hWnd,TBM_GETBUDDY,(LPARAM)fLocation,0)}
	HWND SetBuddy(HWND hWndBuddy,bool fLocation=true){
		assert(::IsWindow(hWndBuddy));
		return ::SendMessage(m_hWnd,TBM_SETBUDDY,(WPARAM)fLocation,(LPARAM)hWndBuddy);
	}
	HWND GetToolTips()const{return (HWND)::SendMessage(m_hWnd,TBM_GETTOOLTIPS,0,0);}
	void SetToolTips(HWND hWndToolTips){::SendMessage(m_hWnd,TBM_SETTOOLTIPS,(WPARAM)hWndToolTips,0);}
	int SetTipSide(int nLocation){::SendMessage(m_hWnd,TBM_SETTIPSIDE,(WPARAM)nLocation,0);}

	//Operations
	void ClearSel(bool bRedraw=false){::SendMessage(m_hWnd,TBM_CLEARSEL,(WPARAM)bRedraw,0);}
	void ClearTics(bool bRedraw=false){::SendMessage(m_hWnd,TBM_CLEARTICS,(WPARAM)bRedraw,0);}
};
class KTrackBar : public KTrackBarBase<KTrackBar>{
public:
	typedef KTrackBarBase<KTrackBar> __base;
	BEGIN_MSG_MAP
		MSG_HANDLER(WM_LBUTTONDOWN,OnLButtonDown)
	END_MSG_MAP(__base)
	LRESULT OnLButtonDown(UINT uMsg,WPARAM wParam,LPARAM lParam,bool& bHandled){
		
		LONG_PTR lStyle=GetWindowLongPtr(GWL_STYLE);
		SetWindowLongPtr(GWL_STYLE,lStyle & ~WS_VISIBLE);
		LRESULT lRet=__base::ProcessWindowMessage(uMsg,wParam,lParam);
		SetWindowLongPtr(GWL_STYLE,lStyle);

		RECT rectChannel;
		GetChannelRect(&rectChannel);
		int nXPos=(int)(short)LOWORD(lParam); 
		int nMin=0,nMax=0;
		GetRange(nMin,nMax);
		int nNewPos=(int)(1.0*(nXPos-rectChannel.left)*(nMax-nMin)/(rectChannel.right-rectChannel.left)+0.5);
		SetPos(nNewPos);
		return lRet;
	}
};

//tooltips
template<typename T,typename TBase=KWindow>
class KToolTipBase : public KControlBase<T,TBase>{
public:
	typedef KControlBase<T,TBase> __base;
	HWND Create(HWND hWndParent,DWORD dwStyle=TTS_ALWAYSTIP,DWORD dwExStyle=0,
				HINSTANCE hInst=(HINSTANCE)GetModuleHandle(NULL)){
		assert(m_hWnd==NULL);
		return __base::Create(TOOLTIPS_CLASS,hWndParent,NULL,dwStyle,KRect(CW_USEDEFAULT,CW_USEDEFAULT,0,0),
								NULL,dwExStyle,hInst);	
	}
	bool AddTool(HWND hWndTool,TCHAR* tszText){
		TOOLINFO ti;
		::memset(&ti,'\0',sizeof(TOOLINFO));
		
		ti.cbSize	=sizeof(TOOLINFO);
		ti.uFlags	=TTF_IDISHWND|/*TTF_PARSELINKS|*/TTF_SUBCLASS;
		ti.hwnd		=GetParent();
		ti.uId		=(UINT_PTR)hWndTool;
		ti.lpszText	=tszText;

		return 0!=SendMessage(TTM_ADDTOOL,0,(LPARAM)(&ti));
	}
};
class KToolTip : public KToolTipBase<KToolTip>{
};

template<typename T,typename TBase=KWindow>
class KTabCtrlBase : public KControlBase<T,TBase>{
public:
	typedef KControlBase<T,TBase> __base;
	HWND Create(HWND hWndParent,LPCTSTR tszWndName,DWORD dwStyle,RECT& rc,INT_PTR nCtrlID,DWORD dwExStyle=0,
				HINSTANCE hInst=(HINSTANCE)GetModuleHandle(NULL),LPVOID lpParam=NULL){
		assert(m_hWnd==NULL);
		return __base::Create(WC_TABCONTROL,hWndParent,tszWndName,dwStyle,rc,nCtrlID,dwExStyle,hInst,lpParam);	
	}

	HIMAGELIST GetImageList(){return (HIMAGELIST)SendMessage(TCM_GETIMAGELIST);}
	HIMAGELIST SetImageList(HIMAGELIST hIml){return (HIMAGELIST)SendMessage(TCM_SETIMAGELIST,0,(LPARAM)hIml);}
	int GetItemCount(){return (int)SendMessage(TCM_GETITEMCOUNT);}
	bool GetItem(int nItem, TCITEM* pTabCtrlItem){
		return 0!=SendMessage(TCM_GETITEM,(WPARAM)nItem,(LPARAM)pTabCtrlItem);}
	bool SetItem(int nItem, TCITEM* pTabCtrlItem){
		return 0!=SendMessage(TCM_SETITEM,(WPARAM)nItem,(LPARAM)pTabCtrlItem);}
	//bool SetItemExtra(int nBytes);
	bool GetItemRect(int nItem, RECT* pRect){
		return 0!=SendMessage(TCM_GETITEMRECT,(WPARAM)nItem,(LPARAM)pRect);}

	int GetCurSel(){return (int)SendMessage(TCM_GETCURSEL);}
	int SetCurSel(int nItem){return (int)SendMessage(TCM_SETCURSEL,(WPARAM)nItem);}
	void SetCurFocus(int nItem){SendMessage(TCM_SETCURFOCUS,(WPARAM)nItem);}
	int GetCurFocus(){return (int)SendMessage(TCM_GETCURFOCUS);}
	SIZE SetItemSize(SIZE size){
		SIZE tmpSize;
		LRESULT lRet=SendMessage(TCM_SETITEMSIZE,0,(LPARAM)(size.cy<<16|size.cx));
		tmpSize.cx=LOWORD(lRet);
		tmpSize.cy=HIWORD(lRet);
		return tmpSize;
	}
	void SetPadding(SIZE size){SendMessage(TCM_SETPADDING,0,(MAKELPARAM)(size.cx,size.cy);)}
	int GetRowCount(){return (int)SendMessage(TCM_GETROWCOUNT);}

	HWND GetToolTips(){return (HWND)SendMessage(TCM_GETTOOLTIPS);}
	void SetToolTips(HWND hWndTT){SendMessage(TCM_SETTOOLTIPS,(WPARAM)hWndTT);}
	int SetMinTabWidth(int cx){return (int)SendMessage(TCM_SETMINTABWIDTH,0,(LPARAM)cx);}
	DWORD GetExtendedStyle(){return (DWORD)SendMessage(TCM_GETEXTENDEDSTYLE);}
	DWORD SetExtendedStyle(DWORD dwNewStyle,DWORD dwExMask=0){
		return (DWORD)SendMessage(TCM_SETEXTENDEDSTYLE,(WPARAM)dwExMask,(LPARAM)dwNewStyle);}
	//DWORD GetItemState(int nItem, DWORD dwMask) const;
	//BOOL SetItemState(int nItem, DWORD dwMask, DWORD dwState);

	int InsertItem(int nItem, TCITEM* pTabCtrlItem){
		return (int)SendMessage(TCM_INSERTITEM,(WPARAM)nItem,(LPARAM)pTabCtrlItem);}
	int InsertItem(int nItem, LPTSTR tszItem){
		TCITEM tci={0};
		tci.mask	=TCIF_TEXT;
		tci.pszText	=tszItem;
		return InsertItem(nItem,&tci);
	}
	int InsertItem(int nItem, LPTSTR tszItem, int nImage){
		TCITEM tci={0};
		tci.mask	=TCIF_TEXT|TCIF_IMAGE;
		tci.pszText	=tszItem;
		tci.iImage	=nImage;
		return InsertItem(nItem,&tci);
	}
	int InsertItem(int nItem, LPTSTR tszItem,int nImage, LPARAM lParam){
		TCITEM tci={0};
		tci.mask	=TCIF_TEXT|TCIF_IMAGE|TCIF_PARAM;
		tci.pszText	=tszItem;
		tci.iImage	=nImage;
		tci.lParam	=lParam;
		return InsertItem(nItem,&tci);
	}
	bool DeleteItem(int nItem){return 0!=SendMessage(TCM_DELETEITEM,(WPARAM)nItem);}
	bool DeleteAllItems(){return 0!=SendMessage(TCM_DELETEALLITEM);}
	void AdjustRect(RECT* pRect,bool bLarger=false){SendMessage(TCM_ADJUSTRECT,(WPARAM)bLarger,(LPARAM)pRect);}
	void RemoveImage(int nImage){SendMessage(TCM_REMOVEIMAGE,(WPARAM)nImage);}
	int HitTest(TCHITTESTINFO* pHitTestInfo){return (int)SendMessage(TCM_HITTEST,0,(LPARAM)pHitTestInfo);}
	void DeselectAll(bool fExcludeFocus=true){SendMessage(TCM_DESELECTALL,(WPARAM)fExcludeFocus);}
	bool HighlightItem(int idItem, bool fHighlight=true){
		return 0!=SendMessage(TCM_HIGHLIGHTITEM,(WPARAM)idItem,(LPARAM)MAKELONG(fHighlight,0));}
};
class KTabCtrl : public KTabCtrlBase<KTabCtrl>{
};

struct Page{
	HWND 	hWndPage;
	TCHAR*	tszTitle;
};
class KDlgTabCtrl : public KTabCtrlBase<KDlgTabCtrl>{
public:
	BEGIN_MSG_MAP
		REFLECTED_NOTIFY_CODE_HANDLER(TCN_SELCHANGE,OnSelChange)
	END_MSG_MAP(KTabCtrlBase<KDlgTabCtrl>)
	
	KDlgTabCtrl(){}
	KDlgTabCtrl(Page* pPage,int nPageCount){SetPages(pPage,nPageCount);}

	void SetPages(Page* pPage,int nPageCount){
		assert(pPage);
		assert(nPageCount>=1);

		m_pPage=pPage;
		m_nPageCount=nPageCount;
	}
	
	void PreSubclassWindow(){
		for(int i=0;i<m_nPageCount;i++)
			InsertItem(i,m_pPage[i].tszTitle);	

		KRect tabRect,itemRect,posRect;
		int nX, nY, nCX, nCY;

		GetClientRect(&tabRect);
		GetItemRect(0, &itemRect);

		nX=itemRect.left;
		nY=itemRect.bottom+2;
		nCX=tabRect.right-itemRect.left-4;
		nCY=tabRect.bottom-nY-4;
		
		GetWindowRect(&posRect);
		POINT pt;
		pt.x=posRect.left;
		pt.y=posRect.top;
		::ScreenToClient(GetParent(),&pt);
		nX+=pt.x;
		nY+=pt.y;

		m_nCurPage=0;
		::SetWindowPos(m_pPage[0].hWndPage,HWND_TOP,nX,nY,nCX,nCY,SWP_SHOWWINDOW);
		for(int i=1;i<m_nPageCount;i++)
			::SetWindowPos(m_pPage[i].hWndPage,HWND_TOP,nX,nY,nCX,nCY,SWP_HIDEWINDOW);
		
	}

	LRESULT OnSelChange(int nIDFrom,NMHDR* pNmHdr,bool& bHandledReflect){
		int nCurSel=GetCurSel();
		assert(nCurSel<m_nPageCount);

		if(m_nCurPage!=nCurSel){
			::ShowWindow(m_pPage[m_nCurPage].hWndPage,SW_HIDE);
			::ShowWindow(m_pPage[nCurSel].hWndPage,SW_SHOW);
			m_nCurPage=nCurSel;
			::SetFocus(*this);	
		}
		return 0;
	}
	
private:
	Page*	m_pPage;
	int 	m_nPageCount;
	int 	m_nCurPage;
};

template<typename T,typename TBase=KWindow>
class KSpinCtrlBase : public KControlBase<T,TBase>{
public:
	typename KControlBase<T,TBase> __base;
	HWND Create(HWND hWndParent,LPCTSTR tszWndName,DWORD dwStyle,RECT& rc,INT_PTR nCtrlID,DWORD dwExStyle=0,
				HINSTANCE hInst=(HINSTANCE)GetModuleHandle(NULL),LPVOID lpParam=NULL){
		assert(m_hWnd==NULL);
		return __base::Create(UPDOWN_CLASS,hWndParent,tszWndName,dwStyle,rc,nCtrlID,dwExStyle,hInst,lpParam);	
	}
	HWND SetBuddy(HWND hWndBuddy){return SendMessage(UDM_SETBUDDY,(WPARAM)hWndBuddy);}
	HWND GetBuddy(){return SendMessage(UDM_GETBUDDY);}
	void SetRange32(int nMin,int nMax){SendMessage(UDM_SETRANGE32,(WPARAM)nMim,(LPARAM)nMax);}
	void GetRange32(int* pMin,int* pMax){SendMessage(UDM_GETRANGE32,(WPARAM)pMin,(LPARAM)pMax);}
	int GetPos32(){return (int)SendMessage(UDM_GETPOS32);}
};

template<typename T,typename TBase=KWindow>
class KProgressBarBase : public KControlBase<T,TBase>{
public:
	typedef KControlBase<T,TBase> __base;
	HWND Create(HWND hWndParent,LPCTSTR tszWndName,DWORD dwStyle,RECT& rc,INT_PTR nCtrlID,DWORD dwExStyle=0,
				HINSTANCE hInst=(HINSTANCE)GetModuleHandle(NULL),LPVOID lpParam=NULL){
		assert(m_hWnd==NULL);
		return __base::Create(PROGRESS_CLASS,hWndParent,tszWndName,dwStyle,rc,nCtrlID,dwExStyle,hInst,lpParam);	
	}
	int GetPos(){return SendMessage(PBM_GETPOS);}
	int SetPos(int nNewPos){return SendMessage(PBM_SETPOS,(WPARAM)nNewPos);}
	void GetRange(int& nLower,int& nUpper){
		PBRANGE range;
		SendMessage(PBM_GETRANGE,0,(LPARAM)(&range));
		nLower=range.iLow;
		nUpper=range.iHigh;
	}
	void SetRange32(int nLow,int nHigh){SendMessage(PBM_SETRANGE32,(WPARAM)nLow,(LPARAM)nHigh);}
	//int GetStep(){return SendMessage(PBM_GETSTEP);}	//vista and later
	int SetStep(int nStep){return SendMessage(PBM_SETSTEP,(WPARAM)nStep);}
	int StepIt(){return SendMessage(PBM_STEPIT);}
};
class KProgressBar : public KProgressBarBase<KProgressBar>{
};

template<typename T,typename TBase=KWindow>
class KListViewBase : public KControlBase<T,TBase>{
public:
	typedef KControlBase<T,TBase> __base;
	HWND Create(HWND hWndParent,LPCTSTR tszWndName,DWORD dwStyle,RECT& rc,INT_PTR nCtrlID,DWORD dwExStyle=0,
				HINSTANCE hInst=(HINSTANCE)GetModuleHandle(NULL),LPVOID lpParam=NULL){
		assert(m_hWnd==NULL);
		return __base::Create(WC_LISTVIEW,hWndParent,tszWndName,dwStyle,rc,nCtrlID,dwExStyle,hInst,lpParam);	
	}
	int InsertColumn(int nItemCol,LVCOLUMN* pColumn){
		return SendMessage(LVM_INSERTCOLUMN,(WPARAM)nItemCol,(LPARAM)pColumn);
	}
	int InsertColumn(int nItemCol,TCHAR* tszHeading,int nWidth=-1,int nFormat=LVCFMT_LEFT){
		LVCOLUMN column;
		column.mask 	=LVCF_TEXT|LVCF_FMT;
		column.pszText 	=tszHeading;
		column.fmt 		=nFormat;
		if(nWidth!=-1){
			column.mask	|=LVCF_WIDTH;
			column.cx	=nWidth;
		}
		return InsertColumn(nItemCol,&column);
	}
	int SetExtendedStyle(int nNewStyle){return SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE,0,(LPARAM)nNewStyle);}
	UINT GetItemState(int nItem,UINT nMask){return SendMessage(LVM_GETITEMSTATE,(WPARAM)nItem,(LPARAM)nMask);}
	bool SetItemState(int nItem,UINT nNewState,UINT nMask){
		LVITEM item;
		item.state		=nNewState;
		item.stateMask	=nMask;
		return 0!=SendMessage(LVM_SETITEMSTATE,(WPARAM)nItem,(LPARAM)&item);
	}
	int GetCountPerPage(){return SendMessage(LVM_GETCOUNTPERPAGE);}
	bool GetItemPosition(int nItem,POINT* pPoint){
		return 0!=SendMessage(LVM_GETITEMPOSITION,(WPARAM)nItem,(LPARAM)pPoint);
	}
	int InsertItem(LVITEM* pItem){return SendMessage(LVM_INSERTITEM,0,(LPARAM)pItem);}
	int InsertItem(UINT nMask, int nItem, TCHAR* tszItemText, UINT nState, UINT nStateMask,int nImage, LPARAM lParam){
		LVITEM item;
		item.mask 		= nMask;
		item.iItem 		= nItem;
		item.iSubItem 	= 0;
		item.pszText 	= tszItemText;
		item.state 		= nState;
		item.stateMask 	= nStateMask;
		item.iImage 	= nImage;
		item.lParam 	= lParam;
		return InsertItem(&item);
	}
	int InsertItem(int nItem,TCHAR* tszItemText){
		LVITEM item;
		item.mask 		=LVIF_TEXT;
		item.iItem 		=nItem;
		item.iSubItem 	=0;
		item.pszText 	=tszItemText;
		return InsertItem(&item);
	}
	bool SetItemText(int nItem,int nSubItem,TCHAR* tszText){
		LVITEM item;
		item.iSubItem	=nSubItem;
		item.pszText	=tszText;
		return 0!=SendMessage(LVM_SETITEMTEXT,(WPARAM)nItem,(LPARAM)&item);
	}
	int InsertRow(int nItem,TCHAR* tszItemText,TCHAR** ptszSubItemText){
		//insert item
		LVITEM item;
		item.mask		=LVIF_TEXT;
		item.iItem		=nItem;
		item.iSubItem	=0;
		item.pszText	=tszItemText;
		int nRet=InsertItem(&item);

		//set subitems
		int nCount=GetColumnCount();
		for(int i=1;i<nCount;i++){
			SetItemText(nItem,i,ptszSubItemText[i-1]);
		}
		return nRet;
	}
	HWND GetHeader(){return (HWND)SendMessage(LVM_GETHEADER);}
	int GetColumnCount(){return ::SendMessage(GetHeader(),HDM_GETITEMCOUNT,0,0);}
	bool SetTextColor(COLORREF clr){return 0!=SendMessage(LVM_SETTEXTCOLOR,0,(LPARAM)clr);}
	bool SetTextBkColor(COLORREF clr){return 0!=SendMessage(LVM_SETTEXTBKCOLOR,0,(LPARAM)clr);}
	int GetItemCount(){return SendMessage(LVM_GETITEMCOUNT);}
	bool GetItem(LVITEM* pLvItem){return 0!=SendMessage(LVM_GETITEM,0,(LPARAM)pLvItem);}
	bool DeleteAllItems(){return 0!=SendMessage(LVM_DELETEALLITEMS);}
	int GetItemText(int nItem,int nSubItem,TCHAR* tszText,int nTextMax){
		LVITEM item;
		item.mask		=LVIF_TEXT;
		item.iItem		=nItem;
		item.iSubItem	=nSubItem;
		item.pszText	=tszText;
		item.cchTextMax	=nTextMax;
		return SendMessage(LVM_GETITEMTEXT,(WPARAM)nItem,(LPARAM)&item); 
	}
	int SubItemHitTest(LPLVHITTESTINFO pHitTestInfo){return (int)SendMessage(LVM_SUBITEMHITTEST,0,(LPARAM)pHitTestInfo);}
	int GetNextItem(int nStart,UINT uFlags){return (int)SendMessage(LVM_GETNEXTITEM,(WPARAM)nStart,(LPARAM)uFlags);}
	int GetSelectedItem(){return GetNextItem(-1,LVNI_SELECTED);}
	bool DeleteItem(int nItem){return 0!=SendMessage(LVM_DELETEITEM,(WPARAM)nItem);}
};
class KListView : public KListViewBase<KListView>{
};

template<typename T,typename TBase=KWindow>
class KHeaderCtrlBase : public KControlBase<T,TBase>{
public:
	typedef KControlBase<T,TBase> __base;
	HWND Create(HWND hWndParent,LPCTSTR tszWndName,DWORD dwStyle,RECT& rc,INT_PTR nCtrlID,DWORD dwExStyle=0,
				HINSTANCE hInst=(HINSTANCE)GetModuleHandle(NULL),LPVOID lpParam=NULL){
		assert(m_hWnd==NULL);
		return __base::Create(WC_HEADER,hWndParent,tszWndName,dwStyle,rc,nCtrlID,dwExStyle,hInst,lpParam);	
	}
	int GetItemCount(){return SendMessage(HDM_GETITEMCOUNT);}
	bool GetItem(int nItem,HDITEM* pHeaderItem){return 0!=SendMessage(HDM_GETITEM,(WPARAM)nItem,(LPARAM)pHeaderItem);}
	bool GetItemRect(int nItem,RECT* pItemRect){return 0!=SendMessage(HDM_GETITEMRECT,(WPARAM)nItem,(LPARAM)pItemRect);}
};
class KHeaderCtrl : public KHeaderCtrlBase<KHeaderCtrl>{
};

} //namespace kwinui
#endif //__KCMNCTRLS_H__
