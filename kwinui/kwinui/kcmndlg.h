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

/* common dialog wrap
 *
 * 03/15/2007
 * proguru
 *
 */

#ifndef __KCMNDLG_H__
#define __KCMNDLG_H__

#include "shlobj.h"
#include "objbase.h"

#include "kwin.h"
#include "kutil.h"
#include "KDlgTemplateBuilder.h"

#include "Commdlg.h"
#include <cassert>

#pragma comment(lib,"shell32.lib")
#pragma comment(lib,"Comdlg32.lib")
#pragma comment(lib,"ole32.lib")
 
namespace kwinui{

template<typename T,typename TBase=KWindow>
class KSelectFolderDlg : public KWindowRoot<T,TBase>{
public:
	KSelectFolderDlg(HWND hWndOwner=0,LPCTSTR tszTitle=0){
			m_bi.hwndOwner		=hWndOwner;
			m_bi.pidlRoot		=0;
			m_bi.pszDisplayName	=0;
			m_bi.lpszTitle		=tszTitle;
			m_bi.ulFlags		=BIF_NEWDIALOGSTYLE|BIF_RETURNONLYFSDIRS;
			m_bi.lpfn			=0;
			m_bi.lParam			=0;
			m_bi.iImage			=0;

			m_tszFolderName		=new TCHAR[MAX_PATH];
			m_pidl				=0;
			//::CoInitializeEx(0,COINIT_APARTMENTTHREADED);
	}
	~KSelectFolderDlg(){SAFE_DEL_PTRS(m_tszFolderName);ReleasePIDL();/*::CoUninitialize();*/}

	bool DoModal(){
		m_pidl=::SHBrowseForFolder(&m_bi);
		if(!m_pidl)
			return false;
		
		if(FALSE==::SHGetPathFromIDList(m_pidl,m_tszFolderName)){
			ReleasePIDL();
			return false;	//should free m_pidl also
		}

		ReleasePIDL();
		return true;
	}
	LPTSTR FolderName(){
		return m_tszFolderName;
	}
protected:
	BROWSEINFO		m_bi;
	LPCITEMIDLIST 	m_pidl;
	TCHAR*			m_tszFolderName;
private:
	void ReleasePIDL(){
		if(m_pidl){
			//LPMALLOC pMalloc;
			//if(S_OK==::SHGetMalloc(&pMalloc)){
			//	pMalloc->Free((void*)m_pidl);
			//	pMalloc->Release();
			//	m_pidl=0;
			//}
			::CoTaskMemFree((void*)m_pidl);
			m_pidl=0;
		}
	}
};

class KFolderDialog : public KSelectFolderDlg<KFolderDialog>{
public:
	KFolderDialog(HWND hWndOwner=0,LPCTSTR tszTitle=0):KSelectFolderDlg<KFolderDialog>(hWndOwner,tszTitle){}
};

#define FILENAMEBUFFER	512
template<typename T,typename TBase=KWindow>
class KFileDialog : public KWindowRoot<T,TBase>{
public:
	KFileDialog(HWND hWndOwner=0,LPCTSTR tszFilter=0,LPCTSTR tszTitle=0,DWORD dwFlags=0,bool bOpenFileDialog=true){
		::memset(&m_ofn,'\0',sizeof(OPENFILENAME));

		m_tszFileBuffer		=new TCHAR[FILENAMEBUFFER];
		::memset(m_tszFileBuffer,'\0',FILENAMEBUFFER);

		m_ofn.lStructSize	=sizeof(OPENFILENAME);
		m_ofn.hwndOwner		=hWndOwner;
		m_ofn.lpstrFilter	=tszFilter;
		m_ofn.lpstrTitle	=tszTitle;
		m_ofn.Flags			=dwFlags;
		m_ofn.lpstrFile		=m_tszFileBuffer;
		m_ofn.nMaxFile		=FILENAMEBUFFER;

		m_bOpenFileDialog	=bOpenFileDialog;
		
	}
	~KFileDialog(){SAFE_DEL_PTRS(m_tszFileBuffer);}

	bool DoModal(){
		bool bRet=false;
		if(m_bOpenFileDialog)
			bRet=(0!=::GetOpenFileName(&m_ofn));
		else
			bRet=(0!=::GetSaveFileName(&m_ofn));
		
		return bRet;
	}
	void SetDefExt(TCHAR* tszDefExt){
		assert(tszDefExt);
		m_ofn.lpstrDefExt=tszDefExt;
	}
	LPTSTR FullFileName(){
		return m_tszFileBuffer;
	}
	TCHAR* FileName(){
		return m_tszFileBuffer+m_ofn.nFileOffset;
	}
	TCHAR* FileNameNoExt(){	//caller realse the memory
		int FileNameLen=m_ofn.nFileExtension-m_ofn.nFileOffset-1;
		TCHAR* tszName=new TCHAR[FileNameLen+1];
		_tcsncpy(tszName,m_tszFileBuffer+m_ofn.nFileOffset,FileNameLen);
		tszName[FileNameLen]=_T('\0');
		return tszName;
	}
	LPTSTR FileExt(){
		LPTSTR tszExt=0;
		if(m_ofn.nFileExtension)
			tszExt=m_tszFileBuffer+m_ofn.nFileExtension;
		return tszExt;
	}
	TCHAR* FilePath(){	//caller release the memory
		TCHAR* tszPath=new TCHAR[m_ofn.nFileOffset];
		_tcsncpy(tszPath,m_tszFileBuffer,m_ofn.nFileOffset);
		tszPath[m_ofn.nFileOffset-1]=_T('\0');
		return tszPath;
	}
protected:
	OPENFILENAME	m_ofn;
private:
	bool		m_bOpenFileDialog;
	TCHAR*		m_tszFileBuffer;
};

class KOpenFileDialog : public KFileDialog<KOpenFileDialog>{
public:
	KOpenFileDialog(HWND hWndOwner=0,LPCTSTR tszFilter=0,LPCTSTR tszTitle=0,DWORD dwFlags=0)
		:KFileDialog<KOpenFileDialog>(hWndOwner,tszFilter,tszTitle,dwFlags){
	}
};
class KSaveFileDialog : public KFileDialog<KSaveFileDialog>{
public:
	KSaveFileDialog(HWND hWndOwner=0,LPCTSTR tszFilter=0,LPCTSTR tszTitle=0,DWORD dwFlags=OFN_OVERWRITEPROMPT)
		:KFileDialog<KSaveFileDialog>(hWndOwner,tszFilter,tszTitle,dwFlags,false){
	}
};

} //namespace kwinui
#endif //__KCMNDLG_H__
