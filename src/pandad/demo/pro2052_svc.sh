#!/bin/bash
#
# Title:pro2052_svc.sh
#
# Description:
#   Start SVC search.
#   Advance SVC by one (i.e. AIR to WX).
#   Unable to detect what the current SVC selection is.
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
RADIO=pro2052
#RADIO=radio0bc
#
#
./panda -r "$RADIO" -k ping
#
# manual mode
./panda -r "$RADIO" -k raw/KEY01
#
# disable squelch notification
./panda -r "$RADIO" -k raw/QUF
#
# disable priority notification
./panda -r "$RADIO" -k raw/RIF
#
# disable priority scan
./panda -r "$RADIO" -k raw/PRF
#
# advance to next service
./panda -r "$RADIO" -k raw/KEY10
./panda -r "$RADIO" -k raw/KEY10
./panda -r "$RADIO" -k raw/MD
#
