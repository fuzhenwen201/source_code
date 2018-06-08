# login design
----

## 1. protocol define(`TT->SG`)
-----

1. use tcp connect
2. special tcp header + request content

### 1.1 header format
-----

colum name|size(byte)|data|note
----------|----|----|----
header sepcial|1|`0x8F`|if value is __not equal 0x8F, discarded__
reserved|1|0x00|reserved 
tag|2|0x0021|business type
body length|4|unsigned int|message body length

### 1.2 login message(message body)
-----

Login message format is json. Request josn(__TT->SG__):
```
{"reqid":"17086ce540b2460997819cdfe5a852fa","sessionid":"1492575178073475","groupid":0,"clientid":14,"ipaddress":"192.168.14.1","password":"a","status":0}
```

## 2 transfer request(`SG->CM`) 
-----

### 2.1 publish message
------

__Publish Login Message__

+ GroupName: __LOGIN__
+ Event: __CLIENTMANAGE__
+ no reqire `ACK`
+ tag: 0x0021


## 3. Wait reply(`CM->SG`)
-----

1. Message format:

```
{"reqid":"17086ce540b2460997819cdfe5a852fa","sessionid":"1492575178073475","groupid":0,"clientid":14,"ipaddress":"192.168.14.1","password":"a","status":1, "sgid":1}
```

2. Status
 + 0: default
 + 1: verify success
 + 2: fail

3. Subscribe message: `LOGIN.SGID`

+ GroupName: __LOGIN__
+ Event: __SGID__
+ no reqire `ACK`
+ tag: 0x0022

## 4. CM push userinfo and open position and close position
----

### 4.1 userinfo

+ GroupName: __USERINFO__
+ Event: __SGID__
+ no reqire `ACK`
+ tag: 0x0024
+ content: the same as PULL.POSITION json 
```
{
	"reqid":"17086ce540b2460997819cdfe5a852fa",
	"sessionid":"1492575178073475",
	"groupid":1234,
	"clientid":2345,
	"accountid":3579,
	"prevbalance":0,
	"changed":2000,
	"margin":2000,
	"freemargin":0,
	"profit":0,
	"type":16,
	"closebalance":0,
	"status":0,
	"sgid":1
}
```

### 4.2 OCPOSITION

+ GroupName: __OCPOSITION__
+ Event: __SGID__
+ no reqire `ACK`
+ tag: 0x002A
+ content like this:
```
{
	{"reqid":"5dda0649becf443e8c07f6da2fd0f009",
		"sessionid":"1492575178073475",
		"tag:"32,
		"groupid":1234,
		"clientid":2345,
		"sgid":1, ,
		"total":100,
		"index":1,
		"size":10,
		"body":
			[
			{
				"reqid":"5dda0649becf443e8c07f6da2fd0f009",
				"orderid":"5dda0649becf443e8c07f6da2fd0f009",
				"orderstatus":0,
				"assetid":0,
				"direction":0,
				"amount":100,
				"expiryperiod":5,
				"opentime":1490687207969888,
				"openprice":1017980,
				"closeprice":0,
				"payout":0,
				"sgid":1,
				"teid":1
			},
			...
			]
	}
}
```

