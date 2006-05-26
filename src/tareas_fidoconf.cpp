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

#include <qlist.h>
#include <qregexp.h>

#include "tareas_fidoconf.h"

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

	void Add(bool Value)
	{
		bool *BoolVal = new bool;
		*BoolVal = Value;
		append(BoolVal);
	}

	void Toggle()
	{
		*at(count() - 1) = !(*at(count() - 1));
	};
};

QChar CommentCharacter = '#';

QStringList AreasDump;
QStringList AreasFiles;

QStringList Environment;

TBoolList IfDefList;

int FindEnvVariable(QString Key)
{
	for (uint i = 0; i < Environment.count(); i++)
		if (strcompare(gettoken(Environment[i], 1), Key))
			return (int)i;

	return -1;
}

void RemoveEnvVariable(QString Key)
{
	int i = FindEnvVariable(Key);
	if (i != -1)
		Environment.remove(Environment.at(i));
}

void AppendEnvVariable(QString Key, QString Val = QString::null)
{
	RemoveEnvVariable(Key);
	Environment << Key.upper() + (Val.isEmpty() ? QString::null : (" " + Val));
}

QString ExpandEnvVariables(QString Val)
{
	QString ret = Val;

	for (uint i = 0; i < Environment.count(); i++)
	{
		QString tmp = gettoken(Environment[i], 1);
		ret = ret.replace(tmp, Environment[i].mid(tmp.length() + 1), false);
	}

	return ret;
}

bool ValidateDefines(QString Val)
{
	int i = Val.find("!=");
	if (i != -1)
	{
		Val.replace("!=", " ");
		return !strcompare(gettoken(Val, 1), gettoken(Val, 2));
	} else
	{
		i = Val.find("==");
		if (i != -1)
		{
			Val.replace("==", " ");
			return strcompare(gettoken(Val, 1), gettoken(Val, 2));
		}
	}

	return FindEnvVariable(Val) > -1;
}

