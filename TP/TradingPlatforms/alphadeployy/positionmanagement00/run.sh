#!/bin/sh

PMPATH=${PWD}
export LD_LIBRARY_PATH=$PMPATH/../xservice/
chmod a+x positionmanagement_V1.0.10
(nohup $PMPATH/positionmanagement_V1.0.10 --flagfile=../configs/positionmanagement4141.cfg &)

cd /app/positionmanagement01
PMPATH=${PWD}
chmod a+x positionmanagement_V1.0.10
(nohup $PMPATH/positionmanagement_V1.0.10 --flagfile=../configs/positionmanagement4142.cfg &)

cd /app/positionmanagement02
PMPATH=${PWD}
chmod a+x positionmanagement_V1.0.10
(nohup $PMPATH/positionmanagement_V1.0.10 --flagfile=../configs/positionmanagement4143.cfg)
