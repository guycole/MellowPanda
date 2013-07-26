#!/bin/bash
#
# Title:pro2052t3.sh
#
# Description:
#   Store aviation frequencies to 
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
#./panda -r pro2052 -k raw/QUN
#
# manual
./panda -r pro2052 -k raw/KEY01
#
# inspect scanner mode
./panda -r pro2052 -k raw/MD
#
# scan
#./panda -r pro2052 -k raw/KEY00
#
# inspect scanner mode
./panda -r pro2052 -k raw/MD
#
# trunk mode
./panda -r pro2052 -k raw/KEY13
#
#
./panda -r pro2052 -k raw/PM002 F01322000
#./panda -r pro2052 -k raw/PM003 F01198000
./panda -r pro2052 -k raw/PM001
#
./panda -r pro2052 -k raw/PM052 F08541875
./panda -r pro2052 -k raw/PM053 F08569625
./panda -r pro2052 -k raw/PM054 F08574625
./panda -r pro2052 -k raw/PM055 F08579625
./panda -r pro2052 -k raw/PM056 F08584265
./panda -r pro2052 -k raw/PM057 F08589625
./panda -r pro2052 -k raw/PM058 F08599625

./panda -r pro2052 -k raw/MD
#./panda -r pro2052 -k raw/KEY00

#
#./panda -r pro2052 -k raw/RF01241000
#
#./panda -r pro2052 -k raw/WI
#
#./panda -r pro2052 -k raw/RM
#
#./panda -r pro2052 -k raw/RF01322000
#
#./panda -r pro2052 -k raw/WI
#
#./panda -r pro2052 -k raw/RM
#
#./panda -r pro2052 -k raw/SG
#
#./panda -r pro2052 -k raw/QU
#
#
