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

TAreas::TAreas(QString fname)
{
	AreasDump.clear();
	AreasFiles.clear();

	Q_UNUSED(fname);
/*
	if (ReadFile(fname))
		qDebug("Areas config file readed sucessfully.");
*/

	setAutoDelete(true);
}

#if defined(Q_OS_WIN)
#	define FIXED_FILE_CASE(x)	x.lower()
#else
#	define FIXED_FILE_CASE(x)	x
#endif

/* This function is test for future fidoconfig parser */
bool TAreas::ReadFile(QString fname)
{
	bool ret = false;

	if (testexists(fname))
	{
		if (AreasFiles.findIndex(FIXED_FILE_CASE(fname)) == -1)
		{
			qDebug("Reading areas config file %s.", fname.ascii());

			AreasFiles.append(FIXED_FILE_CASE(fname));

			QFile AreasFile(fname);
			if (FileOpenFunc(&AreasFile, IO_ReadOnly))
			{
				QTextStream AreasStream(&AreasFile);
				AreasStream.setEncoding(QTextStream::Locale);

				for (;;)
				{
					QString tmp = AreasStream.readLine().simplifyWhiteSpace();
					if (tmp.isEmpty())
						continue;
					if (tmp.at(0) == '#')
						continue;

					for (int i = 0;;)
					{
						i = tmp.find('#', i);
						if (i > 0)
						{
						    	if (tmp.at(i - 1).isSpace())
							{
								tmp.truncate(i);
								tmp = tmp.stripWhiteSpace();
								break;
							}
						} else
							break;
					}

					if (strcompare(gettoken(tmp, 1), "include"))
					{
						tmp = gettoken(tmp, 2);
						if (!ReadFile(tmp))
						{
							qDebug("Can't open included file %s.", tmp.ascii());
							break;
						}
					} else
						AreasDump.append(tmp);

					if (AreasStream.atEnd())
					{
						ret = true;
						break;
					}
				}

				FileCloseFunc(&AreasFile);
			} else
				qDebug("Can't open areas config file.");
		} else
		{
			qDebug("File %s alredy included in areas config file.", fname.ascii());
			ret = true;
		}
	} else
		qDebug("Areas config file not found.");

	return ret;
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
