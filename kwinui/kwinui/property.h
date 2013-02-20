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
 *	property support for KWinUI
 *	June 25,2008
 *
 */

#ifndef __PROPERTY_H__
#define __PROPERTY_H__
#pragma warning(disable:4355)

#define PROPERTY_READ_ONLY 		1
#define PROPERTY_WRITE_ONLY 	2
#define PROPERTY_READ_WRITE 	3

template<typename Container,typename ValueType, int nPropType>
class __Property{
public:
	typedef void (Container::*SetTypePtr)(ValueType value);
	typedef ValueType (Container::*GetTypePtr)();

	__Property():m_pContainer(0),m_pSet(0),m_pGet(0){}
	__Property(Container* pContainer,SetTypePtr pSet,GetTypePtr pGet):m_pContainer(pContainer){
		setSetter(pSet);
		setGetter(pGet);
	}

	void setContainer(Container* pContainer){
		m_pContainer=pContainer;
	}

	void setSetter(SetTypePtr pSet){
		if((nPropType==PROPERTY_WRITE_ONLY) || (nPropType==PROPERTY_READ_WRITE))
			m_pSet=pSet;
		else
			m_pSet=0;
	}

	void setGetter(GetTypePtr pGet){
		if((nPropType==PROPERTY_READ_ONLY)||(nPropType==PROPERTY_READ_WRITE))
			m_pGet=pGet;
		else
			m_pGet=0;
	}

	ValueType operator=(const ValueType& value){	//overload =
		assert(m_pContainer && m_pSet);
		(m_pContainer->*m_pSet)(value);
		return value;
	}

	operator ValueType(){							//
		assert(m_pContainer && m_pGet);
		return (m_pContainer->*m_pGet)();
	}
private:
	Container* 	m_pContainer;
	GetTypePtr	m_pGet;
	SetTypePtr	m_pSet;
};

#endif //__PROPERTY_H__
