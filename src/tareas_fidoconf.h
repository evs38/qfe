/***************************************************************************
 *   Copyright (C) 2005-2007 by Alexander Shiyan                           *
 *   shc@users.sourceforge.net                                             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef _TAREAS_FIDOCONF_H_
#define _TAREAS_FIDOCONF_H_

#include <qlist.h>

#include "tareas.h"

class TBoolList : public QBitArray
{
public:
	inline void Clear()
	{
		resize(0);
	};

	bool CheckValid()
	{
		bool ret = true;

		for (uint32_t i = 0; i < count(); i++)
			ret = ret && at(i);

		return ret;
	};

	void Add(bool Value)
	{
		resize(count() + 1);
		setBit(count() - 1, Value);
	}

	inline void Delete()
	{
		if (count() > 0)
			resize(count() - 1);
	};

	inline void Toggle()
	{
		if (count() > 0)
			toggleBit(count() - 1);
	};
};

class TAreas_Fidoconfig_PvtObject
{
public:
	QChar CommentCharacter;

	QStringList AreasDump;
	QStringList AreasFiles;
	QStringList Environment;

	TBoolList IfDefList;
};

bool InitAreas_Fidoconf(TAreas*);
bool RescanAreas_Fidoconf(TAreas*);
void DoneAreas_Fidoconf(TAreas*);

#endif /* _TAREAS_FIDOCONF_H_ */
