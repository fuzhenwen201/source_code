
DROP DATABASE IF EXISTS db_trad_rm;
CREATE DATABASE db_trad_rm;
USE db_trad_rm;

DROP TABLE IF EXISTS `tb_rm_trade`;
CREATE TABLE `tb_rm_trade` (
  `ClientID` INT(11) DEFAULT NULL,
  `GroupID` INT(11) DEFAULT NULL,
  `AccountID` INT(11) DEFAULT NULL,
  `BoAccountID` INT(11) DEFAULT NULL,
  `ReqID` VARCHAR(50) DEFAULT NULL,
  `OrderID` VARCHAR(50) NOT NULL,
  `OrderStatus` INT(11) DEFAULT NULL,
  `ExpiryTime` VARCHAR(45) DEFAULT NULL,
  `OpenTime` VARCHAR(45) DEFAULT NULL,
  `OpenPrice` BIGINT(20) NULL DEFAULT NULL,
  `ClosePrice` BIGINT(20) NULL DEFAULT NULL,
  `Direction` INT(11) DEFAULT NULL,
  `ExpiryPeriod` INT(11) DEFAULT NULL,
  `Amount` BIGINT(20) NULL DEFAULT NULL,
  `PayOut` INT(11) DEFAULT NULL,
  `AssetID` INT(11) DEFAULT NULL,
  `type` INT(11) DEFAULT NULL,
  `comment` VARCHAR(32) DEFAULT NULL,
  PRIMARY KEY (`OrderID`)
) ENGINE=INNODB DEFAULT CHARSET=utf8;

/*DROP DATABASE IF EXISTS RMDB;
CREATE DATABASE IF NOT EXISTS `RMDB` DEFAULT CHARACTER SET latin1 ;
USE `RMDB`;*/
CREATE TABLE IF NOT EXISTS `openposition` (
  `ClientID` int(11) DEFAULT NULL,
  `GroupID` int(11) DEFAULT NULL,
  `AccountID` int(11) DEFAULT NULL,
  `BoAccountID` int(11) DEFAULT NULL,
  `RequestID` varchar(50) DEFAULT NULL,
  `OrderID` varchar(50) NOT NULL,
  `OrderStatus` int(11) DEFAULT NULL,
  `ExpiryTime` varchar(45) DEFAULT NULL,
  `OpenTime` varchar(45) DEFAULT NULL,
  `OpenPrice` BIGINT(20) NULL DEFAULT NULL,
  `ClosePrice` BIGINT(20) NULL DEFAULT NULL,
  `Direction` int(11) DEFAULT NULL,
  `Amount` BIGINT(20) NULL DEFAULT NULL,
  `PayOut` int(11) DEFAULT NULL,
  `AssetID` int(11) DEFAULT NULL,
  PRIMARY KEY (`OrderID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE IF NOT EXISTS `closeposition` (
  `ClientID` int(11) DEFAULT NULL,
  `GroupID` int(11) DEFAULT NULL,
  `AccountID` int(11) DEFAULT NULL,
  `BoAccountID` int(11) DEFAULT NULL,
  `RequestID` varchar(50) DEFAULT NULL,
  `OrderID` varchar(50) NOT NULL,
  `OrderStatus` int(11) DEFAULT NULL,
  `ExpiryTime` varchar(45) DEFAULT NULL,
  `OpenTime` varchar(45) DEFAULT NULL,
  `OpenPrice` BIGINT(20) NULL DEFAULT NULL,
  `ClosePrice` BIGINT(20) NULL DEFAULT NULL,
  `Direction` int(11) DEFAULT NULL,
  `Amount` BIGINT(20) NULL DEFAULT NULL,
  `PayOut` int(11) DEFAULT NULL,
  `AssetID` int(11) DEFAULT NULL,
  `Created` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`OrderID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;


DELIMITER $$
CREATE PROCEDURE `InsertClosePosition`(in cid int, in gid int, in aid int, boacount int, in rid varchar(50), in oid varchar(50), in orderstatus int, in etime varchar(50),
in otime varchar(50), in oprice int, in cprice int, in direction int, in amount int, in payout int, in assetId int)
BEGIN
INSERT INTO `closeposition`
(`ClientID`,`GroupID`,`AccountID`,`BoAccountID`,`RequestID`,`OrderID`,`OrderStatus`,`ExpiryTime`,`OpenTime`,`OpenPrice`,`ClosePrice`,`Direction`,`Amount`,`PayOut`, `AssetID`,`Created`)
VALUES
(cid, gid, aid, boacount, rid, oid, orderstatus, etime, otime, oprice, cprice, direction, amount, payout, assetId, now());

END $$
DELIMITER ;

DELIMITER $$
CREATE PROCEDURE `insertOpenPosition`(in cid int, in gid int, in aid int, in boacount int, in rid varchar(50), in oid varchar(50), in ostatus int, in etime varchar(50), in otime varchar(50), in oprice int, 
in cprice int, in direction int, in amount int, in payout int, in assetId int)
BEGIN
INSERT INTO `openposition`
(`ClientID`,`GroupID`,`AccountID`,`BoAccountID`,`RequestID`,`OrderID`,`OrderStatus`,`ExpiryTime`,`OpenTime`,`OpenPrice`,`ClosePrice`,
`Direction`,`Amount`,`PayOut`,AssetID)
VALUES
(cid, gid, aid, boacount, rid, oid, ostatus, etime, otime, oprice,cprice, direction, amount, payout, assetId);
END $$
DELIMITER ;

