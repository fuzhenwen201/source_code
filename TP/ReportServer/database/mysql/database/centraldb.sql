/*
SQLyog Ultimate v12.09 (64 bit)
MySQL - 5.6.33-0ubuntu0.14.04.1 : Database - centraldb
*********************************************************************
*/

SET NAMES utf8 ;

SET SQL_MODE='';

SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 ;
SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 ;
SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' ;
SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 ;
CREATE DATABASE  IF NOT EXISTS `centraldb`  DEFAULT CHARACTER SET utf8;

USE `centraldb`;

/*Table structure for table `account` */

DROP TABLE IF EXISTS `account`;

CREATE TABLE `account` (
  `AccountID` int(11) NOT NULL,
  `GroupID` int(11) DEFAULT NULL,
  `ClientID` int(11) DEFAULT NULL,
  `Balance` int(11) NOT NULL,
  `PrevBalance` int(11) DEFAULT NULL,
  `ChangedDeposit` int(11) DEFAULT NULL,
  `Margin` int(11) DEFAULT NULL,
  `FreeMargin` int(11) DEFAULT NULL,
  `CloseBalance` int(11) DEFAULT NULL
  `DirtyBit` int(11) DEFAULT NULL,
  `BOAccountID` int(11) DEFAULT NULL,
  `FXAccountID` int(11) DEFAULT NULL,
  PRIMARY KEY (`AccountID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Table structure for table `admin` */

DROP TABLE IF EXISTS `admin`;

CREATE TABLE `admin` (
  `adminid` int(11) NOT NULL,
  `name` varchar(45) NOT NULL,
  `password` varchar(45) NOT NULL,
  `address` varchar(255) NOT NULL,
  `phoneno` varchar(20) NOT NULL,
  `isDeleted` tinyint(1) DEFAULT '0',
  PRIMARY KEY (`adminid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Table structure for table `boaccount` */

DROP TABLE IF EXISTS `boaccount`;

CREATE TABLE `boaccount` (
  `AccountID` int(11) DEFAULT NULL,
  `BOAccountID` int(11) NOT NULL AUTO_INCREMENT,
  `BOAssets` varchar(50) CHARACTER SET utf8 DEFAULT NULL,
  PRIMARY KEY (`BOAccountID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Table structure for table `closeposition` */

DROP TABLE IF EXISTS `closeposition`;

CREATE TABLE `closeposition` (
  `ClientID` int(11) NOT NULL,
  `GroupID` int(11) DEFAULT NULL,
  `AccountID` int(11) DEFAULT NULL,
  `BoAccountID` int(11) DEFAULT NULL,
  `RequestID` int(11) DEFAULT NULL,
  `OrderID` varchar(50) CHARACTER SET utf8 NOT NULL,
  `OrderStatus` int(11) DEFAULT NULL,
  `ExpiryTime` varchar(45) CHARACTER SET utf8 DEFAULT NULL,
  `OpenTime` varchar(45) CHARACTER SET utf8 DEFAULT NULL,
  `OpenPrice` int(11) DEFAULT NULL,
  `ClosePrice` int(11) DEFAULT NULL,
  `Direction` int(11) DEFAULT NULL,
  `Amount` int(11) DEFAULT NULL,
  `PayOut` int(11) DEFAULT NULL,
  PRIMARY KEY (`OrderID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Table structure for table `manager` */

DROP TABLE IF EXISTS `manager`;

CREATE TABLE `manager` (
  `ManagerID` int(11) NOT NULL DEFAULT '0',
  `GroupID` int(11) DEFAULT NULL,
  `GroupName` varchar(45) CHARACTER SET utf8 DEFAULT NULL,
  `Passwords` varchar(50) CHARACTER SET utf8 DEFAULT NULL,
  `FirstName` varchar(50) CHARACTER SET utf8 DEFAULT NULL,
  `LastName` varchar(50) CHARACTER SET utf8 DEFAULT NULL,
  `Email` varchar(60) CHARACTER SET utf8 DEFAULT NULL,
  `Address` varchar(255) CHARACTER SET utf8 DEFAULT NULL,
  `City` varchar(45) CHARACTER SET utf8 DEFAULT NULL,
  `State` varchar(45) CHARACTER SET utf8 DEFAULT NULL,
  `Country` varchar(45) CHARACTER SET utf8 DEFAULT NULL,
  `Pin` varchar(45) CHARACTER SET utf8 DEFAULT NULL,
  `isDeleted` tinyint(1) DEFAULT '0',
  PRIMARY KEY (`ManagerID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Table structure for table `manager_right` */

DROP TABLE IF EXISTS `manager_right`;

CREATE TABLE `manager_right` (
  `ManagerID` int(11) NOT NULL DEFAULT '0',
  `CreateUser` int(11) DEFAULT '0',
  `DeleteTrader` int(11) DEFAULT '0',
  `UpdateTrader` int(11) DEFAULT '0',
  `ViewTrader` int(11) DEFAULT '0',
  `ViewTraderDetail` int(11) DEFAULT '0',
  `CreateManager` int(11) DEFAULT '0',
  `DeleteManager` int(11) DEFAULT '0',
  `UpdateManager` int(11) DEFAULT '0',
  `ViewManager` int(11) DEFAULT '0',
  `ViewManagerDetails` int(11) DEFAULT '0',
  `ModifyManagerAccess` int(11) DEFAULT '0',
  `Credit` int(11) DEFAULT '0',
  `Balance` int(11) DEFAULT '0',
  `OpenTrades` int(11) DEFAULT '0',
  `BoMonitoring` int(11) DEFAULT '0',
  PRIMARY KEY (`ManagerID`),
  UNIQUE KEY `ManagerID_UNIQUE` (`ManagerID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Table structure for table `modifyrecords` */

DROP TABLE IF EXISTS `modifyrecords`;

CREATE TABLE `modifyrecords` (
  `ClientID` int(11) DEFAULT NULL,
  `Time` varchar(50) CHARACTER SET utf8 DEFAULT NULL,
  `IP` varchar(50) CHARACTER SET utf8 DEFAULT NULL,
  `Request` varchar(2000) CHARACTER SET utf8 DEFAULT NULL,
  `Reply` varchar(2000) CHARACTER SET utf8 DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Table structure for table `onlinemanager` */

DROP TABLE IF EXISTS `onlinemanager`;

CREATE TABLE `onlinemanager` (
  `LoginCod` int(11) NOT NULL AUTO_INCREMENT,
  `ManagerId` int(11) NOT NULL,
  `LoginFrom` varchar(45) CHARACTER SET utf8 NOT NULL,
  `LoginTo` varchar(45) CHARACTER SET utf8 DEFAULT NULL,
  PRIMARY KEY (`LoginCod`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Table structure for table `openposition` */

DROP TABLE IF EXISTS `openposition`;

CREATE TABLE `openposition` (
  `ClientID` int(11) NOT NULL,
  `GroupID` int(11) DEFAULT NULL,
  `AccountID` int(11) DEFAULT NULL,
  `BoAccountID` int(11) DEFAULT NULL,
  `RequestID` int(11) DEFAULT NULL,
  `OrderID` varchar(50) CHARACTER SET utf8 NOT NULL,
  `OrderStatus` int(11) DEFAULT NULL,
  `ExpiryTime` varchar(45) CHARACTER SET utf8 DEFAULT NULL,
  `OpenTime` varchar(45) CHARACTER SET utf8 DEFAULT NULL,
  `OpenPrice` int(11) DEFAULT NULL,
  `ClosePrice` int(11) DEFAULT NULL,
  `Direction` int(11) DEFAULT NULL,
  `Amount` int(11) DEFAULT NULL,
  `PayOut` int(11) DEFAULT NULL,
  PRIMARY KEY (`OrderID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Table structure for table `trader` */

DROP TABLE IF EXISTS `trader`;

CREATE TABLE `trader` (
  `ClientID` int(11) NOT NULL,
  `GroupID` int(11) DEFAULT NULL,
  `GroupName` varchar(45) CHARACTER SET utf8 DEFAULT NULL,
  `Passwords` varchar(30) CHARACTER SET utf8 DEFAULT NULL,
  `FirstName` varchar(50) CHARACTER SET utf8 DEFAULT NULL,
  `LastName` varchar(50) CHARACTER SET utf8 DEFAULT NULL,
  `Email` varchar(100) CHARACTER SET utf8 DEFAULT NULL,
  `Address` varchar(100) CHARACTER SET utf8 DEFAULT NULL,
  `City` varchar(45) CHARACTER SET utf8 DEFAULT NULL,
  `State` varchar(45) CHARACTER SET utf8 DEFAULT NULL,
  `Country` varchar(45) CHARACTER SET utf8 DEFAULT NULL,
  `Pin` varchar(45) CHARACTER SET utf8 DEFAULT NULL,
  `IsDeleted` int(1) DEFAULT NULL,
  PRIMARY KEY (`ClientID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Table structure for table `trader_group` */

DROP TABLE IF EXISTS `trader_group`;

CREATE TABLE `trader_group` (
  `GroupID` int(11) NOT NULL,
  `GroupName` varchar(45) DEFAULT NULL,
  `TotalTraders` int(11) DEFAULT NULL,
  `ManagerID` int(11) DEFAULT NULL,
  PRIMARY KEY (`GroupID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/* Procedure structure for procedure `InsertAccount` */

DROP PROCEDURE IF EXISTS  `InsertAccount` ;

DELIMITER $$

CREATE PROCEDURE `InsertAccount`(in accountId int, in groupID int, in clientID int, in balance int, in prevBalance int, in changedDeposit int, in margin int, 
in freeMargin int, in closeBalance int,in dirtyBit int,in BOAccountID int, in FXAccountID int)
BEGIN

INSERT INTO `account`
	(`AccountId`,`GroupID`,`ClientID`,`Balance`,`PrevBalance`,`ChangedDeposit`,`Margin`,`FreeMargin`,`CloseBalance`,`DirtyBit`,`BOAccountID`,`FXAccountID`)
	VALUES(accountId, groupID,clientID,balance,prevBalance,changedDeposit,margin,freeMargin,closeBalance,dirtyBit,bOAccountID,fXAccountID);
   

END $$
DELIMITER ;

/* Procedure structure for procedure `InsertAdmin` */

DROP PROCEDURE IF EXISTS  `InsertAdmin` ;

DELIMITER $$

CREATE PROCEDURE `InsertAdmin`(in adminId int, in name varchar(50), in password varchar(50), in address varchar(90), in phoneno varchar(20), in isDeleted tinyint(1))
BEGIN

INSERT INTO `admin`
	(`adminid`,`name`,`password`,`address`,`phoneno`,`isDeleted`)
	VALUES(adminid,name,password,address,phoneno,isDeleted);
   

END $$
DELIMITER ;

/* Procedure structure for procedure `InsertBoAccount` */

DROP PROCEDURE IF EXISTS  `InsertBoAccount` ;

DELIMITER $$

CREATE  PROCEDURE `InsertBoAccount`(in AccountID int, out BOAccountID int, in BOAssets int)
BEGIN
INSERT INTO `boaccount`
(`AccountID`,`BOAssets`)
VALUES
(AccountID,BOAssets);
set BOAccountID=mysql_insert_id();
END $$
DELIMITER ;

/* Procedure structure for procedure `InsertClosePosition` */

DROP PROCEDURE IF EXISTS  `InsertClosePosition` ;

DELIMITER $$

CREATE  PROCEDURE `InsertClosePosition`(in cid int, in gid int, in aid int, boacount int, in rid int, in oid varchar(50), in orderstatus int, in etime varchar(50),
in otime varchar(50), in oprice int, in cprice int, in direction int, in amount int, in payout int, in assetId int)
BEGIN
INSERT INTO `riskmanagmentdb`.`closeposition`
(`ClientID`,`GroupID`,`AccountID`,`BoAccountID`,`RequestID`,`OrderID`,`OrderStatus`,`ExpiryTime`,`OpenTime`,`OpenPrice`,`ClosePrice`,`Direction`,`Amount`,`PayOut`, AssetID)
VALUES
(cid, gid, aid, boacount, rid, oid, orderstatus, etime, otime, oprice, cprice, direction, amount, payout, assetId);

END $$
DELIMITER ;

/* Procedure structure for procedure `InsertManager` */

DROP PROCEDURE IF EXISTS  `InsertManager` ;

DELIMITER $$

CREATE PROCEDURE `InsertManager`(in ManagerId int,in GroupID int,in groupName varchar(50), in Passwords varchar(50), in FirstName varchar(50), in LastName varchar(50), in Email varchar(50), in Address varchar(50), in city varchar(50), in state varchar(50), in country varchar(50), in pin varchar(50))
BEGIN

INSERT INTO `manager`
(`ManagerID`,`GroupID`,`GroupName`,`Passwords`,`FirstName`,`LastName`,`Email`,`Address`,`City`,`State`,`Country`,`Pin`)
VALUES(managerId,GroupID,groupName,Passwords,FirstName,LastName,Email,Address, city, state, country, pin);


END $$
DELIMITER ;

/* Procedure structure for procedure `InsertModifyRecord` */

DROP PROCEDURE IF EXISTS  `InsertModifyRecord` ;

DELIMITER $$

CREATE  PROCEDURE `InsertModifyRecord`(in clientId int, in time varchar(50), in ip varchar(50), in request varchar(2000), in reply varchar(2000))
BEGIN
INSERT INTO `modifyrecords`
(`ClientID`,`Time`,`IP`,`Request`,`Reply`)
VALUES
(clientId,time,ip, request, reply);

END $$
DELIMITER ;

/* Procedure structure for procedure `insertOpenPosition` */

DROP PROCEDURE IF EXISTS  `insertOpenPosition` ;

DELIMITER $$

CREATE  PROCEDURE `insertOpenPosition`(in cid int, in gid int, in aid int, in boacount int, in rid int, in oid varchar(50), in ostatus int, in etime varchar(50), in otime varchar(50), in oprice int, 
in cprice int, in dirction int, in amount int, in payout int, in assetId int)
BEGIN
INSERT INTO `riskmanagmentdb`.`openposition`
(`ClientID`,`GroupID`,`AccountID`,`BoAccountID`,`RequestID`,`OrderID`,`OrderStatus`,`ExpiryTime`,`OpenTime`,`OpenPrice`,`ClosePrice`,
`Direction`,`Amount`,`PayOut`,AssetID)
VALUES
(cid, gid, aid, boacount, rid, oid, ostatus, etime, otime, oprice,cprice, direction, amount, payout, assetId);
END $$
DELIMITER ;

/* Procedure structure for procedure `InsertTrader` */

DROP PROCEDURE IF EXISTS  `InsertTrader` ;

DELIMITER $$

CREATE  PROCEDURE `InsertTrader`(in clientId int, in groupId int, in groupName varchar(50), in passwords varchar(50), in firstName varchar(50), in lastName varchar(50), in email varchar(50), 
in address varchar(50), in city varchar(50), in state varchar(50), in country varchar(50), in pin varchar(50),in isDeleted int)
BEGIN

INSERT INTO `trader`
(`GroupID`,`GroupName`,`ClientID`,`Passwords`,`FirstName`,`LastName`,`Email`,`Address`,`City`,`State`,`Country`,`Pin`,`IsDeleted`)
VALUES (groupId,groupName,clientId,passwords,firstName,lastName,email,address, city, state,country,pin,0);
END $$
DELIMITER ;

/* Procedure structure for procedure `insetManagerRight` */

DROP PROCEDURE IF EXISTS  `insetManagerRight` ;

DELIMITER $$

CREATE  PROCEDURE `insetManagerRight`(in managerId int, in createUser int, in deleteUser int, in updateUser int, in viewTrader int, in viewTradeDetail int, in createManager int,
in deleteManager int, in updateManager int,in viewManager int, in viewManagerDetails int, in modifymanagerAccess int, in credit int,in balance int, in openTrades int,in boMonitoring int)
BEGIN
INSERT INTO `manager_right`
(`ManagerID`,`CreateUser`,`DeleteTrader`,`UpdateTrader`,`ViewTrader`,`ViewTraderDetail`,`CreateManager`,`DeleteManager`,`UpdateManager`,`ViewManager`,`ViewManagerDetails`,`ModifyManagerAccess`,`Credit`,`Balance`,`OpenTrades`,`BoMonitoring`)
VALUES
(managerId,createUser,deleteUser,updateUser,viewTrader,viewTraderDetail,createManager,deleteManager,updateManager,viewManager,viewManagerDetails,modifyManagerAccess,credit,balance,openTrades,boMonitoring);

END $$
DELIMITER ;

/* Procedure structure for procedure `RetriveTrader` */

DROP PROCEDURE IF EXISTS  `RetriveTrader` ;

DELIMITER $$

CREATE PROCEDURE `RetriveTrader`(in clientid int)
BEGIN
if clientid<>0 then 
select t.ClientID as ClientID, t.FirstName, t.LastName , t.GroupID as GroupID, t.GroupName as GroupName, t.Email as Email, t.Address as Address, t.Passwords as Passwords,
a.AccountId as AccountId, a.Balance as Balance, a.PrevBalance as PrevBalance, a.Margin as Margin, a.FreeMargin as FreeMargin, a.ChangedDeposit as ChangedDeposit, a.CloseBalance as CloseBalance, a.DirtyBit as DirtyBit, a.BOAccountID as BoAccount, a.FXAccountID as FxAccount
from trader as t join `account` as a on t.ClientID = a.ClientID where t.ClientID=clientid;
else
select t.ClientID as ClientID, t.FirstName,t.LastName, t.GroupID as GroupID, t.GroupName as GroupName, t.Email as Email, t.Address as Address, t.Passwords as Passwords,
a.AccountId as AccountId, a.Balance as Balance, a.PrevBalance as PrevBalance, a.Margin as Margin, a.FreeMargin as FreeMargin, a.ChangedDeposit as ChangedDeposit, a.CloseBalance as CloseBalance, a.DirtyBit as DirtyBit, a.BOAccountID as BoAccount, a.FXAccountID as FxAccount
from trader as t join `account` as a on t.ClientID = a.ClientID;
end if;
END $$
DELIMITER ;

/* Procedure structure for procedure `UpdateAccount` */

DROP PROCEDURE IF EXISTS  `UpdateAccount` ;

DELIMITER $$

CREATE PROCEDURE `UpdateAccount`(in AcountId int, in balance int, in prevBalance int, in changeDeposit int, in margin int, 
in freeMargin int, in closeBalance int, in dirtyBit int)
BEGIN


UPDATE `account`
SET
`Balance` = balance,
`PrevBalance` = prevBalance,
`ChangedDeposit` = changeDeposit,
`Margin` = margin,
`FreeMargin` = freeMargin,
`CloseBalance`=closeBalance,
`DirtyBit`=dirtyBit
WHERE `AccountId` = AcountId;

END $$
DELIMITER ;

/* Procedure structure for procedure `UpdateManager` */

DROP PROCEDURE IF EXISTS  `UpdateManager` ;

DELIMITER $$

CREATE  PROCEDURE `UpdateManager`(in GroupId int,in groupName varchar(50), in MenagerId int, in Passwords varchar(50), in Email varchar(50), in Address varchar(50),
in city varchar(50), in state varchar(50), in country varchar(50), in pin varchar(50))
BEGIN
UPDATE `manager`
SET
`GroupID` = GroupID,
`GroupName`=groupName,
`Passwords` = Passwords,
`Email` = Email,
`Address` = Address,
`City`= city,
`State`=state,
`Country`=country,
`Pin`=pin
WHERE `ManagerID` = MenagerId;
END $$
DELIMITER ;

/* Procedure structure for procedure `UpdateTrader` */

DROP PROCEDURE IF EXISTS  `UpdateTrader` ;

DELIMITER $$

CREATE PROCEDURE `UpdateTrader`(in id int, in GroupID int,in groupName varchar(50), in Paswd varchar(50), in fname varchar(50), in lname varchar(50), in Email varchar(50), in Address varchar(50),
in city varchar(50), in state varchar(50), in country varchar(50), in pin varchar(50), in isDelete int)
BEGIN
UPDATE `trader`
SET
`GroupID` = GroupID,
`GroupName` = GroupName,
`Passwords` = Paswd,
`FirstName` = fname,
`LastName` = lname,
`Email` = Email,
`Address` = Address,
`City` = city,
`State` = state,
`Country` = country,
`Pin` = pin,
`IsDeleted`=isDelete
WHERE `ClientID` = id;
END $$
DELIMITER ;

SET SQL_MODE=@OLD_SQL_MODE ;
SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS ;
SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS ;
SET SQL_NOTES=@OLD_SQL_NOTES ;
