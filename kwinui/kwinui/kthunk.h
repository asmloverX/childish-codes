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
 *	thunk with DEP support
 *
 *	author:proguru
 *	July 9,2008
 */
/*
 *	modify x64 thunk code according to the feedback from Loaden 
 *	"http://topic.csdn.net/u/20090322/08/b6bf82ca-8ba2-452b-92f8-bb2adb05a1ef.html"
 *	maybe also "http://www.qpsoft.com/blog/x64-thunk-callback-conversion/"
 *	
 *  proguru 
 *	June 04,2009
 */

#ifndef __KTHUNK_H__
#define __KTHUNK_H__
#include "windows.h"
 
namespace kwinui{

//#define USE_THISCALL_CONVENTION	//turn it off for c++ builder compatibility

#ifdef USE_THISCALL_CONVENTION
	#define WNDPROC_THUNK_LENGTH  	29 	//For __thiscall calling convention ONLY,assign m_hWnd by thunk
	#define GENERAL_THUNK_LENGTH	10
	#define KCALLBACK 			   		//__thiscall is default
#else
	#define WNDPROC_THUNK_LENGTH   	22	//__stdcall calling convention ONLY,assign m_hWnd by thunk
	#define GENERAL_THUNK_LENGTH	16
	#define KCALLBACK __stdcall
#endif

#define WNDPROC_THUNK_LENGTH_X64	28
#define GENERAL_THUNK_LENGTH_X64	34

static HANDLE g_hHeapExecutable;

class KThunkBase{
public:
	KThunkBase(SIZE_T size){
		if(!g_hHeapExecutable){		//first thunk,create the executable heap
			g_hHeapExecutable=::HeapCreate(HEAP_CREATE_ENABLE_EXECUTE,0,0);
			//if (!g_hHeapExecutable) abort
		}
		m_szMachineCode=(unsigned char*)::HeapAlloc(g_hHeapExecutable,HEAP_ZERO_MEMORY,size);
	}
	~KThunkBase(){
		if(g_hHeapExecutable)
			::HeapFree(g_hHeapExecutable,0,(void*)m_szMachineCode);
	}
	inline void* GetThunkedCodePtr(){return &m_szMachineCode[0];}
protected:
	unsigned char*	m_szMachineCode;
};

class KWndProcThunk : public KThunkBase{
public:
#ifndef _WIN64
	KWndProcThunk():KThunkBase(WNDPROC_THUNK_LENGTH){}
#else	//_WIN64
	KWndProcThunk():KThunkBase(WNDPROC_THUNK_LENGTH_X64){}
#endif

