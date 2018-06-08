# !/bin/bash

( sleep 10 ; \
rabbitmq-plugins enable rabbitmq_federation
rabbitmq-plugins enable rabbitmq_federation_management
rabbitmqctl add_user demo demo
rabbitmqctl add_vhost /demo
rabbitmqctl set_permissions -p /demo demo ".*" ".*" ".*" 
rabbitmqctl set_user_tags demo administrator
rabbitmq-plugins enable rabbitmq_management ) &
rabbitmq-server $@
#service rabbitmq-server restart
