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

#ifndef _TNODES_H_
#define _TNODES_H_

#include <qfile.h>
#include <qlist.h>
#include <qstring.h>
#include <qlistview.h>
#include <qdatastream.h>
#include <qapplication.h>

#include "funchider.h"
#include "ttypes.h"
#include "utils.h"

class TNode;

#include "tconfig.h"
#include "tnodeitem.h"
#include "tfidoconfig.h"

#define COMPILED_LIST_VERSION	0

#define NDL_MAX_LINE_LENGTH	128

#define RECORD_TYPE_ZONE	0

#define RECORD_TYPE_REGION	1
#define RECORD_TYPE_HOST	2

#define RECORD_TYPE_HUB		3
#define RECORD_TYPE_PVT		4
#define RECORD_TYPE_HOLD	5
#define RECORD_TYPE_DOWN	6
#define RECORD_TYPE_NODE	7

#define RECORD_TYPE_POINT	8

class TNode
{
public:
	TNode(uint16_t*, uint16_t*, uint16_t*, uint16_t*, uint32_t, char*);
	TNode(QDataStream*);

	fidoaddr addr;
	uint32_t type;
	QString line;

	void SaveToStream(QDataStream*);
};

class TNodes : public QList<TNode>
{
public:
	TNodes();

	QListView *ListView;

	void AssignListView(QListView*);
	uint32_t TestCount();
	void Compile();
	void SaveToStream(QString);
	bool CheckModified(QString);
	bool LoadFromStream(QString);

private:
	int Find(uint16_t, uint16_t, uint16_t, uint16_t = 0);
	void AddNode(uint16_t*, uint16_t*, uint16_t*, uint16_t*, uint32_t, char*);
	bool ParseString(char*, uint16_t*, char*);
	void ProcessLine(int32_t, char*, uint16_t*, uint16_t*, uint16_t*);
	bool ProcessList(char*, nodelist*);

protected:
	virtual int compareItems(QPtrCollection::Item, QPtrCollection::Item);
};

#endif /* _TNODES_H_ */
