#!/bin/sh

SGPATH=${PWD}

export LD_LIBRARY_PATH=$SGPATH/../xservice/
chmod a+x solutiongateway_V1.0.10
(nohup $SGPATH/solutiongateway_V1.0.10 --flagfile=../configs/solutiongateway5011.cfg &)

cd /app/solutiongateway01
SGPATH=${PWD}
chmod a+x solutiongateway_V1.0.10
(nohup $SGPATH/solutiongateway_V1.0.10 --flagfile=../configs/solutiongateway5012.cfg &)

cd /app/solutiongateway02
SGPATH=${PWD}
chmod a+x solutiongateway_V1.0.10
(nohup $SGPATH/solutiongateway_V1.0.10 --flagfile=../configs/solutiongateway5013.cfg)
	

