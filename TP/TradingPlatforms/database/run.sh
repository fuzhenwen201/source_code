#!/bin/bash
redis-server redis.conf &
./entrypoint.sh mysqld
