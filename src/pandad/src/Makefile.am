# Copyright (C) 2010 Guy Cole <gsc@digiburo.com>
#  
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without 
# modifications, as long as this notice is preserved.
# 
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

EXTRA_DIST = pandad.conf

bin_PROGRAMS = panda 
sbin_PROGRAMS = pandad
sysconf_DATA = pandad.conf
panda_SOURCES = client.c client.h log.c log.h utility.c utility.h \
	configuration.c configuration.h socket.c socket.h device.c device.h \
        rs232.c rs232.h icom.c icom.h bearcat.c bearcat.h
pandad_SOURCES = daemon.c daemon.h log.c log.h device.c device.h \
        rs232.c rs232.h utility.c utility.h configuration.c configuration.h \
        socket.c socket.h eclectic.c eclectic.h command.c command.h \
        bearcat.c bearcat.h icom.c icom.h
