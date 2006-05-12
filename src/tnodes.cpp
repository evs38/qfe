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

#include "tnodes.h"

#include "mainwindow.h"

extern TConfig *Config;

#define PROGRESS_DIV_FACTOR	5

TNode::TNode(uint16_t *zone, uint16_t *region, uint16_t *node, uint16_t *point, uint32_t _type, char *info)
{
	addr.zone = *zone;
	addr.net = *region;
	addr.node = *node;
	addr.point = *point;

	type = _type;
	line = (info != NULL) ? info : QString::null;
}

TNode::TNode(QDataStream *Stream)
{
	uint32_t tmp;
	char *raw;

	Stream->readRawBytes((char*)&addr, sizeof(fidoaddr));
	*Stream >> type;
	*Stream >> tmp;
	Stream->readBytes(raw, tmp);
	line = raw;
	delete raw;
}

void TNode::SaveToStream(QDataStream *Stream)
{
	Stream->writeRawBytes((char*)&addr, sizeof(fidoaddr));
	*Stream << type;
	*Stream << (uint32_t)line.length();
	Stream->writeBytes(line.ascii(), line.length());
}

TNodes::TNodes()
{
	ListView = NULL;
	setAutoDelete(true);
}

void TNodes::AssignListView(QListView *lv)
{
	TNode *Node;
	TNodeItem *ZoneRoot = NULL, *HostRoot = NULL, *NodeRoot = NULL, *Point = NULL;

	ListView = lv;
	if (ListView != NULL)
	{
		ListView->clear();
		for (uint32_t i = 0; i < count(); i++)
		{
			Node = at(i);
			switch (Node->type)
			{
				case RECORD_TYPE_ZONE:
					ZoneRoot = new TNodeItem(ListView, ZoneRoot, Node);
					HostRoot = ZoneRoot;
					NodeRoot = ZoneRoot;
					Point = NULL;
					break;
				case RECORD_TYPE_REGION:
				case RECORD_TYPE_HOST:
					if (ZoneRoot != NULL)
					{
						if (Node->addr.zone != ZoneRoot->Node->addr.zone)
						{
							HostRoot = new TNodeItem(ListView, ZoneRoot, Node);
							ZoneRoot = NULL;
						} else
							HostRoot = new TNodeItem(ZoneRoot, HostRoot, Node);
					} else
						HostRoot = new TNodeItem(ListView, HostRoot, Node);
					NodeRoot = HostRoot;
					Point = NULL;
					break;
				case RECORD_TYPE_HUB:
				case RECORD_TYPE_PVT:
				case RECORD_TYPE_HOLD:
				case RECORD_TYPE_DOWN:
				case RECORD_TYPE_NODE:
					if (HostRoot != NULL)
					{
						if (Node->addr.zone != HostRoot->Node->addr.zone)
						{
							if (ZoneRoot != NULL)
							{
								if (Node->addr.net != HostRoot->Node->addr.net)
									NodeRoot = new TNodeItem(ZoneRoot, NodeRoot, Node);
								else
									NodeRoot = new TNodeItem(HostRoot, NodeRoot, Node);
								HostRoot = NULL;
							} else
								NodeRoot = new TNodeItem(HostRoot, NodeRoot, Node);
							ZoneRoot = NULL;
						} else
							NodeRoot = new TNodeItem(HostRoot, NodeRoot, Node);
					} else if (ZoneRoot != NULL)
						NodeRoot = new TNodeItem(ZoneRoot, NodeRoot, Node);
					else
						NodeRoot = new TNodeItem(ListView, NodeRoot, Node);
					Point = NULL;
					break;
				case RECORD_TYPE_POINT:
					if (NodeRoot != NULL)
					{
						if ((Node->addr.zone == NodeRoot->Node->addr.zone) && (Node->addr.net == NodeRoot->Node->addr.net) && (Node->addr.node == NodeRoot->Node->addr.node))
							Point = new TNodeItem(NodeRoot, Point, Node);
						else
							Point = NULL;

					}
					break;
				default:
					break;
			}
		}
	}
}

int TNodes::Find(uint16_t zone, uint16_t region, uint16_t node, uint16_t point)
{
	TNode *It = new TNode(&zone, &region, &node, &point, 0, NULL);

	int ret = find(It);

	delete It;

	return ret;
}

