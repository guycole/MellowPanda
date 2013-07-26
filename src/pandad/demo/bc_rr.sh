#!/bin/bash
#
# Title:bc_rr.sh
#
# Description:
#   Railroad service search
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
# search button
panda -r "$RADIO" -k raw/KEY09H
#
# select option 6 (railroad)
panda -r "$RADIO" -k "raw/KEY02 6"
#
# enter
panda -r "$RADIO" -k raw/KEY04
#
