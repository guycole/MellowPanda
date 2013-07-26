#!/bin/bash
#
# Title:bc_trunk1.sh
#
# Description:
#  Store trunk frequencies to a bearcat scanner
#  Redding public safety trunk system.  Moto-II SmartNet
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
#
panda -r "$RADIO" -k ping
#
# manual mode
panda -r "$RADIO" -k raw/KEY01
#
# should return 01
panda -r "$RADIO" -k raw/MD
#
# menu mode
panda -r "$RADIO" -k raw/KEY11
#
# select trunk data
panda -r "$RADIO" -k "raw/KEY02 3"
#
# enter bank 2
panda -r "$RADIO" -k "raw/KEY02 2"
#
# trunk type
panda -r "$RADIO" -k "raw/KEY02 1"
#
# enter trunk type (type2 800 MHz)
panda -r "$RADIO" -k "raw/KEY02 1"
#
# back out of menu tree
panda -r "$RADIO" -k raw/KEY11
panda -r "$RADIO" -k raw/KEY11
panda -r "$RADIO" -k raw/KEY11

#
#
panda -r "$RADIO" -k "raw/PM051 08541875"
panda -r "$RADIO" -k "raw/KEY13H"
panda -r "$RADIO" -k "raw/PM052 08569625"
panda -r "$RADIO" -k "raw/KEY13H"
panda -r "$RADIO" -k "raw/PM053 08574625"
panda -r "$RADIO" -k "raw/KEY13H"
panda -r "$RADIO" -k "raw/PM054 08579625"
panda -r "$RADIO" -k "raw/KEY13H"
panda -r "$RADIO" -k "raw/PM055 08584625"
panda -r "$RADIO" -k "raw/KEY13H"
panda -r "$RADIO" -k "raw/PM056 08589625"
panda -r "$RADIO" -k "raw/KEY13H"
panda -r "$RADIO" -k "raw/PM057 08599625"
panda -r "$RADIO" -k "raw/KEY13H"
panda -r "$RADIO" -k "raw/PM058 08609625"
panda -r "$RADIO" -k "raw/KEY13H"

panda -r "$RADIO" -k "raw/MA051"
panda -r "$RADIO" -k "raw/MA052"
panda -r "$RADIO" -k "raw/MA053"
panda -r "$RADIO" -k "raw/MA054"
panda -r "$RADIO" -k "raw/MA055"
panda -r "$RADIO" -k "raw/MA056"
panda -r "$RADIO" -k "raw/MA057"
panda -r "$RADIO" -k "raw/MA058"

#
# set active bank
panda -r "$RADIO" -k "raw/SB B"

#
# scan
panda -r "$RADIO" -k raw/KEY00

#
# trunk
panda -r "$RADIO" -k raw/KEY13

#
# speaker mute auto
panda -r "$RADIO" -k raw/MUA

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
#
panda -r "$RADIO" -k raw/ID
panda -r "$RADIO" -k raw/IS
panda -r "$RADIO" -k raw/RG