void TNodes::AddNode(uint16_t *zone, uint16_t *region, uint16_t *node, uint16_t *point, uint32_t _type, char *info)
{
	TNode *It = new TNode(zone, region, node, point, _type, NULL);

	int found = find(It);
	if (found > -1)
	{
		if ((info != NULL) && at(found)->line.isEmpty())
			at(found)->line = info;
		delete It;
	} else {
		if (info != NULL)
			It->line = info;
		append(It);
	}
}

bool TNodes::ParseString(char *str, uint16_t *num, char *strout)
{
	char *tmp = str + 1;

	if (*str == ',')
	{
		while((*tmp >= '0') && (*tmp <= '9'))
			tmp++;
		*tmp = '\0';
		*num = (uint16_t)atol(str + 1);
		qstrcpy(strout, tmp + 1);
		return true;
	}
	return false;
}

void TNodes::ProcessLine(int32_t format, char *line, uint16_t *zone, uint16_t *region, uint16_t *node)
{
	char info[NDL_MAX_LINE_LENGTH];
	uint16_t n, point = 0;

	if (*line == ',')
	{
		switch (format)
		{
			case points4d:
				break;
			case points24:
				if ((*zone > 0) && (*region > 0) && (*node > 0) && ParseString(line, &n, &info[0]))
					if (Find(*zone, *region, *node) > -1)
						AddNode(zone, region, node, &n, RECORD_TYPE_POINT, &info[0]);
				break;
			default:
				if ((*zone > 0) && (*region > 0) && ParseString(line, &n, &info[0]))
				{
					*node = n;
					AddNode(zone, region, node, &point, RECORD_TYPE_NODE, &info[0]);
				}
				break;
		}
	} else {
		switch (format)
		{
			case points4d:
				if ((*zone > 0) && (*region > 0) && (*node > 0) && !strnicmp(line, "point", 5))
				{
					if (ParseString(line + 5, &n, &info[0]))
						if (n > 0)
							AddNode(zone, region, node, &n, RECORD_TYPE_POINT, &info[0]);
				} else if (!strnicmp(line, "boss", 4))
				{
					*zone = 0;
					*region = 0;
					*node = 0;
					if (sscanf(line + 4, ",%s", &info[0]) == 1)
					{
						hs_addr addr;
						if (!str2addr(&info[0], &addr))
						{
							*zone = (uint16_t)addr.zone;
							*region = (uint16_t)addr.net;
							*node = (uint16_t)addr.node;
						} else {
							*zone = 0;
							*region = 0;
							*node = 0;
						}
					}
				} else {
					*zone = 0;
					*region = 0;
					*node = 0;
				}
				break;
			case points24:
				*node = 0;
				if ((*zone > 0) && (*region > 0) && !strnicmp(line, "host", 4))
				{
					if (ParseString(line + 4, &n, &info[0]))
					{
						*node = n;
						AddNode(zone, region, node, &point, RECORD_TYPE_NODE, &info[0]);
					}
				}
				break;
			default:
				*node = 0;
				if ((*zone > 0) && (*region > 0) && !strnicmp(line, "hub", 3))
				{
					if (ParseString(line + 3, &n, &info[0]))
					{
						*node = n;
						AddNode(zone, region, node, &point, RECORD_TYPE_HUB, &info[0]);
					}
				} else if ((*zone > 0) && (*region > 0) && !strnicmp(line, "pvt", 3))
				{
					if (ParseString(line + 3, &n, &info[0]))
					{
						*node = n;
						AddNode(zone, region, node, &point, RECORD_TYPE_PVT, &info[0]);
					}
				} else if ((*zone > 0) && (*region > 0) && !strnicmp(line, "hold", 4))
				{
					if (ParseString(line + 4, &n, &info[0]))
					{
						*node = n;
						AddNode(zone, region, node, &point, RECORD_TYPE_HOLD, &info[0]);
					}
				} else if ((*zone > 0) && (*region > 0) && !strnicmp(line, "down", 4))
				{
					if (ParseString(line + 4, &n, &info[0]))
					{
						*node = n;
						AddNode(zone, region, node, &point, RECORD_TYPE_DOWN, &info[0]);
					}
				} else if (!strnicmp(line, "zone", 4))
				{
					*region = 0;
					if (ParseString(line + 4, &n, &info[0]))
					{
						*zone = n;
						AddNode(zone, region, node, &point, RECORD_TYPE_ZONE, &info[0]);
					}
				} else if ((*zone > 0) && !strnicmp(line, "region", 6))
				{
					if (ParseString(line + 6, &n, &info[0]))
					{
						*region = n;
						AddNode(zone, region, node, &point, RECORD_TYPE_REGION, &info[0]);
					}
				} else if ((*zone > 0) && !strnicmp(line, "host", 4))
				{
					if (ParseString(line + 4, &n, &info[0]))
					{
						*region = n;
						AddNode(zone, region, node, &point, RECORD_TYPE_HOST, &info[0]);
					}
				}
				break;
		}
	}
}

