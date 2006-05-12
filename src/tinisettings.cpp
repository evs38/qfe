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

#include "tinisettings.h"

TIniSettings::TIniSettings(QString FileName) : QFile(FileName)
{
	QString tmp, key, sect = QString::null;
	IniLines.clear();
	if (exists())
	{
		if (FileOpenFunc(this, IO_ReadOnly))
		{
			QTextStream txt(this);
			txt.setEncoding(QTextStream::UnicodeUTF8);
			while (!txt.atEnd())
			{
				tmp = txt.readLine().stripWhiteSpace();
				if (tmp.startsWith("#") || tmp.startsWith(";"))
					continue;
				if (tmp.startsWith("[") && tmp.endsWith("]"))
					sect = tmp.mid(1, tmp.length() - 2);
				else if (!sect.isEmpty())
				{
					int32_t seppos = tmp.find("=");
					if (seppos > -1)
					{
						key = tmp;
						key.truncate(seppos);
						tmp = tmp.mid(seppos + 1);
						AddInternal(sect, key, tmp);
					}
				}
			}
			FileCloseFunc(this);
		} else
			errormessage(NULL, QObject::tr("Can't read config file."));
	}
}

TIniSettings::~TIniSettings()
{
	Flush();
}

void TIniSettings::Flush()
{
	int32_t i1, i2;
	uint32_t i;
	QString sect = QString::null, line, tmp1, tmp2;

	if (IniLines.count() > 1)
		IniLines.sort();

	if (FileOpenFunc(this, IO_WriteOnly | IO_Truncate))
	{
		QTextStream txt(this);
		txt.setEncoding(QTextStream::UnicodeUTF8);
		for (i = 0; i < IniLines.count(); i++)
		{
			line = IniLines[i];
			i1 = line.find("]");
			if (i1 > -1)
				tmp1 = line.mid(1, i1 - 1);
			else
				continue;
			i2 = line.find("=");
			if (i2 > -1)
				tmp2 = line.mid(i1 + 1, i2 - (i1 + 1));
			else
				continue;
			line = line.mid(i2 + 1);
			if (sect != tmp1)
			{
				if (!sect.isEmpty())
					txt << "\n";
				txt << "[" << tmp1 << "]\n";
				sect = tmp1;
			}
			txt << QString("%1=%2").arg(tmp2).arg(line) << "\n";
		}
		FileCloseFunc(this);
	} else
		errormessage(NULL, QObject::tr("Can't write config file."));
}

int32_t TIniSettings::Find(QString section, QString key)
{
	for (uint32_t i = 0; i < IniLines.count(); i++)
		if (IniLines[i].startsWith(QString("[%1]%2=").arg(section).arg(key), false))
			return i;

	return -1;
}

void TIniSettings::AddInternal(QString section, QString key, QString value)
{
	if (Find(section, key) < 0)
		IniLines.append(QString("[%1]%2=%3").arg(section).arg(key).arg(value));
}

QString TIniSettings::GetString(QString section, QString key, QString defvalue)
{
	int32_t i = Find(section, key);
	if (i > -1)
	{
		QString tmp = IniLines[i];
		int32_t seppos = tmp.find("=");
		if (seppos > -1)
		{
			tmp = tmp.mid(seppos + 1);
			if ((tmp != "\\0") && !tmp.isEmpty())
				return tmp;
		}
	}

	return defvalue;
}

void TIniSettings::SetString(QString section, QString key, QString value)
{
	value = value.isNull() ? "" : value;
	int32_t i = Find(section, key);
	if (i > -1)
		IniLines[i] = QString("[%1]%2=%3").arg(section).arg(key).arg(value);
	else
		IniLines.append(QString("[%1]%2=%3").arg(section).arg(key).arg(value));
}

void TIniSettings::RemoveString(QString section, QString key)
{
	int32_t i = Find(section, key);
	if (i > -1)
		IniLines.remove(IniLines.at(i));
}
