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
 * 12/09/2006
 */
#include "ktypes.h"
 
namespace kwinui{

KSize::KSize(){cx=0;cy=0;}
KSize::KSize(int nX,int nY){cx=nX;cy=nY;}
KSize::KSize(const SIZE& size){cx=size.cx;cy=size.cy;}
KSize::KSize(POINT& pt){cx=pt.x;cy=pt.y;}

KPoint::KPoint(){x=0;y=0;}
KPoint::KPoint(long nX,long nY){x=nX;y=nY;}

void KPoint::Offset(long nX,long nY){x+=nX;y+=nY;}

KRect::KRect(){left=top=right=bottom=0;};
KRect::KRect(int l,int t,int r,int b){left=l;top=t;right=r;bottom=b;}
KRect::KRect(const RECT& rect){ left=rect.left; top=rect.top; right=rect.right; bottom=rect.bottom;}

int KRect::Width(void){return right-left;}
int KRect::Height(void){return bottom-top;}
void KRect::OffsetRect(int nX,int nY){left+=nX;right+=nX;top+=nY;bottom+=nY;}
void KRect::Offset(int nX,int nY){left+=nX;right+=nX;top+=nY;bottom+=nY;}
void KRect::OffsetRect(const POINT& pt){left+=pt.x;right+=pt.x;top+=pt.y;bottom+=pt.y;}
void KRect::Offset(const POINT& pt){left+=pt.x;right+=pt.x;top+=pt.y;bottom+=pt.y;}

void KRect::DeflateRect(int x,int y){left+=x;right-=x;top+=y;bottom-=y;}
void KRect::DeflateRect(SIZE size){left+=size.cx;right-=size.cx;top+=size.cy;bottom-=size.cy;}

POINT KRect::CenterPoint(){
	POINT pt;
	pt.x=(left+right)/2;
	pt.y=(top+bottom)/2;
	return pt;
}
bool KRect::PtInRect(POINT pt){return 0!=::PtInRect(*this,pt);}
} //namespace kwinui