bool TNodes::ProcessList(char *fn, nodelist *ndl)
{
	int linelen, odd = 0;
	uint16_t region = 0, node = 0;
	char line[NDL_MAX_LINE_LENGTH + 1];
	uint16_t zone = (uint16_t)((ndl->defaultZone > 0) ? ndl->defaultZone : Config->GetInt(CONFIG_SECTION_FIDO, CONFIG_FIDODEFZONE, 2));
	QFile ndf(fn);

	if (FileOpenFunc(&ndf, IO_ReadOnly))
	{
		while (!ndf.atEnd())
		{
			if ((odd++ % PROGRESS_DIV_FACTOR) == 0)
				((TMainWindow*)qApp->mainWidget())->ShowProgress((ndf.at() * 100) / ndf.size(), ndl->nodelistName, PROGRESS_TYPE_COMPILE);
			linelen = ndf.readLine(&line[0], NDL_MAX_LINE_LENGTH);
			while ((linelen > 0) && (line[linelen - 1] < 0x21))
				linelen--;
			if ((linelen >= 6) && (line[0] != ';'))
			{
				line[linelen] = '\0';
				ProcessLine(ndl->format, &line[0], &zone, &region, &node);
			} else if (linelen <= 0)
				break;
		}
		FileCloseFunc(&ndf);
		return true;
	}

	return false;
}

uint32_t TNodes::TestCount()
{
	uint32_t i, j, ret = 0;

	debugmessage(QString("Found %1 nodelist(s) in config.").arg(Config->FidoConfig->nodelistCount));
	if ((Config->FidoConfig->nodelistCount > 0) && (Config->FidoConfig->nodelistCount < 100))
		for (i = 0; i < Config->FidoConfig->nodelistCount; i++)
		{
			nodelist ndl = Config->FidoConfig->nodelists[i];
			QDir ndldir(Config->FidoConfig->nodelistDir, QString(ndl.nodelistName) + ".???", QDir::Time | QDir::Reversed, QDir::Files | QDir::Readable);

			j = 0;
		        while (j < ndldir.count())
			{
				if (testexists(Config->FidoConfig->nodelistDir + ndldir.entryList()[j]))
				{
					ret++;
					break;
				} else
					j++;
			}
		}

	return ret;
}

void TNodes::Compile()
{
	uint32_t i, j;

	for (i = 0; i < Config->FidoConfig->nodelistCount; i++)
	{
		nodelist ndl = Config->FidoConfig->nodelists[i];
		QDir ndldir(Config->FidoConfig->nodelistDir, QString(ndl.nodelistName) + ".???", QDir::Time | QDir::Reversed, QDir::Files | QDir::Readable);
		j = 0;
	        while (j < ndldir.count())
		{
			QString ndlfile = Config->FidoConfig->nodelistDir + ndldir.entryList()[j];
			if (testexists(ndlfile))
				if (ProcessList((char*)ndlfile.ascii(), &ndl))
					break;
			j++;
		}
	}

	sort();

	((TMainWindow*)qApp->mainWidget())->ShowProgress(100, QString::null, PROGRESS_TYPE_COMPILE);
	((TMainWindow*)qApp->mainWidget())->HideProgress();
}

