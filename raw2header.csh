#!/bin/csh -f

if("$#argv" != "1") then
  echo "USAGE: $0 <basefilename>"
  exit
endif

if(!(-r "${1}.raw")) then
  echo "${1}.raw can not be read."
  exit
endif

echo "static const unsigned char $1[] = {" > "${1}.h"
hexdump -vf ~/c/cosmos/hexdump_format "${1}.raw" >> "${1}.h"
echo "~};" | tr '~' '\t' >> "${1}.h"
