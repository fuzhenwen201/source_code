
-- this 
ALTER TABLE `db_trad_rm`.`openposition` CHANGE COLUMN `OpenPrice` `OpenPrice` BIGINT(20) NULL DEFAULT NULL  , CHANGE COLUMN `ClosePrice` `ClosePrice` BIGINT(20) NULL DEFAULT NULL  , CHANGE COLUMN `Amount` `Amount` BIGINT(20) NULL DEFAULT NULL  ;


ALTER TABLE `db_trad_rm`.`closeposition` CHANGE COLUMN `OpenPrice` `OpenPrice` BIGINT(20) NULL DEFAULT NULL  , CHANGE COLUMN `ClosePrice` `ClosePrice` BIGINT(20) UNSIGNED ZEROFILL NULL DEFAULT NULL  , CHANGE COLUMN `Amount` `Amount` BIGINT(20) NULL DEFAULT NULL  ;

ALTER TABLE `db_trad_rm`.`tb_rm_trade` CHANGE COLUMN `OpenPrice` `OpenPrice` BIGINT(20) NULL DEFAULT NULL  , CHANGE COLUMN `ClosePrice` `ClosePrice` BIGINT(20) NULL DEFAULT NULL  , CHANGE COLUMN `Amount` `Amount` BIGINT(20) NULL DEFAULT NULL  ;