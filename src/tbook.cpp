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

#include "tbook.h"

TBook::TBook(QString fname)
{
	FileName = fname;
	XMLFile = new QFile(FileName);

	setAutoDelete(true);

	Read();
}

TBook::~TBook()
{
	FileCloseFunc(XMLFile);
	delete XMLFile;
	delete Doc;
}

void TBook::Read()
{
	QString Content = QString::null;

	clear();

	if (FileOpenFunc(XMLFile, IO_ReadOnly))
	{
		QTextStream Stream(XMLFile);
		Stream.setEncoding(QTextStream::UnicodeUTF8);
		Content = TextStreamReadFunc(&Stream);
		FileCloseFunc(XMLFile);
	}
	Doc = new QDomDocument();
	if (!Doc->setContent(Content))
	{
		delete Doc;
		Doc = new QDomDocument("AddressBook");
	}
	Root = Doc->documentElement();
	if (Root.isNull())
	{
		Root = Doc->createElement("book");
		Doc->appendChild(Root);
	}

	QDomNodeList List = Root.elementsByTagName("item");
	for (uint32_t i = 0; i < List.count(); i++)
		if (List.item(i).isElement())
		{
			QDomElement Elem = List.item(i).toElement();
			if (Elem.isNull())
				continue;

			TBookItem *Item = Add(Elem);
			Item->Name = Elem.attribute("name", QString::null);
			Item->EMail = Elem.attribute("email", QString::null);
			Item->Address = Elem.attribute("address", QString::null);
			Item->Comment = Elem.attribute("comment", QString::null);
			Item->Photo = Elem.attribute("photo", QString::null);
		}
}

void TBook::clear()
{
	while (count() > 0)
		Delete(0);
}

void TBook::Save()
{
	for (uint32_t i = 0; i < count(); i++)
	{
		TBookItem *Item = at(i);
		if (Item->Node.isNull())
		{
			Item->Node = Doc->createElement("item");
			Root.appendChild(Item->Node);
		}
		Item->Node.setAttribute("name", Item->Name);
		if (!Item->EMail.isEmpty())
			Item->Node.setAttribute("email", Item->EMail);
		if (!Item->Address.isEmpty())
			Item->Node.setAttribute("address", Item->Address);
		if (!Item->Comment.isEmpty())
			Item->Node.setAttribute("comment", Item->Comment);
		if (!Item->Photo.isEmpty())
			Item->Node.setAttribute("photo", Item->Photo);
	}

	if (FileOpenFunc(XMLFile, IO_WriteOnly | IO_Truncate))
	{
		QTextStream Stream(XMLFile);
		Stream.setEncoding(QTextStream::UnicodeUTF8);
		Stream << Doc->toString();
		FileCloseFunc(XMLFile);
	}
}

TBookItem *TBook::Add()
{
	TBookItem *Item = new TBookItem();
	append(Item);
	return Item;
}

TBookItem *TBook::Add(QDomElement Elem)
{
	TBookItem *Item = new TBookItem(Elem);
	append(Item);
	return Item;
}

int TBook::Index(TBookItem *Item)
{
	for (uint32_t i = 0; i < count(); i++)
		if (Item == at(i))
			return i;
	return -1;
}

void TBook::Delete(uint32_t index)
{
	if (index < count())
	{
		TBookItem *Item = at(index);
		if (!Item->Node.isNull())
			Root.removeChild(Item->Node);
		remove(at(index));
	}
}

void TBook::MakeUserLst(QStringList *Lst)
{
	Lst->clear();
	for (uint32_t i = 0; i < count(); i++)
		Lst->append(at(i)->Name);
}

TBookItem *TBook::FindUser(QString Addr, QString Name)
{
	for (uint32_t i = 0; i < count(); i++)
		if ((at(i)->Address == Addr) && (strcompare(at(i)->Name, Name)))
			return at(i);
	return NULL;
}
