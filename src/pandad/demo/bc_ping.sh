#!/bin/bash
#
# Title:bc_ping.sh
#
# Description:
#   ping a radio
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
