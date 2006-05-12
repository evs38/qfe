Name: areafix
Version: 1.9
Release: 25062005
Copyright: GPL
Summary: Areafix library for Husky packages.
Group: Applications/Communications
Packager: Alexander Shiyan <shc@users.sourceforge.net>
BuildRoot: %{_tmppath}/%{name}-%{version}
URL: http://husky.sourceforge.net/
Source0: areafix.tar.bz2
Source1: huskymak.cfg
#Patch0: qfe.patch
Provides: libareafix.so, libareafix.a, areafix-devel
Requires: huskylib = %{version}-%{release}, fidoconf = %{version}-%{release}, smapi >= 2.5
BuildPrereq: huskylib-devel = %{version}-%{release}, fidoconf-devel = %{version}-%{release}, smapi-devel >= 2.5

%description
This library is a core part of HUSKY portable fidonet software.

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

install -c -m 555 libareafix.so.%{version}.0 $LIBDIR

RPMPWD=`pwd`

cd $LIBDIR
ln -sf libareafix.so.%{version}.0 libareafix.so.%{version}
ln -sf libareafix.so.%{version} libareafix.so
cd $RPMPWD

install -c -m 444 libareafix.a $LIBDIR

install -c -m 444 %{name}/*.h $INCDIR

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-, root, root)
%{_libdir}/libareafix.*
%{_includedir}/%{name}/*.h
%doc COPYING ChangeLog HISTORY TODO

%post
/sbin/ldconfig

%postun
rmdir --ignore-fail-on-non-empty %{_includedir}/%{name}
/sbin/ldconfig

%changelog
* Sat Jun 25 2005 Alexander Shiyan <shc@users.sourceforge.net>
- Wrote specfile; first build.
