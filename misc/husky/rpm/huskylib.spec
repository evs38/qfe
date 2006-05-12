Name: huskylib
Version: 1.9
Release: 25062005
Copyright: GPL
Summary: Base libraries for Husky packages.
Group: Applications/Communications
Packager: Alexander Shiyan <shc@users.sourceforge.net>
BuildRoot: %{_tmppath}/%{name}-%{version}
URL: http://husky.sourceforge.net/
Source0: huskylib.tar.bz2
Source1: huskymak.cfg
Provides: libhusky.so, libhusky.a, gnmsgid, huskylib-devel
BuildPrereq: gcc >= 3.0

%description
This package provides base libraries for Husky - Portable Fidonet Software.

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

install -c -m 555 libhusky.so.%{version}.0 $LIBDIR

RPMPWD=`pwd`

cd $LIBDIR
ln -sf libhusky.so.%{version}.0 libhusky.so.%{version}
ln -sf libhusky.so.%{version} libhusky.so
cd $RPMPWD

install -c -m 444 libhusky.a $LIBDIR

install -c -s -m 555 gnmsgid $BINDIR
install -c -m 444 %{name}/*.h $INCDIR
install -c -m 444 man/*.1 $MANDIR

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-, root, root)
%{_bindir}/gnmsgid
%{_libdir}/libhusky.*
%{_includedir}/%{name}/*.h
%{_mandir}/*.1
%doc COPYING ChangeLog HISTORY License

%post
/sbin/ldconfig

%postun
rmdir --ignore-fail-on-non-empty %{_includedir}/%{name}
/sbin/ldconfig

%changelog
* Sat Jun 25 2005 Alexander Shiyan <shc@users.sourceforge.net>
- Wrote specfile; first build.
