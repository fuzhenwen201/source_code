#!/bin/sh

MAPATH=${PWD}
chmod a+x MsgServer_V1.0.11
export LD_LIBRARY_PATH=$MAPATH/../xservice/
(nohup $MAPATH/MsgServer_V1.0.11 &)

cd /app/networkconfiguration
java -jar NetFollower-V1.0.11.jar




