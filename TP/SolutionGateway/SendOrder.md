# Send Order
----

## 1. protocol define (`TT->SG`)
---

The same as login design, except tag is __2__
1. use tcp connect
2. special tcp header + request content

### 1.1 header format
-----

colum name|size(byte)|data|note
----------|----|----|----
header sepcial|1|`0x8F`|if value is __not equal 0x8F, discarded__
reserved|1|0x00|reserved 
tag|2|<ul><li>`1: sendorder`</li><li>2: orderans(openprice/closeprice)</li><li>8: login</li></ur>|business type
body length|4|unsigned int|message body length

### 1.2 SendOrder message(message body)
-----

SendOrder message format is json. Request josn(__TT->SG__):
```
{"reqid":"C766F89F4DF000012998131065709BF0","groupid":1234,"clientid":2345,"assetid":3456,"direction":1,"expiryperiod":10,"amount":1000}
```

## 2 Transfer Request(`SG->PM`  & `SG->PM`)
----

Publish `SENDORDER.$`

### 2.1 SG Proccess 
----

1. SG parser TCP message header.
2. If first header is __equal__ `0x8F`, Next step, otherwise, Step 8; 
3. Judge tag value. if `tag == 0x01`, Next step; otherwise, Step 8;
4. Parser header of `length`. Verify body message length. If success, Next step; otherwise, Step 8;
5. Append SG identity to the body message which format is json;
6. Pubish message: `SENDORDER.$` which conntent like 
```
{"reqid":"C766F89F4DF000012998131065709BF0","groupid":1234,"clientid":2345,"assetid":3456,"direction":1,"expiryperiod":10,"amount":1000,"sgid":1}
```
7. Async wait `ACK` of sendorder, validate, orderans
8. Terminated current __SendOrder__ progress, accept next TCP connect

### 2.2 Code
------

```cpp
unsigned char Direction;	/* up or down*/
unsigned char Transfer; /* transfer */
char* ReqID;				/* Universal uniq 16B */
unsigned int GroupID;		/* universal unique */
unsigned int ClientID;		/* group unique */
unsigned int AssetID;		/* instrument */
unsigned int ExpiryPeriod;	/* time period eg. 5 second */
unsigned int Amount;		/* amount of inverstment */
unsigned int SGID;			/* solution gateway seq ID */
```

### 2.3 publish message
------

__Publish SendOrder Message__

group name|event name|route key|comment
----------|----------|--------|-------
SENDORDER|$|SENDORDER.$|Require `ACK`


## 3. Wait Reply
-----

Reply from
+ `PM->SG` 
+ `TE->SG`
-----

### 3.1 Message format:
-----

```
{"reqid":"33d554a38ff44e1cbb292a59b8e77138","reqstatus":133,"sgid_teid":1}
```

### 3.2 Reqstatus
----

+ 0: default
+ bit0:1 __`ACK`__
+ bit1:1 __`REJECT`__
+ bit2:1 __SG__
+ bit3:1 __PM__
+ bit4:1 __TE__
+ bit5:1 __MT__
+ bit6:1 __CM__
+ bit7: 0: openprice, 1:closeprice

### 3.3 code
------

```cpp
unsigned char BOAccountID; /* 1B */
unsigned char Direction;	/* up or down*/
/*OrderStatus
bit0: 1: openprice/ACK
bit1: 1: invalid
bit2: 1: WIN
bit3: 1: LOSS
bit4: 1: REJECT
 */
unsigned char OrderStatus; /* 1B */
char* ReqID;				/* Universal uniq 16B */
char* Json;
unsigned int GroupID;		/* universal unique */
unsigned int ClientID;		/* group unique */
unsigned int AssetID;		/* instrument */
unsigned int ExpiryPeriod;	/* time period eg. 5 second */
unsigned int Amount;		/* amount of inverstment */
unsigned int SGID;			/* solution gateway seq ID */
unsigned long AccountID;	/*8B */
long PCredit;	/*8B */
long Balance;	/*8B */
```


## 4 Subscribe message: `VALID.SGID`
-----

__VALID.SGID__

group name|event name|route key|comment
----------|----------|--------|-------
VALID|SGID|VALID.SGID|Require `ACK`, the SGID is SG identity

### 4.1 Process VALID
------

1 When __SG__ publish message `SENDORDER.$`, _PM__ & __TE__ will receive them (__PM__ subscribe __SENDORDER.POSITION__, __TE__ subscribe __SENDORDER.TRADINGENGIN__)
2 PM Receive `SENDOEDER.POSITION`  Query Redis. If Query success, Step 4, otherwise Next Step 
3. Query MySql. If Success, Next Step; otherwise, Step 7;
4. Update MySql database(lock margin etc)
5. Publish `VALID.SGID` to SG, and Publish `VALIDATE.$` to __TE__
6. Async wait `ACK` of `VALIDATE.SGID` and `VALIDATE.$`
7. Accept next sendorder 

### 4.2 INVALIDATE
----

If __PM__ query database and publish invalidate to __TE__, then, __PM__ will be publish `ORDERAND` to __SG__ to terminate current trade.

## 5 Subscibe message: `ORDANS`
-----

__ORDANS__

group name|event name|route key|comment
----------|----------|--------|-------
ORDANS or ORDERANS|SGID or POSITION|ORDANS.SGID or ORDERANS.POSITION|Require `ACK`

### 5.1 OrderStatus
----

+ default 0
+ bit0:1 __ACK__
+ bit1:1 __INVALID__
+ bit2:1 __WIN__
+ bit3:1 __LOSS__
+ bit4:1 __REJECT__
+ bit7: __0: openprice__, __1: closeprice__

### 5.2 ORD[ER]ANS
----

+ first orderans if is openprice(publish by __TE__)
+ second orderans is closeprice(publish by __TE__)
+ when invalid, the orderans was published by __PM__

### 5.3 Code
-----

```cpp
/*OrderStatus
  bit0: 1: openprice/ACK
  bit1: 1: invalid
  bit2: 1: WIN
  bit3: 1: LOSS
  bit4: 1: REJECT
 */
unsigned char OrderStatus; /* 1B */
unsigned char Direction;	/* up or down*/
char* ReqID;				/* Universal uniq 16B */
char* Json;
unsigned int GroupID;		/* universal unique 4B*/
unsigned int ClientID;		/* group unique 4B*/
unsigned int OrderID; /* server unique 4B */
unsigned int AssetID;		/* instrument */
long PayOut; /* the amount of payout 8B */
int Amount;
unsigned int OpenTime;	/* open Time(Serve Provide) 4B */
int OpenPrice;	/* open Price(Serve Provide) 4B */
int ClosePrice;	/* close Price(Serve Provide) 4B */
unsigned int SGID;			/* solution gateway seq ID */
unsigned int TEID;			/* solution gateway seq ID */
```



