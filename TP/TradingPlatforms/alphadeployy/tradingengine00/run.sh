#!/bin/sh

TEPATH=${PWD}

export LD_LIBRARY_PATH=$TEPATH/../xservice/
chmod a+x tradingengine_V1.0.10
(nohup $TEPATH/tradingengine_V1.0.10 --flagfile=../configs/tradingengine5151.cfg &)

cd /app/tradingengine01
TEPATH=${PWD}
chmod a+x tradingengine_V1.0.10
(nohup $TEPATH/tradingengine_V1.0.10 --flagfile=../configs/tradingengine5152.cfg &)

cd /app/tradingengine02
TEPATH=${PWD}
chmod a+x tradingengine_V1.0.10
(nohup $TEPATH/tradingengine_V1.0.10 --flagfile=../configs/tradingengine5153.cfg &)

cd /app/netfollower

java -jar NetFollower-V1.0.10.jar 


	

