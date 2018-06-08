#!/bin/bash

docker-compose ps
docker-compose kill report reports

docker rm $(docker-compose ps -q)
docker-compose ps
