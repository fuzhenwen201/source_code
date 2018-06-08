/*
SQLyog Ultimate v12.09 (64 bit)
MySQL - 5.6.37-log : Database - CMDB
*********************************************************************
*/

/*!40101 SET NAMES utf8 */;

/*!40101 SET SQL_MODE=''*/;

/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;
CREATE DATABASE /*!32312 IF NOT EXISTS*/`CMDB` /*!40100 DEFAULT CHARACTER SET latin1 */;

USE `CMDB`;

/*Table structure for table `account` */

DROP TABLE IF EXISTS `account`;

CREATE TABLE `account` (
  `AccountID` int(11) NOT NULL AUTO_INCREMENT,
  `GroupID` int(11) DEFAULT NULL,
  `ClientID` int(11) NOT NULL,
  `Balance` bigint(20) NOT NULL,
  `PrevBalance` bigint(20) DEFAULT NULL,
  `ChangedDeposit` bigint(20) DEFAULT NULL,
  `Margin` bigint(20) DEFAULT NULL,
  `FreeMargin` bigint(20) DEFAULT NULL,
  `CloseBalance` bigint(20) DEFAULT NULL,
  `DirtyBit` int(11) DEFAULT NULL,
  `BOAccountID` int(11) DEFAULT NULL,
  `FXAccountID` int(11) DEFAULT NULL,
  `modifiedTime` timestamp(6) NULL DEFAULT NULL,
  PRIMARY KEY (`AccountID`),
  UNIQUE KEY `ClientID_UNIQUE` (`ClientID`),
  CONSTRAINT `ClientID_FK1` FOREIGN KEY (`ClientID`) REFERENCES `trader` (`ClientID`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB AUTO_INCREMENT=0 DEFAULT CHARSET=utf8;

/*Table structure for table `boaccount` */

DROP TABLE IF EXISTS `boaccount`;

CREATE TABLE `boaccount` (
  `AccountID` int(11) DEFAULT NULL,
  `BOAccountID` int(11) NOT NULL AUTO_INCREMENT,
  `BOAssets` varchar(50) DEFAULT NULL,
  PRIMARY KEY (`BOAccountID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Table structure for table `loggedin_trader` */

DROP TABLE IF EXISTS `loggedin_trader`;

CREATE TABLE `loggedin_trader` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `clientId` int(11) DEFAULT NULL,
  `ipAddress` varchar(45) DEFAULT NULL,
  `macAddress` varchar(45) DEFAULT NULL,
  `hardDiskSerialNo` varchar(100) DEFAULT NULL,
  `reqId` varchar(100) DEFAULT NULL,
  `loginTime` timestamp(6) NULL DEFAULT NULL,
  `logoutTime` timestamp(6) NULL DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=0 DEFAULT CHARSET=utf8;

/*Table structure for table `modify_deposit` */

DROP TABLE IF EXISTS `modify_deposit`;

CREATE TABLE `modify_deposit` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `orderId` varchar(100) NOT NULL,
  `managerId` int(11) NOT NULL,
  `clientId` int(11) NOT NULL,
  `prevBalance` bigint(20) DEFAULT NULL,
  `amount` bigint(20) DEFAULT NULL,
  `balance` bigint(20) DEFAULT NULL,
  `type` varchar(45) DEFAULT NULL,
  `comment` varchar(250) DEFAULT NULL,
  `createdDate` timestamp(6) NULL DEFAULT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `orderId_UNIQUE` (`orderId`),
  KEY `MD_clientId_FK_idx` (`clientId`),
  CONSTRAINT `MD_clientId_FK` FOREIGN KEY (`clientId`) REFERENCES `trader` (`ClientID`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB AUTO_INCREMENT=0 DEFAULT CHARSET=utf8;

/*Table structure for table `online_trader` */

DROP TABLE IF EXISTS `online_trader`;

CREATE TABLE `online_trader` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `clientId` int(11) NOT NULL,
  `loginReqMessage` varchar(1000) DEFAULT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `clientId_UNIQUE` (`clientId`),
  KEY `onlineTraderClientId_FK_idx` (`clientId`),
  CONSTRAINT `onlineTraderClientId_FK` FOREIGN KEY (`clientId`) REFERENCES `trader` (`ClientID`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB AUTO_INCREMENT=0 DEFAULT CHARSET=utf8;

/*Table structure for table `trader` */

DROP TABLE IF EXISTS `trader`;

CREATE TABLE `trader` (
  `ClientID` int(11) NOT NULL AUTO_INCREMENT,
  `GroupID` int(11) NOT NULL,
  `Passwords` varchar(45) DEFAULT NULL,
  `FirstName` varchar(50) DEFAULT NULL,
  `LastName` varchar(50) DEFAULT NULL,
  `Email` varchar(100) DEFAULT NULL,
  `Address` varchar(250) DEFAULT NULL,
  `City` varchar(100) DEFAULT NULL,
  `State` varchar(45) DEFAULT NULL,
  `Country` varchar(45) DEFAULT NULL,
  `Pin` varchar(45) DEFAULT NULL,
  `IsDeleted` tinyint(1) DEFAULT '0',
  `isLogin` tinyint(1) DEFAULT '0',
  `createdDate` timestamp NULL DEFAULT NULL,
  `updatedDate` timestamp NULL DEFAULT NULL,
  `dirtyBit` int(11) DEFAULT '0',
  `isEnable` tinyint(1) DEFAULT '1',
  PRIMARY KEY (`ClientID`),
  KEY `GroupId_index` (`GroupID`),
  KEY `idx_delete` (`IsDeleted`)
) ENGINE=InnoDB AUTO_INCREMENT=0 DEFAULT CHARSET=utf8;

/* Procedure structure for procedure `InsertAccount` */

/*!50003 DROP PROCEDURE IF EXISTS  `InsertAccount` */;

DELIMITER $$

/*!50003 CREATE PROCEDURE `InsertAccount`(inout Id int, in groupID int, in clientID int, in balance bigint, in prevBalance bigint, in changedDeposit bigint, in margin bigint, 
in freeMargin bigint, in closeBalance bigint, in dirtyBit int, in BOAccountID int, in FXAccountID int, in modifiedTime TIMESTAMP)
BEGIN
if ifnull(Id,0)=0 then 
	INSERT INTO `account`
	(`GroupID`,`ClientID`,`Balance`,`PrevBalance`,`ChangedDeposit`,`Margin`,`FreeMargin`,`CloseBalance`,`DirtyBit`,`BOAccountID`,`FXAccountID`, `modifiedTime`)
	VALUES(groupID,clientID,balance,prevBalance,changedDeposit,margin,freeMargin,closeBalance,dirtyBit,bOAccountID,fXAccountID,modifiedTime);
  set Id = (SELECT  AUTO_INCREMENT  FROM INFORMATION_SCHEMA.TABLES WHERE TABLE_SCHEMA = DATABASE() AND TABLE_NAME = 'account')-1;
    
else 
INSERT INTO `account`
	(`AccountId`,`GroupID`,`ClientID`,`Balance`,`PrevBalance`,`ChangedDeposit`,`Margin`,`FreeMargin`,`CloseBalance`,`DirtyBit`,`BOAccountID`,`FXAccountID`, `modifiedTime`)
	VALUES(Id, groupID,clientID,balance,prevBalance,changedDeposit,margin,freeMargin,closeBalance,dirtyBit,bOAccountID,fXAccountID,modifiedTime);
    set Id = (SELECT  AUTO_INCREMENT  FROM INFORMATION_SCHEMA.TABLES WHERE TABLE_SCHEMA = DATABASE() AND TABLE_NAME = 'account')-1;
end if;
END */$$
DELIMITER ;

/* Procedure structure for procedure `InsertBoAccount` */

/*!50003 DROP PROCEDURE IF EXISTS  `InsertBoAccount` */;

DELIMITER $$

/*!50003 CREATE PROCEDURE `InsertBoAccount`(in AccountID int, out BOAccountID int, in BOAssets int)
BEGIN
INSERT INTO `boaccount`
(`AccountID`,`BOAssets`)
VALUES
(AccountID,BOAssets);
set BOAccountID=mysql_insert_id();
END */$$
DELIMITER ;

/* Procedure structure for procedure `InsertTrader` */

/*!50003 DROP PROCEDURE IF EXISTS  `InsertTrader` */;

DELIMITER $$

/*!50003 CREATE PROCEDURE `InsertTrader`(inout ClentID int,IN GroupID int, in Passwords nvarchar(50), in FirstName nvarchar(50), in LastName nvarchar(50), in Email nvarchar(50), 
in Address nvarchar(50), in city nvarchar(50), in state nvarchar(50), in country nvarchar(50), in pin varchar(50), in dirtyBit int, in isEnable TINYINT)
BEGIN
if ifnull(ClentID,0)=0 then 
INSERT INTO `trader`
(`GroupID`,`Passwords`,`FirstName`,`LastName`,`Email`,`Address`,`City`,`State`,`Country`,`Pin`,`IsDeleted`, `isLogin`, `createdDate`, `updatedDate`, `dirtyBit`, `isEnable`)
VALUES (GroupID,Passwords,FirstName,LastName,Email,Address, city, state,country,pin,0,0, now(), now(), dirtyBit, isEnable);

set ClentID = (SELECT  AUTO_INCREMENT  FROM INFORMATION_SCHEMA.TABLES WHERE TABLE_SCHEMA = DATABASE() AND TABLE_NAME = 'trader')-1;
else 
INSERT INTO `trader`
(`GroupID`,`ClientID`,`Passwords`,`FirstName`,`LastName`,`Email`,`Address`,`City`,`State`,`Country`,`Pin`,`IsDeleted`, `isLogin`, `createdDate`, `updatedDate`, `dirtyBit`, `isEnable`)
VALUES (GroupID,ClentID,Passwords,FirstName,LastName,Email,Address, city, state,country,pin,0,0, now(), now(), dirtyBit, isEnable);

set ClentID = (SELECT  AUTO_INCREMENT  FROM INFORMATION_SCHEMA.TABLES WHERE TABLE_SCHEMA = DATABASE() AND TABLE_NAME = 'trader')-1;
end if;
END */$$
DELIMITER ;

/* Procedure structure for procedure `RetriveTrader` */

/*!50003 DROP PROCEDURE IF EXISTS  `RetriveTrader` */;

DELIMITER $$

/*!50003 CREATE PROCEDURE `RetriveTrader`(in clientid int)
BEGIN
if clientid<>0 then 
select t.ClientID as ClientID, t.FirstName, t.LastName , t.GroupID as GroupID, t.Email as Email, t.Address as Address, t.Passwords as Passwords,t.City, t.State, t.Country, t.Pin,t.isEnable,
a.AccountId as AccountId, a.Balance as Balance, a.PrevBalance as PrevBalance, a.Margin as Margin, a.FreeMargin as FreeMargin, a.ChangedDeposit as ChangedDeposit, a.CloseBalance as CloseBalance, a.DirtyBit as DirtyBit, a.BOAccountID as BoAccount, a.FXAccountID as FxAccount
from trader as t join `account` as a on t.ClientID = a.ClientID where t.ClientID=clientid AND t.IsDeleted=0;
else
select t.ClientID as ClientID, t.FirstName,t.LastName, t.GroupID as GroupID, t.Email as Email, t.Address as Address, t.Passwords as Passwords,t.City, t.State, t.Country, t.Pin,t.isEnable,
a.AccountId as AccountId, a.Balance as Balance, a.PrevBalance as PrevBalance, a.Margin as Margin, a.FreeMargin as FreeMargin, a.ChangedDeposit as ChangedDeposit, a.CloseBalance as CloseBalance, a.DirtyBit as DirtyBit, a.BOAccountID as BoAccount, a.FXAccountID as FxAccount
from trader as t join `account` as a on t.ClientID = a.ClientID AND t.IsDeleted=0;
end if;
END */$$
DELIMITER ;

/* Procedure structure for procedure `UpdateAccount` */

/*!50003 DROP PROCEDURE IF EXISTS  `UpdateAccount` */;

DELIMITER $$

/*!50003 CREATE PROCEDURE `UpdateAccount`(in accId int, in balance bigint, in prevBalance bigint, in changeDeposit bigint, in margin bigint, 
in freeMargin bigint, in closeBalance bigint, in dirtyBit int, in modifiedTime TIMESTAMP)
BEGIN

update account set 
	Balance = balance , 
	PrevBalance = prevBalance, 
	ChangedDeposit = changeDeposit , 
	Margin = margin, 
    FreeMargin = freeMargin, 
    CloseBalance = closeBalance, 
    DirtyBit = dirtyBit, 
    modifiedTime = modifiedTime
where AccountID = accId;

END */$$
DELIMITER ;

/* Procedure structure for procedure `UpdateTrader` */

/*!50003 DROP PROCEDURE IF EXISTS  `UpdateTrader` */;

DELIMITER $$

/*!50003 CREATE PROCEDURE `UpdateTrader`(in id int, in GroupID int, in Paswd varchar(50), in fname nvarchar(50), in lname nvarchar(50), in Email nvarchar(50), in Address nvarchar(100),
in city nvarchar(50), in state nvarchar(50), in country nvarchar(50), in pin varchar(50), in isDelete int, in updatedDate TIMESTAMP, in dirtyBit int, in isEnable TINYINT)
BEGIN
UPDATE `trader`
SET
`GroupID` = GroupID,
`Passwords` = Paswd,
`FirstName` = fname,
`LastName` = lname,
`Email` = Email,
`Address` = Address,
`City` = city,
`State` = state,
`Country` = country,
`Pin` = pin,
`IsDeleted`=isDelete,
`updatedDate` = updatedDate,
`dirtyBit` = dirtyBit, 
`isEnable` = isEnable

WHERE `ClientID` = id;
END */$$
DELIMITER ;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;