void TNodes::SaveToStream(QString FileName)
{
	bool found;
	QString tmp;
	uint32_t i, j, cnt = 0;

	QFile file(FileName);
	if (FileOpenFunc(&file, IO_WriteOnly | IO_Truncate))
	{
		QDataStream Stream(&file);

		Stream << (uint32_t)COMPILED_LIST_VERSION;
		Stream << (uint32_t)Config->FidoConfig->nodelistCount;

		for (i = 0; i < Config->FidoConfig->nodelistCount; i++)
		{
			nodelist ndl = Config->FidoConfig->nodelists[i];
			QDir ndldir(Config->FidoConfig->nodelistDir, QString(ndl.nodelistName) + ".???", QDir::Time | QDir::Reversed, QDir::Files | QDir::Readable);
			found = false;
			j = 0;
		        while (j < ndldir.count())
			{
				if (testexists(Config->FidoConfig->nodelistDir + ndldir.entryList()[j]))
				{
					found = true;
					break;
				} else
					j++;
			}
			if (found)
			{
				QFileInfo fi(Config->FidoConfig->nodelistDir + ndldir.entryList()[j]);

				tmp = fi.fileName();
				Stream << (uint32_t)tmp.length();
				Stream.writeBytes(tmp.ascii(), tmp.length());
				Stream << fi.lastModified();

				cnt++;
			}
		}
		Stream << (uint32_t)cnt;

		Stream << (uint32_t)count();

		j = 0;
		for (i = 0; i < count(); i++)
		{
			if ((j++ % PROGRESS_DIV_FACTOR) == 0)
				((TMainWindow*)qApp->mainWidget())->ShowProgress((i * 100) / count(), FileName, PROGRESS_TYPE_SAVENODES);
			at(i)->SaveToStream(&Stream);
		}

		((TMainWindow*)qApp->mainWidget())->ShowProgress(100, QString::null, PROGRESS_TYPE_SAVENODES);
		((TMainWindow*)qApp->mainWidget())->HideProgress();

		FileCloseFunc(&file);
	}
}

bool TNodes::CheckModified(QString FileName)
{
	bool ret = false;
	QString list;
	char *raw;
	uint32_t i, tmp, cnt;

	if (!testexists(FileName))
		return true;

	QFile file(FileName);
	if (FileOpenFunc(&file, IO_ReadOnly))
	{
		QDataStream Stream(&file);

		Stream >> tmp;
		if (tmp == COMPILED_LIST_VERSION)
		{
			Stream >> cnt;
			for (i = 0; i < cnt; i++)
			{
				Stream >> tmp;
				Stream.readBytes(raw, tmp);
				list = raw;
				delete raw;
				if (testexists(Config->FidoConfig->nodelistDir + list))
				{
					QDateTime dt;
					QFileInfo fi(Config->FidoConfig->nodelistDir + list);

					Stream >> dt;
					if (dt != fi.lastModified())
					{
						ret = true;
						break;
					}
				} else
				{
					ret = true;
					break;
				}
			}
		} else
			ret = true;

		FileCloseFunc(&file);
	} else
		ret = true;

	return ret;
}

bool TNodes::LoadFromStream(QString FileName)
{
	QString list;
	char *raw;
	bool ret = true;
	uint32_t i, tmp, cnt;

	if (!testexists(FileName))
		return false;

	QFile file(FileName);
	if (FileOpenFunc(&file, IO_ReadOnly))
	{
		QDataStream Stream(&file);

		Stream >> tmp;
		if (tmp == COMPILED_LIST_VERSION)
		{
			Stream >> cnt;
			for (i = 0; i < cnt; i++)
			{
				Stream >> tmp;
				Stream.readBytes(raw, tmp);
				list = raw;
				delete raw;
				if (testexists(Config->FidoConfig->nodelistDir + list))
				{
					QDateTime dt;
					QFileInfo fi(Config->FidoConfig->nodelistDir + list);

					Stream >> dt;
					if (dt != fi.lastModified())
					{
						ret = false;
						break;
					}
				} else
				{
					ret = false;
					break;
				}
			}
			Stream >> tmp;
			if ((tmp == cnt) && ret)
			{
				Stream >> cnt;
				tmp = 0;
				for (i = 0; i < cnt; i++)
				{
					if ((tmp++ % PROGRESS_DIV_FACTOR) == 0)
						((TMainWindow*)qApp->mainWidget())->ShowProgress((i * 100) / cnt, FileName, PROGRESS_TYPE_LOADNODES);
					append(new TNode(&Stream));
				}
				((TMainWindow*)qApp->mainWidget())->ShowProgress(100, QString::null, PROGRESS_TYPE_LOADNODES);
				((TMainWindow*)qApp->mainWidget())->HideProgress();
			}
		} else
			ret = false;

		FileCloseFunc(&file);
	} else
		ret = false;

	return ret;
}

int TNodes::compareItems(QPtrCollection::Item It1, QPtrCollection::Item It2)
{
	return compareaddr(&((TNode*)It1)->addr, &((TNode*)It2)->addr);
}
