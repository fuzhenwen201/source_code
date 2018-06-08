## 1. Publish & Subscribe ##
--------

need rabbitmq(amqp-cpp) support, and wrapper amqp-cpp as tnode
### 1.1 Publish `SENDORDER.$` ###
--------

+ group name: SENDORDER
+ event name: $
+ call funtion syntax: publish_Service event group msg tag

### 1.2 Subscribe `USER.[SGID]` ###
--------

+ subscribe group USER
+ subscribe event: SGID
+ call funtion syntax: group_consumer event group

## 2 tool & library ###
--------

+ libuv
+ http_parser
+ ab(ab -n 1000 -c 100 -k http://localhost:port/)(for test)
+ curl(curl http://localhost:port/)(for test)
+ httpie(http post http://localhost:port/ name='xyz' ...)(for test json)
+ libgoogle-glog-dev
+ automake (libuv need)
+ libtool\* (libuv need)

## 3 Publish message
--------
+ __sendorder:__ `http post http://localhost:port/ msgtype=2 reqid='C766F89F4DF000012998131065709BF0' groupid=1234 clientid=2345 assetid=3456 direction=1 expiryperiod=5 amount=1000 sgid=1`
+ __mark data:__ `post http://localhost:port/md {\"symbols\":["EUR_USD","AUD_CAD"]}`
+ port: 5001


## 4. apache benchmark
--------

### 4.1 ab: command 
```bash
ab -p post_json.txt -T application/json -n 10000 -c 1000 -k 'http://192.168.10.200:5002/md'
```

+ n: sum
+ c: batch
+ k: keep-alive
+ T: special json
+ p: post request

### 4.2 post file format

```bash
hugo@ubuntu-server:~$ cat post_json.txt 
{"symbols" : ["EUR_USD","CHF_HKD","CHF_HKD"]}
```

### 4.3 bench

```bash
root@ubuntu-server:/home/hugo# ab -p post_json.txt -T application/json -n 10000 -c 2000 -k 'http://192.168.10.200:5002/md'
This is ApacheBench, Version 2.3 <$Revision: 1528965 $>
Copyright 1996 Adam Twiss, Zeus Technology Ltd, http://www.zeustech.net/
Licensed to The Apache Software Foundation, http://www.apache.org/

Benchmarking 192.168.10.200 (be patient)
Completed 1000 requests
Completed 2000 requests
Completed 3000 requests
Completed 4000 requests
Completed 5000 requests
Completed 6000 requests
Completed 7000 requests
Completed 8000 requests
Completed 9000 requests
Completed 10000 requests
Finished 10000 requests


Server Software:        
Server Hostname:        192.168.10.200
Server Port:            5002

Document Path:          /md
Document Length:        211 bytes

Concurrency Level:      2000
Time taken for tests:   0.608 seconds
Complete requests:      10000
Failed requests:        0
Keep-Alive requests:    10000
Total transferred:      2750000 bytes
Total body sent:        2120000
HTML transferred:       2110000 bytes
Requests per second:    16442.64 [#/sec] (mean)
Time per request:       121.635 [ms] (mean)
Time per request:       0.061 [ms] (mean, across all concurrent requests)
Transfer rate:          4415.75 [Kbytes/sec] received
 							3404.14 kb/s sent
							7819.89 kb/s total

Connection Times (ms)
			  min  mean[+/-sd] median   max
Connect:        0    2  10.5      0      59
Processing:    17   30   4.4     29      50
Waiting:       17   30   4.4     29      50
Total:         24   33  11.2     29      99
				  
Percentage of the requests served within a certain time (ms)
	50%     29
	66%     32
	75%     35
	80%     36
	90%     38
	95%     70
	98%     78
	99%     79
	100%     99 (longest request)
```

## 5 add Thread Pool
-----

+ c++ thread pool
+ optimal consumer callback

## 6. fix bug 
------

fix bug before accept(memset memory of http_request) 


