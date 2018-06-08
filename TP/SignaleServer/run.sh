#!/bin/bash
docker-compose up -d redis
docker-compose up -d msg_tcp
docker-compose up -d DC DCB
