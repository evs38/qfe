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

#include <qregexp.h>

#include "tareas_fidoconf.h"

#define FIDOCONF_TOKEN_SEPARATORS	" \t"

extern TConfig *Config;

int FindEnvVariable(TAreas *Base, QString Key)
{
	TAreas_Fidoconfig_PvtObject *b_obj = (TAreas_Fidoconfig_PvtObject*)Base->AreasPvtData;

	for (PLATF_U i = 0; i < b_obj->Environment.count(); i++)
		if (strcompare(gettoken(b_obj->Environment[i], 1), Key))
			return (int)i;

	return -1;
}

void RemoveEnvVariable(TAreas *Base, QString Key)
{
	TAreas_Fidoconfig_PvtObject *b_obj = (TAreas_Fidoconfig_PvtObject*)Base->AreasPvtData;

	int i = FindEnvVariable(Base, Key);
	if (i != -1)
		b_obj->Environment.remove(b_obj->Environment.at(i));
}

void AppendEnvVariable(TAreas *Base, QString Key, QString Val = QString::null)
{
	TAreas_Fidoconfig_PvtObject *b_obj = (TAreas_Fidoconfig_PvtObject*)Base->AreasPvtData;

	RemoveEnvVariable(Base, Key);
	b_obj->Environment << Key.upper() + (Val.isEmpty() ? QString::null : (" " + Val));
}

QString ExpandEnvVariables(TAreas *Base, QString Val)
{
	TAreas_Fidoconfig_PvtObject *b_obj = (TAreas_Fidoconfig_PvtObject*)Base->AreasPvtData;

	QString ret = Val;

	for (PLATF_U i = 0; i < b_obj->Environment.count(); i++)
	{
		QString tmp = gettoken(b_obj->Environment[i], 1);
		ret = ret.replace(tmp, b_obj->Environment[i].mid(tmp.length() + 1), false);
	}

	return ret;
}

bool ValidateDefines(TAreas *Base, QString Val)
{
	int i = Val.find("!=");
	if (i != -1)
	{
		Val.replace("!=", " ");
		return !strcompare(gettoken(Val, 1), gettoken(Val, 2));
	} else
	{
		i = Val.find("=");
		if (i != -1)
		{
			Val.replace("=", " ");
			return strcompare(gettoken(Val, 1), gettoken(Val, 2));
		}
	}

	return FindEnvVariable(Base, Val) > -1;
}

