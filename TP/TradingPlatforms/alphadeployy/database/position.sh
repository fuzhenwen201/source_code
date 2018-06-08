#########################################################################
# File Name: position.sh
# Author: xyz
# mail: xiao13149920@foxmail.com
# Created Time: Tue 16 May 2017 07:27:52 PM CST
#########################################################################
#!/bin/bash

#Script to run automated sql queries

#Declaring mysql DB connection 
MASTER_DB_USER='root'
MASTER_DB_PASSWD='root'
MASTER_DB_PORT=3306
MASTER_DB_HOST='localhost'
MASTER_DB_NAME='test'
DAY=`date +%Y%m%d`
NEXT_DAY=`date +%Y-%m-%d -d "+1 day"`

echo ${DAY}
echo ${NEXT_DAY}

#Prepare sql query
CREATE_PM_SEND_ORDER_LOG="CREATE TABLE IF NOT EXISTS \`pm_send_order_log_$DAY\` (
\`id\` int(10) unsigned NOT NULL AUTO_INCREMENT,
\`sessionid\` varchar(33) NOT NULL,
\`reqid\` varchar(33) NOT NULL,
\`groupid\` int(10) unsigned DEFAULT NULL,
\`clientid\` int(11) DEFAULT NULL,
\`assetid\` int(11) DEFAULT '0',
\`direction\` tinyint(4) NOT NULL DEFAULT '0',
\`expiryperiod\` int(11) NOT NULL DEFAULT '0' COMMENT 'seconds',
\`amount\` int(11) DEFAULT '0',
\`sgid\` int(11) DEFAULT '0',
\`transfer\` tinyint(4) DEFAULT '0' COMMENT 'default 0',
\`insert_time\` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT 'current time',
\`update_time\` bigint(20) NOT NULL DEFAULT '0',
PRIMARY KEY (\`id\`),
KEY \`idx_reqid\` (\`reqid\`),
KEY \`idx_groupid_clientid\` (\`groupid\`,\`clientid\`)
) ENGINE=InnoDB AUTO_INCREMENT=0 DEFAULT CHARSET=utf8;"

CREATE_PM_VALIDATE_ORDER_LOG="CREATE TABLE IF NOT EXISTS \`pm_validate_order_log_$DAY\` (
\`id\` int(10) unsigned NOT NULL AUTO_INCREMENT,
\`sessionid\` varchar(33) NOT NULL,
\`reqid\` varchar(33) NOT NULL,
\`groupid\` int(10) unsigned DEFAULT NULL,
\`clientid\` int(11) DEFAULT NULL,
\`accountid\` bigint(20) DEFAULT NULL,
\`boaccountid\` bigint(20) DEFAULT NULL,
\`assetid\` int(11) DEFAULT '0',
\`direction\` tinyint(4) NOT NULL DEFAULT '0',
\`expiryperiod\` int(11) NOT NULL DEFAULT '0' COMMENT 'seconds',
\`amount\` int(11) DEFAULT '0',
\`orderstatus\` tinyint(4) unsigned DEFAULT NULL,
\`margin\` bigint(20) DEFAULT NULL,
\`freemargin\` bigint(20) DEFAULT NULL,
\`profit\` bigint(20) DEFAULT NULL,
\`status\` tinyint(4) DEFAULT NULL,
\`sgid\` int(11) DEFAULT '0',
\`teid\` int(11) DEFAULT '0',
\`transfer\` tinyint(4) DEFAULT '0' COMMENT 'default 0',
\`insert_time\` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT 'current time',
\`update_time\` bigint(20) NOT NULL DEFAULT '0',
PRIMARY KEY (\`id\`),
KEY \`idx_reqid\` (\`reqid\`),
KEY \`idx_groupid_clientid\` (\`groupid\`,\`clientid\`)
) ENGINE=InnoDB AUTO_INCREMENT=0 DEFAULT CHARSET=utf8;"

CREATE_PM_ORDER_ANS_LOG="CREATE TABLE IF NOT EXISTS \`pm_order_ans_log_$DAY\` (
\`id\` int(10) unsigned NOT NULL AUTO_INCREMENT,
\`sessionid\` varchar(33) NOT NULL,
\`reqid\` varchar(33) NOT NULL DEFAULT '',
\`groupid\` int(11) NOT NULL,
\`clientid\` int(11) NOT NULL,
\`orderid\` varchar(33) NOT NULL,
\`orderstatus\` tinyint(3) unsigned NOT NULL DEFAULT '0',
\`assetid\` int(11) NOT NULL DEFAULT '0',
\`direction\` tinyint(4) NOT NULL DEFAULT '0',
\`amount\` int(11) DEFAULT '0',
\`payout\` bigint(20) NOT NULL DEFAULT '0',
\`opentime\` bigint(20) unsigned DEFAULT '0',
\`openprice\` int(11) DEFAULT '0',
\`closeprice\` int(11) DEFAULT '0',
\`passive\` tinyint(3) unsigned DEFAULT '0' COMMENT '0: TE->PM 1:PM->SG',
\`sgid\` int(11) DEFAULT '0',
\`teid\` int(11) DEFAULT '0',
\`insert_time\` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
\`update_time\` bigint(20) NOT NULL DEFAULT '0',
PRIMARY KEY (\`id\`),
KEY \`idx_reqid\` (\`reqid\`),
KEY \`idx_groupid_clientid\` (\`groupid\`,\`clientid\`),
KEY \`idx_orderid\` (\`orderid\`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;"

CREATE_POSITION_MANAGEMENT_LOG="CREATE TABLE IF NOT EXISTS \`position_management_log_$DAY\` (
\`id\` int(10) unsigned NOT NULL AUTO_INCREMENT,
\`sessionid\` varchar(33) NOT NULL,
\`reqid\` varchar(33) NOT NULL,
\`groupid\` int(10) unsigned DEFAULT NULL,
\`clientid\` int(11) DEFAULT NULL,
\`accountid\` bigint(20) DEFAULT NULL,
\`balance\` bigint(20) DEFAULT NULL,
\`prevbalance\` bigint(20) DEFAULT NULL,
\`changed\` bigint(20) DEFAULT NULL,
\`margin\` bigint(20) DEFAULT NULL,
\`freemargin\` bigint(20) DEFAULT NULL,
\`profit\` bigint(20) DEFAULT NULL,
\`type\` tinyint(4) unsigned DEFAULT NULL,
\`closebalance\` bigint(20) DEFAULT NULL,
\`status\` tinyint(4) unsigned DEFAULT NULL,
\`check\` tinyint(4) unsigned DEFAULT NULL,
\`assetid\` int(11) DEFAULT '0',
\`direction\` tinyint(4) NOT NULL DEFAULT '0',
\`expiryperiod\` int(11) NOT NULL DEFAULT '0' COMMENT 'seconds',
\`amount\` int(11) DEFAULT '0',
\`sgid\` int(11) DEFAULT '0',
\`transfer\` tinyint(4) DEFAULT '0' COMMENT 'default 0',
\`insert_time\` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT 'current time',
\`update_time\` bigint(20) NOT NULL DEFAULT '0',
PRIMARY KEY (\`id\`),
KEY \`idx_reqid\` (\`reqid\`),
KEY \`idx_groupid_clientid\` (\`groupid\`,\`clientid\`)
) ENGINE=InnoDB AUTO_INCREMENT=0 DEFAULT CHARSET=utf8;"

CREATE_POSITION_MANAGEMENT="CREATE TABLE IF NOT EXISTS \`position_management\` (
\`id\` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
\`groupid\` int(10) unsigned DEFAULT NULL,
\`clientid\` int(10) unsigned DEFAULT NULL,
\`sgid\` int(10) unsigned DEFAULT NULL COMMENT 'sgid',
\`accountid\` bigint(20) DEFAULT NULL,
\`balance\` bigint(20) DEFAULT '0',
\`prevbalance\` bigint(20) DEFAULT '0',
\`changed\` bigint(20) DEFAULT '0',
\`margin\` bigint(20) DEFAULT '0' COMMENT 'locked in trading,must be positive',
\`freemargin\` bigint(20) DEFAULT '0' COMMENT 'valid amount,must be positive',
\`profit\` bigint(20) DEFAULT '0' COMMENT 'result of trade',
\`type\` tinyint(3) unsigned DEFAULT '0' COMMENT 'trade case',
\`closebalance\` int(11) DEFAULT '0',
\`status\` tinyint(4) DEFAULT '0',
\`check\` tinyint(4) DEFAULT '0',
\`update_time\` bigint(20) NOT NULL DEFAULT '0',
\`insert_time\` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
PRIMARY KEY (\`id\`),
UNIQUE KEY \`idx_groupid_clientid\` (\`groupid\`,\`clientid\`)
) ENGINE=InnoDB AUTO_INCREMENT=0 DEFAULT CHARSET=utf8;"

T_POSITION_MANAGEMENT_AFTER_INSERT="DELIMITER \$\$
USE \`${MASTER_DB_NAME}\`\$\$
DROP TRIGGER /*!50032 IF EXISTS */ \`t_position_management_after_insert\`\$\$
CREATE
/*!50017 DEFINER = '${MASTER_DB_USER}'@'${MASTER_DB_HOST}' */
TRIGGER \`t_position_management_after_insert\` AFTER INSERT ON \`position_management\` 
FOR EACH ROW BEGIN
set @sgid=(SELECT redis_hset(CONCAT_WS('_', 'position', new.groupid, new.clientid), 'sgid',CAST(new.sgid AS CHAR(10))));
SET @balance = (SELECT redis_hset(CONCAT_WS('_', 'position', new.groupid, new.clientid), 'balance',CAST(new.balance AS CHAR(20))));
SET @prevbalance = (SELECT redis_hset(CONCAT_WS('_', 'position', new.groupid, new.clientid), 'prevbalance',CAST(new.prevbalance AS CHAR(20))));
SET @changed = (SELECT redis_hset(CONCAT_WS('_', 'position', new.groupid, new.clientid), 'changed',CAST(new.changed AS CHAR(20))));
SET @margin = (SELECT redis_hset(CONCAT_WS('_', 'position', new.groupid, new.clientid), 'margin',CAST(new.margin AS CHAR(20))));
SET @freemargin = (SELECT redis_hset(CONCAT_WS('_', 'position', new.groupid, new.clientid), 'freemargin',CAST(new.freemargin AS CHAR(20))));
SET @profit = (SELECT redis_hset(CONCAT_WS('_', 'position', new.groupid, new.clientid), 'profit',CAST(new.profit AS CHAR(20))));
SET @type = (SELECT redis_hset(CONCAT_WS('_', 'position', new.groupid, new.clientid), 'type',CAST(new.type AS CHAR(4))));
SET @closebalance = (SELECT redis_hset(CONCAT_WS('_', 'position', new.groupid, new.clientid), 'closebalance',CAST(new.closebalance AS CHAR(11))));
SET @status = (SELECT redis_hset(CONCAT_WS('_', 'position', new.groupid, new.clientid), 'status',CAST(new.status AS CHAR(4))));
SET @check = (SELECT redis_hset(CONCAT_WS('_', 'position', new.groupid, new.clientid), 'check',CAST(new.check AS CHAR(4))));
END;
\$\$
DELIMITER ;"

T_POSITION_MANAGEMENT_AFTER_UPDATE="DELIMITER \$\$
USE \`${MASTER_DB_NAME}\`\$\$
DROP TRIGGER /*!50032 IF EXISTS */ \`t_position_management_after_update\`\$\$
CREATE
/*!50017 DEFINER = '${MASTER_DB_USER}'@'${MASTER_DB_HOST}' */
TRIGGER \`t_position_management_after_update\` AFTER UPDATE ON \`position_management\` 
FOR EACH ROW BEGIN
IF old.sgid <> new.sgid
THEN SET @sgid =
(SELECT
	redis_hset (
		CONCAT_WS(
			'_', 'position', new.groupid, new.clientid
		), 'sgid', CAST(new.sgid AS CHAR(20))
	)) ;
END IF ;
IF old.balance <> new.balance 
THEN SET @balance = 
(SELECT 
redis_hset (
CONCAT_WS(
'_', 'position', new.groupid, new.clientid
), 'balance', CAST(new.balance AS CHAR(20))
)) ;
END IF ;
IF old.prevbalance <> new.prevbalance 
THEN SET @prevbalance = 
(SELECT 
redis_hset (
CONCAT_WS(
'_', 'position', new.groupid, new.clientid
), 'prevbalance', CAST(new.prevbalance AS CHAR(20))
)) ;
END IF ;
IF old.changed <> new.changed 
THEN SET @changed = 
(SELECT 
redis_hset (
CONCAT_WS(
'_', 'position', new.groupid, new.clientid
), 'changed', CAST(new.changed AS CHAR(20))
)) ;
END IF ;
IF old.margin <> new.margin 
THEN SET @margin = 
(SELECT 
redis_hset (
CONCAT_WS(
'_', 'position', new.groupid, new.clientid
), 'margin', CAST(new.margin AS CHAR(20))
)) ;
END IF ;
IF old.freemargin <> new.freemargin 
THEN SET @freemargin = 
(SELECT 
redis_hset (
CONCAT_WS(
'_', 'position', new.groupid, new.clientid
), 'freemargin', CAST(new.freemargin AS CHAR(20))
)) ;
END IF ;
IF old.profit <> new.profit 
THEN SET @profit = 
(SELECT 
redis_hset (
CONCAT_WS(
'_', 'position', new.groupid, new.clientid
), 'profit', CAST(new.profit AS CHAR(20))
)) ;
END IF ;
IF old.changed <> new.changed 
THEN SET @type = 
(SELECT 
redis_hset (
CONCAT_WS(
'_', 'position', new.groupid, new.clientid
), 'type', CAST(new.type AS CHAR(4))
)) ;
END IF ;
IF old.closebalance <> new.closebalance 
THEN SET @closebalance = 
(SELECT 
redis_hset (
CONCAT_WS(
'_', 'position', new.groupid, new.clientid
), 'closebalance', CAST(new.closebalance AS CHAR(11))
)) ;
END IF ;
IF old.status <> new.status 
THEN SET @status = 
(SELECT 
redis_hset (
CONCAT_WS(
'_', 'position', new.groupid, new.clientid
), 'status', CAST(new.status AS CHAR(4))
)) ;
END IF ;
IF old.check <> new.check 
THEN SET @check = 
(SELECT 
redis_hset (
CONCAT_WS(
'_', 'position', new.groupid, new.clientid
), 'check', CAST(new.check AS CHAR(4))
)) ;
END IF ;
END;
\$\$
DELIMITER ;"
FUN_UUID_TO_UNIXTIME="DELIMITER \$\$

CREATE FUNCTION \`uuid2unixtime\`(param VARCHAR(36)) RETURNS bigint(20)
BEGIN

    DECLARE ret BIGINT;

     SELECT CONV(

            CONCAT(

                   SUBSTRING(param,14,3),

                   SUBSTRING(param,9,4),

                   SUBSTRING(param,1,8))

                   ,16,10)

            DIV 10000

            - (141427 * 24 * 60 * 60 * 1000 ) INTO ret FROM DUAL;

RETURN ret;

END \$\$
DELIMITER ;"

PRO_POSITION_CREATE_TABLE_BY_DAY="DELIMITER \$\$
USE \`${MASTER_DB_NAME}\`\$\$
DROP PROCEDURE IF EXISTS \`pro_position_create_table_by_day\`\$\$
CREATE DEFINER=\`${MASTER_DB_USER}\`@\`${MASTER_DB_HOST}\` PROCEDURE \`pro_position_create_table_by_day\`()
BEGIN
DECLARE yestodayTableName VARCHAR(20);
DECLARE currTableName VARCHAR(20);
DECLARE tomorrowTableName VARCHAR(20);
DECLARE afterTomorrowTableName VARCHAR(20);
SET yestodayTableName = DATE_FORMAT(ADDDATE(CURDATE(), INTERVAL -1 DAY), '%Y%m%d');
SET currTableName = DATE_FORMAT(CURDATE(), '%Y%m%d');
SET tomorrowTableName = DATE_FORMAT(ADDDATE(CURDATE(), INTERVAL 1 DAY), '%Y%m%d');
SET afterTomorrowTableName = DATE_FORMAT(ADDDATE(CURDATE(), INTERVAL 2 DAY), '%Y%m%d');
-- create table pm_order_ans_log_yyyymmdd
SET @sqlbuf=CONCAT('CREATE TABLE IF NOT EXISTS pm_order_ans_log_', 
currTableName, 
' like pm_order_ans_log_',
yestodayTableName);
SELECT @sqlbuf;
PREPARE stmt FROM @sqlbuf;
EXECUTE stmt;
DEALLOCATE PREPARE stmt;

-- create table pm_order_ans_log_yyyymmdd
SET @sqlbuf=CONCAT('CREATE TABLE IF NOT EXISTS pm_order_ans_log_', 
tomorrowTableName, 
' like pm_order_ans_log_',
yestodayTableName);
SELECT @sqlbuf;
PREPARE stmt FROM @sqlbuf;
EXECUTE stmt;
DEALLOCATE PREPARE stmt;

-- create table pm_order_ans_log_yyyymmdd
SET @sqlbuf=CONCAT('CREATE TABLE IF NOT EXISTS pm_order_ans_log_', 
afterTomorrowTableName, 
' like pm_order_ans_log_',
yestodayTableName);
SELECT @sqlbuf;
PREPARE stmt FROM @sqlbuf;
EXECUTE stmt;
DEALLOCATE PREPARE stmt;
-- --------------------------------------
/*
pm_send_order_log_
*/
-- create table pm_send_order_log_yyyymmdd
SET @sqlbuf=CONCAT('CREATE TABLE IF NOT EXISTS pm_send_order_log_', 
currTableName, 
' like pm_send_order_log_',
yestodayTableName);
SELECT @sqlbuf;
PREPARE stmt FROM @sqlbuf;
EXECUTE stmt;
DEALLOCATE PREPARE stmt;

-- create table pm_send_order_log_yyyymmdd
SET @sqlbuf=CONCAT('CREATE TABLE IF NOT EXISTS pm_send_order_log_', 
tomorrowTableName, 
' like pm_send_order_log_',
yestodayTableName);
SELECT @sqlbuf;
PREPARE stmt FROM @sqlbuf;
EXECUTE stmt;
DEALLOCATE PREPARE stmt;

-- create table pm_send_order_log_yyyymmdd
SET @sqlbuf=CONCAT('CREATE TABLE IF NOT EXISTS pm_send_order_log_', 
afterTomorrowTableName, 
' like pm_send_order_log_',
yestodayTableName);
SELECT @sqlbuf;
PREPARE stmt FROM @sqlbuf;
EXECUTE stmt;
DEALLOCATE PREPARE stmt;

/*validate*/
-- pm_validate_order_log_
SET @sqlbuf=CONCAT('CREATE TABLE IF NOT EXISTS pm_validate_order_log_', 
currTableName, 
' like pm_validate_order_log_',
yestodayTableName);
SELECT @sqlbuf;
PREPARE stmt FROM @sqlbuf;
EXECUTE stmt;
DEALLOCATE PREPARE stmt;

-- create table pm_validate_order_log
SET @sqlbuf=CONCAT('CREATE TABLE IF NOT EXISTS pm_validate_order_log_', 
tomorrowTableName, 
' like pm_validate_order_log_',
yestodayTableName);
SELECT @sqlbuf;
PREPARE stmt FROM @sqlbuf;
EXECUTE stmt;
DEALLOCATE PREPARE stmt;

-- create table pm_validate_order_log
SET @sqlbuf=CONCAT('CREATE TABLE IF NOT EXISTS pm_validate_order_log_', 
afterTomorrowTableName, 
' like pm_validate_order_log_',
yestodayTableName);
SELECT @sqlbuf;
PREPARE stmt FROM @sqlbuf;
EXECUTE stmt;
DEALLOCATE PREPARE stmt;

/*position_management_log_*/
SET @sqlbuf=CONCAT('CREATE TABLE IF NOT EXISTS position_management_log_', 
currTableName, 
' like position_management_log_',
yestodayTableName);
SELECT @sqlbuf;
PREPARE stmt FROM @sqlbuf;
EXECUTE stmt;
DEALLOCATE PREPARE stmt;

-- create table sg_send_order_log_yyyymmdd
SET @sqlbuf=CONCAT('CREATE TABLE IF NOT EXISTS position_management_log_', 
tomorrowTableName, 
' like position_management_log_',
yestodayTableName);
SELECT @sqlbuf;
PREPARE stmt FROM @sqlbuf;
EXECUTE stmt;
DEALLOCATE PREPARE stmt;

-- create table sg_send_order_log_yyyymmdd
SET @sqlbuf=CONCAT('CREATE TABLE IF NOT EXISTS position_management_log_', 
afterTomorrowTableName, 
' like position_management_log_',
yestodayTableName);
SELECT @sqlbuf;
PREPARE stmt FROM @sqlbuf;
EXECUTE stmt;
DEALLOCATE PREPARE stmt;
END\$\$
DELIMITER ;"

EVENT_POSITION_CREATE_TABLE_BY_DAY="DELIMITER \$\$
SET GLOBAL event_scheduler = ON\$\$   
DROP EVENT IF EXISTS \`${MASTER_DB_NAME}\`.\`event_position_create_table_by_day\` \$\$
CREATE	DEFINER = '${MASTER_DB_USER}'@'${MASTER_DB_HOST}' EVENT \`${MASTER_DB_NAME}\`.\`event_position_create_table_by_day\`
ON SCHEDULE
EVERY 6 HOUR
STARTS '${NEXT_DAY} 00:00:00'
DO
BEGIN
CALL pro_position_create_table_by_day();
END\$\$
DELIMITER ;"

#echo ${CREATE_PM_SEND_ORDER_LOG}
#echo ${CREATE_PM_VALIDATE_ORDER_LOG}
#echo ${CREATE_PM_ORDER_ANS_LOG}
#echo ${CREATE_POSITION_MANAGEMENT_LOG}
#echo ${CREATE_POSITION_MANAGEMENT}
#echo ${CREATE_SG_ORDER_ANS_LOG}
#echo ${T_POSITION_MANAGEMENT_AFTER_INSERT}
#echo ${T_POSITION_MANAGEMENT_AFTER_UPDATE}
#echo ${PRO_POSITION_CREATE_TABLE_BY_DAY}
#echo ${EVENT_POSITION_CREATE_TABLE_BY_DAY}
#echo ${FUN_UUID_TO_UNIXTIME}

#mysql command to connect to database

mysql -u$MASTER_DB_USER -p$MASTER_DB_PASSWD -P$MASTER_DB_PORT -h$MASTER_DB_HOST -D$MASTER_DB_NAME <<EOF 
${CREATE_PM_SEND_ORDER_LOG}
${CREATE_PM_VALIDATE_ORDER_LOG}
${CREATE_PM_ORDER_ANS_LOG}
${CREATE_POSITION_MANAGEMENT_LOG}
${CREATE_POSITION_MANAGEMENT}
${CREATE_SG_ORDER_ANS_LOG}
${T_POSITION_MANAGEMENT_AFTER_INSERT}
${T_POSITION_MANAGEMENT_AFTER_UPDATE}
${PRO_POSITION_CREATE_TABLE_BY_DAY}
${EVENT_POSITION_CREATE_TABLE_BY_DAY}
${FUN_UUID_TO_UNIXTIME}
EOF

echo "End of script"


