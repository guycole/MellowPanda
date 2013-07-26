#!/bin/bash
#
# Title:bc780_atis.sh
#
# Description:
#   Manual tune to KRDD ATIS
#
# Development Environment:
#   Ubuntu 8.10
#
# Legalise:  
#   Copyright (C) 2010 Digital Burro, INC.
#
# Author:
#   G.S. Cole (guycole at gmail dot com)
#
# Maintenance History:
#   $Id$
#
#   $Log$
#
PATH=/bin:/usr/bin:/etc:/usr/local/bin; export PATH
#
#RADIO=pro2052
RADIO=bc780.a
#
panda -r "$RADIO" -k ping
#
# squelch notification
panda -r "$RADIO" -k raw/QUF
#
# priority notification
panda -r "$RADIO" -k raw/RIF
#
# id notification
panda -r "$RADIO" -k raw/IDF
#
# mute auto
panda -r "$RADIO" -k raw/MUA
#
# manual mode
panda -r "$RADIO" -k raw/KEY01
#
# should return 01
panda -r "$RADIO" -k raw/MD
#
# radio tune
panda -r "$RADIO" -k raw/RF01241000
#
