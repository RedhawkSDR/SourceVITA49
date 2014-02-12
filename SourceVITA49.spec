#
# This file is protected by Copyright. Please refer to the COPYRIGHT file
# distributed with this source distribution.
#
# This file is part of REDHAWK.
#
# REDHAWK is free software: you can redistribute it and/or modify it
# under the terms of the GNU Lesser General Public License as published by the
# Free Software Foundation, either version 3 of the License, or (at your
# option) any later version.
#
# REDHAWK is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
# FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License
# for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this program.  If not, see http://www.gnu.org/licenses/.
#
# By default, the RPM will install to the standard REDHAWK SDR root location (/var/redhawk/sdr)
# You can override this at install time using --prefix /new/sdr/root when invoking rpm (preferred method, if you must)
%{!?_sdrroot: %define _sdrroot /var/redhawk/sdr}
%define _prefix %{_sdrroot}
Prefix: %{_prefix}

# Point install paths to locations within our target SDR root
%define _sysconfdir    %{_prefix}/etc
%define _localstatedir %{_prefix}/var
%define _mandir        %{_prefix}/man
%define _infodir       %{_prefix}/info

Name: SourceVITA49
Summary: Component %{name}
Version: 1.1.0
Release: 5%{?dist}
License: None
Group: REDHAWK/Components
Source: %{name}-%{version}.tar.gz
BuildRoot: %{_tmppath}/%{name}-root

Requires: redhawk >= 1.8.6
BuildRequires: redhawk-devel >= 1.8.6

# Interface requirements
Requires: bulkioInterfaces >= 1.8.6
BuildRequires: bulkioInterfaces >= 1.8.6

# C++ requirements
Requires: libVITA49
BuildRequires: libVITA49-devel


%description
The SourceVITA49 REDHAWK component connects to a UDP/multicast or TCP VITA49 packet stream and converts the headers to SRI Keywords and data to the BULKIO interface of the user's choice for use within REDHAWK domain applications.

The Keywords generated from the packet are documented in the attached readme.txt
 * Commit: __REVISION__
 * Source Date/Time: __DATETIME__


%prep
%setup

%build
# Implementation cpp
pushd cpp
./reconf
%ifarch x86_64
%define _bindir %{_prefix}/dom/components/SourceVITA49/cpp_x86_64
%else
%define _bindir %{_prefix}/dom/components/SourceVITA49/cpp_i686
%endif
%configure
make
popd

%install
rm -rf $RPM_BUILD_ROOT
# Implementation cpp

pushd cpp
%ifarch x86_64
%define _bindir %{_prefix}/dom/components/SourceVITA49/cpp_x86_64 
%else
%define _bindir %{_prefix}/dom/components/SourceVITA49/cpp_i686
%endif
make install DESTDIR=$RPM_BUILD_ROOT
popd

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,redhawk,redhawk)
%dir %{_prefix}/dom/components/%{name}
%{_prefix}/dom/components/%{name}/SourceVITA49.spd.xml
%{_prefix}/dom/components/%{name}/SourceVITA49.prf.xml
%{_prefix}/dom/components/%{name}/SourceVITA49.scd.xml
%ifarch x86_64
%{_prefix}/dom/components/%{name}/cpp_x86_64
%else 
%{_prefix}/dom/components/%{name}/cpp_i686
%endif
