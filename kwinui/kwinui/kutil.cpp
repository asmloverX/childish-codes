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
#include "kutil.h"
 
namespace kwinui{
#pragma warning(disable: 4996 4311 4312)
                               
//char convert routine
//caller release the resource allocated
char* WCharToChar(wchar_t* wszInput,bool bUTF_8){	//caller release the resource
	assert(wszInput);
	int cchOutput=0;
	if(bUTF_8)
		cchOutput=::WideCharToMultiByte(CP_UTF8,0,wszInput,-1,NULL,0,0,0);
	else
		cchOutput=::WideCharToMultiByte(CP_ACP,0,wszInput,-1,NULL,0,0,0);

	if(!cchOutput)
		return 0;

	char* szTmp=new char[cchOutput];
	if(!szTmp)
		return 0;
	
	int ret=0;
	if(bUTF_8)
		ret=::WideCharToMultiByte(CP_UTF8,0,wszInput,-1,szTmp,cchOutput,0,0);
	else
		ret=::WideCharToMultiByte(CP_ACP,0,wszInput,-1,szTmp,cchOutput,0,0);

	if(!ret){
		delete[] szTmp;
		return 0;
	}
	return szTmp;
};

wchar_t* CharToWChar(const char* szInput,bool bUTF_8){	//caller release the resource
	assert(szInput);
	int cchOutput=0;
	if(bUTF_8)
		cchOutput=::MultiByteToWideChar(CP_UTF8,0,szInput,-1,NULL,0);
	else
		cchOutput=::MultiByteToWideChar(CP_ACP,0,szInput,-1,NULL,0);
	if(!cchOutput)
		return 0;

	wchar_t* wszTmp=new wchar_t[cchOutput];
	if(!wszTmp)
		return 0;

	int ret=0;
	if(bUTF_8)
		ret=::MultiByteToWideChar(CP_UTF8,0,szInput,-1,wszTmp,cchOutput);
	else
		ret=::MultiByteToWideChar(CP_ACP,0,szInput,-1,wszTmp,cchOutput);
	if(!ret){
		delete[] wszTmp;
		return 0;
	}
	return wszTmp;
};

//tchar convert function
wchar_t* TCharToWChar(TCHAR* tszInput){	//caller release the resource 
	assert(tszInput);
#ifdef _UNICODE
	wchar_t* wszTmp=new wchar_t[wcslen(tszInput)+1];
	if(!wszTmp)
		return 0;

	::wcscpy(wszTmp,tszInput);	
	return wszTmp;
#else
	return CharToWChar(tszInput);
#endif
};

char* TCharToChar(TCHAR* tszInput,bool bUTF_8){	//caller release the resource
	assert(tszInput);
#ifdef _UNICODE
	return WCharToChar(tszInput,bUTF_8);
#else
	char* szTmp=new char[strlen(tszInput)+1];
	if(!szTmp)
		return 0;

	::strcpy(szTmp,tszInput);
	return szTmp;
#endif
};

TCHAR* CharToTChar(const char* szInput,bool bUTF_8){	//caller release the resource
	assert(szInput);
#ifdef _UNICODE
	return CharToWChar(szInput,bUTF_8);
#else
	char* szTmp=0;
	if(bUTF_8){
		wchar_t* tszTmp=CharToWChar(szInput,true);
		szTmp=WCharToChar(tszTmp,false);
		delete tszTmp;
	}else{
		szTmp=new char[strlen(szInput)+1];
		if(!szTmp)
			return 0;

		::strcpy(szTmp,szInput);
	}
	return szTmp;
#endif
};

TCHAR* WCharToTChar(wchar_t* wszInput){		//caller release the resource
	assert(wszInput);
#ifdef _UNICODE
	wchar_t* wszTmp=new wchar_t[wcslen(wszInput)+1];
	if(!wszTmp)
		return 0;

	::wcscpy(wszTmp,wszInput);	
	return wszTmp;
#else
	return WCharToChar(wszInput);
#endif
};

TCHAR* itoa(int n, int radix){
	TCHAR* 		p;
	int 		minus;
	static TCHAR 	buf[36];

	p = &buf[36];
	*--p = _T('\0');

	if (n < 0){
		minus = 1;
	  	n = -n;
	}
	else
		minus = 0;

	if (n == 0)
		*--p =_T('0');
	else
		while (n>0) {
	   		*--p = _T("0123456789ABCDEF")[n % radix];
	   		n /= radix;
	  	}
	if (minus)
		*--p = _T('-');
	return p;
}

TCHAR* itoa(int n,int radix,TCHAR* tszOut){
	TCHAR* 	p;
	int 	minus=0;
	TCHAR	buf[36];
	TCHAR*	retval=tszOut;

	p=&buf[36];
	*--p=_T('\0');//0x00;

	if(n<0){
		minus=1;
		n=-n;
	}

	if(!n)
		*--p=_T('0');
	else
		while(n>0){
			*--p=_T("0123456789ABCDEF")[n % radix];
			n/=radix;
		}

	if(minus)
		*--p=_T('-');
	while(*tszOut++=*p++);
	return retval;
}

int atoi(const TCHAR* s/*,int radix*/){
	if(!s)
		return 0;
	int value=0,len=0,base=1,idx=0;
	const TCHAR* p=s;

	while(len++,*s++);
	len--;
	if(!len)
		return 0;

	for(idx=len-1;idx>0;--idx){
		value+=(p[idx]-_T('0'))*base;
		base*=10;
	}
	if(p[idx]=='-')
		value*=-1;
	else
		value+=(p[idx]-_T('0'))*base;

	return value;
}

TCHAR* GetModuleFilePath(TCHAR* tszFilePath,HMODULE hModule){
	TCHAR tszFileName[MAX_PATH];
	if(!::GetModuleFileName(hModule,tszFileName,MAX_PATH))
		return 0;
	
	TCHAR* tsztmp;
	if(tsztmp=::_tcsrchr(tszFileName,_T('\\'))){
		*tsztmp=_T('\0');
		::_tcscpy(tszFilePath,tszFileName);
		return tszFilePath;
	}
	return 0;
}

void CreateMultiPathDirect(TCHAR* tszFullPath){
	assert(tszFullPath);
	TCHAR* tszFind=0;
	TCHAR* tszSubpath=new TCHAR[::_tcslen(tszFullPath)+1];

	if(tszFind=::_tcschr(tszFullPath,_T('\\'))){
		while(tszFind=::_tcschr(tszFind+1,_T('\\'))){
			::_tcsncpy(tszSubpath,tszFullPath,tszFind-tszFullPath);
			tszSubpath[tszFind-tszFullPath]=_T('\0');
			::CreateDirectory(tszSubpath,NULL);
		}
		::CreateDirectory/*_tmkdir*/(tszFullPath,NULL);
	}
}

TCHAR* FormatTime_mm_ss(TCHAR* tszTime,int nSeconds){
	assert(nSeconds>=0 && tszTime);
	int nHours=0,nMinutes=0,nSecs=0;

	nHours	=nSeconds/3600;
	nMinutes=nSeconds%3600;

	nSecs	=nMinutes%60;
	nMinutes/=60;

	::_stprintf(tszTime,_T("%02d:%02d"),nMinutes,nSecs);	
	return tszTime;
}
TCHAR* FormatTime_mm_ss_mmm(TCHAR* tszTime,int nMilliSeconds){
	assert(nMilliSeconds>=0 && tszTime);

	int nHours=0,nMinutes=0,nSecs=0,nMiSecs;
	nSecs	=nMilliSeconds/1000;
	nMiSecs	=nMilliSeconds%1000;	

	nHours	=nSecs/3600;
	nMinutes=nSecs%3600;

	nSecs	=nMinutes%60;
	nMinutes/=60;

	::_stprintf(tszTime,_T("%02d:%02d.%03d"),nMinutes,nSecs,nMiSecs);	
	return tszTime;
}

TCHAR* FormatTime_hh_mm_ss(TCHAR* tszTime,int nSeconds){
	assert(nSeconds>=0 && tszTime);

	int nHours=0,nMinutes=0,nSecs=0;

	nHours	=nSeconds/3600;
	nMinutes=nSeconds%3600;

	nSecs	=nMinutes%60;
	nMinutes/=60;

	::_stprintf(tszTime,_T("%02d:%02d:%02d"),nHours,nMinutes,nSecs);	
	return tszTime;
}
bool FormatTimeGreatThan(TCHAR* tszSrcTime,TCHAR* tszDstTime){
	assert(tszSrcTime && tszDstTime);
	return (::_tcscmp(tszSrcTime,tszDstTime) > 0); 
}
int DecodeTime(TCHAR* tszTime){	//milliseconds
	assert(tszTime);
	int nSecs=0;
	TCHAR tszTmp[3];
	::_tcsncpy(tszTmp,tszTime,2);
	nSecs+=_ttoi(tszTmp)*3600;
	::_tcsncpy(tszTmp,tszTime+3,2);
	nSecs+=_ttoi(tszTmp)*60;
	::_tcsncpy(tszTmp,tszTime+6,2);
	nSecs+=_ttoi(tszTmp);

	return nSecs*1000;
} 
} //namespace kwinui
