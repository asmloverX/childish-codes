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
 * 	12/09/2006
 *	proguru
 *
 * 	add windows X64 support,01/27/2008
 */

#ifndef __KDEF_H__
#define __KDEF_H__

#define WIN32_LEAN_AND_MEAN
#include "windows.h"

#ifndef __in
	#define __in	//modifier for input parameters
#endif
#ifndef __out
	#define __out  //modifier for output parameters
#endif

#ifndef __inout
	#define __inout
#endif

#ifdef __BORLANDC__

#ifndef STRICT
	#define STRICT			//turn it on for c++ builder compatibility
#endif	//STRICT		

#define NOMINMAX

#ifndef max
	#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif	//max

#ifndef min
	#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

#ifndef LONG_MIN
	#define LONG_MIN        (-2147483647L-1)
#endif

#ifndef LONG_MAX
	#define LONG_MAX        2147483647L
#endif

#endif	//__BORLANDC__

#ifdef _WIN32_WINNT
	#undef _WIN32_WINNT
#endif
#define _WIN32_WINNT 0x0500

#ifndef SAFE_DEL_PTR
	#define SAFE_DEL_PTR(p)		\
		if(p){			\
			delete p;	\
			p=0;		\
		}
#endif	//SAFE_DEL_PTR

#ifndef SAFE_DEL_PTRS
	#define SAFE_DEL_PTRS(p)	\
		if(p){			\
			delete[] p;	\
			p=0;		\
		}			

#endif	//SAFE_DEL_PTRS

#ifndef SAFE_RELEASE
	#define SAFE_RELEASE(p) 	\
        	if(p){         		\
            	p->Release();	\
            	p=0;           	\
        	}
#endif //SAFE_RELEASE

#ifndef SAFE_DEL_OBJ
	#define SAFE_DEL_OBJ(obj)	\
		if(obj){				\
			::DeleteObject(obj);\
			obj=0;				\
		}						
#endif	//SAFE_DEL_OBJ

//IDC_STATIC,for all static controls
#ifndef IDC_STATIC
	#define IDC_STATIC (-1)
#endif //IDC_STATIC

#define RECT_W(rc) ((rc).right-(rc).left)
#define RECT_H(rc) ((rc).bottom-(rc).top)

#ifndef _WIN64
	#ifdef SetWindowLongPtrA
	#undef SetWindowLongPtrA
	inline LONG_PTR SetWindowLongPtrA(HWND hWnd,int nIndex,LONG_PTR dwNewLong){
		return(::SetWindowLongA(hWnd,nIndex,LONG(dwNewLong)));
	}
	#endif

	#ifdef SetWindowLongPtrW
	#undef SetWindowLongPtrW
	inline LONG_PTR SetWindowLongPtrW(HWND hWnd, int nIndex, LONG_PTR dwNewLong){
		return(::SetWindowLongW(hWnd,nIndex,LONG(dwNewLong)));
	}
	#endif

	#ifdef GetWindowLongPtrA
	#undef GetWindowLongPtrA
	inline LONG_PTR GetWindowLongPtrA(HWND hWnd,int nIndex){
		return(::GetWindowLongA(hWnd,nIndex));
	}
	#endif

	#ifdef GetWindowLongPtrW
	#undef GetWindowLongPtrW
	inline LONG_PTR GetWindowLongPtrW(HWND hWnd,int nIndex){
		return(::GetWindowLongW(hWnd, nIndex));
	}
	#endif
	#ifdef GetNextWindow
	#undef GetNextWindow
	inline HWND GetNextWindow(HWND hWnd,UINT wCmd){
		return ::GetWindow(hWnd,wCmd);
	}
	#endif
#endif //_WIN64

#endif //__KDEF_H__
