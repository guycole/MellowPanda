!/bin/bash
#
# Title:pcr_off.sh
#
# Description:
#   radio off
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
RADIO=pcr1000.a
#
# power off
panda -r "$RADIO" -k raw/H100
#
