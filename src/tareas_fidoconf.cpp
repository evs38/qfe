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

#include "tareas_fidoconf.h"

QStringList AreasDump;
QStringList AreasFiles;

bool ReadIncludeFile(QString FileName)
{
	bool ret = false;

	if (testexists(FileName))
	{
		if (AreasFiles.findIndex(FIXED_FILE_CASE(FileName)) == -1)
		{
			debugmessage(QString("Reading areas config file %1.").arg(FileName));

			AreasFiles.append(FIXED_FILE_CASE(FileName));

			QFile AreasFile(FileName);
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
						debugmessage(QString("Processing \"include\" token in file %1 at %2.").arg(FileName).arg(AreasFile.at()));
						tmp = gettoken(tmp, 2);
						if (!ReadIncludeFile(tmp))
						{
							debugmessage(QString("Can't open included file %1.").arg(tmp));
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
				debugmessage("Can't open areas config file.");
		} else
		{
			debugmessage(QString("File %1 alredy included in areas config file.").arg(FileName));
			ret = true;
		}
	} else
		debugmessage("Areas config file not found.");

	return ret;
}

bool InitAreas_Fidoconf(TAreas *Base)
{
	AreasDump.clear();
	AreasFiles.clear();

	if (ReadIncludeFile(Base->FileName))
	{
		debugmessage("Areas config file readed sucessfully.");
		return true;
	}

	debugmessage("Areas config file read fail.");
	return false;
}
