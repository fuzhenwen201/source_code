#!/bin/bash
sed -i 's/V1.0.11/V1.0.12/g' docker-compose.yml 
sed -i 's/V1.0.11/V1.0.12/g'  MarketServer/msg_tcp/run.sh
