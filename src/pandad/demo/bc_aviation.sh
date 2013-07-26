#!/bin/bash
#
# Title:bc_aviation.sh
#
# Description:
#   Store aviation frequencies to a bearcat scanner.
#   First bank.  Sets RDD tower as a priority channel.
#   Enables squelch and priority notification.
#   Should resume scanning after emitter detection.
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
# manual mode
panda -r "$RADIO" -k raw/KEY01
#
# should return 01
panda -r "$RADIO" -k raw/MD
#
# RDD tower
panda -r "$RADIO" -k "raw/PM001 01198000"
panda -r "$RADIO" -k "raw/TA C 001 KRDD Tower"
#
# Benton
panda -r "$RADIO" -k "raw/PM002 01228000"
panda -r "$RADIO" -k "raw/TA C 002 Benton"
#
# Red Bluff
panda -r "$RADIO" -k "raw/PM003 01230000"
panda -r "$RADIO" -k "raw/TA C 003 KRBL"
#
# emergency
panda -r "$RADIO" -k "raw/PM004 01215000"
panda -r "$RADIO" -k "raw/TA C 004 Emergency"
#
# flight watch
panda -r "$RADIO" -k "raw/PM005 01220000"
panda -r "$RADIO" -k "raw/TA C 005 Flight Watch"
#
# air to air
panda -r "$RADIO" -k "raw/PM006 01227500"
panda -r "$RADIO" -k "raw/TA C 006 Air To Air"
#
# oakland
# 
panda -r "$RADIO" -k "raw/PM010 01199750"
panda -r "$RADIO" -k "raw/TA C 010 Oakland"
panda -r "$RADIO" -k "raw/PM011 01322000"
panda -r "$RADIO" -k "raw/TA C 011 Oakland"
panda -r "$RADIO" -k "raw/PM012 01349750"
panda -r "$RADIO" -k "raw/TA C 012 Oakland"
panda -r "$RADIO" -k "raw/PM013 03062000"
panda -r "$RADIO" -k "raw/TA C 013 Oakland"
panda -r "$RADIO" -k "raw/PM014 03503000"
panda -r "$RADIO" -k "raw/TA C 014 Oakland"
panda -r "$RADIO" -k "raw/PM015 03792000"
panda -r "$RADIO" -k "raw/TA C 015 Oakland"
#
# define priority channel
panda -r "$RADIO" -k "raw/PC A004"
#
#
panda -r "$RADIO" -k "raw/TA B A Aviation"
#
# squelch notification
panda -r "$RADIO" -k raw/QUN
#
# priority notification
panda -r "$RADIO" -k raw/RIN
#
# mute auto
panda -r "$RADIO" -k raw/MUA
#
# select scan bank A
panda -r "$RADIO" -k "raw/SB A"
#
# scan
panda -r "$RADIO" -k raw/KEY00
#
# enable priority scan
panda -r "$RADIO" -k raw/PRN
#
#
panda -r "$RADIO" -k raw/MD
panda -r "$RADIO" -k "raw/PC A"
#
