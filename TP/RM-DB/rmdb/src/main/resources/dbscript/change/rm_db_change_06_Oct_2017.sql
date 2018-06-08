USE `db_trad_rm`;
DROP procedure IF EXISTS `InsertClosePosition`;

DELIMITER $$
USE `db_trad_rm`$$
CREATE DEFINER=`root`@`localhost` PROCEDURE `InsertClosePosition`(in cid int, in gid int, in aid int, boacount int, in rid varchar(50), in oid varchar(50), in orderstatus int, in etime varchar(50),
in otime varchar(50), in oprice bigint, in cprice bigint, in direction int, in amount bigint, in payout int, in assetId int)
BEGIN
INSERT INTO `closeposition`
(`ClientID`,`GroupID`,`AccountID`,`BoAccountID`,`RequestID`,`OrderID`,`OrderStatus`,`ExpiryTime`,`OpenTime`,`OpenPrice`,`ClosePrice`,`Direction`,`Amount`,`PayOut`, `AssetID`,`Created`)
VALUES
(cid, gid, aid, boacount, rid, oid, orderstatus, etime, otime, oprice, cprice, direction, amount, payout, assetId, now());

END$$

DELIMITER ;



USE `db_trad_rm`;
DROP procedure IF EXISTS `insertOpenPosition`;

DELIMITER $$
USE `db_trad_rm`$$
CREATE DEFINER=`root`@`localhost` PROCEDURE `insertOpenPosition`(in cid int, in gid int, in aid int, in boacount int, in rid varchar(50), in oid varchar(50), in ostatus int, in etime varchar(50), in otime varchar(50), in oprice bigint, 
in cprice bigint, in direction int, in amount bigint, in payout int, in assetId int)
BEGIN
INSERT INTO `openposition`
(`ClientID`,`GroupID`,`AccountID`,`BoAccountID`,`RequestID`,`OrderID`,`OrderStatus`,`ExpiryTime`,`OpenTime`,`OpenPrice`,`ClosePrice`,
`Direction`,`Amount`,`PayOut`,AssetID)
VALUES
(cid, gid, aid, boacount, rid, oid, ostatus, etime, otime, oprice,cprice, direction, amount, payout, assetId);
END$$

DELIMITER ;