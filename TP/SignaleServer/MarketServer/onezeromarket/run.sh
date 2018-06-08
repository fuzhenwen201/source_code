# !/bin/sh
ONEZEROPATH=`pwd`
export LD_LIBRARY_PATH=$ONEZEROPATH/../xservice
(nohup $ONEZEROPATH/OnezFixMarket_V1.0.10)
