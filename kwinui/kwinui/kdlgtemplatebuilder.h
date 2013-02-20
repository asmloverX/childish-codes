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
 *	dialog template in memory
 *
 *
 */

#ifndef __KDLGTEMPLATEBUILDER_H__
#define __KDLGTEMPLATEBUILDER_H__
#include "windows.h"
 
namespace kwinui{

//Helper class to build a dialog template in memory.
class KDlgTemplateBuilder{
public:
	KDlgTemplateBuilder(UINT nBufLen=1024);
	~KDlgTemplateBuilder();
	
	enum{BUTTON=0x0080,EDIT,STATIC,LISTBOX,SCROLLBAR,COMBOBOX};	//Windows predefined atom names

	DLGTEMPLATE* GetTemplate();
	// functions to build the template
	DLGTEMPLATE* Begin(DWORD dwStyle, const RECT& rc,LPTSTR tszCaption,DWORD dwStyleEx=0,WORD wFontSize=0,
			LPTSTR tszTypeFace=0);
	void AddItem(LPTSTR tszClass, DWORD dwStyle, const RECT& rc,LPTSTR text,WORD nID=-1,DWORD dwStyleEx=0);
protected:
	WORD* m_pBuffer;						// internal buffer holds dialog template
	WORD* m_pNext;							// next WORD to copy stuff
	WORD* m_pEndBuf;						// end of buffer

	//align ptr to nearest DWORD
	WORD* AlignDWORD(WORD* ptr);
	// Append text to buffer. Convert to Unicode if necessary.
	// Return pointer to next character after terminating NULL.
	WORD* AddText(WORD* buf,LPTSTR text);
};
} //namespace kwinui
#endif //__KDLGTEMPLATEBUILDER_H__
