# Deposite Design
-----

## 1. protocol define(`PM<->CM`)
-----

1. Use rabbitmq transafer message
2. json format
```
{"reqid":123456789,"clientid":1234,"groupid":2345,"accountid":12345,"type":0,"prevbalance":0,"change":2000,"margin":0,"freemargin":0,"closebalance":0,"profit":0,"status":0}
```

## 2. Request Deposite(`CM->PM`)
-----

1. Publish message: __`PULL.POSITION`__
2. Subscribe message: __`PULL.SGID`__

## 3. Column name
----

1. status:
 + 0: default
 + 1: success
 + 2: margin not enough
 + 4: formula error(prevbalance+changed+profit =Margin+FreeMargin)
 + 8: format error(not json)
 + 16: margin is invalid(maybe is negative)
 + 32: pull is not exists(groupid and clientid not exists)
 + 128: other error
2. type:
 + 0: default
 + 1: init(first asyn: CM->PM)
 + 2: deposite
 + 4: withdrawal
 + 8: feedback(PM->CM, PM push to CM)

## 3. reply Deposite(`PM->CM`)
-----

1. Subscribe message: __`PULL.POSITION`__
2. Publish message: __`PULL.SGID`__

## 4. Process (`PM`)
1. Subscribe __`PULL.POSITION`__
2. Parser json, if success, next step, otherwise, step 6;
3. Verify margin/formula, if success, next step, otherwise, step 6;
4. Update database, merger data;
5. Publish __`PULL.SGID`__
6. Accept next request

