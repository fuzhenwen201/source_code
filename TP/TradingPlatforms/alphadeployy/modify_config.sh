#!/bin/bash
sed -i 's/13.126.191.171/35.154.44.66/g' netfollower/config/application.yml
sed -i 's/13.126.191.171/35.154.44.66/g' netmanager/config/application.properties
sed -i 's/13.126.191.171/35.154.44.66/g' database/hiredis.sql
sed -i 's/13.126.191.171/35.154.44.66/g' configs/config.properties
sed -i 's/13.126.191.171/35.154.44.66/g' configs/monitorbridge.cfg
sed -i 's/13.126.191.171/35.154.44.66/g' configs/positionmanagement4141.cfg
sed -i 's/13.126.191.171/35.154.44.66/g' configs/positionmanagement4142.cfg
sed -i 's/13.126.191.171/35.154.44.66/g' configs/positionmanagement4143.cfg
sed -i 's/13.126.191.171/35.154.44.66/g' configs/RMconfig.properties
sed -i 's/13.126.185.102/52.66.45.64/g' configs/tradingengine5152.cfg
sed -i 's/13.126.185.102/52.66.45.64/g' configs/tradingengine5153.cfg
sed -i 's/13.126.185.102/52.66.45.64/g' configs/tradingengine5151.cfg
