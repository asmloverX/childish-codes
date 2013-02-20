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
 *
 *
 *
 */
#define _CRT_SECURE_NO_WARNINGS
#include "KDlgTemplateBuilder.h"
#include "kutil.h"
#include <cassert>
 
namespace kwinui{

//start of KDlgTemplateBuilder
//Helper class to build a dialog template in memory.
KDlgTemplateBuilder::KDlgTemplateBuilder(UINT nBufLen){
	m_pBuffer   =new WORD[nBufLen];
	m_pNext     =m_pBuffer;
	m_pEndBuf   =m_pNext + nBufLen;
}
KDlgTemplateBuilder::~KDlgTemplateBuilder(){delete[] m_pBuffer;}

DLGTEMPLATE* KDlgTemplateBuilder::GetTemplate(){return (DLGTEMPLATE*)m_pBuffer;}

// functions to build the template
DLGTEMPLATE* KDlgTemplateBuilder::Begin(DWORD dwStyle, const RECT& rc,LPTSTR tszCaption,DWORD dwStyleEx,
		WORD wFontSize,LPTSTR tszTypeFace){
	assert(m_pBuffer==m_pNext);			// call Begin first and only once!

	DLGTEMPLATE* hdr	=(DLGTEMPLATE*)m_pBuffer;
	hdr->style 			= dwStyle;		// copy style..
	hdr->dwExtendedStyle= dwStyleEx; 	// ..and extended, too
	hdr->cdit 			= 0;			// number of items: zero
	
	hdr->x	=(short)rc.left;			// Set dialog rectangle.
	hdr->y	=(short)rc.top;
	hdr->cx =(short)(rc.right-rc.left);
	hdr->cy =(short)(rc.bottom-rc.top);

	// Append trailing items: menu, class, caption. I only use caption.
	m_pNext		=(WORD*)(hdr+1);
	*m_pNext++	=0;				// menu (none)
	*m_pNext++ 	=0;				// dialog class (use standard)
	m_pNext 	=AddText(m_pNext,tszCaption);	// append dialog caption
	
	//append font size and typeface
	if(dwStyle & DS_SETFONT){
		assert(wFontSize && tszTypeFace);
		*m_pNext++	=wFontSize;
		m_pNext		=AddText(m_pNext,tszTypeFace);
	}

	assert(m_pNext < m_pEndBuf);
	return hdr;
}
void KDlgTemplateBuilder::AddItem(LPTSTR tszClass, DWORD dwStyle, const RECT& rc,LPTSTR text,WORD nID,DWORD dwStyleEx){
	assert(m_pNext < m_pEndBuf);

	DLGITEMTEMPLATE& it = *((DLGITEMTEMPLATE*)AlignDWORD(m_pNext));	//initialize DLGITEMTEMPLATE 
	it.style 			=dwStyle;
	it.dwExtendedStyle 	=dwStyleEx;

	it.x  = (short)rc.left;
	it.y  = (short)rc.top;
	it.cx = (short)(rc.right-rc.left);
	it.cy = (short)(rc.bottom-rc.top);
	it.id = nID;
	
	m_pNext = (WORD*)(&it+1);
	//the pointer address below 0x10000 is invalidated on win32,so...
	if(reinterpret_cast<unsigned int>(tszClass) <= 0xFFFF){		// add class
		*m_pNext++ = 0xFFFF;									// next WORD is atom
		*m_pNext++ = (WORD)tszClass;							// ..atom identifier
	}else
		m_pNext=AddText(m_pNext,tszClass);

	if(text){
		if(reinterpret_cast<unsigned int>(text) <= 0xFFFF){	// append title
			*m_pNext++ 	=0xFFFF;							// next is resource id
			*m_pNext++ 	=(WORD)text;	
		}else
			m_pNext		=AddText(m_pNext, text);	 
	}else		//have not a title
		*m_pNext++=0;

	*m_pNext++ 	=0;					// no creation data

	DLGTEMPLATE* hdr = (DLGTEMPLATE*)m_pBuffer;	// increment control/item count
	hdr->cdit++;

	assert(m_pNext<m_pEndBuf);
}

//align ptr to nearest DWORD
WORD* KDlgTemplateBuilder::AlignDWORD(WORD* ptr){
	/*
	ptr++;						// round up to nearest DWORD
	LPARAM lp = (LPARAM)ptr;			// convert to long
	lp &= 0xFFFFFFFC;				// make sure on DWORD boundary
	return (WORD*)lp;
	*/
	ULONG   ul;  

	ul   =(ULONG)ptr;  
	ul   +=3;  
	ul   >>=2;  
	ul   <<=2;  
	return (WORD*)ul;  
}
// Append text to buffer. Convert to Unicode if necessary.
// Return pointer to next character after terminating NULL.
WORD* KDlgTemplateBuilder::AddText(WORD* buf,LPTSTR text){
	if(text){
		wchar_t* wszTmp=kwinui::TCharToWChar(text);
		::wcscpy((wchar_t*)buf,wszTmp);
		buf+=::wcslen((wchar_t*)buf)+1;
		delete[] wszTmp;
	}else
		*buf++=0;

	return buf;
}
} //namespace kwinui
//end of KDlgTemplateBuilder
