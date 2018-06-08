/*
SQLyog Ultimate v12.09 (64 bit)
MySQL - 5.6.37-log : Database - admdb
*********************************************************************
*/

/*!40101 SET NAMES utf8 */;

/*!40101 SET SQL_MODE=''*/;

/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;
CREATE DATABASE /*!32312 IF NOT EXISTS*/`admdb` /*!40100 DEFAULT CHARACTER SET utf8 */;

USE `admdb`;

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
  KEY `account_GroupID_FK_idx` (`GroupID`),
  CONSTRAINT `Account_GroupID_FK1` FOREIGN KEY (`GroupID`) REFERENCES `group_common` (`GroupID`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `ClientID_FK1` FOREIGN KEY (`ClientID`) REFERENCES `trader` (`ClientID`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB AUTO_INCREMENT=0 DEFAULT CHARSET=utf8;

/*Table structure for table `admin` */

DROP TABLE IF EXISTS `admin`;

CREATE TABLE `admin` (
  `adminid` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(45) NOT NULL,
  `password` varchar(45) NOT NULL,
  `address` varchar(255) DEFAULT NULL,
  `phoneno` varchar(20) DEFAULT NULL,
  `isDeleted` tinyint(1) NOT NULL DEFAULT '0',
  `isSuperAdmin` tinyint(1) NOT NULL DEFAULT '0',
  PRIMARY KEY (`adminid`)
) ENGINE=InnoDB AUTO_INCREMENT=0 DEFAULT CHARSET=utf8;

/*Table structure for table `bo_symbol` */

DROP TABLE IF EXISTS `bo_symbol`;

CREATE TABLE `bo_symbol` (
  `SymbolID` int(11) NOT NULL,
  `BoCalType` int(11) NOT NULL,
  `ReturnRate` double DEFAULT NULL,
  `ModifiedTime` timestamp NOT NULL,
  UNIQUE KEY `SymbolID_UNIQUE` (`SymbolID`),
  CONSTRAINT `SymbolID_FK6` FOREIGN KEY (`SymbolID`) REFERENCES `symbol` (`SymbolID`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Table structure for table `boaccount` */

DROP TABLE IF EXISTS `boaccount`;

CREATE TABLE `boaccount` (
  `BOAccountID` int(11) NOT NULL AUTO_INCREMENT,
  `AccountID` int(11) DEFAULT NULL,
  `BOAssets` varchar(50) DEFAULT NULL,
  PRIMARY KEY (`BOAccountID`)
) ENGINE=InnoDB AUTO_INCREMENT=0 DEFAULT CHARSET=utf8;

/*Table structure for table `closeposition` */

DROP TABLE IF EXISTS `closeposition`;

CREATE TABLE `closeposition` (
  `ClientID` int(11) NOT NULL,
  `GroupID` int(11) DEFAULT NULL,
  `AccountID` int(11) DEFAULT NULL,
  `BoAccountID` int(11) DEFAULT NULL,
  `RequestID` varchar(50) DEFAULT NULL,
  `OrderID` varchar(50) NOT NULL,
  `OrderStatus` int(11) DEFAULT NULL,
  `ExpiryTime` varchar(45) DEFAULT NULL,
  `OpenTime` varchar(45) DEFAULT NULL,
  `OpenPrice` bigint(20) DEFAULT NULL,
  `ClosePrice` bigint(20) DEFAULT NULL,
  `Direction` int(11) DEFAULT NULL,
  `Amount` bigint(20) DEFAULT NULL,
  `PayOut` int(11) DEFAULT NULL,
  `AssetID` int(11) DEFAULT NULL,
  `Created` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`OrderID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Table structure for table `group_archieve` */

DROP TABLE IF EXISTS `group_archieve`;

CREATE TABLE `group_archieve` (
  `GroupID` int(11) NOT NULL,
  `InActivePeriod` int(15) DEFAULT NULL,
  `MaxBalance` double DEFAULT NULL,
  `ModifiedTime` timestamp NULL DEFAULT NULL,
  `PendingDeletedOrder` int(11) DEFAULT NULL,
  KEY `GroupID_FK1_idx` (`GroupID`),
  CONSTRAINT `GroupID_FK1` FOREIGN KEY (`GroupID`) REFERENCES `group_common` (`GroupID`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Table structure for table `group_common` */

DROP TABLE IF EXISTS `group_common`;

CREATE TABLE `group_common` (
  `GroupID` int(11) NOT NULL AUTO_INCREMENT,
  `IsEnable` tinyint(1) NOT NULL DEFAULT '0',
  `GroupName` varchar(40) NOT NULL,
  `OTP` varchar(30) DEFAULT NULL,
  `IsEnableForceOTP` tinyint(1) NOT NULL DEFAULT '0',
  `OwnerName` varchar(40) DEFAULT NULL,
  `SupportedPage` varchar(45) DEFAULT NULL,
  `DefaultDeposit` decimal(18,2) NOT NULL,
  `DepositCurrency` varchar(100) DEFAULT NULL,
  `DefaultLeverage` double DEFAULT NULL,
  `AnnualInterst` decimal(5,2) DEFAULT '0.00',
  `CreatedTime` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP,
  `ModifiedTime` timestamp NULL DEFAULT NULL,
  `IsDeleted` tinyint(1) NOT NULL DEFAULT '0',
  PRIMARY KEY (`GroupID`)
) ENGINE=InnoDB AUTO_INCREMENT=0 DEFAULT CHARSET=utf8;

/*Table structure for table `group_margin` */

DROP TABLE IF EXISTS `group_margin`;

CREATE TABLE `group_margin` (
  `GroupID` int(11) NOT NULL,
  `CallLevel` int(15) DEFAULT NULL,
  `StopOutLevel` varchar(40) DEFAULT NULL,
  `LevelFormat` varchar(20) DEFAULT '0',
  `FreeMargin` varchar(100) DEFAULT NULL,
  `VirtualCredit` double DEFAULT NULL,
  `IsHedgedMarginCalulated` tinyint(1) DEFAULT '0',
  `IsSkipHedged` tinyint(1) DEFAULT '0',
  `ModifiedTime` timestamp NULL DEFAULT NULL,
  KEY `GroupID_FK2` (`GroupID`),
  CONSTRAINT `GroupID_FK2` FOREIGN KEY (`GroupID`) REFERENCES `group_common` (`GroupID`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Table structure for table `group_report` */

DROP TABLE IF EXISTS `group_report`;

CREATE TABLE `group_report` (
  `GroupID` int(11) NOT NULL,
  `IsReport` tinyint(1) DEFAULT '0',
  `SmtpServer` varchar(40) DEFAULT NULL,
  `TemplatePath` varchar(20) DEFAULT NULL,
  `SmtpLogin` varchar(45) DEFAULT NULL,
  `SupportEmail` varchar(45) DEFAULT NULL,
  `IsCopyReport` tinyint(1) DEFAULT '0',
  `Signature` varchar(45) DEFAULT '0',
  `ModifiedTime` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  UNIQUE KEY `GroupID_UNIQUE` (`GroupID`),
  CONSTRAINT `GroupID_FK3` FOREIGN KEY (`GroupID`) REFERENCES `group_common` (`GroupID`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Table structure for table `group_rights` */

DROP TABLE IF EXISTS `group_rights`;

CREATE TABLE `group_rights` (
  `GroupID` int(11) NOT NULL,
  `TimeOut` int(15) NOT NULL,
  `NewsView` varchar(40) DEFAULT NULL,
  `NewsLanguage` varchar(20) DEFAULT '0',
  `MaxSymbol` int(4) DEFAULT NULL,
  `MaxOrders` int(4) DEFAULT NULL,
  `TradeSignals` varchar(45) DEFAULT NULL,
  `IsMailSystem` tinyint(1) DEFAULT '0',
  `IsSwap` tinyint(1) DEFAULT '0',
  `IsEA` tinyint(1) DEFAULT '0',
  `IsPendingOrderExpiry` tinyint(1) DEFAULT '0',
  `IsTrailingStop` tinyint(1) DEFAULT '0',
  `IsReqPriceInIE` tinyint(1) DEFAULT '0',
  `IsHedgeProhibit` tinyint(1) DEFAULT '0',
  `IsFifo` tinyint(1) DEFAULT '0',
  `IsParClose` tinyint(1) DEFAULT '0',
  `IsRiskWarning` tinyint(1) DEFAULT '0',
  `ModifiedTime` timestamp NULL DEFAULT NULL,
  KEY `GroupID_FK4` (`GroupID`),
  CONSTRAINT `GroupID_FK4` FOREIGN KEY (`GroupID`) REFERENCES `group_common` (`GroupID`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Table structure for table `group_security` */

DROP TABLE IF EXISTS `group_security`;

CREATE TABLE `group_security` (
  `GroupID` int(11) NOT NULL,
  `SecurityID` int(11) NOT NULL,
  `IsInstruments` tinyint(1) DEFAULT '0',
  `IsTrade` tinyint(1) DEFAULT '0',
  `IsRequestMode` tinyint(1) DEFAULT '0',
  `ExecutionMode` varchar(25) DEFAULT NULL,
  `SpreadDiff` double DEFAULT NULL,
  `MaxDeviation` double DEFAULT NULL,
  `IsFreeMargin` tinyint(1) DEFAULT '0',
  `IsFastConfirmation` tinyint(1) DEFAULT '0',
  `IsCloseTwoOrder` tinyint(1) DEFAULT '0',
  `IsCloseAllOrder` tinyint(1) DEFAULT '0',
  `AutoClose` varchar(25) DEFAULT NULL,
  `MinLot` int(25) DEFAULT NULL,
  `MaxLot` int(25) DEFAULT NULL,
  `StepSize` int(25) DEFAULT NULL,
  `StdCommission` double DEFAULT NULL,
  `TaxCommission` double DEFAULT NULL,
  `AgentCommission` double DEFAULT NULL,
  `IsEnable` tinyint(1) DEFAULT '0',
  `ModifiedTime` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  UNIQUE KEY `CK1_GROUP_SECURITYID` (`GroupID`,`SecurityID`),
  KEY `GroupID_FK5_idx` (`GroupID`),
  KEY `FK1_SecurityID_Security_SecurityID_idx` (`SecurityID`),
  CONSTRAINT `FK1_SecurityID_Security_SecurityID` FOREIGN KEY (`SecurityID`) REFERENCES `security` (`SecurityID`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `GroupID_FK5` FOREIGN KEY (`GroupID`) REFERENCES `group_common` (`GroupID`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Table structure for table `group_symbol` */

DROP TABLE IF EXISTS `group_symbol`;

CREATE TABLE `group_symbol` (
  `GroupID` int(11) NOT NULL,
  `SymbolID` int(11) NOT NULL,
  `LongPositionSwap` mediumtext,
  `ShortPositionSwap` mediumtext,
  `MarginPercentage` double DEFAULT NULL,
  `ModifiedTime` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  UNIQUE KEY `CK1_GROUP_SYMBOL` (`GroupID`,`SymbolID`),
  KEY `GroupID_FK6` (`GroupID`),
  KEY `SymbolId_FK_idx` (`SymbolID`),
  CONSTRAINT `GroupID_FK6` FOREIGN KEY (`GroupID`) REFERENCES `group_common` (`GroupID`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `SymbolId_FK` FOREIGN KEY (`SymbolID`) REFERENCES `symbol` (`SymbolID`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Table structure for table `group_trading_condition` */

DROP TABLE IF EXISTS `group_trading_condition`;

CREATE TABLE `group_trading_condition` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `groupId` int(11) NOT NULL,
  `minimumBoAmount` bigint(20) DEFAULT NULL,
  `maximumBoAmount` bigint(20) DEFAULT NULL,
  `payout` bigint(20) NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`),
  UNIQUE KEY `groupId_UNIQUE` (`groupId`),
  KEY `GTC_groupId_FK_idx` (`groupId`),
  CONSTRAINT `GTC_groupId_FK` FOREIGN KEY (`groupId`) REFERENCES `group_common` (`GroupID`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB AUTO_INCREMENT=0 DEFAULT CHARSET=utf8;

/*Table structure for table `loggedin_admin` */

DROP TABLE IF EXISTS `loggedin_admin`;

CREATE TABLE `loggedin_admin` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `adminId` int(11) NOT NULL,
  `ipAddress` varchar(45) DEFAULT NULL,
  `macAddress` varchar(45) DEFAULT NULL,
  `hardDiskSerialNo` varchar(45) DEFAULT NULL,
  `reqId` varchar(45) DEFAULT NULL,
  `sessionId` varchar(45) DEFAULT NULL,
  `loginTime` timestamp(6) NULL DEFAULT NULL,
  `logoutTime` timestamp(6) NULL DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `adminId_FK_idx` (`adminId`),
  CONSTRAINT `adminId_FK` FOREIGN KEY (`adminId`) REFERENCES `admin` (`adminid`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB AUTO_INCREMENT=0 DEFAULT CHARSET=utf8;

/*Table structure for table `loggedin_manager` */

DROP TABLE IF EXISTS `loggedin_manager`;

CREATE TABLE `loggedin_manager` (
  `loginId` int(11) NOT NULL AUTO_INCREMENT,
  `managerId` int(11) NOT NULL,
  `ipAddress` varchar(45) DEFAULT NULL,
  `macAddress` varchar(45) DEFAULT NULL,
  `hardDiskSerialNo` varchar(45) DEFAULT NULL,
  `reqId` varchar(45) DEFAULT NULL,
  `sessionId` varchar(45) DEFAULT NULL,
  `loginTime` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP,
  `logoutTime` timestamp NULL DEFAULT NULL,
  PRIMARY KEY (`loginId`)
) ENGINE=InnoDB AUTO_INCREMENT=0 DEFAULT CHARSET=utf8;

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
  PRIMARY KEY (`id`),
  KEY `loggedIn_trader_clientID_FK_idx` (`clientId`),
  CONSTRAINT `loggedIn_trader_clientID_FK` FOREIGN KEY (`clientId`) REFERENCES `CMDB`.`trader` (`ClientID`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Table structure for table `manager` */

DROP TABLE IF EXISTS `manager`;

CREATE TABLE `manager` (
  `ManagerID` int(11) NOT NULL AUTO_INCREMENT,
  `GroupID` int(11) DEFAULT NULL,
  `Passwords` varchar(50) DEFAULT NULL,
  `FirstName` varchar(50) DEFAULT NULL,
  `LastName` varchar(50) DEFAULT NULL,
  `Email` varchar(60) DEFAULT NULL,
  `Address` varchar(255) DEFAULT NULL,
  `City` varchar(45) DEFAULT NULL,
  `State` varchar(45) DEFAULT NULL,
  `Country` varchar(45) DEFAULT NULL,
  `Pin` varchar(45) DEFAULT NULL,
  `isDeleted` tinyint(1) DEFAULT '0',
  `isEnable` tinyint(1) NOT NULL DEFAULT '1',
  PRIMARY KEY (`ManagerID`)
) ENGINE=InnoDB AUTO_INCREMENT=0 DEFAULT CHARSET=utf8;

/*Table structure for table `manager_against_group` */

DROP TABLE IF EXISTS `manager_against_group`;

CREATE TABLE `manager_against_group` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `ManagerId` int(11) NOT NULL,
  `GroupId` int(11) NOT NULL,
  PRIMARY KEY (`id`),
  KEY `FK1_ManagerID_Manager_idx` (`ManagerId`),
  KEY `FK2_GroupID_group_common_idx` (`GroupId`),
  CONSTRAINT `FK1_ManagerID_manager` FOREIGN KEY (`ManagerId`) REFERENCES `manager` (`ManagerID`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `MAG_GroupID_FK2` FOREIGN KEY (`GroupId`) REFERENCES `group_common` (`GroupID`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB AUTO_INCREMENT=0 DEFAULT CHARSET=utf8;

/*Table structure for table `manager_operation` */

DROP TABLE IF EXISTS `manager_operation`;

CREATE TABLE `manager_operation` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `loginId` int(11) NOT NULL,
  `createdDate` timestamp NULL DEFAULT NULL,
  `operation` varchar(45) DEFAULT NULL,
  `message` varchar(1000) CHARACTER SET utf8 DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `loginId_FK_idx` (`loginId`),
  CONSTRAINT `loginId_FK` FOREIGN KEY (`loginId`) REFERENCES `loggedin_manager` (`loginId`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB AUTO_INCREMENT=0 DEFAULT CHARSET=utf8;

/*Table structure for table `manager_right` */

DROP TABLE IF EXISTS `manager_right`;

CREATE TABLE `manager_right` (
  `slno` int(11) NOT NULL AUTO_INCREMENT,
  `ManagerID` int(11) NOT NULL,
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
  PRIMARY KEY (`slno`),
  UNIQUE KEY `ManagerID_UNIQUE` (`ManagerID`),
  CONSTRAINT `ManagerID_FK1` FOREIGN KEY (`ManagerID`) REFERENCES `manager` (`ManagerID`) ON DELETE NO ACTION ON UPDATE NO ACTION
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

/*Table structure for table `openposition` */

DROP TABLE IF EXISTS `openposition`;

CREATE TABLE `openposition` (
  `ClientID` int(11) NOT NULL,
  `GroupID` int(11) DEFAULT NULL,
  `AccountID` int(11) DEFAULT NULL,
  `BoAccountID` int(11) DEFAULT NULL,
  `RequestID` int(11) DEFAULT NULL,
  `OrderID` varchar(50) NOT NULL,
  `OrderStatus` int(11) DEFAULT NULL,
  `ExpiryTime` varchar(45) DEFAULT NULL,
  `OpenTime` varchar(45) DEFAULT NULL,
  `OpenPrice` int(11) DEFAULT NULL,
  `ClosePrice` int(11) DEFAULT NULL,
  `Direction` int(11) DEFAULT NULL,
  `Amount` int(11) DEFAULT NULL,
  `PayOut` int(11) DEFAULT NULL,
  PRIMARY KEY (`OrderID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Table structure for table `security` */

DROP TABLE IF EXISTS `security`;

CREATE TABLE `security` (
  `SecurityID` int(11) NOT NULL AUTO_INCREMENT,
  `SecurityName` varchar(50) CHARACTER SET utf8 DEFAULT NULL,
  `IsEnable` tinyint(1) DEFAULT '0',
  `Description` varchar(45) DEFAULT NULL,
  `CreatedTime` timestamp NULL DEFAULT NULL,
  `ModifiedTime` timestamp NULL DEFAULT NULL,
  `IsDeleted` tinyint(4) DEFAULT NULL,
  `SecurityType` varchar(4) DEFAULT 'bo',
  PRIMARY KEY (`SecurityID`)
) ENGINE=InnoDB AUTO_INCREMENT=0 DEFAULT CHARSET=utf8;

/*Table structure for table `sybmol_master` */

DROP TABLE IF EXISTS `sybmol_master`;

CREATE TABLE `sybmol_master` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `SymbolID` int(11) NOT NULL,
  `Name` varchar(45) NOT NULL,
  `Type` varchar(45) NOT NULL,
  PRIMARY KEY (`id`,`SymbolID`),
  UNIQUE KEY `SymbolID_UNIQUE` (`SymbolID`),
  UNIQUE KEY `Name_UNIQUE` (`Name`)
) ENGINE=InnoDB AUTO_INCREMENT=0 DEFAULT CHARSET=utf8;

/*Table structure for table `symbol` */

DROP TABLE IF EXISTS `symbol`;

CREATE TABLE `symbol` (
  `SymbolID` int(11) NOT NULL AUTO_INCREMENT,
  `SymbolName` varchar(20) DEFAULT NULL,
  `SourceName` varchar(20) DEFAULT NULL,
  `Digits` int(11) DEFAULT '0',
  `Description` varchar(60) DEFAULT NULL,
  `ExecutionMode` varchar(20) DEFAULT NULL,
  `Currency` varchar(10) DEFAULT NULL,
  `Trade` varchar(20) DEFAULT NULL,
  `BackGroundColor` varchar(20) DEFAULT NULL,
  `MarginCurrency` varchar(20) DEFAULT NULL,
  `MaxLot` int(11) DEFAULT '0',
  `DefaultSpread` varchar(45) DEFAULT NULL,
  `IsLong` tinyint(1) DEFAULT '0',
  `LimitStopLevel` int(11) DEFAULT '0',
  `FreezeLevel` int(11) DEFAULT '0',
  `SpreadBalBid` int(11) DEFAULT '0',
  `SpreadBalAsk` int(11) DEFAULT '0',
  `CreatedTime` timestamp NULL DEFAULT NULL,
  `ModifiedTime` timestamp NULL DEFAULT NULL,
  `IsDeleted` tinyint(1) DEFAULT '0',
  `Orders` varchar(60) DEFAULT NULL,
  `SecurityID` int(11) DEFAULT '0',
  `SymbolSourceID` int(11) DEFAULT '0',
  PRIMARY KEY (`SymbolID`),
  UNIQUE KEY `SymbolID_UNIQUE` (`SymbolID`),
  KEY `FK1_Security_SecurityID_idx` (`SecurityID`),
  KEY `FK1_symbol_sourceSymbolD_symbol_SymbolID_idx` (`SymbolSourceID`),
  CONSTRAINT `FK1_symbol_securityID_security_securityID` FOREIGN KEY (`SecurityID`) REFERENCES `security` (`SecurityID`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `FK1_symbol_sourceSymbolD_symbol_SymbolID` FOREIGN KEY (`SymbolSourceID`) REFERENCES `symbol` (`SymbolID`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB AUTO_INCREMENT=0 DEFAULT CHARSET=utf8;

/*Table structure for table `symbol_calculation` */

DROP TABLE IF EXISTS `symbol_calculation`;

CREATE TABLE `symbol_calculation` (
  `SymbolID` int(11) NOT NULL,
  `ContractSize` decimal(18,2) DEFAULT NULL,
  `InitialMargin` decimal(18,2) DEFAULT NULL,
  `Maintenace` decimal(18,2) DEFAULT NULL,
  `Hedged` decimal(18,2) DEFAULT NULL,
  `TickSize` decimal(18,2) DEFAULT NULL,
  `TickPrice` decimal(18,2) DEFAULT NULL,
  `MarginPercentage` decimal(18,2) DEFAULT NULL,
  `ProfitCal` varchar(200) DEFAULT NULL,
  `MarginCal` varchar(200) DEFAULT NULL,
  `IsSHMMode` tinyint(1) DEFAULT NULL,
  `ModifiedTime` timestamp NULL DEFAULT NULL,
  PRIMARY KEY (`SymbolID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Table structure for table `symbol_filteration` */

DROP TABLE IF EXISTS `symbol_filteration`;

CREATE TABLE `symbol_filteration` (
  `SymbolID` int(11) NOT NULL,
  `AutomaticLimit` decimal(4,1) DEFAULT NULL,
  `Filter` int(11) DEFAULT NULL,
  `Smoothing` int(11) DEFAULT NULL,
  `IsRealTimeDataFeed` tinyint(1) DEFAULT '0',
  `IsSaveAllIncomingPrice` tinyint(1) DEFAULT '0',
  `FiltrationLevel` int(11) DEFAULT NULL,
  `IgnoreQuot` int(11) DEFAULT NULL,
  `ModifiedTime` timestamp NULL DEFAULT NULL,
  UNIQUE KEY `SymbolID_UNIQUE` (`SymbolID`),
  KEY `SymbolID_FK3_idx` (`SymbolID`),
  CONSTRAINT `SymbolID_FK3` FOREIGN KEY (`SymbolID`) REFERENCES `symbol` (`SymbolID`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Table structure for table `symbol_session` */

DROP TABLE IF EXISTS `symbol_session`;

CREATE TABLE `symbol_session` (
  `SymbolID` int(11) DEFAULT NULL,
  `Day` varchar(10) DEFAULT NULL,
  `OpenQuote` time(4) DEFAULT NULL,
  `CloseQuote` time(4) DEFAULT NULL,
  `OpenTrade` time(4) DEFAULT NULL,
  `CloseTrade` time(4) DEFAULT NULL,
  `ModifiedTime` timestamp NULL DEFAULT NULL,
  KEY `SymbolID_FK4_idx` (`SymbolID`),
  CONSTRAINT `SymbolID_FK4` FOREIGN KEY (`SymbolID`) REFERENCES `symbol` (`SymbolID`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Table structure for table `symbol_swaps` */

DROP TABLE IF EXISTS `symbol_swaps`;

CREATE TABLE `symbol_swaps` (
  `SymbolID` int(11) NOT NULL,
  `IsEnableSwap` tinyint(1) NOT NULL DEFAULT '0',
  `Type` varchar(300) DEFAULT NULL,
  `LongPosition` decimal(18,2) NOT NULL DEFAULT '0.00',
  `ShortPosition` decimal(18,2) NOT NULL DEFAULT '0.00',
  `ThirdDaySwap` varchar(45) DEFAULT NULL,
  `IsUseOpenPrice` tinyint(1) NOT NULL DEFAULT '0',
  `IsChangeVariation` tinyint(1) NOT NULL DEFAULT '0',
  `ModifiedTime` timestamp NOT NULL,
  UNIQUE KEY `SymbolID_UNIQUE` (`SymbolID`),
  KEY `SymbolID_FK1_idx` (`SymbolID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Table structure for table `time_period` */

DROP TABLE IF EXISTS `time_period`;

CREATE TABLE `time_period` (
  `groupId` int(11) NOT NULL,
  `period` int(11) NOT NULL,
  `profitpercentage` int(11) DEFAULT NULL,
  UNIQUE KEY `CK1_` (`groupId`,`period`),
  KEY `timePeriod_groupId_FK_idx` (`groupId`),
  CONSTRAINT `timePeriod_groupId_FK` FOREIGN KEY (`groupId`) REFERENCES `group_common` (`GroupID`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

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
  `City` varchar(45) DEFAULT NULL,
  `State` varchar(45) DEFAULT NULL,
  `Country` varchar(45) DEFAULT NULL,
  `Pin` varchar(45) DEFAULT NULL,
  `IsDeleted` tinyint(1) DEFAULT '0',
  `isLogin` tinyint(1) DEFAULT '0',
  `createdDate` timestamp NULL DEFAULT NULL,
  `updatedDate` timestamp NULL DEFAULT NULL,
  `dirtyBit` int(11) NOT NULL DEFAULT '0',
  `isEnable` tinyint(1) NOT NULL DEFAULT '1',
  `InvestorPassword` varchar(20) DEFAULT NULL,
  `Phone` varchar(11) DEFAULT NULL,
  `IdNumber` int(11) DEFAULT NULL,
  `PhonePassword` varchar(20) DEFAULT NULL,
  `TraderComment` varchar(60) DEFAULT NULL,
  `Leverage` int(11) DEFAULT '0',
  `TaxRate` double DEFAULT '0',
  `AgentAccount` int(11) DEFAULT '0',
  `IsAllowChnagePassword` tinyint(1) DEFAULT '0',
  `IsEnableOTP` tinyint(1) DEFAULT '0',
  `IsReadOnly` tinyint(1) DEFAULT '0',
  `IsSendReport` tinyint(1) DEFAULT '0',
  `Status` varchar(11) DEFAULT NULL,
  `LeadSource` varchar(20) DEFAULT NULL,
  `MetaQuotesId` int(11) DEFAULT '0',
  PRIMARY KEY (`ClientID`),
  KEY `trader_Group_ID_FK_idx` (`GroupID`),
  CONSTRAINT `trader_GroupID_FK` FOREIGN KEY (`GroupID`) REFERENCES `group_common` (`GroupID`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB AUTO_INCREMENT=0 DEFAULT CHARSET=utf8;

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

/*!50003 DROP PROCEDURE IF EXISTS  `InsertAccount` */;

DELIMITER $$

/*!50003 CREATE PROCEDURE `InsertAccount`(inout Id int, in groupID int, in clientID int, in balance bigint, in prevBalance bigint, in changedDeposit bigint, in margin bigint, 
in freeMargin bigint, in closeBalance bigint,in dirtyBit int,in BOAccountID int, in FXAccountID int, in modifiedTime TIMESTAMP)
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

/* Procedure structure for procedure `InsertClosePosition` */

/*!50003 DROP PROCEDURE IF EXISTS  `InsertClosePosition` */;

DELIMITER $$

/*!50003 CREATE PROCEDURE `InsertClosePosition`(in cid int, in gid int, in aid int, boacount int, in rid varchar(50), in oid varchar(50), in orderstatus int, in etime varchar(50),
in otime varchar(50), in oprice bigint, in cprice bigint, in direction int, in amount bigint, in payout int, in assetId int)
BEGIN
INSERT INTO `closeposition`
(`ClientID`,`GroupID`,`AccountID`,`BoAccountID`,`RequestID`,`OrderID`,`OrderStatus`,`ExpiryTime`,`OpenTime`,`OpenPrice`,`ClosePrice`,`Direction`,`Amount`,`PayOut`, `AssetID`,`Created`)
VALUES
(cid, gid, aid, boacount, rid, oid, orderstatus, etime, otime, oprice, cprice, direction, amount, payout, assetId, now());

END */$$
DELIMITER ;

/* Procedure structure for procedure `insertgroup` */

/*!50003 DROP PROCEDURE IF EXISTS  `insertgroup` */;

DELIMITER $$

/*!50003 CREATE PROCEDURE `insertgroup`(in groupId int, in groupName varchar(45), in isEnable tinyint(1), in OTPStatus int, in isEnableForceOTP int, in ownerName varchar(45), in supportPage varchar(45),
in defaultDeposit double, in depositCurrency varchar(20),in defaultLeverage int, in anualInterest int, in inActivePeriod int, in maxBalance int,in isPendingDeletedOrder int, in callLevel int,in stopOutLevel int, in levelFormat int, in freeMargin int, in virtualCredit int, in isSkipHedged tinyint(1), in isHedgedMarginCalculated tinyint(1), in timeOut int(4), in newsView varchar(45), in newsLanguage varchar(45), in maxSymbol int(10), in maxOrder int(10), in tradeSignals varchar(40), in isMailSystem tinyint(1), in isSwap tinyint(1), in isEA tinyint(1), in isPendingOrderExpiry tinyint(1), in isRiskWarning tinyint(1), in createdTime timestamp, in modifiedTime timestamp, in isReport tinyint(1),
in smtpServer varchar(45), in templatePath varchar(45), in smtpLogin varchar(45), in supportEmail varchar(45), in isCopyReport tinyint(1), in signature varchar(70), in SecurityID varchar(20), in isInstruments tinyint(1), in isTrade tinyint(1), in isRequestMode tinyint(1), in executionMode varchar(45), 
in spreadDiff double, in maxDeviation double, in isFreeMargin tinyint(1),in isFastConfirmation tinyint(1),in isCloseTwoOrder tinyint(1), in isCloseAllOrder tinyint(1), in autoClose varchar(25), in minLot int(10), in maxLot int(10), in stepSize int(10), in stdCommission double , in taxCommission double, in agentCommission double, in symbolId varchar(20), in longPositionSwap double, in shortPositionSwap double, in marginPercentage double)
BEGIN
DECLARE EXIT HANDLER FOR SQLEXCEPTION
BEGIN
      ROLLBACK;
 END;
START TRANSACTION;

INSERT INTO `admdb`.`group`
(`GroupID`,`GroupName`,`IsEnable`,`OTPStatus`,`IsEnableForceOTP`,`OwnerName`,`SupportPage`,`DefaultDeposit`,`DepositCurrency`,`DefaultLeverage`,`AnualInterest`,`InActivePeriod`,`MaxBalance`,`IsPendingDeletedOrder`,`CallLevel`, `StopOutLevel`, `LevelFormat`,`FreeMargin`,`VirtualCredit`,`IsSkipHedged`,
`IsHedgedMarginCalculated`,`TimeOut`, `NewsView`, `NewsLanguage`,`MaxSymbol`,`MaxOrder`,`TradeSignals`,`IsMailSystem`,`IsSwap`,`IsEA`,`IsPendingOrderExpiry`,`IsTrailingStop`,`IsReqPricesInIE`,`IsHedgeProhibit``IsFifo`,`IsParClose`,`IsRiskWarning`,`CreatedTime`,`ModifyTime`)
VALUES (groupID,groupName,isEnable,OTPStatus,IsEnableForceOTP,OwnerName,supportPage,defaultDeposit,depositCurrency,defaultLeverage, anualInterest,inActivePeriod,maxBalance,isPendingDeletedOrder, callLevel , stopOutLevel, levelFormat, freeMargin, virtualCredit, isSkipHedged, isHedgedMarginCalulated,isReport, smtpServer, templatePath, smtpLogin, supportEmail, isCopyReport,signature,createdTime, modifyTime);


INSERT INTO `admdb`.`group_report` (`GroupID`,`IsReport`,`SmtpServer`,`TemplatePath`,`SmtpLogin`,`SupportEmail`,`IsCopyReport`,`Signature`,`ModifyTime`)
VALUES (groupID,isReport, smtpServer, templatePath, smtpLogin, supportEmail, isCopyReport,signature,modifyTime);

INSERT INTO `admdb`.`group_security`
(`GroupID`,`SecurityID`,`IsInstruments`,`IsTrade`,`IsRequestMode`,`ExecutionMode`,`SpreadDiff`,`MaxDeviation`,`IsFreeMargin`,`IsFastConfirmation`,`IsCloseTwoOrder`,`IsCloseAllOrder`,`AutoClose`,`MinLot`,`MaxLot`,`StepSize`,`StdCommission`,`TaxCommission`,`AgentCommission`,`ModifyTime`)
VALUES
(groupId,securityId,isInstruments,isTrade,isRequestMode,executionMode,spreadDiff,maxDeviation,isFreeMargin,isFastConfirmation,isCloseTwoOrder,isCloseAllOrder,autoClose,minLot,maxLot,stepSize,stdCommission,taxCommission,agentCommission,modifyTime);

INSERT INTO `admdb`.`group_symbol`
(`GroupID`,`SymbolID`,`LongPositionSwap`,`ShortPositionSwap`,`MarginPercentage`,`ModifyTime`)
VALUES
(groupId,symbolId,longPositionSwap,shortPositionSwap,marginPercentage,modifyTime);


COMMIT;
END */$$
DELIMITER ;

/* Procedure structure for procedure `InsertManager` */

/*!50003 DROP PROCEDURE IF EXISTS  `InsertManager` */;

DELIMITER $$

/*!50003 CREATE PROCEDURE `InsertManager`(inout id int,in GroupID int, in Passwords nvarchar(50), in FirstName nvarchar(50), in LastName nvarchar(50), in Email nvarchar(50), in Address nvarchar(50), in city nvarchar(50), in state nvarchar(50), in country nvarchar(50), in pin varchar(50))
BEGIN
if ifnull(id,0)=0 then 
INSERT INTO `manager`
(`GroupID`,`Passwords`,`FirstName`,`LastName`,`Email`,`Address`,`City`,`State`,`Country`,`Pin`, `isEnable`)
VALUES(GroupID,Passwords,FirstName,LastName,Email,Address, city, state, country, pin, 1);
set id = (SELECT  AUTO_INCREMENT  FROM INFORMATION_SCHEMA.TABLES WHERE TABLE_SCHEMA = DATABASE() AND TABLE_NAME = 'manager')-1;
else
INSERT INTO `manager`
(`ManagerID`,`GroupID`,`Passwords`,`FirstName`,`LastName`,`Email`,`Address`,`City`,`State`,`Country`,`Pin`, `isEnable`)
VALUES(id,GroupID,Passwords,FirstName,LastName,Email,Address, city, state, country, pin, 1);
set id = (SELECT  AUTO_INCREMENT  FROM INFORMATION_SCHEMA.TABLES WHERE TABLE_SCHEMA = DATABASE() AND TABLE_NAME = 'manager')-1;
end if;
END */$$
DELIMITER ;

/* Procedure structure for procedure `insertOpenPosition` */

/*!50003 DROP PROCEDURE IF EXISTS  `insertOpenPosition` */;

DELIMITER $$

/*!50003 CREATE PROCEDURE `insertOpenPosition`(in cid int, in gid int, in aid int, in boacount int, in rid int, in oid varchar(50), in ostatus int, in etime varchar(50), in otime varchar(50), in oprice int, 
in cprice int, in dirction int, in amount int, in payout int, in assetId int)
BEGIN
INSERT INTO `riskmanagmentdb`.`openposition`
(`ClientID`,`GroupID`,`AccountID`,`BoAccountID`,`RequestID`,`OrderID`,`OrderStatus`,`ExpiryTime`,`OpenTime`,`OpenPrice`,`ClosePrice`,
`Direction`,`Amount`,`PayOut`,AssetID)
VALUES
(cid, gid, aid, boacount, rid, oid, ostatus, etime, otime, oprice,cprice, direction, amount, payout, assetId);
END */$$
DELIMITER ;

/* Procedure structure for procedure `InsertTrader` */

/*!50003 DROP PROCEDURE IF EXISTS  `InsertTrader` */;

DELIMITER $$

/*!50003 CREATE PROCEDURE `InsertTrader`(inout ClentID int,IN GroupID int, in Passwords nvarchar(50), in FirstName nvarchar(50), in LastName nvarchar(50), in Email nvarchar(50), 
in Address nvarchar(50), in city nvarchar(50), in state nvarchar(50), in country nvarchar(50), in pin varchar(50), in dirtyBit int, in isEnable TINYINT,
in investorPassword nvarchar(20), in phone nvarchar(11), in idNumber int, in phonePassword nvarchar(20),
in traderComment nvarchar(60), in leverage int, in taxRate double, in agentAccount int, in isAllowChnagePassword TINYINT,
in isEnableOTP TINYINT, in isReadOnly TINYINT, in isSendReport TINYINT,
in statusVal nvarchar(11), in leadSource nvarchar(20), in metaQuotesId int)
BEGIN
if ifnull(ClentID,0)=0 then 
INSERT INTO `trader`
(`GroupID`,`Passwords`,`FirstName`,`LastName`,`Email`,`Address`,`City`,`State`,`Country`,`Pin`,`IsDeleted`, `isLogin`, `createdDate`, `updatedDate`, `dirtyBit`, `isEnable`,
`InvestorPassword`,`Phone`,`IdNumber`,`PhonePassword`,`TraderComment`,`Leverage`,`TaxRate`,
`AgentAccount`,`IsAllowChnagePassword`,`IsEnableOTP`,`IsReadOnly`,`IsSendReport`,`Status`, 
`LeadSource`,`MetaQuotesId`)
VALUES (GroupID,Passwords,FirstName,LastName,Email,Address, city, state,country,pin,0,0, now(), now(), dirtyBit, 1,investorPassword,
 phone,idNumber,phonePassword,traderComment,leverage,taxRate,agentAccount,
isAllowChnagePassword,isEnableOTP,isReadOnly,isSendReport, statusVal, leadSource,metaQuotesId);

set ClentID = (SELECT  AUTO_INCREMENT  FROM INFORMATION_SCHEMA.TABLES WHERE TABLE_SCHEMA = DATABASE() AND TABLE_NAME = 'trader')-1;
else 
INSERT INTO `trader`
(`GroupID`,`ClientID`,`Passwords`,`FirstName`,`LastName`,`Email`,`Address`,
`City`,`State`,`Country`,`Pin`,`IsDeleted`, `isLogin`, `createdDate`, `updatedDate`, 
`dirtyBit`, `isEnable`,`InvestorPassword`,`Phone`,`IdNumber`,`PhonePassword`,`TraderComment`,
`Leverage`,`TaxRate`,`AgentAccount`,`IsAllowChnagePassword`,`IsEnableOTP`,`IsReadOnly`,
`IsSendReport`,`Status`, `LeadSource`,`MetaQuotesId`)
VALUES (GroupID,ClentID,Passwords,FirstName,LastName,Email,Address, city, state,country,pin,0,0, now(), now(), dirtyBit, 1,investorPassword,
 phone,idNumber,phonePassword,traderComment,leverage,taxRate,agentAccount,
isAllowChnagePassword,isEnableOTP,isReadOnly,isSendReport,statusVal, leadSource,metaQuotesId);

set ClentID = (SELECT  AUTO_INCREMENT  FROM INFORMATION_SCHEMA.TABLES WHERE TABLE_SCHEMA = DATABASE() AND TABLE_NAME = 'trader')-1;
end if;
END */$$
DELIMITER ;

/* Procedure structure for procedure `insetManagerRight` */

/*!50003 DROP PROCEDURE IF EXISTS  `insetManagerRight` */;

DELIMITER $$

/*!50003 CREATE PROCEDURE `insetManagerRight`(in id int, in cuser int, in duser int, in upuser int, in vtrader int, in vtraded int, in cmanager int,
in dmanager int, in umanager int,in vmanager int, in vmanagerd int, in mmanagera int, in credit int,in balance int, in openTrades int,in boMonitoring int)
BEGIN
INSERT INTO `manager_right`
(`ManagerID`,`CreateUser`,`DeleteTrader`,`UpdateTrader`,`ViewTrader`,`ViewTraderDetail`,`CreateManager`,`DeleteManager`,`UpdateManager`,`ViewManager`,`ViewManagerDetails`,`ModifyManagerAccess`,`Credit`,`Balance`,`OpenTrades`,`BoMonitoring`)
VALUES
(id,cuser,duser,upuser,vtrader,vtraded,cmanager,dmanager,umanager,vmanager,vmanagerd,mmanagera,credit,balance,openTrades,boMonitoring);

END */$$
DELIMITER ;

/* Procedure structure for procedure `RetrieveGroup` */

/*!50003 DROP PROCEDURE IF EXISTS  `RetrieveGroup` */;

DELIMITER $$

/*!50003 CREATE PROCEDURE `RetrieveGroup`(in groupid int)
BEGIN
if groupid<>0 then 
select g.GroupID, g.IsEnable , g.GroupName , g.IsEnableOTP , g.IsEnableForceOTP, g.OwnerName, 
g.SupportedPage, g.DefaultDeposit, g.DepositCurrency, g.DefaultLeverage, g.AnnualInterst, g.CreatedTime,
 g.ModifiedTime, r.TimeOut, r.NewsView , r.NewsLanguage, r.MaxSymbol , r.MaxOrders, r.TradeSignals, 
r.IsMailSystem, r.IsSwap, r.IsEA, r.IsPendingOrderExpiry, r.IsTrailingStop, r.IsReqPriceInIE, 
r.IsHedgeProhibit, r.IsFifo, r.IsParClose, r.IsRiskWarning,
 a.InActivePeriod, a.MaxBalance, a.IsPendingDeletedOrder,
m.CallLevel as CallLevel, m.StopOutLevel , m.LevelFormat , m.FreeMargin  , m.VirtualCredit , 
m.IsHedgedMarginCalulated , m.IsSkipHedged,
p.IsReport , p.SmtpServer , p.TemplatePath , p.SmtpLogin, p.SupportEmail , p.IsCopyReport , p.Signature
from `admdb`.`group_common` as g join `admdb`.`group_report` as p on g.GroupID = p.GroupID join `admdb`.`group_archieve` as a on g.GroupID = a.GroupID join `admdb`.`group_margin`
 as m on g.GroupID = m.GroupID  join `admdb`.`group_rights` as r on g.GroupID = r.GroupID where g.GroupID = groupid;
else
select g.GroupID as GroupID,  g.GroupName as GroupName,  g.OwnerName as OwnerName,  m.CallLevel as CallLevel,
 m.StopOutLevel as StopOutLevel from `admdb`.`group_common` as g join `admdb`.`group_margin` as m 
on g.GroupID = m.GroupID where g.IsDeleted =0;
end if;
END */$$
DELIMITER ;

/* Procedure structure for procedure `RetrieveSymbol` */

/*!50003 DROP PROCEDURE IF EXISTS  `RetrieveSymbol` */;

DELIMITER $$

/*!50003 CREATE PROCEDURE `RetrieveSymbol`(in symbolid int)
BEGIN
select s.SymbolID as symbolid,  s.SymbolName as symbolname, s.ExecutionMode as executionmode, 
s.Digits as digits, s.Trade as trade, s.DefaultSpread as spread, s.LimitStopLevel 
as stops,
f.Filter as filter,
w.LongPosition as longposition , w.ShortPosition as shortposition
 from `admdb`.`symbol` as s join `admdb`.`symbol_filteration` as f 
on s.SymbolID = f.SymbolID join `admdb`.`symbol_swaps` as w on s.SymbolID = w.SymbolID where s.IsDeleted =0;

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

/*!50003 CREATE PROCEDURE `UpdateAccount`(in AcountId int, in balance bigint, in prevBalance bigint, in changeDeposit bigint, in margin bigint, 
in freeMargin bigint, in closeBalance bigint, in dirtyBit int, in modifiedTime TIMESTAMP)
BEGIN


UPDATE `account`
SET
`Balance` = balance,
`PrevBalance` = prevBalance,
`ChangedDeposit` = changeDeposit,
`Margin` = margin,
`FreeMargin` = freeMargin,
`CloseBalance`=closeBalance,
`DirtyBit`=dirtyBit,
`modifiedTime` = modifiedTime
WHERE `AccountId` = AcountId;

END */$$
DELIMITER ;

/* Procedure structure for procedure `UpdateManager` */

/*!50003 DROP PROCEDURE IF EXISTS  `UpdateManager` */;

DELIMITER $$

/*!50003 CREATE PROCEDURE `UpdateManager`(in FirstName nvarchar(50), in LastName nvarchar(50), in GroupId int, in MenagerId int, in Passwords nvarchar(50), in Email varchar(50), in Address nvarchar(50),
in city nvarchar(50), in state nvarchar(50), in country nvarchar(50), in pin varchar(50), in isEnable TINYINT)
BEGIN
UPDATE `manager`
SET
`FirstName` = FirstName,
`LastName` = LastName,
`GroupID` = GroupID,
`Passwords` = Passwords,
`Email` = Email,
`Address` = Address,
`City`= city,
`State`=state,
`Country`=country,
`Pin`=pin,
`isEnable`=isEnable
WHERE `ManagerID` = MenagerId;
END */$$
DELIMITER ;

/* Procedure structure for procedure `UpdateTrader` */

/*!50003 DROP PROCEDURE IF EXISTS  `UpdateTrader` */;

DELIMITER $$

/*!50003 CREATE PROCEDURE `UpdateTrader`(in id int, in GroupID int, in Paswd nvarchar(50), 
in fname nvarchar(50), in lname nvarchar(50), in Email nvarchar(50), in Address nvarchar(100),
in city nvarchar(50), in state nvarchar(50), in country nvarchar(50), in pin varchar(50),
 in isDelete int, in updatedDate TIMESTAMP, in dirtyBit int, in isEnable TINYINT,
in investorPassword varchar(20), in phone varchar(11), in idNumber int, in phonePassword varchar(20),
in traderComment varchar(60), in leverage int, in taxRate double, in agentAccount int, in isAllowChnagePassword TINYINT,
in isEnableOTP TINYINT, in isReadOnly TINYINT, in isSendReport TINYINT, in statusVal varchar(11), 
in leadSource varchar(20),in metaQuotesId int)
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
`isEnable`=isEnable,
`InvestorPassword`=investorPassword,
`Phone`=phone,
`IdNumber`=idNumber,
`PhonePassword`=phonePassword,
`TraderComment`=traderComment,
`Leverage`=leverage,
`TaxRate`=taxRate,
`AgentAccount`=agentAccount,
`IsAllowChnagePassword`=isAllowChnagePassword,
`IsEnableOTP`=isEnableOTP,
`IsReadOnly`=isReadOnly,
`IsSendReport`=isSendReport,
`Status`=statusVal,
`LeadSource`=leadSource,
`MetaQuotesId`=metaQuotesId

WHERE `ClientID` = id;
END */$$
DELIMITER ;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;
