#!/bin/bash
cd /app/netmanager

java -jar NetManager-V1.0.10.jar &

cd /app/adminservice
java -jar AD_V1.0.10.jar 
