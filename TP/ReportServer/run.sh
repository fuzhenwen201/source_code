#!/bin/bash
docker-compose up -d mysql redis
sleep 2
docker-compose up -d report reports

docker-compose ps