	void Init(INT_PTR pThis, INT_PTR ProcPtr){
#ifndef _WIN64
		#pragma warning(disable: 4311)
		DWORD dwDistance =(DWORD)(ProcPtr) - (DWORD)(&m_szMachineCode[0]) - WNDPROC_THUNK_LENGTH;
		#pragma warning(default: 4311)

	#ifdef USE_THISCALL_CONVENTION
		/*
		For __thiscall, the default calling convention used by Microsoft VC++, The thing needed is
		just set ECX with the value of 'this pointer'

		machine code   					assembly instruction		comment
		---------------------------   	-------------------------	----------
		B9 ?? ?? ?? ??                	mov ecx, pThis 				;Load ecx with this pointer
		50								PUSH EAX			
		8B 44 24 08						MOV EAX, DWORD PTR[ESP+8] 	;EAX=hWnd
		89 01							MOV DWORD PTR [ECX], EAX  	;[pThis]=[ECX]=hWnd
		8B 44 24 04						mov eax,DWORD PTR [ESP+04H]	;eax=(return address)
		89 44 24 08						mov DWORD PTR [ESP+08h],eax	;hWnd=(return address)
		58								POP EAX			
	  	83 C4 04						add ESP,04h			
						
		E9 ?? ?? ?? ??                	jmp ProcPtr  				;Jump to target message handler
		*/
		m_szMachineCode[0] 				= 0xB9;
		*((DWORD*)&m_szMachineCode[1] ) =(DWORD)pThis;
		*((DWORD*)&m_szMachineCode[5] )	=0x24448B50;	
		*((DWORD*)&m_szMachineCode[9] )	=0x8B018908;
		*((DWORD*)&m_szMachineCode[13])	=0x89042444;
		*((DWORD*)&m_szMachineCode[17])	=0x58082444;
		*((DWORD*)&m_szMachineCode[21])	=0xE904C483;
		*((DWORD*)&m_szMachineCode[25]) =dwDistance;	
	#else	
		/*
		 * 01/26/2008 modify
		For __stdcall calling convention, replace 'HWND' with 'this pointer'

		Stack frame before inserting 			Stack frame after inserting

		:      ...      :            			:       ...      :
		|---------------|            			|----------------|
		|     lParam    |            			|     lParam     |
		|---------------|            			|----------------|
		|     wParam    |            			|     wParam     |
		|---------------|            			|----------------|
		|     uMsg  	|            			|     uMsg       |
		|---------------|            			|----------------|
		|     hWnd      |            			| <this pointer> |
		|---------------|            			|----------------|
		| (Return Addr) | <- ESP     			| (Return Addr)  | <-ESP
		|---------------|            			|----------------|
		:      ...      :            			:       ...      | 

		machine code		assembly instruction			comment	
		-------------------	----------------------------	--------------
		51					push ecx
		B8 ?? ?? ?? ??		mov  eax,pThis					;eax=this;
		8B 4C 24 08			mov  ecx,dword ptr [esp+08H]	;ecx=hWnd;
		89 08				mov  dword ptr [eax],ecx  		;[this]=hWnd,if has vftbl shound [this+4]=hWnd			
		89 44 24 08			mov  dword ptr [esp+08H], eax	;Overwite the 'hWnd' with 'this pointer'
		59					pop  ecx
		E9 ?? ?? ?? ??      jmp  ProcPtr                	; Jump to target message handler
		*/

		*((WORD  *) &m_szMachineCode[ 0]) = 0xB851;
		*((DWORD *) &m_szMachineCode[ 2]) = (DWORD)pThis;
		*((DWORD *) &m_szMachineCode[ 6]) = 0x08244C8B;
		*((DWORD *) &m_szMachineCode[10]) = 0x44890889;
		*((DWORD *) &m_szMachineCode[14]) = 0xE9590824;
		*((DWORD *) &m_szMachineCode[18]) = (DWORD)dwDistance;
	#endif //USE_THISCALL_CONVENTION
#else	//_WIN64
		/* 
		For x64 calling convention, RCX hold the 'HWND',copy the 'HWND' to Window object,
	    then insert 'this pointer' into RCX,so perfectly!!!		

		Stack frame before modify				Stack frame after modify

		:      ...      :            			:       ...      :
		|---------------|            			|----------------|
		|               | <-R9(lParam) 			|                | <-R9(lParam)
		|---------------|            			|----------------|
		|               | <-R8(wParam) 			|                | <-R8(wParam)
		|---------------|            			|----------------|
		|               | <-RDX(msg)   			|                | <-RDX(msg)
		|---------------|            			|----------------|
		|               | <-RCX(hWnd)  			|                | <-RCX(this)
		|---------------|            			|----------------|
		| (Return Addr) | <-RSP     			| (Return Addr)	 | <-RSP
		|---------------|            			|----------------|
		:      ...      :            			:	...	 :

		machine code			assembly instruction 	comment
		-------------------   	-----------------------	----
		48B8 ????????????????	mov RAX,pThis
		#4808					mov qword ptr [RAX],RCX	;m_hWnd=[this]=RCX
		488908					mov qword ptr [RAX],RCX	;m_hWnd=[this]=RCX //feedback from Loaden
		4889C1					mov RCX,RAX				;RCX=pThis (488BC8 ?)
		48B8 ????????????????	mov RAX,ProcPtr	
		FFE0					jmp RAX		
		*/
		*((WORD   *)&m_szMachineCode[0] )	=0xB848;
		*((INT_PTR*)&m_szMachineCode[2] )	=pThis;
		//*((DWORD  *)&m_szMachineCode[10])	=0x89480848;
		*((DWORD  *)&m_szMachineCode[10])	=0x48088948;
		//*((DWORD  *)&m_szMachineCode[14])	=0x00B848C1;
		*((DWORD  *)&m_szMachineCode[14])	=0xB848C189;
		//*((INT_PTR*)&m_szMachineCode[17])	=ProcPtr;
		*((INT_PTR*)&m_szMachineCode[18])	=ProcPtr;
		//*((WORD   *)&m_szMachineCode[25])	=0xE0FF;
		*((WORD   *)&m_szMachineCode[26])	=0xE0FF;
#endif
	}
};

class KGeneralThunk : public KThunkBase{
public:
#ifndef _WIN64
	KGeneralThunk():KThunkBase(GENERAL_THUNK_LENGTH){}
#else //_WIN64
	KGeneralThunk():KThunkBase(GENERAL_THUNK_LENGTH_X64){}
#endif

