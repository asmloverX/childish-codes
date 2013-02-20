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
 *  12/09/2006
 */
#ifndef __KMISC_H__
#define __KMISC_H__

#include "windows.h"
 
namespace kwinui{

//draw circle
COLORREF GetColour(double dAngle, COLORREF crBright, COLORREF crDark);
void DrawCircle(HDC hDC, POINT p, LONG lRadius, COLORREF crColour, bool bDashed=false);
void DrawCircle(HDC hDC, POINT p, LONG lRadius, COLORREF crBright, COLORREF crDark);

} //namespace kwinui
#endif //__KMISC_H__
