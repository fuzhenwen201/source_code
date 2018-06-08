#!/bin/sh
MGSGPATH=`pwd`
export LD_LIBRARY_PATH=${MGSGPATH}/../xservice
chmod a+x mgsg_V1.0.10
(nohup $MGSGPATH/mgsg_V1.0.10 --flagfile=../configs/mgsg5021.cfg &)

cd /app/managementsg01
chmod a+x mgsg_V1.0.10
MGSGPATH=`pwd`
(nohup $MGSGPATH/mgsg_V1.0.10 --flagfile=../configs/mgsg5022.cfg &)

cd /app/managementsg02
chmod a+x mgsg_V1.0.10
MGSGPATH=`pwd`
(nohup $MGSGPATH/mgsg_V1.0.10 --flagfile=../configs/mgsg5023.cfg)
