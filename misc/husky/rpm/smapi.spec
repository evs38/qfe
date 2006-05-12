Name: smapi
Version: 2.5
Release: 25062005
Copyright: LGPL
Summary: SMAPI; Modified Squish MSGAPI.
Group: Applications/Communications
Packager: Alexander Shiyan <shc@users.sourceforge.net>
BuildRoot: %{_tmppath}/%{name}-%{version}
URL: http://husky.sourceforge.net/
Source0: smapi.tar.bz2
Source1: huskymak.cfg
Provides: libsmapi.so, libsmapi.a, smapi-devel
Requires: huskylib = %{version}-%{release}
BuildPrereq: huskylib-devel = %{version}-%{release}

%description
This package provides SMAPI library is part for the HUSKY fidosoft project.

%prep
%setup -q -n %{name}

%build
cp -f %{SOURCE1} ..
make RPM_OPT_FLAGS="$RPM_OPT_FLAGS"

%install
[ "$RPM_BUILD_ROOT" != "/" ] && rm -rf $RPM_BUILD_ROOT

BINDIR=$RPM_BUILD_ROOT%{_bindir}
ETCDIR=$RPM_BUILD_ROOT/etc
INCDIR=$RPM_BUILD_ROOT%{_includedir}/%{name}
LIBDIR=$RPM_BUILD_ROOT%{_libdir}
MANDIR=$RPM_BUILD_ROOT%{_mandir}

mkdir -p $BINDIR
mkdir -p $ETCDIR
mkdir -p $INCDIR
mkdir -p $LIBDIR
mkdir -p $MANDIR

install -c -m 555 libsmapi.so.%{version}.0 $LIBDIR

RPMPWD=`pwd`

cd $LIBDIR
ln -sf libsmapi.so.%{version}.0 libsmapi.so.%{version}
ln -sf libsmapi.so.%{version} libsmapi.so
cd $RPMPWD

install -c -m 444 libsmapi.a $LIBDIR

install -c -m 444 %{name}/*.h $INCDIR

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-, root, root)
%{_libdir}/libsmapi.*
%{_includedir}/%{name}/*.h
%doc BUGS COPYING ChangeLog HISTORY LICENSE TODO readme.txt

%post
/sbin/ldconfig

%postun
rmdir --ignore-fail-on-non-empty %{_includedir}/%{name}
/sbin/ldconfig

%changelog
* Sat Jun 25 2005 Alexander Shiyan <shc@users.sourceforge.net>
- Wrote specfile; first build.
