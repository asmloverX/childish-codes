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
 * 12/09/2006
 */

#ifndef __KUTIL_H__
#define __KUTIL_H__

//#pragma warning(disable : 8060)

#include "windows.h"
#include "tchar.h"
#include <cassert>
#include <stdio.h>
 
namespace kwinui{
	//char convert routine
	//caller release the resource returned 
	char* WCharToChar(wchar_t* wszInput,bool bUTF_8=false);
	wchar_t* CharToWChar(const char* szInput,bool bUTF_8=false);

	//tchar convert function
	//caller release the resource returned
	wchar_t* TCharToWChar(TCHAR* tszInput);
	char* TCharToChar(TCHAR* tszInput,bool bUTF_8=false);
	TCHAR* CharToTChar(const char* szInput,bool bUTF_8=false);
	TCHAR* WCharToTChar(wchar_t* wszInput);

	TCHAR* itoa(int n, int radix=10);
	TCHAR* itoa(int n,int radix,TCHAR* tszOut);
	int atoi(const TCHAR* s/*,int radix*/);
	inline bool isdigit(const TCHAR c){return (c>=_T('0') && c<=_T('9'));}

	TCHAR* GetModuleFilePath(TCHAR* tszFilePath,HMODULE hModule=0);
	void CreateMultiPathDirect(TCHAR* tszFullPath);

	//format seconds to mm:ss
	TCHAR* FormatTime_mm_ss(TCHAR* tszTime,int nSeconds);
	//format milliseconds to mm:ss.mmm
	TCHAR* FormatTime_mm_ss_mmm(TCHAR* tszTime,int nMilliSeconds);
	//format seconds to hh:mm:ss
	TCHAR* FormatTime_hh_mm_ss(TCHAR* tszTime,int nSeconds);
	bool FormatTimeGreatThan(TCHAR* tszSrcTime,TCHAR* tszDstTime);
	//decode hh:mm:ss to milliseconds
	int DecodeTime(TCHAR* tszTime);
} //namespace kwinui
#endif //__KUTIL_H__
