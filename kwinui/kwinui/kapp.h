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
 *  app entry point
 *
 *  02/06/2007
 *  author:proguru
 *  modify 02/15/2008
 */

#ifndef __KAPP_H__
#define __KAPP_H__

#include "kthread.h"
#include "tchar.h"
#include "windows.h"
#include "commctrl.h"
#pragma comment(lib,"comctl32.lib")
 
namespace kwinui{

typedef int (KCALLBACK* main_t)(int);
main_t g_realmain=0;

extern "C" int WINAPI _tWinMain(HINSTANCE,HINSTANCE,LPTSTR,int nCmdShow){
	INITCOMMONCONTROLSEX icc;
	memset(&icc,0,sizeof(INITCOMMONCONTROLSEX));
	icc.dwSize	=sizeof(INITCOMMONCONTROLSEX);
	icc.dwICC	=ICC_WIN95_CLASSES;
	::InitCommonControlsEx(&icc);
	
	return g_realmain(nCmdShow);
}

template<typename T,typename TBase=KThread>
class KWinApp : public KThreadRoot<T,TBase>{
public:
	KWinApp(){
		T* pT=static_cast<T*>(this);

		typedef int (KCALLBACK T::*MainThreadProc)(int);	
		m_thunk.Init((INT_PTR)pT, pT->GetThreadProcPtr<MainThreadProc>());
		g_realmain=(main_t)m_thunk.GetThunkedCodePtr();

		m_hThread=::GetCurrentThread();
		m_ulThreadID=::GetCurrentThreadId();
	}
	~KWinApp(){
		if(g_hHeapExecutable){
			::HeapDestroy(g_hHeapExecutable);
			g_hHeapExecutable=0;
		}
	}
	int KCALLBACK Execute(int nCmdShow){
		m_nCmdShow=nCmdShow;

		T* pT=static_cast<T*>(this);
		return pT->Run();
	}
protected:
	int	m_nCmdShow;
};

} //namespace kwinui
#endif //__KAPP_H__
