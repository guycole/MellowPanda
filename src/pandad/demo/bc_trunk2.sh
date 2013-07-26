#!/bin/bash
#
# Title:bc_trunk.sh
#
# Description:
#  Store trunk frequencies to a bearcat BC780 scanner
#  Redding public safety trunk system.  Moto-II SmartNet
#  Define control channel only (frequency plan 1).
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
RADIO=bc780.a
#
#
panda -r "$RADIO" -k ping
#
# manual mode
panda -r "$RADIO" -k raw/KEY01
#
# write control channel to bank 1
panda -r "$RADIO" -k "raw/PM051 08541875"
panda -r "$RADIO" -k "raw/KEY13H"
#
# inspect channel
panda -r "$RADIO" -k raw/MA051
#
#
panda -r "$RADIO" -k "raw/TCN B P1"

panda -r "$RADIO" -k raw/KEY00
panda -r "$RADIO" -k raw/KEY13



#
# squelch notify mode
panda -r "$RADIO" -k raw/QUF

#
# set trunk id monitor function
panda -r "$RADIO" -k raw/IDN

#
# freq mode
panda -r "$RADIO" -k raw/FIN

#
# speaker mute auto
panda -r "$RADIO" -k raw/MUA

#
#
panda -r "$RADIO" -k raw/ID
panda -r "$RADIO" -k raw/IS
panda -r "$RADIO" -k raw/RG
