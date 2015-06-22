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
AC_INIT(rh.SourceVITA49, 3.0.0)
AM_INIT_AUTOMAKE(nostdinc)

AC_PROG_CC
AC_PROG_CXX
AC_PROG_INSTALL

AC_CORBA_ORB
OSSIE_CHECK_OSSIE
OSSIE_SDRROOT_AS_PREFIX

m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

# Dependencies
export PKG_CONFIG_PATH="$PKG_CONFIG_PATH:/usr/local/lib/pkgconfig"
PKG_CHECK_MODULES([PROJECTDEPS], [ossie >= 1.10 omniORB4 >= 4.1.0  ])
PKG_CHECK_MODULES([INTERFACEDEPS], [bulkio >= 1.10])
OSSIE_ENABLE_LOG4CXX
AX_BOOST_BASE([1.41])
AX_BOOST_SYSTEM
AX_BOOST_THREAD
CHECK_VECTOR_IMPL
AC_SUBST([ARCH],[`uname -p`])
export PKG_CONFIG_PATH="$PKG_CONFIG_PATH:${SDR_ROOT}/dom/deps/VITA49_v1/cpp/lib/pkgconfig"
PKG_CHECK_MODULES(VITA49_v1, VITA49_v1 >= 1.0.0)
AC_SUBST(VITA49_v1_CFLAGS)
AC_SUBST(VITA49_v1_LIBS)
CXXFLAGS="$CXXFLAGS $VITA49_v1_CFLAGS"
LIBS="$LIBS $VITA49_v1_LIBS"
AX_BOOST_REGEX

AC_CONFIG_FILES([Makefile])
AC_OUTPUT

