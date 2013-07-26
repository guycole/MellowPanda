#!/bin/bash
#
# Title:pro2052test.sh
#
# Description:
#   Exercise the PRO2052
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
./panda -r pro2052 -k ping
#
./panda -r pro2052 -k raw/QUN
#
./panda -r pro2052 -k raw/FIN
#
./panda -r pro2052 -k raw/KEY01
#
./panda -r pro2052 -k raw/RF01241000
#
./panda -r pro2052 -k raw/WI
#
./panda -r pro2052 -k raw/RM
#
./panda -r pro2052 -k raw/RF01322000
#
./panda -r pro2052 -k raw/WI
#
./panda -r pro2052 -k raw/RM
#
#./panda -r pro2052 -k raw/SG
#
#./panda -r pro2052 -k raw/QU
#
#
