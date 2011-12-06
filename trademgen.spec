#
%global mydocs __tmp_docdir
#
Name:           trademgen
Version:        0.2.1
Release:        1%{?dist}

Summary:        C++ Simulated Travel Demand Generation Library

Group:          System Environment/Libraries 
License:        LGPLv2+
URL:            http://sourceforge.net/projects/%{name}/
Source0:        http://downloads.sourceforge.net/%{name}/%{name}-%{version}.tar.bz2
BuildRoot:      %(mktemp -ud %{_tmppath}/%{name}-%{version}-%{release}-XXXXXX)

BuildRequires:  cmake, python-devel
BuildRequires:  boost-devel, soci-mysql-devel, zeromq-devel, readline-devel
BuildRequires:  stdair-devel


%description
%{name} aims at providing a clean API, and the corresponding C++
implementation, able to generate demand for travel solutions (e.g.,
from JFK to PEK on 25-05-2009) according to characteristics (e.g.,
Willingness-To-Pay, preferred airline, etc).

%{name} makes an extensive use of existing open-source libraries for
increased functionality, speed and accuracy. In particular the
Boost (C++ Standard Extensions: http://www.boost.org) library is used.

Install the %{name} package if you need a library of basic C++ objects
for travel-related demand generation, mainly for simulation purpose.

%package        devel
Summary:        Header files, libraries and development helper tools for %{name}
Group:          Development/Libraries
Requires:       %{name}%{?_isa} = %{version}-%{release}
Requires:       pkgconfig

%description    devel
This package contains the header files, shared libraries and
development helper tools for %{name}. If you would like to develop
programs using %{name}, you will need to install %{name}-devel.

%package        doc
Summary:        HTML documentation for the %{name} library
Group:          Documentation
%if 0%{?fedora} || 0%{?rhel} > 5
BuildArch:      noarch
%endif
BuildRequires:  tex(latex)
BuildRequires:  doxygen, ghostscript

%description    doc
This package contains HTML pages, as well as a PDF reference manual,
for %{name}. All that documentation is generated thanks to Doxygen
(http://doxygen.org). The content is the same as what can be browsed
online (http://%{name}.org).


%prep
%setup -q


%build
%cmake .
make %{?_smp_mflags}

%install
rm -rf $RPM_BUILD_ROOT
make install DESTDIR=$RPM_BUILD_ROOT

mkdir -p %{mydocs}
mv $RPM_BUILD_ROOT%{_docdir}/%{name}-%{version}/html %{mydocs}
rm -f %{mydocs}/html/installdox

%check
ctest

%clean
rm -rf $RPM_BUILD_ROOT

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig


%files
%defattr(-,root,root,-)
%doc AUTHORS ChangeLog COPYING NEWS README
%{_bindir}/%{name}
%{_bindir}/%{name}_with_db
%{_bindir}/py%{name}
%{_bindir}/%{name}_drawBookingArrivals
%{_bindir}/%{name}_extractBookingRequests
%{_libdir}/lib%{name}.so.*
%{_libdir}/libpy%{name}.so.*
%{_mandir}/man1/%{name}.1.*
%{_mandir}/man1/%{name}_with_db.1.*
%{_mandir}/man1/py%{name}.1.*
%{_mandir}/man1/%{name}_drawBookingArrivals.1.*
%{_mandir}/man1/%{name}_extractBookingRequests.1.*

%files devel
%defattr(-,root,root,-)
%{_includedir}/%{name}
%{_bindir}/%{name}-config
%{_libdir}/lib%{name}.so
%{_libdir}/libpy%{name}.so
%{_libdir}/pkgconfig/%{name}.pc
%{_datadir}/aclocal/%{name}.m4
%dir %{_datadir}/%{name}
%{_datadir}/%{name}/CMake
%{_mandir}/man1/%{name}-config.1.*
%{_mandir}/man3/%{name}-library.3.*

%files doc
%defattr(-,root,root,-)
%doc %{mydocs}/html
%doc COPYING


%changelog
* Mon Dec 05 2011 Denis Arnaud <denis.arnaud_fedora@m4x.org> 0.2.1-1
- Upstream update

* Fri Aug 19 2011 Denis Arnaud <denis.arnaud_fedora@m4x.org> 0.1.0-2
- Added (BR) missing packages needed by a mock build

* Sun Aug 07 2011 Denis Arnaud <denis.arnaud_fedora@m4x.org> 0.1.0-1
- First package

