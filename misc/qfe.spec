Name: qfe
Version: 0.5.1
Release: 0.fc6
License: GPL
Summary: Full-featured FTN message editor with a graphical interface.
Group: Applications/Communications
Packager: Alexander Shiyan <shc@users.sourceforge.net>
BuildRoot: %{_tmppath}/%{name}-%{version}
URL: http://qfe.sourceforge.net/
Source: ftp://ftp.sourceforge.net/pub/sourceforge/q/qf/qfe/qfe-%{version}.tar.bz2
Requires: qt > 3.2, qt < 4
BuildPrereq: grep, sed, qt-devel > 3.2

%description
QFE is full-featured cross-platform FTN message editor with a graphical
interface written on C++/Qt and does not depend on either KDE, Gnome or other
window managers.
Operations with FIDO message base use fidoconfig package from HUSKY project.

%description -l ru_RU.UTF-8
QFE это кросс-платформенный почтовый редактор для сети ФИДОНЕТ с графическим
интерфейсом, написан на C++/Qt и не зависит от библиотек KDE, Gnome и других
оконных менеджеров.
Работа с почтовыми базами ФИДО осуществляется при помощи библиотеки fidoconfig
из проекта HUSKY.

%prep
%setup -q -n %{name}

%build
find . -name Makefile -exec rm -f {} \;
export QFE_PREFIX=/usr
qmake "CONFIG+=staticlibs debug_off quiet"
make clean
make RPM_OPT_FLAGS="$RPM_OPT_FLAGS"

%install
rm -rf $RPM_BUILD_ROOT
export QFE_PREFIX=/usr
make INSTALL_ROOT="$RPM_BUILD_ROOT" install

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-, root, root)
%{_bindir}/qfe
%{_datadir}/applications/qfe.desktop
%{_datadir}/icons/fidonet.xpm
%{_datadir}/icons/qfe.xpm
%{_datadir}/pixmaps/qfe.xpm
%{_datadir}/mimelnk/message/*.desktop
%{_datadir}/%{name}/AUTHORS
%{_datadir}/%{name}/COPYING
%{_datadir}/%{name}/README
%{_datadir}/%{name}/TODO
%{_datadir}/%{name}/FAQ
%{_datadir}/%{name}/ChangeLog
%{_datadir}/%{name}/qfe.lsm
%{_datadir}/%{name}/translations/*.qm
%{_datadir}/%{name}/examples/*.bat
%{_datadir}/%{name}/examples/*.sh
%{_datadir}/%{name}/config.sample

%post
if [ -x /usr/bin/gtk-update-icon-cache ]; then
  /usr/bin/gtk-update-icon-cache --ignore-theme-index --quiet /usr/share/icons || :
  /usr/bin/gtk-update-icon-cache --ignore-theme-index --quiet /usr/share/pixmaps || :
fi
update-desktop-database &> /dev/null || :

%postun
if [ -x /usr/bin/gtk-update-icon-cache ]; then
  /usr/bin/gtk-update-icon-cache --ignore-theme-index --quiet /usr/share/icons || :
  /usr/bin/gtk-update-icon-cache --ignore-theme-index --quiet /usr/share/pixmaps || :
fi
update-desktop-database &> /dev/null || :
rmdir --ignore-fail-on-non-empty %{_datadir}/%{name}/translations
rmdir --ignore-fail-on-non-empty %{_datadir}/%{name}

%changelog
* Sat Jan 27 2007 Alexander Shiyan <shc@users.sourceforge.net>
- Update to version 0.5.1.

* Mon May 30 2006 Alexander Shiyan <shc@users.sourceforge.net>
- Add FTN mime-icons.
- Added Russian description is spec.
- Update to version 0.5

* Mon Dec 12 2005 Alexander Shiyan <shc@users.sourceforge.net>
- Update to version 0.4.6.

* Mon Oct 31 2005 Alexander Shiyan <shc@users.sourceforge.net>
- Update to version 0.4.5.
- Add "update-desktop-database" to scripts.

* Mon Jun 06 2005 Alexander Shiyan <shc@users.sourceforge.net>
- Update to version 0.4.
- Add "examples".

* Wed May 18 2005 Alexander Shiyan <shc@users.sourceforge.net>
- Update to version 0.3.

* Fri Apr 22 2005 Alexander Shiyan <shc@users.sourceforge.net>
- Update to version 0.2.

* Thu Apr 07 2005 Alexander Shiyan <shc@users.sourceforge.net>
- Wrote specfile; first build.
