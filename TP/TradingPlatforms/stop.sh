#!/bin/bash

#docker-compose kill accumonitor admin cm ctl mb mgsg5021  pm4141  rmobj rmsource te5151  tpm sg5011  sgd rmrs

docker kill $(docker-compose ps -q)
docker rm $(docker-compose ps -q)
docker-compose ps
