#***************************************************************************
#*   Copyright (C) 2005-2006 by Alexander Shiyan                           *
#*   shc@users.sourceforge.net                                             *
#*                                                                         *
#*   This program is free software; you can redistribute it and/or modify  *
#*   it under the terms of the GNU General Public License as published by  *
#*   the Free Software Foundation; either version 2 of the License, or     *
#*   (at your option) any later version.                                   *
#*                                                                         *
#*   This program is distributed in the hope that it will be useful,       *
#*   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
#*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
#*   GNU General Public License for more details.                          *
#*                                                                         *
#*   You should have received a copy of the GNU General Public License     *
#*   along with this program; if not, write to the                         *
#*   Free Software Foundation, Inc.,                                       *
#*   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
#***************************************************************************

FORMS = ui/about.ui \
        ui/addressbook.ui \
        ui/areaprop.ui \
        ui/areaselect.ui \
        ui/attachform.ui \
        ui/editwindow.ui \
        ui/extmessage.ui \
        ui/finddlg.ui \
        ui/geekform.ui \
        ui/helpwindow.ui \
        ui/mainwindow.ui \
        ui/messattr.ui \
        ui/messprop.ui \
        ui/nodelistbrowser.ui \
        ui/options.ui \
        ui/photownd.ui \
        ui/replacedlg.ui \
        ui/styledlg.ui

HEADERS = funchider.h \
          sndwrapper.h \
          taction.h \
          tactionlist.h \
          tactions.h \
          tarea.h \
          tarea_jam.h \
          tarea_msg.h \
          tarea_squish.h \
          tareaitem.h \
          tareaselectitem.h \
          tareas.h \
          tareas_fidoconf.h \
          tbook.h \
          tbookitem.h \
          tbookviewitem.h \
          tcloseevent.h \
          tconfig.h \
          tfidoconfig.h \
          tfinditem.h \
          tftncodec.h \
          tinisettings.h \
          tmessage.h \
          tmessageitem.h \
          tnodeitem.h \
          tnodes.h \
          tpixmapview.h \
          tshortcutitem.h \
          tspellcheck.h \
          ttagitem.h \
          ttypes.h \
          utils.h \
          ui/about.ui.h \
          ui/addressbook.ui.h \
          ui/areaprop.ui.h \
          ui/areaselect.ui.h \
          ui/attachform.ui.h \
          ui/editwindow.ui.h \
          ui/extmessage.ui.h \
          ui/finddlg.ui.h \
          ui/geekform.ui.h \
          ui/helpwindow.ui.h \
          ui/mainwindow.ui.h \
          ui/messattr.ui.h \
          ui/messprop.ui.h \
          ui/nodelistbrowser.ui.h \
          ui/options.ui.h \
          ui/photownd.ui.h \
          ui/replacedlg.ui.h \
          ui/styledlg.ui.h

SOURCES = funchider.cpp \
          qfe.cpp \
          sndwrapper.cpp \
          taction.cpp \
          tactionlist.cpp \
          tactions.cpp \
          tarea.cpp \
          tarea_jam.cpp \
          tarea_msg.cpp \
          tarea_squish.cpp \
          tareaitem.cpp \
          tareas.cpp \
          tareas_fidoconf.cpp \
          tareaselectitem.cpp \
          tbook.cpp \
          tbookitem.cpp \
          tbookviewitem.cpp \
          tcloseevent.cpp \
          tconfig.cpp \
          tfinditem.cpp \
          tftncodec.cpp \
          tinisettings.cpp \
          tmessage.cpp \
          tmessageitem.cpp \
          tnodeitem.cpp \
          tnodes.cpp \
          tpixmapview.cpp \
          tshortcutitem.cpp \
          tspellcheck.cpp \
          ttagitem.cpp \
          utils.cpp

unix:!mac {
	HEADERS += binreloc.h
	SOURCES += binreloc.c
}

IMAGES = img/icon_about.xpm \
         img/icon_aboutqt.xpm \
         img/icon_additem.xpm \
         img/icon_addressbook.xpm \
         img/icon_addtobook.xpm \
         img/icon_archive.xpm \
         img/icon_area.xpm \
         img/icon_attach.xpm \
         img/icon_browser.xpm \
         img/icon_bug.xpm \
         img/icon_cancel.xpm \
         img/icon_color.xpm \
         img/icon_copy.xpm \
         img/icon_cut.xpm \
         img/icon_database.xpm \
         img/icon_date.xpm \
         img/icon_decode.xpm \
         img/icon_delete.xpm \
         img/icon_delitem.xpm \
         img/icon_delmail.xpm \
         img/icon_desc.xpm \
         img/icon_down.xpm \
         img/icon_draft.xpm \
         img/icon_edit.xpm \
         img/icon_erase.xpm \
         img/icon_exit.xpm \
         img/icon_find.xpm \
         img/icon_fips.xpm \
         img/icon_flag.xpm \
         img/icon_forward.xpm \
         img/icon_geek.xpm \
         img/icon_general.xpm \
         img/icon_golded.xpm \
         img/icon_help.xpm \
         img/icon_home.xpm \
         img/icon_html.xpm \
         img/icon_index.xpm \
         img/icon_insert.xpm \
         img/icon_keyboard.xpm \
         img/icon_left.xpm \
         img/icon_load.xpm \
         img/icon_mark.xpm \
         img/icon_menu.xpm \
         img/icon_multimedia.xpm \
         img/icon_name.xpm \
         img/icon_newmail.xpm \
         img/icon_ok.xpm \
         img/icon_options.xpm \
         img/icon_paste.xpm \
         img/icon_photo.xpm \
         img/icon_plugins.xpm \
         img/icon_print.xpm \
         img/icon_prop.xpm \
         img/icon_question.xpm \
         img/icon_redo.xpm \
         img/icon_replace.xpm \
         img/icon_reply.xpm \
         img/icon_replynet.xpm \
         img/icon_rescan.xpm \
         img/icon_right.xpm \
         img/icon_save.xpm \
         img/icon_sent.xpm \
         img/icon_size.xpm \
         img/icon_spell.xpm \
         img/icon_strings.xpm \
         img/icon_style.xpm \
         img/icon_subject.xpm \
         img/icon_template.xpm \
         img/icon_undo.xpm \
         img/icon_unread.xpm \
         img/icon_unsubscribe.xpm \
         img/icon_up.xpm \
         img/icon_view.xpm \
         img/icon_whatsthis.xpm \
         img/qfe.xpm \
         img/qfe_logo.xpm \
         img/smile_icon1.xpm \
         img/smile_icon2.xpm \
         img/smile_icon3.xpm \
         img/smile_icon4.xpm \
         img/smile_icon5.xpm \
         img/smile_icon6.xpm \
         img/smile_icon7.xpm \
         img/smile_icon8.xpm \
         img/smile_icon9.xpm

DEFAULTCODEC = UTF-8

TRANSLATIONS = ts/qfe_ru.ts
