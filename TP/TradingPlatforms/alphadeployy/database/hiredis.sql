GRANT ALL PRIVILEGES ON *.* TO root@"13.126.191.171" IDENTIFIED BY "root";
CREATE FUNCTION redis_hset RETURNS STRING SONAME 'libposition_management.so';
CREATE USER 'test'@'%' IDENTIFIED BY '123456';
grant select on *.* to test@"%" identified by "123456";

CREATE DATABASE IF NOT EXISTS `test`  DEFAULT CHARACTER SET utf8;

USE `test`;

