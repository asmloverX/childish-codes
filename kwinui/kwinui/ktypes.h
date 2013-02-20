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
 *	windows struct wrap
 *
 * 12/09/2006
 */

#ifndef __KTYPES_H__
#define __KTYPES_H__
#include "windows.h"
 
namespace kwinui{

class KSize : public SIZE{
public:
	KSize();
	KSize(int nX,int nY);
	KSize(const SIZE& size);
	KSize(POINT& pt);

	inline KSize& operator=(const SIZE& size){cx=size.cx;cy=size.cy;return *this;}
	inline operator SIZE(){return static_cast<SIZE>(*this);}
	inline operator SIZE*(){return dynamic_cast<SIZE*>(this);}

	//overload other operator
};

class KPoint : public POINT{
public:
	KPoint();
	KPoint(long nX,long nY);

	inline KPoint& operator=(const POINT& pt){x=pt.x;y=pt.y;return *this;}
	inline operator POINT(){return static_cast<POINT>(*this);}
	inline operator POINT*(){return dynamic_cast<POINT*>(this);}

	inline KPoint& operator-(POINT& pt){x-=pt.x;y-=pt.y;return *this;}

	void Offset(long nX,long nY);
};

class KRect : public RECT{
public:
	KRect();
	KRect(int l,int t,int r,int b);
	KRect(const RECT& rect);

	inline KRect& operator=(const RECT& rect){
		left=rect.left;top=rect.top;right=rect.right;bottom=rect.bottom;return *this;}
	inline operator RECT(){return static_cast<RECT>(*this);}
	inline operator RECT*(){return dynamic_cast<RECT*>(this);}

	int Width(void);
	int Height(void);
	void OffsetRect(int nX,int nY);
	void Offset(int nX,int nY);
	void OffsetRect(const POINT& pt);
	void Offset(const POINT& pt);

	void DeflateRect(int x,int y);
	void DeflateRect(SIZE size);

	POINT CenterPoint();
	bool PtInRect(POINT pt);
};

} //namespace kwinui
#endif //__KTYPES_H__
