#!/bin/bash
#
# Title:bc780_trunk.sh
#
# Description:
#  Store trunk frequencies to a BC780 scanner
#  Redding public safety trunk system.  Moto-II SmartNet
#  Define control channel only.
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
######################################
# select trunk data
./panda -r "$RADIO" -k "raw/KEY02 3"
#
# enter trunk data selection
./panda -r "$RADIO" -k raw/KEY04
######################################
# select bank 1
./panda -r "$RADIO" -k "raw/KEY02 1"
#
# enter bank 1
./panda -r "$RADIO" -k raw/KEY04
######################################
# select trunk type/option 1
./panda -r "$RADIO" -k "raw/KEY02 1"
#
# enter trunk type
./panda -r "$RADIO" -k raw/KEY04
######################################
# select type 2 moto800/option 1
./panda -r "$RADIO" -k "raw/KEY02 1"
#
# enter type2 800
./panda -r "$RADIO" -k raw/KEY04
######################################
# select control channel only
./panda -r "$RADIO" -k "raw/KEY02 0"
#
# enter control channel only
./panda -r "$RADIO" -k raw/KEY04
######################################
# select control channel plan
./panda -r "$RADIO" -k "raw/KEY02 1"
#
# enter control channel plan
./panda -r "$RADIO" -k raw/KEY04
######################################
# menu
./panda -r "$RADIO" -k raw/KEY11
./panda -r "$RADIO" -k raw/KEY11
./panda -r "$RADIO" -k raw/KEY11
######################################
# control channel
./panda -r "$RADIO" -k "raw/PM002 08541875"
# trunk
./panda -r "$RADIO" -k raw/KEY13H
./panda -r "$RADIO" -k raw/KEY00
./panda -r "$RADIO" -k raw/MD
./panda -r "$RADIO" -k raw/IDN
#
