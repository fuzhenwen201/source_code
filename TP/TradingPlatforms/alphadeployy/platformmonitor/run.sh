#!/bin/sh
TPMPATH=`pwd`
chmod a+x platformmonitor_V1.0.10
export LD_LIBRARY_PATH=$TPMPATH/../xservice
(nohup $TPMPATH/platformmonitor_V1.0.10 --flagfile=../configs/platformmonitor.cfg)
