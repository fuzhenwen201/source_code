#!/bin/bash
# TP DEPT.
# Savin Chen
SGDPATH=`pwd`
export LD_LIBRARY_PATH=$SGDPATH/../xservice/
chmod a+x sgdistributor_V1.0.10
#killall -9 SGDistributor
(nohup $SGDPATH/sgdistributor_V1.0.10)
#for test
#sleep 3
#($SGDPATH/inclient_batch_gw 35.154.44.66 5039 1 172.18.0.5 5011 20 1500000 1 &)
#sleep 1
#($SGDPATH/inclient_batch_gw 35.154.44.66 5039 1 172.18.0.6 5012 20 1500000 1 &)
#sleep 1
#($SGDPATH/inclient_batch_gw 35.154.44.66 5039 1 172.18.0.7 5013 20 1500000 1 )