bool ReadIncludedFidoConfFile(QString FileName)
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
					if (tmp.at(0) == CommentCharacter)
						continue;

					for (int i = 0;;)
					{
						i = tmp.find(CommentCharacter, i);
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

					QString tok1, tok2;

					for (;;)
					{
						tok1 = gettoken(tmp, 1);
						tok2 = gettoken(tmp, 2);

						if (strcompare(tok1, "else") && strcompare(tok2, "if"))
						{
							tmp.remove(4, 1);
							continue;
						}
						if (strcompare(tok1, "if") && strcompare(tok2, "defined"))
						{
							tmp.remove(2, 1);
							tmp.remove(5, 4);
							continue;
						}
						if (strcompare(tok1, "if") && strcompare(tok2, "!defined"))
						{
							tmp.remove(2, 2);
							tmp.remove(5, 4);
							tmp.insert(2, 'n');
							continue;
						}
						break;
					}

					if (IfDefList.CheckValid())
					{
						if (strcompare(tok1, "elif"))
						{
							if (IfDefList.count() > 0)
							{
								tmp = ExpandEnvVariables(tmp.mid(5).replace(QRegExp("[\\s]"), QString::null));
								IfDefList.Add(ValidateDefines(tmp));
							} else {
								debugmessage("Misplaces \"elif\" token.");
								break;
							}
						} else if (strcompare(tok1, "elseif"))
						{
							if (IfDefList.count() > 0)
							{
								tmp = ExpandEnvVariables(tmp.mid(7).replace(QRegExp("[\\s]"), QString::null));
								IfDefList.Add(ValidateDefines(tmp));
							} else {
								debugmessage("Misplaces \"elseif\" token.");
								break;
							}
						} else if (strcompare(tok1, "else"))
						{
							if (IfDefList.count() > 0)
							{
								if (!tok2.isEmpty())
									debugmessage("Ignoring text after \"else\" token.");
								IfDefList.Toggle();
							} else {
								debugmessage("Misplaces \"else\" token.");
								break;
							}
						} else if (strcompare(tok1, "endif"))
						{
							if (IfDefList.count() > 0)
								IfDefList.remove(IfDefList.count() - 1);
							else {
								debugmessage("Misplaces \"endif\" token.");
								break;
							}
						} else if ((strcompare(tok1, "if") || strcompare(tok1, "ifdef")) && !tok2.isEmpty())
						{
							tmp = ExpandEnvVariables(tmp.mid(3).replace(QRegExp("[\\s]"), QString::null));
							IfDefList.Add(ValidateDefines(tmp));
						} else if (strcompare(tok1, "ifndef") && !tok2.isEmpty())
						{
							tmp = ExpandEnvVariables(tmp.mid(3).replace(QRegExp("[\\s]"), QString::null));
							IfDefList.Add(!ValidateDefines(tmp));
						} else if (strcompare(tok1, "commentchar") && !tok2.isEmpty())
						{
							if ((tok2.length() != 1) || (QString("!#$%;").find(tok2.at(0)) == -1))
							{
								debugmessage(QString("Invalid \"commentchar\" directive in file %1").arg(FileName));
								break;
							}
							CommentCharacter = tok2.at(0);
							debugmessage(QString("Comment char set to %1.").arg(CommentCharacter));
						} else if ((strcompare(tok1, "set") || strcompare(tok1, "define")) && !tok2.isEmpty())
						{
							int i = tok2.find('=');
							if (i > 0)
							{
								tmp = tok2.mid(i + 1);
								tok2.truncate(i - 1);
							} else
								tmp = gettoken(tmp, 3);
							AppendEnvVariable(tok2, tmp);
						} else if ((strcompare(tok1, "unset") || strcompare(tok1, "undef")) && !tok2.isEmpty())
						{
							RemoveEnvVariable(tok2);
						} else if (strcompare(tok1, "include") && !tok2.isEmpty())
						{
							debugmessage(QString("Processing \"include\" token in file %1 at %2.").arg(FileName).arg(AreasFile.at()));
							if (!ReadIncludedFidoConfFile(tok2))
							{
								debugmessage(QString("Can't open included file %1.").arg(tok2));
								break;
							}
						} else
							AreasDump.append(tmp);
					} else
					{
						if (strcompare(tok1, "elif"))
						{
							tmp = ExpandEnvVariables(tmp.mid(5).replace(QRegExp("[\\s]"), QString::null));
							IfDefList.Add(ValidateDefines(tmp));
						} else if (strcompare(tok1, "elseif"))
						{
							tmp = ExpandEnvVariables(tmp.mid(7).replace(QRegExp("[\\s]"), QString::null));
							IfDefList.Add(ValidateDefines(tmp));
						} else if (strcompare(tok1, "else"))
						{
							if (!tok2.isEmpty())
								debugmessage("Ignoring text after \"else\" token.");
							IfDefList.Toggle();
						} else if (strcompare(tok1, "endif"))
						{
							IfDefList.remove(IfDefList.count() - 1);
						}
					}

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
	CommentCharacter = '#';

	AreasDump.clear();
	AreasFiles.clear();

	Environment.clear();
	AppendEnvVariable("[");
	AppendEnvVariable("]");
	AppendEnvVariable("\"");

#if defined(Q_OS_WIN)
	AppendEnvVariable("OS", "WIN");
#else
	AppendEnvVariable("OS", "UNIX");
#endif

	AppendEnvVariable("MODULE", "QFE");

	IfDefList.clear();

	if (ReadIncludedFidoConfFile(Base->FileName))
	{
		if (IfDefList.count() == 0)
		{
			debugmessage("Areas config file readed sucessfully.");
			//
			//
			//
		} else
			debugmessage("Found difference with count of \"if\" and \"endif\" tokens.");
	} else
		debugmessage("Areas config file read fail.");

	return false;
}

bool RescanAreas_Fidoconf(TAreas *Base)
{
	Q_UNUSED(Base);

	//TODO:
	//
	//for (...) Base->append(...)
	//
	return false;
}

void DoneAreas_Fidoconf(TAreas*)
{
	/* Dummy */
}
