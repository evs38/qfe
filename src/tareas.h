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

#ifndef _TAREAS_H_
#define _TAREAS_H_

#include <qlist.h>

#include "tfidoconfig.h"

class TAreas;

#include "utils.h"

#include "tarea.h"

class TAreas : public QList<TArea>
{
public:
	TAreas();

	int Find(QString);
	TArea *Add(QString, QString, QString, Area_Type, Base_Type, uint16_t, QString, QString, QString, QString, QString, char*);
};

#endif /* _TAREAS_H_ */
