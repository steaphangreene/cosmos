#!/bin/csh -f

if("$#argv" != "1") then
  echo "USAGE: $0 <basefontname>"
  exit
endif

if(!(-r "${1}.tga")) then
  echo "${1}.tga can not be read."
  exit
endif

set sz=`ls -l ${1}.tga | sed 's-  *-;-g' | cut -f5 -d';'`
@ sz -= 44

echo 'WARNING! MAY NOT WORK'
tail -c +19 "${1}.tga" | head -c +$sz > "${1}.raw"
