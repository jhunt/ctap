Name:           libctap
Version:        1.1.6
Release:        1%{?dist}
Summary:        Test Anything Protocol for C

Group:          System Environment/Libraries
License:        GPLv3+
URL:            https://github.com/filefrog/ctap
Source0:        %{name}-%{version}.tar.gz
BuildRoot:      %{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)

BuildRequires:  autoconf
BuildRequires:  automake
BuildRequires:  gcc
BuildRequires:  libtool

%description
ctap is an easy way to get Perl-style TAP testing convenience in C. It
ships as a standalone shared library that you can link to your tests, and a
header file that contains functions and macros for doing things like
assertions, skip/todo blocks and dynamic evaluation.

%package devel
Summary:        Test Anything Protocol for C - Development Files
Group:          Development/Libraries

%description devel
ctap is an easy way to get Perl-style TAP testing convenience in C. It
ships as a standalone shared library that you can link to your tests, and a
header file that contains functions and macros for doing things like
assertions, skip/todo blocks and dynamic evaluation.

This package contains the header files for writing tests with libctap


%prep
%setup -q


%build
%configure
make %{?_smp_mflags}


%install
rm -rf $RPM_BUILD_ROOT
make install DESTDIR=$RPM_BUILD_ROOT


%clean
rm -rf $RPM_BUILD_ROOT


%files
%defattr(-,root,root,-)
%{_libdir}/libctap.so
%{_libdir}/libctap.so.0
%{_libdir}/libctap.so.0.0.0

%files devel
%defattr(-,root,root,-)
/usr/include/ctap.h
%{_libdir}/libctap.a
%{_libdir}/libctap.la


%changelog
* Tue May 19 2015 James Hunt <james@niftylogic.com> 1.1.6-1
- Initial RPM package
