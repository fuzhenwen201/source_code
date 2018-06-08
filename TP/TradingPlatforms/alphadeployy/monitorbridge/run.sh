#!/bin/sh

TMBPATH=${PWD}
chmod a+x monitorbridge_V1.0.10
export LD_LIBRARY_PATH=$TMBPATH/../xservice/

(nohup $TMBPATH/monitorbridge_V1.0.10 --flagfile=../configs/monitorbridge.cfg )


	