bool ReadIncludedFidoConfFile(TAreas *Base, QString FileName)
{
	TAreas_Fidoconfig_PvtObject *b_obj = (TAreas_Fidoconfig_PvtObject*)Base->AreasPvtData;
	bool ret = false;

	if (testexists(FileName))
	{
		if (b_obj->AreasFiles.findIndex(FIXED_FILE_CASE(FileName)) == -1)
		{
			debugmessage(QString("Reading areas config file %1.").arg(FileName));

			b_obj->AreasFiles.append(FIXED_FILE_CASE(FileName));

			QFile AreasFile(FileName);
			if (FileOpenFunc(&AreasFile, IO_ReadOnly))
			{
				QTextStream AreasStream(&AreasFile);
				AreasStream.setEncoding(QTextStream::Latin1);

				for (;;)
				{
					if (AreasStream.atEnd())
					{
						ret = true;
						break;
					}

					QString tmp = AreasStream.readLine().simplifyWhiteSpace();
					if (tmp.isEmpty())
						continue;
					if (tmp.at(0) == b_obj->CommentCharacter)
						continue;

					for (PLATF_S i = 0;;)
					{
						i = tmp.find(b_obj->CommentCharacter, i);
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
						tok1 = gettoken(tmp, 1, FIDOCONF_TOKEN_SEPARATORS);
						tok2 = gettoken(tmp, 2, FIDOCONF_TOKEN_SEPARATORS);

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

					if (b_obj->IfDefList.CheckValid())
					{
						if (strcompare(tok1, "elif"))
						{
							if (b_obj->IfDefList.count() > 0)
							{
								tmp = ExpandEnvVariables(Base, tmp.mid(5).replace(QRegExp("[\\s]"), QString::null));
								b_obj->IfDefList.Add(ValidateDefines(Base, tmp));
							} else {
								debugmessage("Misplaces \"elif\" token.");
								break;
							}
						} else if (strcompare(tok1, "elseif"))
						{
							if (b_obj->IfDefList.count() > 0)
							{
								tmp = ExpandEnvVariables(Base, tmp.mid(7).replace(QRegExp("[\\s]"), QString::null));
								b_obj->IfDefList.Add(ValidateDefines(Base, tmp));
							} else {
								debugmessage("Misplaces \"elseif\" token.");
								break;
							}
						} else if (strcompare(tok1, "else"))
						{
							if (b_obj->IfDefList.count() > 0)
							{
								if (!tok2.isEmpty())
									debugmessage("Ignoring text after \"else\" token.");
								b_obj->IfDefList.Toggle();
							} else {
								debugmessage("Misplaces \"else\" token.");
								break;
							}
						} else if (strcompare(tok1, "endif"))
						{
							if (b_obj->IfDefList.count() > 0)
								b_obj->IfDefList.Delete();
							else {
								debugmessage("Misplaces \"endif\" token.");
								break;
							}
						} else if ((strcompare(tok1, "if") || strcompare(tok1, "ifdef")) && !tok2.isEmpty())
						{
							tmp = ExpandEnvVariables(Base, tmp.mid(3).replace(QRegExp("[\\s]"), QString::null));
							b_obj->IfDefList.Add(ValidateDefines(Base, tmp));
						} else if (strcompare(tok1, "ifndef") && !tok2.isEmpty())
						{
							tmp = ExpandEnvVariables(Base, tmp.mid(3).replace(QRegExp("[\\s]"), QString::null));
							b_obj->IfDefList.Add(!ValidateDefines(Base, tmp));
						} else if (strcompare(tok1, "commentchar") && !tok2.isEmpty())
						{
							if ((tok2.length() != 1) || (QString("!#$%;").find(tok2.at(0)) == -1))
							{
								debugmessage(QString("Invalid \"commentchar\" directive in file %1").arg(FileName));
								break;
							}
							b_obj->CommentCharacter = tok2.at(0);
							debugmessage(QString("Comment char set to %1.").arg(b_obj->CommentCharacter));
						} else if ((strcompare(tok1, "set") || strcompare(tok1, "define")) && !tok2.isEmpty())
						{
							int i = tok2.find('=');
							if (i > 0)
							{
								tmp = tok2.mid(i + 1);
								tok2.truncate(i - 1);
							} else
								tmp = gettoken(tmp, 3, FIDOCONF_TOKEN_SEPARATORS);
							AppendEnvVariable(Base, tok2, tmp);
						} else if ((strcompare(tok1, "unset") || strcompare(tok1, "undef")) && !tok2.isEmpty())
						{
							RemoveEnvVariable(Base, tok2);
						} else if (strcompare(tok1, "include") && !tok2.isEmpty())
						{
							debugmessage(QString("Processing \"include\" token in file %1 at %2.").arg(FileName).arg(AreasFile.at()));
							if (!ReadIncludedFidoConfFile(Base, tok2))
							{
								debugmessage(QString("Can't open included file %1.").arg(tok2));
								break;
							}
						} else
							b_obj->AreasDump.append(tmp);
					} else
					{
						if (strcompare(tok1, "elif"))
						{
							tmp = ExpandEnvVariables(Base, tmp.mid(5).replace(QRegExp("[\\s]"), QString::null));
							b_obj->IfDefList.Add(ValidateDefines(Base, tmp));
						} else if (strcompare(tok1, "elseif"))
						{
							tmp = ExpandEnvVariables(Base, tmp.mid(7).replace(QRegExp("[\\s]"), QString::null));
							b_obj->IfDefList.Add(ValidateDefines(Base, tmp));
						} else if (strcompare(tok1, "else"))
						{
							if (!tok2.isEmpty())
								debugmessage("Ignoring text after \"else\" token.");
							b_obj->IfDefList.Toggle();
						} else if (strcompare(tok1, "endif"))
						{
							b_obj->IfDefList.Delete();
						}
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
	TAreas_Fidoconfig_PvtObject *b_obj = new TAreas_Fidoconfig_PvtObject();
	Base->AreasPvtData = b_obj;

	b_obj->CommentCharacter = '#';

	b_obj->AreasDump.clear();
	b_obj->AreasFiles.clear();

	b_obj->Environment.clear();
	AppendEnvVariable(Base, "[");
	AppendEnvVariable(Base, "]");
	AppendEnvVariable(Base, "\"");
	AppendEnvVariable(Base, "'");
	AppendEnvVariable(Base, "==", "=");

#if defined(Q_OS_WIN)
	AppendEnvVariable(Base, "OS", "WIN");
#else
	AppendEnvVariable(Base, "OS", "UNIX");
#endif

	AppendEnvVariable(Base, "MODULE", "QFE");

	b_obj->IfDefList.Clear();

	if (ReadIncludedFidoConfFile(Base, Base->FileName))
	{
		if (b_obj->IfDefList.count() == 0)
		{
			debugmessage("Areas config file readed sucessfully.");
			//
			//
			//
/*
			QFile Dump("/tmp/husky_dump.log");
			if (FileOpenFunc(&Dump, IO_WriteOnly))
			{
				QTextStream DumpStream(&Dump);
				DumpStream.setEncoding(QTextStream::Locale);
				for (uint32_t i = 0; i < b_obj->AreasDump.count(); i++)
					DumpStream << b_obj->AreasDump[i] << "\n";
				FileCloseFunc(&Dump);
			}
*/
			//
			//

			return true;
		} else
			debugmessage("Found difference with count of \"if\" and \"endif\" tokens.");
	} else
		debugmessage("Areas config file read fail.");

	return false;
}

bool RescanAreas_Fidoconf(TAreas *Base)
{
	TAreas_Fidoconfig_PvtObject *b_obj = (TAreas_Fidoconfig_PvtObject*)Base->AreasPvtData;

	for (PLATF_U i = 0; i < b_obj->AreasDump.count(); i++)
	{
		QString tmp = b_obj->AreasDump[i];
		PLATF_S tok_cnt = tokencount(tmp, FIDOCONF_TOKEN_SEPARATORS);

		if (tok_cnt > 2)
		{
			QString tmp_dsc = QString::null;
			QString tmp_aka = Config->GetAddress(0);
			QString tmp_upl = QString::null;
			QString tok1 = gettoken(tmp, 1, FIDOCONF_TOKEN_SEPARATORS);
			Area_Type tmp_at = AREATYPE_UNKNOWN;
			Base_Type tmp_bt = BASETYPE_UNKNOWN;

			if (strcompare(tok1, "NetMailArea"))
			{
				tmp_at = AREATYPE_NETMAIL;
			} else if (strcompare(tok1, "LocalArea"))
			{
				tmp_at = AREATYPE_LOCALMAIL;
			} else if (strcompare(tok1, "BadArea"))
			{
				tmp_at = AREATYPE_BADMAIL;
			} else if (strcompare(tok1, "DupeArea"))
			{
				tmp_at = AREATYPE_DUPEMAIL;
			} else if (strcompare(tok1, "EchoArea"))
			{
				tmp_at = AREATYPE_ECHOMAIL;
			} else
				continue;

			for (PLATF_S j = 4; j <= tok_cnt; j++)
			{
				/* Find Base Type */
				if (strcompare(gettoken(tmp, j, FIDOCONF_TOKEN_SEPARATORS), "-b"))
				{
					QString tokb = gettoken(tmp, j + 1, FIDOCONF_TOKEN_SEPARATORS);
					if (strcompare(tokb, "Msg"))
					{
						tmp_bt = BASETYPE_MSG;
					} else if (strcompare(tokb, "Jam"))
					{
						tmp_bt = BASETYPE_JAM;
					} else if (strcompare(tokb, "Squish"))
					{
						tmp_bt = BASETYPE_SQUISH;
					} else if (strcompare(tokb, "Passtrough"))
					{
						tmp_bt = BASETYPE_PASSTROUGH;
					}
					break;
				}
			}

			for (PLATF_S j = 4; j <= tok_cnt; j++)
			{
				/* Find Base Description */
				if (strcompare(gettoken(tmp, j, FIDOCONF_TOKEN_SEPARATORS), "-d"))
				{
					tmp_dsc = Config->toUTF((char*)gettoken(tmp, j + 1, FIDOCONF_TOKEN_SEPARATORS).ascii());
					if (tmp_dsc.startsWith("\""))
						tmp_dsc = tmp_dsc.mid(1);

					if (!tmp_dsc.endsWith("\""))
					{
						for (PLATF_S k = j + 2; k <= tok_cnt; k++)
						{
							QString tokb = Config->toUTF((char*)gettoken(tmp, k, FIDOCONF_TOKEN_SEPARATORS).ascii());
							tmp_dsc.append(" " + tokb);
							if (tokb.endsWith("\""))
							{
								tmp_dsc.truncate(tmp_dsc.length() - 1);
								break;
							}
						}
					} else
						tmp_dsc.truncate(tmp_dsc.length() - 1);

					break;
				}
			}

			for (PLATF_S j = 4; j <= (tok_cnt - 1); j++)
			{
				/* Find AKA */

				hs_addr tmp_addr;
				if (strcompare(gettoken(tmp, j, FIDOCONF_TOKEN_SEPARATORS), "-a"))
					if (str2addr(gettoken(tmp, j + 1, FIDOCONF_TOKEN_SEPARATORS), &tmp_addr))
						tmp_aka = addr2str2(tmp_addr);
			}


			for (PLATF_S j = 4; j <= (tok_cnt - 1); j++)
			{
				/* Find Uplink */

				hs_addr tmp_addr;
				if (str2addr(gettoken(tmp, j, FIDOCONF_TOKEN_SEPARATORS), &tmp_addr))
					if (strcompare(gettoken(tmp, j + 1, FIDOCONF_TOKEN_SEPARATORS), "-def"))
						tmp_upl = addr2str2(tmp_addr);
			}

			//
			//
			//

			tmp = QString("Found Area: %1, AKA: %2, Uplink: %3, Desc: %4\n").arg(gettoken(tmp, 2, FIDOCONF_TOKEN_SEPARATORS)).arg(tmp_aka).arg(tmp_upl).arg(tmp_dsc);
			fprintf(stdout, tmp.local8Bit());
		}
	}

	//TODO:
	//
	//for (...) Base->append(...)
	//
	return false;
}

void DoneAreas_Fidoconf(TAreas *Base)
{
	if (Base->AreasPvtData != NULL)
	{
		delete (TAreas_Fidoconfig_PvtObject*)Base->AreasPvtData;
		Base->AreasPvtData = NULL;
	}
}
