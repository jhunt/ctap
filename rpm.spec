Name:           ctap
Version:        2.0.1
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

%description
ctap is an easy way to get Perl-style TAP testing convenience in C. It
ships as a standalone shared library that you can link to your tests, and a
header file that contains functions and macros for doing things like
assertions, skip/todo blocks and dynamic evaluation.

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
/usr/include/ctap.h


%changelog
* Sun Aug  7 2016 James Hunt <james@niftylogic.com> 2.0.1-1
- Bump to upstream 2.0.1 release.

* Fri Oct 16 2015 James Hunt <james@niftylogic.com> 2.0.0-1
- Initial RPM package
