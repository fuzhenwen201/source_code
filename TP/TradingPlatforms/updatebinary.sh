#!/bin/bash
aws s3 cp s3://alphadeploy-tp/TradingServer/adminservice/AD-jars ./alphadeployy/adminservice/AD-jars/ --recursive
aws s3 cp s3://alphadeploy-tp/TradingServer/adminservice/AD_V1.0.10.jar ./alphadeployy/adminservice/
aws s3 cp s3://alphadeploy-tp/TradingServer/clientmanager/CM-jars ./alphadeployy/clientmanager/CM-jars/ --recursive
aws s3 cp s3://alphadeploy-tp/TradingServer/clientmanager/CM_V1.0.10.jar ./alphadeployy/clientmanager/

aws s3 cp s3://alphadeploy-tp/TradingServer/accumonitor/accumonitor-V1.0.10.jar ./alphadeployy/accumonitor/
aws s3 cp s3://alphadeploy-tp/TradingServer/centrallogging/centrallogging-V1.0.10.jar ./alphadeployy/centrallogging/
aws s3 cp s3://alphadeploy-tp/TradingServer/netfollower/NetFollower-V1.0.10.jar ./alphadeployy/netfollower/
aws s3 cp s3://alphadeploy-tp/TradingServer/netmanager/NetManager-V1.0.10.jar ./alphadeployy/netmanager/
aws s3 cp s3://alphadeploy-tp/TradingServer/riskmanagement/trad-rm-object-V1.0.10.jar ./alphadeployy/riskmanagement/
aws s3 cp s3://alphadeploy-tp/TradingServer/riskmanagereport/rmobject-reportserver-V1.0.10.jar  ./alphadeployy/riskmanagereport/
aws s3 cp s3://alphadeploy-tp/TradingServer/riskmanagesource/riskmanage-source-V1.0.10.jar ./alphadeployy/riskmanagesource/
aws s3 cp s3://alphadeploy-tp/TradingServer/managementsg00/mgsg_V1.0.10 ./alphadeployy/managementsg00/
aws s3 cp s3://alphadeploy-tp/TradingServer/managementsg00/mgsg_V1.0.10 ./alphadeployy/managementsg01/
aws s3 cp s3://alphadeploy-tp/TradingServer/managementsg00/mgsg_V1.0.10 ./alphadeployy/managementsg02/
aws s3 cp s3://alphadeploy-tp/TradingServer/monitorbridge/monitorbridge_V1.0.10 ./alphadeployy/monitorbridge/
#aws s3 cp s3://alphadeploy-tp/TradingServer/platformmonitor/platformmonitor_V1.0.10 ./alphadeployy/platformmonitor/
aws s3 cp s3://alphadeploy-tp/TradingServer/positionmanagement00/positionmanagement_V1.0.10 ./alphadeployy/positionmanagement00/
aws s3 cp s3://alphadeploy-tp/TradingServer/positionmanagement00/positionmanagement_V1.0.10 ./alphadeployy/positionmanagement01/
aws s3 cp s3://alphadeploy-tp/TradingServer/positionmanagement00/positionmanagement_V1.0.10 ./alphadeployy/positionmanagement02/
aws s3 cp s3://alphadeploy-tp/TradingServer/solutiongateway00/solutiongateway_V1.0.10  ./alphadeployy/solutiongateway00/
aws s3 cp s3://alphadeploy-tp/TradingServer/solutiongateway00/solutiongateway_V1.0.10  ./alphadeployy/solutiongateway01/
aws s3 cp s3://alphadeploy-tp/TradingServer/solutiongateway00/solutiongateway_V1.0.10  ./alphadeployy/solutiongateway02/
aws s3 cp s3://alphadeploy-tp/TradingServer/sgdistributor/sgdistributor_V1.0.10  ./alphadeployy/sgdistributor/
aws s3 cp s3://alphadeploy-tp/TradingServer/tradingengine00/tradingengine_V1.0.10 ./alphadeployy/tradingengine00/
aws s3 cp s3://alphadeploy-tp/TradingServer/tradingengine00/tradingengine_V1.0.10 ./alphadeployy/tradingengine01/
aws s3 cp s3://alphadeploy-tp/TradingServer/tradingengine00/tradingengine_V1.0.10 ./alphadeployy/tradingengine02/
echo "please execute "mv ./alphadeployy/platformmonitor/platformmonitor_V1.0.10 ./alphadeployy/platformmonitor/platformmonitor_V1.0.11}"
