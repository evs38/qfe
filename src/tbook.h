/***************************************************************************
 *   Copyright (C) 2005-2008 by Alexander Shiyan                           *
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

#ifndef _TBOOK_H_
#define _TBOOK_H_

#include <qdom.h>
#include <qfile.h>
#include <qlist.h>
#include <qstringlist.h>

#include "funchider.h"

class TBook;

#include "tbookitem.h"

#include "utils.h"

class TBook : public QList<TBookItem>
{
public:
	TBook(QString);
	~TBook();

	TBookItem *Add();
	TBookItem *Add(QDomElement);
	int Index(TBookItem*);
	void Delete(uint32_t);
	void Read();
	void Save();
	void clear();
	void MakeUserLst(QStringList*);
	TBookItem* FindUser(QString, QString);

private:
	QFile *XMLFile;
	QString FileName;
	QDomDocument *Doc;
	QDomElement Root;
};

#endif /* _TBOOK_H_ */
