!/bin/bash
#
# Title:pcr_atis.sh
#
# Description:
#   Manual tune to KRDD ATIS
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
# power up
panda -r "$RADIO" -k raw/H101
#
# volume
panda -r "$RADIO" -k raw/J407F
#
# frequency
panda -r "$RADIO" -k raw/K00124100000020300
#
