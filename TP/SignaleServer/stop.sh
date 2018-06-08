#!/bin/bash
docker-compose kill DC DCB  msg_tcp redis

docker rm $(docker-compose ps -q)