   	void Init(INT_PTR pThis, INT_PTR ProcPtr){
#ifndef _WIN64
		#pragma warning(disable: 4311)		
        	DWORD dwDistance = (DWORD)ProcPtr - (DWORD)&m_szMachineCode[0] - GENERAL_THUNK_LENGTH;
		#pragma warning(default: 4311)

	#ifdef USE_THISCALL_CONVENTION
		/*
		  For __thiscall, the default calling convention used by Microsoft VC++, The thing needed is
		  just set ECX with the value of 'this pointer'

		  (ONLY 10 byes machine code needed in this case)

		  machine code   		assembly instruction   	comment
		  --------------------  ----------------------	---------------
		  B9 ?? ?? ?? ??       	mov    ecx, pThis 		; Load ecx with this pointer
		  E9 ?? ?? ?? ??        jmp    ProcPtr  		; Jump to target message handler
		*/
		m_szMachineCode[0] 				= 0xB9;
		m_szMachineCode[5] 				= 0xE9;

		*((DWORD *) &m_szMachineCode[1])= (DWORD) pThis;
		*((DWORD *) &m_szMachineCode[6])= dwDistance;
	#else
		/* 
		   For __stdcall calling convention, insert 'this pointer' into stack

		   Stack frame before inserting 		Stack frame after inserting

		   :      ...      :          			:       ...      :
		   |---------------|          			|----------------|
		   |    		   |          			|     			 |
		   |---------------|          			|----------------|
		   |     		   |          			|     			 |
		   |---------------|          			|----------------|
		   |   		 	   |          			|   		     |
		   |---------------|          			|----------------|
		   |      	   	   |          			|      		 	 |
		   |---------------|           			|----------------|
		   | (Return Addr) | <- ESP    			| <this pointer> | <- New item inserted by this thunk code
		   |---------------|           			|----------------|
		   :      ...      :           			| (Return Addr)  | <- ESP
												|----------------|
												:       ...      :

		  (16 bytes machine code needed in this case)

		  machine code 					assembly instruction 			comment	
		  ---------------------------   ------------------------------	------
		  FF 34 24                      push  dword ptr [esp]      		; Save (or duplicate) the Return Addr into stack
		  C7 44 24 04 ?? ?? ?? ??       mov   dword ptr [esp+4], pThis 	; Overwite the old Return Addr with 'this'
		  E9 ?? ?? ?? ??                jmp   ProcPtr                	; Jump to target message handler
		*/

		m_szMachineCode[11] 				= 0xE9; 

		*((DWORD *) &m_szMachineCode[ 0]) 	= 0x002434FF;
		*((DWORD *) &m_szMachineCode[ 3]) 	= 0x042444C7;
		*((DWORD *) &m_szMachineCode[ 7]) 	= (DWORD) pThis;
		*((DWORD *) &m_szMachineCode[12]) 	= dwDistance;
	#endif	//USE_THISCALL_CONVENTION
#else	//_WIN64
		/* For x64 calling convention,insert 'this pointer' to RCX.
		 * Important: only for callback function with less than or equal THREE parameters,
		 * otherwise use _K_GENERAL_THUNKED_DATA_FOUR_PARAMETERS(not implementation yet!)
		 
		
		Stack frame before modify			Stack frame after modify

		:      ...      :            			:       ...      :
		|---------------|            			|----------------|
		|               | <-R8(third para)		|                | <-R9(third para)
		|---------------|            			|----------------|
		|               | <-RDX(sencond para)  	|                | <-R8(second para)
		|---------------|            			|----------------|
		|               | <-RCX(first para)  	|                | <-RDX(first para)
		|---------------|            			|----------------|
		| (Return Addr) | <-RSP     			|		       	 | <-RCX(this pointer)
		|---------------|            			|----------------|
		:      ...      :            			| (Return Addr)	 | <-RSP
												|----------------|
												:       ...      :	

		machine code			assembly instruction	comment 
		-------------------		--------------------	-------
		FF 34 24				push qword ptr [RSP]	
		4D 89 C1				mov  R9,R8
		49 89 D0				mov  R8,RDX
	 	48 89 CA				mov  RDX,RCX
		48 B9 ????????????????	mov  RCX,pThis	
		48B8  ????????????????	mov RAX,ProcPtr
		FFE0					jmp RAX		
		*/
		*((INT_PTR*)&m_szMachineCode[0] )	=0x8949C1894D2434FF;
		*((INT_PTR*)&m_szMachineCode[8] )	=0x0000B948CA8948D0;
		*((INT_PTR*)&m_szMachineCode[14])	=pThis;	
		*((WORD   *)&m_szMachineCode[22])	=0xB848;
		*((INT_PTR*)&m_szMachineCode[24])	=ProcPtr;	
		*((WORD   *)&m_szMachineCode[32])	=0xE0FF;
#endif	//_WIN64
    }
};
} //namespace kwinui
#endif //__KTHUNK_H__
