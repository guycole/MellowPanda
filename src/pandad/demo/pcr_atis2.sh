#!/bin/bash
#
# Title:pcr_atis.sh
#
# Description:
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
RADIO=radio2pcr
#
./panda -r "$RADIO" -k ping
#
./panda -r "$RADIO" -k raw/H101
./panda -r "$RADIO" -k raw/J4050
./panda -r "$RADIO" -k raw/K00124100000020300
#

