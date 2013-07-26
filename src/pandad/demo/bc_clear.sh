#!/bin/bash
#
# Title:bc_clear.sh
#
# Description:
#   Clear the memory
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
panda -r "$RADIO" -k raw/AC
#
