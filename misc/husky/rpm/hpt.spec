Name: hpt
Version: 1.9
Release: 25062005
Copyright: GPL
Summary: HPT is a FidoNet tosser.
Group: Applications/Communications
Packager: Alexander Shiyan <shc@users.sourceforge.net>
BuildRoot: %{_tmppath}/%{name}-%{version}
URL: http://husky.sourceforge.net/
Source0: hpt.tar.bz2
Source1: huskymak.cfg
Patch0: hpt-compile.patch
#Patch1: qfe.patch
Provides: pktinfo, txt2pkt, hptlink, hpttree
Requires: huskylib = %{version}-%{release}, fidoconf = %{version}-%{release}, areafix = %{version}-%{release}, smapi >= 2.5
BuildPrereq: huskylib-devel = %{version}-%{release}, fidoconf-devel = %{version}-%{release}, areafix-devel = %{version}-%{release}, smapi-devel >= 2.5

%description
HPT is a FidoNet tosser. It is a part of HUSKY portable fidonet software.

%prep
%setup -q -n %{name}
%patch0 -p1
#%patch1 -p1

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

install -c -s -m 555 hpt $BINDIR
install -c -s -m 555 pktinfo $BINDIR
install -c -s -m 555 txt2pkt $BINDIR
install -c -s -m 555 hptlink $BINDIR
install -c -s -m 555 hpttree $BINDIR

install -c -m 444 man/*.1 $MANDIR

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-, root, root)
%{_bindir}/hpt
%{_bindir}/pktinfo
%{_bindir}/txt2pkt
%{_bindir}/hptlink
%{_bindir}/hpttree
%{_mandir}/*.1
%doc BUGS COPYING CREDITS ChangeLog HISTORY README TODO

%changelog
* Sat Jun 25 2005 Alexander Shiyan <shc@users.sourceforge.net>
- Wrote specfile; first build.
