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


#include "kmisc.h"
#include <cmath>
#include <cassert>
 
namespace kwinui{
//draw circle
COLORREF GetColour(double dAngle, COLORREF crBright, COLORREF crDark){
#define Rad2Deg	180.0/3.1415 
#define LIGHT_SOURCE_ANGLE	-2.356		// -2.356 radians = -135 degrees, i.e. From top left

	assert(dAngle > -3.1416 && dAngle < 3.1416);
	double dAngleDifference = LIGHT_SOURCE_ANGLE - dAngle;

	if (dAngleDifference < -3.1415) dAngleDifference = 6.293 + dAngleDifference;
	else if (dAngleDifference > 3.1415) dAngleDifference = 6.293 - dAngleDifference;

	double Weight = 0.5*(::cos(dAngleDifference)+1.0);

	BYTE Red   = (BYTE) (Weight*GetRValue(crBright) + (1.0-Weight)*GetRValue(crDark));
	BYTE Green = (BYTE) (Weight*GetGValue(crBright) + (1.0-Weight)*GetGValue(crDark));
	BYTE Blue  = (BYTE) (Weight*GetBValue(crBright) + (1.0-Weight)*GetBValue(crDark));

	return RGB(Red, Green, Blue);
}

void DrawCircle(HDC hDC, POINT p, LONG lRadius, COLORREF crColour, bool bDashed){
	int nDashLength = 1;
	LONG lError, lXoffset, lYoffset;
	int  nDash = 0;
	bool bDashOn = TRUE;

	//Check to see that the coordinates are valid
	assert( (p.x + lRadius <= LONG_MAX) && (p.y + lRadius <= LONG_MAX) );
	assert( (p.x - lRadius >= LONG_MIN) && (p.y - lRadius >= LONG_MIN) );

	//Set starting values
	lXoffset = lRadius;
	lYoffset = 0;
	lError   = -lRadius;

	do {
		if (bDashOn) {
			::SetPixelV(hDC,p.x + lXoffset, p.y + lYoffset, crColour);
			::SetPixelV(hDC,p.x + lXoffset, p.y - lYoffset, crColour);
			::SetPixelV(hDC,p.x + lYoffset, p.y + lXoffset, crColour);
			::SetPixelV(hDC,p.x + lYoffset, p.y - lXoffset, crColour);
			::SetPixelV(hDC,p.x - lYoffset, p.y + lXoffset, crColour);
			::SetPixelV(hDC,p.x - lYoffset, p.y - lXoffset, crColour);
			::SetPixelV(hDC,p.x - lXoffset, p.y + lYoffset, crColour);
			::SetPixelV(hDC,p.x - lXoffset, p.y - lYoffset, crColour);
		}

		//Advance the error term and the ant X axis step
		lError += lYoffset++;

		//Check to see if error term has overflowed
		if ((lError += lYoffset) >= 0)
			lError -= --lXoffset * 2;

		if (bDashed && (++nDash == nDashLength)) {
			nDash = 0;
			bDashOn = !bDashOn;
		}

	} while (lYoffset <= lXoffset);	//Continue until halfway point
} 

void DrawCircle(HDC hDC, POINT p, LONG lRadius, COLORREF crBright, COLORREF crDark){
	LONG lError, lXoffset, lYoffset;

	//Check to see that the coordinates are valid
	assert( (p.x + lRadius <= LONG_MAX) && (p.y + lRadius <= LONG_MAX) );
	assert( (p.x - lRadius >= LONG_MIN) && (p.y - lRadius >= LONG_MIN) );

	//Set starting values
	lXoffset = lRadius;
	lYoffset = 0;
	lError   = -lRadius;

	do {
		 double Pi = 3.141592654, Pi_on_2 = Pi * 0.5;//Three_Pi_on_2 = Pi * 1.5;
		COLORREF crColour;
		double   dAngle = atan2((double)lYoffset,(double) lXoffset);

		//Draw the current pixel, reflected across all eight arcs
		crColour = GetColour(dAngle, crBright, crDark);
		::SetPixelV(hDC,p.x + lXoffset, p.y + lYoffset, crColour);

		crColour = GetColour(Pi_on_2 - dAngle, crBright, crDark);
		::SetPixelV(hDC,p.x + lYoffset, p.y + lXoffset, crColour);

		crColour = GetColour(Pi_on_2 + dAngle, crBright, crDark);
		::SetPixelV(hDC,p.x - lYoffset, p.y + lXoffset, crColour);

		crColour = GetColour(Pi - dAngle, crBright, crDark);
		::SetPixelV(hDC,p.x - lXoffset, p.y + lYoffset, crColour);

		crColour = GetColour(-Pi + dAngle, crBright, crDark);
		::SetPixelV(hDC,p.x - lXoffset, p.y - lYoffset, crColour);

		crColour = GetColour(-Pi_on_2 - dAngle, crBright, crDark);
		::SetPixelV(hDC,p.x - lYoffset, p.y - lXoffset, crColour);

		crColour = GetColour(-Pi_on_2 + dAngle, crBright, crDark);
		::SetPixelV(hDC,p.x + lYoffset, p.y - lXoffset, crColour);

		crColour = GetColour(-dAngle, crBright, crDark);
		::SetPixelV(hDC,p.x + lXoffset, p.y - lYoffset, crColour);

		//Advance the error term and the ant X axis step
		lError += lYoffset++;

		//Check to see if error term has overflowed
		if ((lError += lYoffset) >= 0)
			lError -= --lXoffset * 2;

	} while (lYoffset <= lXoffset);	//Continue until halfway point
} 
} //namespace kwinui
