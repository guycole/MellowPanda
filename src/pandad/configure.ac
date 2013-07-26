# Copyright (C) 2010 Guy Cole <gsc@digiburo.com>
#  
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without 
# modifications, as long as this notice is preserved.
# 
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

AC_INIT([mellow-panda],
        [1.0.1],
        [Guy Cole guycole@gmail.com],
        [mellow-panda])
AC_CONFIG_AUX_DIR(config)
AM_CONFIG_HEADER(config.h)
AM_INIT_AUTOMAKE([dist-bzip2])

#
# define configuration directory
#
CFLAGS=-DCONFIGURATION_DIRECTORY='\"${sysconfdir}\"'

AC_PROG_CC
AC_PROG_INSTALL
AC_AIX
AC_ISC_POSIX
AC_MINIX
AC_STDC_HEADERS
AC_PROG_RANLIB

AC_CONFIG_FILES([
   Makefile
   README
   demo/Makefile
   doc/Makefile
   m4/Makefile
   src/Makefile
])

AC_OUTPUT
