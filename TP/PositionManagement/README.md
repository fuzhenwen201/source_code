# Position Management
----

## 1. Pushlish & Subscribe
------

need rabbitmq(amqp-cpp) support, and wrapper amqp-cpp as tnode

### 1.1 Publish `VALIDATE.$`
--------

+ group name: VALIDATE
+ event name: $
+ call funtion syntax: publish_Service event group msg tag
+ tag: 0x0003
+ MsgName: ValidateOrder

###  1.2 Publish `VALID.[SGID]`
------

+ group name: VALID
+ event name: SGID
+ call funtion syntax: publish_Service event group msg tag
+ tag: 0x0003
+ MsgName: ValidateOrder

###  1.3 Publish `USER.[SGID]`
------

+ group name: USER
+ event name: SGID
+ call funtion syntax: publish_Service event group msg tag
+ aim: response __`ACK`__ for  __`SGID`__

###  1.4 Publish `REPORT.MONITOR`
------

+ group name: REPORT
+ event name: MONITOR
+ call funtion syntax: publish_Service event group msg tag
+ aim: report exception to monitor


## 2 struct & class define
### 2.1 validateOrder
```cpp
/* 2B tag 0x0003 */
struct ValidateOrder {
	public:
		ValidateOrder();
		ValidateOrder(unsigned char boAccountID,
				unsigned char direction,
				const char* reqID,
				unsigned int groupID,
				unsigned int clientID,
				unsigned int assetID,
				unsigned int expiryPeriod,
				unsigned int amount,
				unsigned int sgID,
				unsigned long accountID);
		~ValidateOrder();
		ValidateOrder(const ValidateOrder& r);
		ValidateOrder& operator=(const ValidateOrder& r);
		ValidateOrder(ValidateOrder&& r);
		ValidateOrder& operator=(ValidateOrder&& r);

	public:
		unsigned char BOAccountID; /* 1B */
		unsigned char Direction;	/* up or down*/
		char* ReqID;				/* Universal uniq 16B */
		unsigned int GroupID;		/* universal unique */
		unsigned int ClientID;		/* group unique */
		unsigned int AssetID;		/* instrument */
		unsigned int ExpiryPeriod;	/* time period eg. 5 second */
		unsigned int Amount;		/* amount of inverstment */
		unsigned int SGID;			/* solution gateway seq ID */
		unsigned long AccountID;	/*8B */

};
```


