#!/bin/bash
#
# Title:bc780_wx.sh
#
# Description:
#  Tune to NOAA weather
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
# disable squelch notification
panda -r "$RADIO" -k raw/QUF
#
# disable priority notification
panda -r "$RADIO" -k raw/RIF
#
# disable priority scan
panda -r "$RADIO" -k raw/PRF
#
# manual mode
panda -r "$RADIO" -k raw/KEY01
#
# select weather
panda -r "$RADIO" -k raw/KEY10
#
