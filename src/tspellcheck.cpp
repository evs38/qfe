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

#include "tspellcheck.h"

#if defined(ENABLE_ASPELL)
#	include <aspell.h>
#	warning "Aspell support is under development."
	AspellConfig *spell_config = NULL;
	AspellCanHaveError *possible_err = NULL;
	AspellSpeller *spell_checker = NULL;
#endif

bool SpellCheckInit()
{
#if defined(ENABLE_ASPELL)
	debugmessage("Spell checker is still under construction...");
	if (spell_config == NULL)
		spell_config = new_aspell_config();
	return (spell_config != NULL);
#else
	return false;
#endif
}

void SpellCheckDone()
{
#if defined(ENABLE_ASPELL)
	if (spell_config != NULL)
	{
		SpellCheckDestroySpeller();
		delete_aspell_config(spell_config);
		spell_config = NULL;
	}
#endif
}

void SpellCheckSetup()
{
#if defined(ENABLE_ASPELL)
	if (spell_config != NULL)
	{
		aspell_config_replace(spell_config, "encoding", "utf-8");
		aspell_config_replace(spell_config, "lang", "en_US");//!!!
		//
	}
#endif
}

bool SpellCheckCreateSpeller(QWidget *Wnd)
{
#if defined(ENABLE_ASPELL)
	possible_err = new_aspell_speller(spell_config);
	if (aspell_error_number(possible_err) != 0)
	{
		errormessage(Wnd, aspell_error_message(possible_err));
		return false;
	} else
		spell_checker = to_aspell_speller(possible_err);
	return (spell_checker != NULL);
#else
	Q_UNUSED(Wnd);
	return false;
#endif
}

void SpellCheckDestroySpeller()
{
#if defined(ENABLE_ASPELL)
	delete_aspell_speller(spell_checker);
#endif
}

//int correct = aspell_speller_check(spell_checker, word, size);

/*
AspellWordList * suggestions = aspell_speller_suggest(spell_checker, word, size);
AspellStringEnumeration * elements = aspell_word_list_elements(suggestions);
 const char * word;
 while ( (word = aspell_string_enumeration_next(aspell_elements)) != NULL )
 {
   // add to suggestion list
 }
 delete_aspell_string_enumeration(elements);
*/
