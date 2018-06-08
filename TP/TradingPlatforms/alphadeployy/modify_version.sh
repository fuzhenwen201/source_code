#!/bin/bash
sed -i 's/V1.0.10/V1.0.11/g' ./sgdistributor/run.sh
sed -i 's/V1.0.10/V1.0.11/g' ./managementsg00/run.sh
sed -i 's/V1.0.10/V1.0.11/g' ./adminservice/run.sh
sed -i 's/V1.0.10/V1.0.11/g' ./positionmanagement00/run.sh
sed -i 's/V1.0.10/V1.0.11/g' ./tradingengine00/run.sh
sed -i 's/V1.0.10/V1.0.11/g' ./platformmonitor/run.sh
sed -i 's/V1.0.10/V1.0.11/g' ./monitorbridge/run.sh
sed -i 's/V1.0.10/V1.0.11/g' ./solutiongateway00/run.sh
sed -i 's/V1.0.10/V1.0.11/g' ../updatebinary.sh
sed -i 's/V1.0.10/V1.0.11/g' ../docker-compose.yml

