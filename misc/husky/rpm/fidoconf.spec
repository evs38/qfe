Name: fidoconf
Version: 1.9
Release: 25062005
Copyright: GPL
Summary: Fidoconfig library for Husky packages.
Group: Applications/Communications
Packager: Alexander Shiyan <shc@users.sourceforge.net>
BuildRoot: %{_tmppath}/%{name}-%{version}
URL: http://husky.sourceforge.net/
Source0: fidoconf.tar.bz2
Source1: huskymak.cfg
#Patch0: qfe.patch
Provides: libfidoconfig.so, libfidoconfig.a, tparser, fidoconf-devel
Requires: huskylib = %{version}-%{release}
BuildPrereq: huskylib-devel = %{version}-%{release}

%description
This library is a core part of HUSKY portable fidonet software which
parsing and manipulating config file.

%prep
%setup -q -n %{name}
#%patch0 -p1

%build
cp -f %{SOURCE1} ..
make RPM_OPT_FLAGS="$RPM_OPT_FLAGS"

%install
[ "$RPM_BUILD_ROOT" != "/" ] && rm -rf $RPM_BUILD_ROOT

BINDIR=$RPM_BUILD_ROOT%{_bindir}
ETCDIR=$RPM_BUILD_ROOT%{_bindir}/../etc
INCDIR=$RPM_BUILD_ROOT%{_includedir}/%{name}
LIBDIR=$RPM_BUILD_ROOT%{_libdir}
MANDIR=$RPM_BUILD_ROOT%{_mandir}

mkdir -p $BINDIR
mkdir -p $ETCDIR
mkdir -p $INCDIR
mkdir -p $LIBDIR
mkdir -p $MANDIR

install -c -m 555 libfidoconfig.so.%{version}.0 $LIBDIR

RPMPWD=`pwd`

cd $LIBDIR
ln -sf libfidoconfig.so.%{version}.0 libfidoconfig.so.%{version}
ln -sf libfidoconfig.so.%{version} libfidoconfig.so
cd $RPMPWD

install -c -m 444 libfidoconfig.a $LIBDIR

install -c -s -m 555 linked $BINDIR
install -c -s -m 555 tparser $BINDIR
install -c -s -m 555 fconf2* $BINDIR
install -c -s -m 555 fecfg2fconf $BINDIR
install -c -m 555 util/linkedto $BINDIR
install -c -m 555 util/fconf2na.pl $BINDIR
install -c -m 555 util/fconf2areasbbs.pl $BINDIR

install -c -m 444 %{name}/*.h $INCDIR
install -c -m 444 %{name}/*.pas $INCDIR
install -c -m 444 man/*.1 $MANDIR

install -c -m 666 doc/config $ETCDIR/husky.config

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-, root, root)
%{_bindir}/linked
%{_bindir}/tparser
%{_bindir}/fconf2*
%{_bindir}/fecfg2fconf
%{_bindir}/linkedto
%{_libdir}/libfidoconfig.*
%{_includedir}/%{name}/*.h
%{_includedir}/%{name}/*.pas
%{_mandir}/*.1
%doc COPYING ChangeLog HISTORY README.TXT TODO
%dir %{_bindir}/../etc
%config %{_bindir}/../etc/husky.config

%post
/sbin/ldconfig

%postun
rmdir --ignore-fail-on-non-empty %{_includedir}/%{name}
/sbin/ldconfig

%changelog
* Sat Jun 25 2005 Alexander Shiyan <shc@users.sourceforge.net>
- Wrote specfile; first build.
