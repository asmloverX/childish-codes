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
 */

#ifndef __KTHREAD_H__
#define __KTHREAD_H__

#ifdef __BORLANDC__
	#ifndef __MT__
		#define __MT__
	#endif //__MT__
#endif //__BORLANDC__ 

#define WM_EX_IDLE	(WM_APP+10000)

#include "kdef.h"
#include "kthunk.h"
#include "tchar.h"
#include "windows.h"
#include "process.h"
#include <cassert>
 
namespace kwinui{


class KThread{
public:
	KThread():m_ulThreadID(0),m_hThread(0),m_nRetCode(0){}
	operator HANDLE(){return m_hThread;}		
	operator unsigned long(){return (unsigned long)m_ulThreadID;}

	unsigned long SuspendThread(){return ::SuspendThread(m_hThread);}
	unsigned long ResumeThread(){return ::ResumeThread(m_hThread);}
	bool Terminate(unsigned long ulExitCode){return 0!=::TerminateThread(m_hThread,ulExitCode);}
	bool GetExitCode(unsigned long* pulExitCode){return 0!=::GetExitCodeThread(m_hThread,pulExitCode);}
	unsigned long WaitFor(unsigned long ulMillisecs=INFINITE){return ::WaitForSingleObject(m_hThread,ulMillisecs);}
	bool CloseHandle(){return 0!=::CloseHandle(m_hThread);}
	unsigned int GetThreadID(){return m_ulThreadID;}

	bool PostThreadMessage(UINT uMsg,WPARAM wParam=0,LPARAM lParam=0){
		return 0!=::PostThreadMessage(m_ulThreadID,uMsg,wParam,lParam);
	}
	bool PostThreadMessageUntilSuccess(UINT uMsg,WPARAM wParam=0,LPARAM lParam=0){
		bool bPostSucess=PostThreadMessage(uMsg,wParam,lParam);
		while(!bPostSucess)
			bPostSucess=PostThreadMessage(uMsg,wParam,lParam);
		return bPostSucess;
	}
protected:
	unsigned int	m_ulThreadID;
	HANDLE			m_hThread;
	int 			m_nRetCode;
};

template<typename T,typename TBase=KThread>
class  KThreadRoot : public TBase{
public:
	KThreadRoot():m_hWndMain(0){}
	unsigned int Run(){
		T* pT=static_cast<T*>(this);

		if(pT->InitInstance())
			pT->MessageLoop();

		pT->ExitInstance();
		return m_nRetCode;
	}
	void MessageLoop(){
		MSG msg;

		T* pT=static_cast<T*>(this);
		for(;;){
			while(!::PeekMessage(&msg,NULL,0,0,PM_NOREMOVE))
				if(!pT->OnIdle())
					::WaitMessage();
			do{
				if(!::GetMessage(&msg,NULL,0,0)){
					m_nRetCode=(int)msg.wParam;
					return;
				}
				if(msg.hwnd){
					::TranslateMessage(&msg);
					::DispatchMessage(&msg);
				}else
					pT->OnThreadMessage(&msg);
			}while(::PeekMessage(&msg,NULL,0,0,PM_NOREMOVE));
		}
	}

	template<typename func_t>
	INT_PTR GetThreadProcPtr(){	
		union{
			func_t	 	proc;
			INT_PTR		dwProcAddr;			
		}u;
		u.proc=&T::Execute;	
		return u.dwProcAddr;
	}

	bool InitInstance(){return false;}
	void ExitInstance(){}
	bool OnIdle(){
		if(m_hWndMain)
			return 0!=::SendMessage(m_hWndMain,WM_EX_IDLE,0,0);
		return false;
	}
	void OnThreadMessage(MSG* pMsg){}
protected:
	HWND			m_hWndMain;
	KGeneralThunk	m_thunk;
};

template<typename T,typename TBase=KThread>
class KThreadImpl : public KThreadRoot<T,TBase>{
public:
	KThreadImpl(unsigned int nInitFlag=0,void* pParams=0,unsigned int stack_size=0,void *security=0){
		T* pT=static_cast<T*>(this);

		typedef unsigned int (KCALLBACK T::*ThreadProc)(void*);	
		m_thunk.Init((INT_PTR)pT, pT->GetThreadProcPtr<ThreadProc>());

		m_hThread=(HANDLE)::_beginthreadex(security,stack_size,
					(unsigned int (__stdcall *)(void *))m_thunk.GetThunkedCodePtr(),
					pParams,nInitFlag,&m_ulThreadID);
	}
	unsigned int KCALLBACK Execute(void* pParam){
		m_pThreadParams=pParam;

		T* pT=static_cast<T*>(this);
		return pT->Run();
	}
protected:
	void* m_pThreadParams;
};

class KWinThread : public KThreadImpl<KWinThread>{};

#define BEGIN_THREADMSG_MAP	\
	void ProcessThreadMessage(UINT uMsg,WPARAM wParam,LPARAM lParam){ 

#define THREADMSG_HANDLER(msg,func)	\
	if(uMsg==msg)			\
		return func(wParam,lParam);

#define END_THREADMSG_MAP	\
	return;}

template<typename T,typename TBase=KThread>
class KThreadBase : public KThreadImpl<T,TBase>{
public:
	KThreadBase(void *security=0,unsigned int stack_size=0,void* pParams=0,unsigned int nInitFlag=0)
		:KThreadImpl<T,TBase>(security,stack_size,pParams,nInitFlag){}

	void ProcessThreadMessage(UINT uMsg,WPARAM wParam,LPARAM lParam){
		return;
	}
	void MessageLoop(){
		MSG msg;

		T* pT=static_cast<T*>(this);
		while(::GetMessage(&msg,NULL,0,0))
			pT->ProcessThreadMessage(msg.message,msg.wParam,msg.lParam);
		
		m_nRetCode=(int)msg.wParam;
	}
};

template<typename T,typename TBase=KThread>
class KThreadWithMsgQueue : public KThreadImpl<T,TBase>{
public:
	typedef KThreadImpl<T,TBase> __base;
	KThreadWithMsgQueue(HANDLE hEventCanPostMsg,unsigned int nInitFlag=0,void* pParams=0,
			unsigned int stack_size=0,void *security=0)
		:m_hEventCanPostMsg(hEventCanPostMsg),__base(nInitFlag,pParams,stack_size,security){}

	unsigned int KCALLBACK Execute(void* pParam){
		MSG msg;
		::PeekMessage(&msg,NULL,0,0,PM_NOREMOVE);
		if(::SetEvent(m_hEventCanPostMsg)!=0)
			return 0;
		return -1;
	}
private:
	HANDLE m_hEventCanPostMsg;
};
} //namespace kwinui
#endif //__KTHREAD_H__
