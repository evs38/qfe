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

extern TConfig *Config;

void TGeekForm::init()
{
        setIcon(*Config->FindPixmap("icon_geek"));

	BtnOk->setIconSet(QIconSet(*Config->FindPixmap("icon_ok")));

	Config->GetSize(this, size().width(), size().height());
	Config->GetPos(this);
}

void TGeekForm::closeEvent(QCloseEvent *e)
{
	Config->SetPos(this);
	Config->SetSize(this);
	e->accept();
}

void TGeekForm::GeekSignResult(QString *Tok)
{
	*Tok = Tok->mid(1);
	if (!Tok->isEmpty())
		if (Tok->at(0).isLetter())
			*Tok = Tok->mid(1);
	if (Tok->startsWith("++"))
		*Tok = QObject::tr("Very positive");
	else if (Tok->startsWith("+"))
		*Tok = QObject::tr("Positive");
	else if (Tok->isEmpty())
		*Tok = QObject::tr("Neutral");
	else if (Tok->startsWith("--"))
		*Tok = QObject::tr("Very negative");
	else if (Tok->startsWith("-"))
		*Tok = QObject::tr("Negative");
	else
		*Tok = QString::null;
}

bool TGeekForm::SetUp(QString Buffer, bool REdition)
{
	uint32_t i, tc = tokencount(Buffer);
	QString OrigTok, Tok, GeekResult = QString::null, Gender = QString::null;

	//TODO: "Russian edition" not yet complete...
	for (i = 0; i < tc; i++)
	{
		Tok = gettoken(Buffer, i + 1);
		OrigTok = Tok;
		switch (Tok.at(0).unicode())
		{
			case 'B':
				GeekSignResult(&Tok);
				if (!Tok.isEmpty())
				{
					GeekResult.append("<tr><td><b>" + QObject::tr("Rate BBS") + ":</b></td><td>" + Tok + "</td></tr>");
					continue;
				}
				break;
			case 'C':
				Tok = Tok.mid(1);
				if (Tok.compare("++") == 0)
					Tok = QObject::tr("Computers are a large part of my existence");
				if (Tok.compare("+") == 0)
					Tok = QObject::tr("Computers are fun and I enjoy using them");
				if (Tok.isEmpty())
					Tok = QObject::tr("Computers are a tool, nothing more");
				if (Tok.compare("-") == 0)
					Tok = QObject::tr("Anything more complicated than my calculator and I'm screwed");
				if (Tok.compare("--") == 0)
					Tok = QObject::tr("Not use");
				else
					break;
				GeekResult.append("<tr><td><b>" + QObject::tr("Computer skill") + ":</b></td><td>" + Tok + "</td></tr>");
				continue;
			case 'D':
				GeekSignResult(&Tok);
				if (!Tok.isEmpty())
				{
					GeekResult.append("<tr><td><b>" + QObject::tr("Rate DOS") + ":</b></td><td>" + Tok + "</td></tr>");
					continue;
				}
				break;
			case 'F':
				GeekSignResult(&Tok);
				if (!Tok.isEmpty())
				{
					GeekResult.append("<tr><td><b>" + QObject::tr("Rate FTN") + ":</b></td><td>" + Tok + "</td></tr>");
					continue;
				}
				break;
			case 'G':
			{
				GeekResult.append("<tr><td><b>" + QObject::tr("Work") + ":</b></td><td>");
				uint32_t j, wc;
				QString WorkTok;
				Tok = Tok.mid(1);
				wc = tokencount(Tok, "/");
				for (j = 0; j < wc; j++)
				{
					WorkTok = gettoken(Tok, j + 1, "/");
					if (WorkTok.compare("B") == 0)
					{
						GeekResult.append(((j == 0) ? QString::null : "<br>") + QObject::tr("Business"));
					} else if (WorkTok.compare("C") == 0)
					{
						GeekResult.append(((j == 0) ? QString::null : "<br>") + QObject::tr("Classics"));
					} else if (WorkTok.compare("CA") == 0)
					{
						GeekResult.append(((j == 0) ? QString::null : "<br>") + QObject::tr("Commercial Arts"));
					} else if (WorkTok.compare("CC") == 0)
					{
						GeekResult.append(((j == 0) ? QString::null : "<br>") + QObject::tr("Communications"));
					} else if (WorkTok.compare("CM") == 0)
					{
						GeekResult.append(((j == 0) ? QString::null : "<br>") + QObject::tr("Computer Management"));
					} else if (WorkTok.compare("CS") == 0)
					{
						GeekResult.append(((j == 0) ? QString::null : "<br>") + QObject::tr("Computer Science"));
					} else if (WorkTok.compare("E") == 0)
					{
						GeekResult.append(((j == 0) ? QString::null : "<br>") + QObject::tr("Engineering"));
					} else if (WorkTok.compare("ED") == 0)
					{
						GeekResult.append(((j == 0) ? QString::null : "<br>") + QObject::tr("Education"));
					} else if (WorkTok.compare("FA") == 0)
					{
						GeekResult.append(((j == 0) ? QString::null : "<br>") + QObject::tr("Fine Arts"));
					} else if (WorkTok.compare("G") == 0)
					{
						GeekResult.append(((j == 0) ? QString::null : "<br>") + QObject::tr("Government"));
					} else if (WorkTok.compare("H") == 0)
					{
						GeekResult.append(((j == 0) ? QString::null : "<br>") + QObject::tr("Humanities"));
					} else if (WorkTok.compare("IT") == 0)
					{
						GeekResult.append(((j == 0) ? QString::null : "<br>") + QObject::tr("Information Technology"));
					} else if (WorkTok.compare("J") == 0)
					{
						GeekResult.append(((j == 0) ? QString::null : "<br>") + QObject::tr("Jurisprudence"));
					} else if (WorkTok.compare("L") == 0)
					{
						GeekResult.append(((j == 0) ? QString::null : "<br>") + QObject::tr("Literature"));
					} else if (WorkTok.compare("LI") == 0)
					{
						GeekResult.append(((j == 0) ? QString::null : "<br>") + QObject::tr("Linguistic"));
					} else if (WorkTok.compare("LS") == 0)
					{
						GeekResult.append(((j == 0) ? QString::null : "<br>") + QObject::tr("Library Science"));
					} else if (WorkTok.compare("M") == 0)
					{
						GeekResult.append(((j == 0) ? QString::null : "<br>") + QObject::tr("Math"));
					} else if (WorkTok.compare("MC") == 0)
					{
						GeekResult.append(((j == 0) ? QString::null : "<br>") + QObject::tr("Mass Communications"));
					} else if (WorkTok.compare("MD") == 0)
					{
						GeekResult.append(((j == 0) ? QString::null : "<br>") + QObject::tr("Medicine"));
					} else if (WorkTok.compare("MU") == 0)
					{
						GeekResult.append(((j == 0) ? QString::null : "<br>") + QObject::tr("Music"));
					} else if (WorkTok.compare("P") == 0)
					{
						GeekResult.append(((j == 0) ? QString::null : "<br>") + QObject::tr("Philosophy"));
					} else if (WorkTok.compare("PA") == 0)
					{
						GeekResult.append(((j == 0) ? QString::null : "<br>") + QObject::tr("Performing Arts"));
					} else if (WorkTok.compare("PR") == 0)
					{
						GeekResult.append(((j == 0) ? QString::null : "<br>") + QObject::tr("Programming"));
					} else if (WorkTok.compare("PS") == 0)
					{
						GeekResult.append(((j == 0) ? QString::null : "<br>") + QObject::tr("Psychology"));
					} else if (WorkTok.compare("S") == 0)
					{
						GeekResult.append(((j == 0) ? QString::null : "<br>") + QObject::tr("Science"));
					} else if (WorkTok.compare("SS") == 0)
					{
						GeekResult.append(((j == 0) ? QString::null : "<br>") + QObject::tr("Social Science"));
					} else if (WorkTok.compare("TW") == 0)
					{
						GeekResult.append(((j == 0) ? QString::null : "<br>") + QObject::tr("Technical Writing"));
					} else if ((WorkTok.compare("O") == 0) || (WorkTok.compare("*") == 0))
					{
						GeekResult.append(((j == 0) ? QString::null : "<br>") + QObject::tr("Other"));
					} else if (WorkTok.compare("U") == 0)
					{
						GeekResult.append(((j == 0) ? QString::null : "<br>") + QObject::tr("Undecided"));
					} else if ((WorkTok.compare("!") == 0) || (WorkTok.compare("?") == 0))
					{
						GeekResult.append(((j == 0) ? QString::null : "<br>") + QObject::tr("No qualifications"));
					} else
						break;
				}
				GeekResult.append("</td></tr>");
				continue;
			}
			case 'H':
				Tok = Tok.mid(1);
				if (Tok.compare("++") == 0)
					Tok = QObject::tr("Below a belt");
				else if (Tok.compare("+") == 0)
					Tok = QObject::tr("From shoulders and up to a belt");
				else if (Tok.isEmpty())
					Tok = QObject::tr("Dense hairdress, but not long");
				else if (Tok.compare("-") == 0)
					Tok = QObject::tr("Short hairstyle");
				else if (Tok.compare("--") == 0)
					Tok = QObject::tr("Bald");
				else
					break;
				GeekResult.append("<tr><td><b>" + QObject::tr("Length of hair") + ":</b></td><td>" + Tok + "</td></tr>");
				continue;
			case 'I':
				GeekSignResult(&Tok);
				if (!Tok.isEmpty())
				{
					GeekResult.append("<tr><td><b>" + QObject::tr("Rate Internet") + ":</b></td><td>" + Tok + "</td></tr>");
					continue;
				}
				break;
			case 'M':
				GeekSignResult(&Tok);
				if (!Tok.isEmpty())
				{
					GeekResult.append("<tr><td><b>" + QObject::tr("Rate Macintosh") + ":</b></td><td>" + Tok + "</td></tr>");
					continue;
				}
				break;
			case 'N':
				GeekSignResult(&Tok);
				if (!Tok.isEmpty())
				{
					GeekResult.append("<tr><td><b>" + QObject::tr("Rate USENET News") + ":</b></td><td>" + Tok + "</td></tr>");
					continue;
				}
				break;
			case 'O':
				GeekSignResult(&Tok);
				if (!Tok.isEmpty())
				{
					GeekResult.append("<tr><td><b>" + QObject::tr("Rate OS/2") + ":</b></td><td>" + Tok + "</td></tr>");
					continue;
				}
				break;
			case 'P':
				Tok = Tok.mid(1);
				switch (Tok.at(0).unicode())
				{
					case 'E':
						//
						break;
					case 'S':
						//
						break;
					default:
						break;
				}
				break;
			case 'L':
			case 'U':
				GeekSignResult(&Tok);
				if (!Tok.isEmpty())
				{
					GeekResult.append("<tr><td><b>" + QObject::tr("Rate UNIX (Linux)") + ":</b></td><td>" + Tok + "</td></tr>");
					continue;
				}
				break;
			case 'W':
				GeekSignResult(&Tok);
				if (!Tok.isEmpty())
				{
					GeekResult.append("<tr><td><b>" + QObject::tr("Rate Windows") + ":</b></td><td>" + Tok + "</td></tr>");
					continue;
				}
				break;
			case 'Y':
				//
				break;
			case 'a':
			{
				Tok = Tok.mid(1);
				bool ok;
				int age = Tok.toInt(&ok);
				if (!ok || (age < 1))
				{
					if (Tok.compare("+++") == 0)
						Tok = QObject::tr("60 and up");
					else if (Tok.compare("++") == 0)
						Tok = "50-59";
					else if (Tok.compare("+") == 0)
						Tok = "40-49";
					else if (Tok.isEmpty())
						Tok = "30-39";
					else if (Tok.compare("-") == 0)
						Tok = "25-29";
					else if (Tok.compare("--") == 0)
						Tok = "20-24";
					else if (Tok.compare("---") == 0)
						Tok = "15-19";
					else if (Tok.compare("----") == 0)
						Tok = "10-14";
					else if (Tok.compare("-----") == 0)
						Tok = QObject::tr("9 and under");
					else if (Tok.compare("?") == 0)
						Tok = QObject::tr("Immortal");
					else
						break;
				} else
					Tok = QString::number(age);
				GeekResult.append("<tr><td><b>" + QObject::tr("Age") + ":</b></td><td>" + Tok + "</td></tr>");
				continue;
			}
			case 'c':
				//
				//fix "cc"
				//
				break;
			case 'd':
				Tok = Tok.mid(1);
				if (Tok.compare("++") == 0)
					Tok = QObject::tr("Conservative");
				else if (Tok.compare("+") == 0)
					Tok = QObject::tr("Good leisure-wear");
				else if (Tok.isEmpty())
					Tok = QObject::tr("I dress a lot like those found in catalog ads");
				else if (Tok.compare("-") == 0)
					Tok = QObject::tr("Jeans and a t-shirt");
				else if (Tok.compare("--") == 0)
					Tok = QObject::tr("My t-shirts go a step further and have a trendy political message on them");
				else
					break;
				GeekResult.append("<tr><td><b>" + QObject::tr("Dress") + ":</b></td><td>" + Tok + "</td></tr>");
				continue;
			case 'e':
				Tok = Tok.mid(1);
				switch (Tok.at(0).unicode())
				{
					case 'q':
						//
						break;
					default:
						break;
				}
				break;
			case 'h':
				Tok = Tok.mid(1);
				switch (Tok.at(0).unicode())
				{
					case 'w':
						//
						break;
					default:
						break;
				}
				break;
			case 'l':
				Tok = Tok.mid(1);
				switch (Tok.at(0).unicode())
				{
					case 'n':
						//
						break;
					default:
						break;
				}
				break;
			case 'p':
				Tok = Tok.mid(1);
				switch (Tok.at(0).unicode())
				{
					case 'n':
						//
						break;
					case 'r':
						//
						break;
					default:
						break;
				}
				break;
			case 's':
				//
				// fix "sl"
				// fix "sw"
				// fix "sx"
				//
				break;
			case 'x':
				Tok = Tok.mid(1);
				if (!Tok.isEmpty())
					if (Tok.at(0) == '+')
					{
						Gender = QObject::tr("Female");
						continue;
					}
				break;
			case 'y':
				Tok = Tok.mid(1);
				if (!Tok.isEmpty())
					if (Tok.at(0) == '+')
					{
						Gender = QObject::tr("Male");
						continue;
					}
				break;
			default:
				break;
		}
		debugmessage("Unknown Geek-code prefix: " + OrigTok);
	}

	if (!GeekResult.isEmpty())
	{
		if (!Gender.isEmpty())
			GeekResult.append("<tr><td><b>" + QObject::tr("Gender") + ":</b></td><td>" + Gender + "</td></tr>");

		GeekBrowser->setText(GeekResult.prepend("<p align=\"center\"><table>").append("</table></p>"));

		if (REdition)
			setCaption(caption() + " (" + QObject::tr("Russian edition") + ")");

		return true;
	}

	return false;
}
