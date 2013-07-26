#!/bin/bash
#
# Title:bc780_trunk.sh
#
# Description:
#  Store trunk frequencies to a BC780 scanner
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
#PATH=/bin:/usr/bin:/etc:/usr/local/bin; export PATH
#
#RADIO=pro2052
RADIO=radio0bc
#
#
./panda -r "$RADIO" -k ping
#
# manual mode
./panda -r "$RADIO" -k raw/KEY01
#
# should return 01
./panda -r "$RADIO" -k raw/MD
#
# menu
./panda -r "$RADIO" -k raw/KEY11
#
# select trunk data
./panda -r "$RADIO" -k "raw/KEY02 3"
#
# enter bank 1
./panda -r "$RADIO" -k raw/KEY04
#
#

# enter trunk type
./panda -r "$RADIO" -k raw/KEY04
#
#

# enter type2 800
./panda -r "$RADIO" -k raw/KEY04
#
#
#
# select control channel only
./panda -r "$RADIO" -k "raw/KEY02 0"
#
# enter control channel only
./panda -r "$RADIO" -k raw/KEY04

#
# select contrl channel plan
./panda -r "$RADIO" -k "raw/KEY02 1"
#
# enter control channel plan
./panda -r "$RADIO" -k raw/KEY04

# menu
./panda -r "$RADIO" -k raw/KEY11
./panda -r "$RADIO" -k raw/KEY11
./panda -r "$RADIO" -k raw/KEY11
./panda -r "$RADIO" -k raw/KEY11

./panda -r "$RADIO" -k raw/KEY01
./panda -r "$RADIO" -k "raw/PM002 08541875"

./panda -r "$RADIO" -k raw/KEY13H

./panda -r "$RADIO" -k raw/KEY00
./panda -r "$RADIO" -k raw/KEY13

#########################33


#./panda -r "$RADIO" -k "raw/TR B 2"
#./panda -r "$RADIO" -k "raw/KEY13H"
#./panda -r "$RADIO" -k "raw/KEY02 2"
#./panda -r "$RADIO" -k "raw/KEY04"
#./panda -r "$RADIO" -k "raw/PM002 08541875"
#./panda -r "$RADIO" -k "raw/PM003 08569625"
#./panda -r "$RADIO" -k "raw/PM004 08574625"
#./panda -r "$RADIO" -k "raw/PM005 08579625"
#./panda -r "$RADIO" -k "raw/PM006 08584625"
#./panda -r "$RADIO" -k "raw/PM007 08589625"
#./panda -r "$RADIO" -k "raw/PM008 08599625"
#./panda -r "$RADIO" -k "raw/PM009 08609625"
#./panda -r "$RADIO" -k "raw/KEY13"
#
#./panda -r "$RADIO" -k raw/MUA
#./panda -r "$RADIO" -k raw/QUF
#./panda -r "$RADIO" -k raw/IDN
#./panda -r "$RADIO" -k raw/FIN
#./panda -r "$RADIO" -k raw/ID
#./panda -r "$RADIO" -k raw/IS
#./panda -r "$RADIO" -k raw/RG
#
# scan
#./panda -r "$RADIO" -k raw/KEY13
#./panda -r "$RADIO" -k "raw/SB B"
#./panda -r "$RADIO" -k raw/KEY00
#
#./panda -r "$RADIO" -k raw/KEY07
#./panda -r "$RADIO" -k "raw/KEY02 2"
#./panda -r "$RADIO" -k raw/KEY09
#
