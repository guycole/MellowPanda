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
# dump channel 1 
./pandad -x panda://device/raw/pro2052/PM001
# dump channel 2
./pandad -x panda://device/raw/pro2052/PM002
# dump channel 51 
./pandad -x panda://device/raw/pro2052/PM051
# dump channel 52
./pandad -x panda://device/raw/pro2052/PM052
#
# delay on?
./pandad -x panda://device/raw/pro2052/DL
#
# lock out?
./pandad -x panda://device/raw/pro2052/LO
#
# limit search lower limit
./pandad -x panda://device/raw/pro2052/LL
#
# limit search upper limit
./pandad -x panda://device/raw/pro2052/LU
#
# status of current channel
./pandad -x panda://device/raw/pro2052/MA
#
# scanner mode (page 53 for results)
./pandad -x panda://device/raw/pro2052/MD
#
# check bank priority channel
./pandad -x panda://device/raw/pro2052/PC A
#
# check priority enabled
./pandad -x panda://device/raw/pro2052/PR
#
# squelch monitor
./pandad -x panda://device/raw/pro2052/QU
#
# priority monitor
./pandad -x panda://device/raw/pro2052/RI
#
# modulation
./pandad -x panda://device/raw/pro2052/RM
#
# scan bank 
./pandad -x panda://device/raw/pro2052/SB
#
# squelch 
./pandad -x panda://device/raw/pro2052/SQ
#
# search skip
./pandad -x panda://device/raw/pro2052/SS01
#
# tune to redding ATIS
./pandad -x panda://device/raw/pro2052/RF01241000
#
# trunk id
./pandad -x panda://device/raw/pro2052/IC
#
# trunk id monitor
./pandad -x panda://device/raw/pro2052/ID
#
# confirm EDACS
./pandad -x panda://device/raw/pro2052/AF
#
# test S bits
./pandad -x panda://device/raw/pro2052/BT
#
# test frequency identification
./pandad -x panda://device/raw/pro2052/FI
#
# test speaker muting
./pandad -x panda://device/raw/pro2052/MU
#
# test EDACS ID range
./pandad -x panda://device/raw/pro2052/RG
#
# test scanner ID
./pandad -x panda://device/raw/pro2052/SI
#
# test active trunk bank
./pandad -x panda://device/raw/pro2052/TB
#
# test active trunk detection
./pandad -x panda://device/raw/pro2052/TD
