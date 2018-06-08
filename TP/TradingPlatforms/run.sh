#!/bin/bash
docker kill $(docker-compose ps -q)
docker rm $(docker-compose ps -q)
docker-compose up -d rabbitmq
docker-compose up -d zookeeper
docker-compose up -d redis
docker-compose up -d sqlredis
sleep 10
docker-compose up -d sg5011
#docker-compose up -d --no-recreate
