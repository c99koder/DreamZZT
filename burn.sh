#!/bin/sh
rm tmp.iso
mkisofs -R -l -o tmp.iso cd
cdrecord dev=0,0,0 speed=4 -multi -eject -data tmp.iso
