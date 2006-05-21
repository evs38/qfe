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

#ifndef _TAREAS_FIDOCONF_H_
#define _TAREAS_FIDOCONF_H_

#include <qlist.h>

#include "tareas.h"

class TBoolList : public QList<bool>
{
public:
	TBoolList()
	{
		setAutoDelete(true);
	};

	bool CheckValid()
	{
		if (count() == 0)
			return true;
		return *at(count() - 1);
	};
	void Toggle()
	{
		*at(count() - 1) = !(*at(count() - 1));
	};
};

bool InitAreas_Fidoconf(TAreas*);
bool RescanAreas_Fidoconf(TAreas*);
void DoneAreas_Fidoconf(TAreas*);

#endif /* _TAREAS_FIDOCONF_H_ */
