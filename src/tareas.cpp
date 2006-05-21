/***************************************************************************
 *   Copyright (C) 2005-2006 by Alexander Shiyan                           *
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

#include "tareas.h"

#include "tareas_fidoconf.h"

TAreas::TAreas(QString _FileName)
{
	FileName = _FileName;

#if defined(ENABLE_DEBUG)
	/* This calls is just test for future areas config parser */
	if (InitAreas_Fidoconf(this))
	{
		RescanAreas_Fidoconf(this);
		DoneAreas_Fidoconf(this);
	}
#endif

	setAutoDelete(true);
}

int TAreas::Find(QString _name)
{
	for (uint32_t i = 0; i < count(); i++)
		if (strcompare(at(i)->Name, _name))
			return i;

	return -1;
}

TArea *TAreas::Add(QString _Name, QString _Desc, QString _Path, Area_Type _AType, Base_Type _BType, uint16_t _DefZone, QString _Aka, QString _Group, QString _Uplink, QString _UplinkPwd, QString _UplinkRbt, char *_User)
{
	TArea *It = new TArea(_AType, _BType, _DefZone);

	if (It->Init(_Name, _Desc, _Path, _Aka, _Group, _Uplink, _UplinkPwd, _UplinkRbt, _User))
		if (It->Open())
		{
			if (It->Rescan())
			{
				append(It);
				It->Close();
				return It;
			}
			It->Close();
		}

	delete It;

	return NULL;
}
