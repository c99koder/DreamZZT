#!/bin/sh
PROD=cd
mkisofs -G $PROD/IP.BIN -l -R -J -o session1.iso $PROD
dd bs=1024 count=36 < session1.iso > session2.iso
dd bs=1024 count=564 < /dev/zero >> session2.iso
cdrecord dev=0,0,0 speed=4 -multi -xa1 session1.iso
cdrecord dev=0,0,0 speed=4 -eject -xa1 session2.iso
rm session1.iso
rm session2.iso
