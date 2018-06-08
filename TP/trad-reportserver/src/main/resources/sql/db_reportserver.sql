

CREATE DATABASE /*!32312 IF NOT EXISTS*/`db_reportserver` /*!40100 DEFAULT CHARACTER SET latin1 */;

USE `db_reportserver`;

/*Table structure for table `mt4_config` */

DROP TABLE IF EXISTS `mt4_config`;

CREATE TABLE `mt4_config` (
  `CONFIG` int(11) NOT NULL,
  `VALUE_INT` int(11) DEFAULT NULL,
  `VALUE_STR` char(255) DEFAULT NULL,
  PRIMARY KEY (`CONFIG`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Data for the table `mt4_config` */

insert  into `mt4_config`(`CONFIG`,`VALUE_INT`,`VALUE_STR`) values (0,0,NULL),(1,1498512117,NULL),(2,0,NULL),(3,NULL,',JTVBOC,ANTVBOC,'),(4,NULL,'119.81.146.246:443'),(5,3300,NULL),(1000,950,NULL);

/*Table structure for table `mt4_prices` */

DROP TABLE IF EXISTS `mt4_prices`;

CREATE TABLE `mt4_prices` (
  `SYMBOL` char(16) NOT NULL,
  `TIME` datetime NOT NULL,
  `BID` double NOT NULL,
  `ASK` double NOT NULL,
  `LOW` double NOT NULL,
  `HIGH` double NOT NULL,
  `DIRECTION` int(11) NOT NULL,
  `DIGITS` int(11) NOT NULL,
  `SPREAD` int(11) NOT NULL,
  `MODIFY_TIME` datetime NOT NULL,
  PRIMARY KEY (`SYMBOL`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;


/*Table structure for table `mt4_trades` */

DROP TABLE IF EXISTS `mt4_trades`;

CREATE TABLE `mt4_trades` (
  `TICKET` int(11) NOT NULL,
  `LOGIN` int(11) NOT NULL,
  `SYMBOL` char(16) NOT NULL,
  `DIGITS` int(11) NOT NULL,
  `CMD` int(11) NOT NULL,
  `VOLUME` int(11) NOT NULL,
  `OPEN_TIME` datetime NOT NULL,
  `OPEN_PRICE` double NOT NULL,
  `SL` double NOT NULL,
  `TP` double NOT NULL,
  `CLOSE_TIME` datetime NOT NULL,
  `EXPIRATION` datetime NOT NULL,
  `REASON` int(11) NOT NULL DEFAULT '0',
  `CONV_RATE1` double NOT NULL,
  `CONV_RATE2` double NOT NULL,
  `COMMISSION` double NOT NULL,
  `COMMISSION_AGENT` double NOT NULL,
  `SWAPS` double NOT NULL,
  `CLOSE_PRICE` double NOT NULL,
  `PROFIT` double NOT NULL,
  `TAXES` double NOT NULL,
  `COMMENT` char(32) NOT NULL,
  `INTERNAL_ID` int(11) NOT NULL,
  `MARGIN_RATE` double NOT NULL,
  `TIMESTAMP` int(11) NOT NULL,
  `MAGIC` int(11) NOT NULL DEFAULT '0',
  `GW_VOLUME` int(11) NOT NULL DEFAULT '0',
  `GW_OPEN_PRICE` int(11) NOT NULL DEFAULT '0',
  `GW_CLOSE_PRICE` int(11) NOT NULL DEFAULT '0',
  `MODIFY_TIME` datetime NOT NULL,
  PRIMARY KEY (`TICKET`),
  KEY `INDEX_LOGIN` (`LOGIN`),
  KEY `INDEX_CMD` (`CMD`),
  KEY `INDEX_OPENTIME` (`OPEN_TIME`),
  KEY `INDEX_CLOSETIME` (`CLOSE_TIME`),
  KEY `INDEX_STAMP` (`TIMESTAMP`),
  KEY `idx_log_cm` (`LOGIN`,`CMD`,`OPEN_TIME`),
  KEY `close_time_cmd` (`CLOSE_TIME`,`CMD`,`SYMBOL`),
  KEY `LOGIN` (`LOGIN`,`CLOSE_TIME`,`CMD`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;


/*Table structure for table `mt4_trades_archive` */

DROP TABLE IF EXISTS `mt4_trades_archive`;

CREATE TABLE `mt4_trades_archive` (
  `TICKET` int(11) NOT NULL,
  `LOGIN` int(11) NOT NULL,
  `SYMBOL` char(16) NOT NULL,
  `DIGITS` int(11) NOT NULL,
  `CMD` int(11) NOT NULL,
  `VOLUME` int(11) NOT NULL,
  `OPEN_TIME` datetime NOT NULL,
  `OPEN_PRICE` double NOT NULL,
  `SL` double NOT NULL,
  `TP` double NOT NULL,
  `CLOSE_TIME` datetime NOT NULL,
  `EXPIRATION` datetime NOT NULL,
  `REASON` int(11) NOT NULL DEFAULT '0',
  `CONV_RATE1` double NOT NULL,
  `CONV_RATE2` double NOT NULL,
  `COMMISSION` double NOT NULL,
  `COMMISSION_AGENT` double NOT NULL,
  `SWAPS` double NOT NULL,
  `CLOSE_PRICE` double NOT NULL,
  `PROFIT` double NOT NULL,
  `TAXES` double NOT NULL,
  `COMMENT` char(32) NOT NULL,
  `INTERNAL_ID` int(11) NOT NULL,
  `MARGIN_RATE` double NOT NULL,
  `TIMESTAMP` int(11) NOT NULL,
  `MAGIC` int(11) NOT NULL DEFAULT '0',
  `GW_VOLUME` int(11) NOT NULL DEFAULT '0',
  `GW_OPEN_PRICE` int(11) NOT NULL DEFAULT '0',
  `GW_CLOSE_PRICE` int(11) NOT NULL DEFAULT '0',
  `MODIFY_TIME` datetime NOT NULL,
  PRIMARY KEY (`TICKET`),
  KEY `INDEX_LOGIN` (`LOGIN`),
  KEY `INDEX_CMD` (`CMD`),
  KEY `INDEX_OPENTIME` (`OPEN_TIME`),
  KEY `INDEX_CLOSETIME` (`CLOSE_TIME`),
  KEY `INDEX_STAMP` (`TIMESTAMP`),
  KEY `idx_log_cm` (`LOGIN`,`CMD`,`OPEN_TIME`),
  KEY `close_time_cmd` (`CLOSE_TIME`,`CMD`,`SYMBOL`),
  KEY `LOGIN` (`LOGIN`,`CLOSE_TIME`,`CMD`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Data for the table `mt4_trades_archive` */

/*Table structure for table `mt4_trades_full` */

DROP TABLE IF EXISTS `mt4_trades_full`;

CREATE TABLE `mt4_trades_full` (
  `TICKET` int(11) NOT NULL AUTO_INCREMENT,
  `ORDERID` varchar(36) NOT NULL,
  `LOGIN` int(11) NOT NULL,
  `SYMBOL` char(16) NOT NULL,
  `DIGITS` int(11) NOT NULL,
  `CMD` int(11) NOT NULL,
  `VOLUME` int(11) NOT NULL,
  `OPEN_TIME` datetime NOT NULL,
  `OPEN_PRICE` double NOT NULL,
  `SL` double NOT NULL,
  `TP` double NOT NULL,
  `CLOSE_TIME` datetime NOT NULL,
  `EXPIRATION` datetime NOT NULL,
  `REASON` int(11) NOT NULL DEFAULT '0',
  `CONV_RATE1` double NOT NULL,
  `CONV_RATE2` double NOT NULL,
  `COMMISSION` double NOT NULL,
  `COMMISSION_AGENT` double NOT NULL,
  `SWAPS` double NOT NULL,
  `CLOSE_PRICE` double NOT NULL,
  `PROFIT` double NOT NULL,
  `TAXES` double NOT NULL,
  `COMMENT` char(32) NOT NULL,
  `INTERNAL_ID` int(11) NOT NULL,
  `MARGIN_RATE` double NOT NULL,
  `TIMESTAMP` int(11) NOT NULL,
  `MAGIC` int(11) NOT NULL DEFAULT '0',
  `GW_VOLUME` int(11) NOT NULL DEFAULT '0',
  `GW_OPEN_PRICE` int(11) NOT NULL DEFAULT '0',
  `GW_CLOSE_PRICE` int(11) NOT NULL DEFAULT '0',
  `MODIFY_TIME` datetime NOT NULL,
  PRIMARY KEY (`TICKET`),
  UNIQUE KEY `ORDERID` (`ORDERID`),
  KEY `INDEX_LOGIN` (`LOGIN`),
  KEY `INDEX_CMD` (`CMD`),
  KEY `INDEX_OPENTIME` (`OPEN_TIME`),
  KEY `INDEX_CLOSETIME` (`CLOSE_TIME`),
  KEY `INDEX_STAMP` (`TIMESTAMP`),
  KEY `idx_log_cm` (`LOGIN`,`CMD`,`OPEN_TIME`),
  KEY `close_time_cmd` (`CLOSE_TIME`,`CMD`,`SYMBOL`),
  KEY `LOGIN` (`LOGIN`,`CLOSE_TIME`,`CMD`)
) ENGINE=InnoDB AUTO_INCREMENT=12936 DEFAULT CHARSET=utf8;

/*Table structure for table `mt4_users` */

DROP TABLE IF EXISTS `mt4_users`;

CREATE TABLE `mt4_users` (
  `LOGIN` int(11) NOT NULL,
  `GROUP` char(16) NOT NULL,
  `ENABLE` int(11) NOT NULL,
  `ENABLE_CHANGE_PASS` int(11) NOT NULL,
  `ENABLE_READONLY` int(11) NOT NULL,
  `ENABLE_OTP` int(11) NOT NULL DEFAULT '0',
  `PASSWORD_PHONE` char(32) NOT NULL,
  `NAME` char(128) NOT NULL,
  `COUNTRY` char(32) NOT NULL,
  `CITY` char(32) NOT NULL,
  `STATE` char(32) NOT NULL,
  `ZIPCODE` char(16) NOT NULL,
  `ADDRESS` char(128) NOT NULL,
  `LEAD_SOURCE` char(32) NOT NULL DEFAULT '',
  `PHONE` char(32) NOT NULL,
  `EMAIL` char(48) NOT NULL,
  `COMMENT` char(64) NOT NULL,
  `ID` char(32) NOT NULL,
  `STATUS` char(16) NOT NULL,
  `REGDATE` datetime NOT NULL,
  `LASTDATE` datetime NOT NULL,
  `LEVERAGE` int(11) NOT NULL,
  `AGENT_ACCOUNT` int(11) NOT NULL,
  `TIMESTAMP` int(11) NOT NULL,
  `BALANCE` double NOT NULL,
  `PREVMONTHBALANCE` double NOT NULL,
  `PREVBALANCE` double NOT NULL,
  `CREDIT` double NOT NULL,
  `INTERESTRATE` double NOT NULL,
  `TAXES` double NOT NULL,
  `SEND_REPORTS` int(11) NOT NULL,
  `MQID` int(10) unsigned NOT NULL DEFAULT '0',
  `USER_COLOR` int(11) NOT NULL,
  `EQUITY` double NOT NULL DEFAULT '0',
  `MARGIN` double NOT NULL DEFAULT '0',
  `MARGIN_LEVEL` double NOT NULL DEFAULT '0',
  `MARGIN_FREE` double NOT NULL DEFAULT '0',
  `CURRENCY` char(16) NOT NULL DEFAULT '',
  `API_DATA` blob,
  `MODIFY_TIME` datetime NOT NULL,
  PRIMARY KEY (`LOGIN`),
  KEY `INDEX_STAMP` (`TIMESTAMP`),
  KEY `login` (`LOGIN`,`GROUP`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;


DELIMITER $$
DROP PROCEDURE IF EXISTS updateBalanceUser $$
CREATE PROCEDURE updateBalanceUser 
(IN login INT, IN balance DOUBLE, IN preBalance DOUBLE, IN margin DOUBLE, IN margin_level DOUBLE, IN margin_free DOUBLE)
BEGIN
UPDATE mt4_users SET 
BALANCE=balance, PREVBALANCE=preBalance, MARGIN=margin, MARGIN_LEVEL=margin_level, MARGIN_FREE=margin_free 
WHERE LOGIN=login;
END
$$
DELIMITER ;

DELIMITER $$
CREATE TRIGGER trig_trades_insert 
AFTER INSERT ON mt4_trades_full FOR EACH ROW
BEGIN
     INSERT INTO mt4_trades VALUES(new.TICKET,new.LOGIN, new.SYMBOL, new.DIGITS, new.CMD,
			new.VOLUME, new.OPEN_TIME, new.OPEN_PRICE, new.SL, new.TP,
			new.CLOSE_TIME, new.EXPIRATION, new.REASON, new.CONV_RATE1, new.CONV_RATE2, 
			new.COMMISSION, new.COMMISSION_AGENT, new.SWAPS, new.CLOSE_PRICE, new.PROFIT,
			new.TAXES, new.COMMENT, new.INTERNAL_ID, new.MARGIN_RATE, new.`TIMESTAMP`, 
			new.MAGIC, new.GW_VOLUME, new.GW_OPEN_PRICE, new.GW_CLOSE_PRICE, new.MODIFY_TIME);
END;
$$
DELIMITER ;

DELIMITER $$
CREATE TRIGGER trig_trades_update 
AFTER UPDATE ON mt4_trades_full FOR EACH ROW
BEGIN
     UPDATE mt4_trades SET LOGIN=new.LOGIN, SYMBOL=new.SYMBOL, DIGITS=new.DIGITS, CMD=new.CMD,
			VOLUME=new.VOLUME, OPEN_TIME=new.OPEN_TIME, OPEN_PRICE=new.OPEN_PRICE, SL=new.SL, TP=new.TP,
			CLOSE_TIME=new.CLOSE_TIME, EXPIRATION=new.EXPIRATION, REASON=new.REASON, CONV_RATE1=new.CONV_RATE1, CONV_RATE2=new.CONV_RATE2, 
			COMMISSION=new.COMMISSION, COMMISSION_AGENT=new.COMMISSION_AGENT, SWAPS=new.SWAPS, CLOSE_PRICE=new.CLOSE_PRICE, PROFIT=new.PROFIT,
			TAXES=new.TAXES, COMMENT=new.COMMENT, INTERNAL_ID=new.INTERNAL_ID, MARGIN_RATE=new.MARGIN_RATE, `TIMESTAMP`=new.`TIMESTAMP`, 
			MAGIC=new.MAGIC, GW_VOLUME=new.GW_VOLUME, GW_OPEN_PRICE=new.GW_OPEN_PRICE, GW_CLOSE_PRICE=new.GW_CLOSE_PRICE, MODIFY_TIME=new.MODIFY_TIME WHERE TICKET=new.TICKET;
END;
$$
DELIMITER ;

DELIMITER $$
DROP TRIGGER IF EXISTS trig_trades_delete $$
CREATE TRIGGER trig_trades_delete AFTER DELETE ON mt4_trades_full
FOR EACH ROW 
BEGIN
DELETE FROM mt4_trades WHERE ticket=old.ticket;
END
$$
DELIMITER ;