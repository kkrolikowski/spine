-- MySQL dump 10.13  Distrib 5.6.24, for Win64 (x86_64)
--
-- Host: demo-server-ubuntu    Database: spine
-- ------------------------------------------------------
-- Server version	5.5.47-0ubuntu0.14.04.1-log

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `configver`
--

DROP TABLE IF EXISTS `configver`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `configver` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `scope` varchar(45) DEFAULT NULL,
  `version` int(11) DEFAULT NULL,
  `systemid` varchar(45) DEFAULT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `id_UNIQUE` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `log_host`
--

DROP TABLE IF EXISTS `log_host`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `log_host` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `category` varchar(64) DEFAULT NULL,
  `state` varchar(16) DEFAULT NULL,
  `timestamp` bigint(11) DEFAULT NULL,
  `serverid` int(10) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `netif_stats`
--

DROP TABLE IF EXISTS `netif_stats`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `netif_stats` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `in` bigint(20) DEFAULT NULL,
  `out` bigint(20) DEFAULT NULL,
  `time_stamp` bigint(20) DEFAULT NULL,
  `system_id` int(11) DEFAULT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `id_UNIQUE` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `service_checks`
--

DROP TABLE IF EXISTS `service_checks`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `service_checks` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `service` varchar(45) DEFAULT NULL,
  `status` varchar(45) DEFAULT NULL,
  `host_id` int(11) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `settings_smtp`
--

DROP TABLE IF EXISTS `settings_smtp`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `settings_smtp` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `host` varchar(45) DEFAULT 'localhost',
  `port` int(11) DEFAULT '25',
  `login` varchar(45) DEFAULT 'none',
  `password` varchar(45) DEFAULT 'none',
  `ssl` int(11) DEFAULT '0',
  `spine_from` varchar(45) DEFAULT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `id_UNIQUE` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `sysinfo`
--

DROP TABLE IF EXISTS `sysinfo`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `sysinfo` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `ip` varchar(45) DEFAULT NULL,
  `ext_ip` varchar(45) DEFAULT NULL,
  `hostname` varchar(45) DEFAULT NULL,
  `distro` varchar(45) DEFAULT NULL,
  `uptime` int(11) DEFAULT NULL,
  `hdd_total` bigint(11) DEFAULT NULL,
  `hdd_free` bigint(11) DEFAULT NULL,
  `ram_total` bigint(11) DEFAULT NULL,
  `ram_free` bigint(11) DEFAULT NULL,
  `system_id` varchar(45) DEFAULT NULL,
  `seen` bigint(11) DEFAULT NULL,
  `host_status` varchar(1) DEFAULT NULL,
  `cpu_usage` int(11) DEFAULT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `id_UNIQUE` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `sysusers`
--

DROP TABLE IF EXISTS `sysusers`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `sysusers` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `login` varchar(45) DEFAULT NULL,
  `pass` varchar(256) DEFAULT NULL,
  `fullname` varchar(45) DEFAULT NULL,
  `email` varchar(45) DEFAULT NULL,
  `system_id` int(11) DEFAULT NULL,
  `gecos` varchar(45) DEFAULT NULL,
  `uid` int(11) DEFAULT '0',
  `gid` int(11) DEFAULT '0',
  `active` int(1) DEFAULT '1',
  `expiration` varchar(45) DEFAULT 'Never',
  `shell` int(1) DEFAULT '0',
  `sshkeys` int(1) DEFAULT '0',
  `status` varchar(1) DEFAULT NULL,
  `sudo` varchar(1) DEFAULT '0',
  PRIMARY KEY (`id`),
  UNIQUE KEY `id_UNIQUE` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `sysusers_sshkeys`
--

DROP TABLE IF EXISTS `sysusers_sshkeys`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `sysusers_sshkeys` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `sshkey` varchar(512) DEFAULT NULL,
  `user_id` int(11) DEFAULT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `id_UNIQUE` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `users`
--

DROP TABLE IF EXISTS `users`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `users` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `login` varchar(45) DEFAULT NULL,
  `pass` varchar(256) DEFAULT NULL,
  `fullname` varchar(45) DEFAULT NULL,
  `email` varchar(45) DEFAULT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `id_UNIQUE` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `www`
--

DROP TABLE IF EXISTS `www`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `www` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `ServerName` varchar(128) DEFAULT NULL,
  `ServerAlias` text,
  `DocumentRoot` varchar(256) DEFAULT NULL,
  `htaccess` text,
  `user_id` int(11) DEFAULT NULL,
  `system_id` int(11) DEFAULT NULL,
  `status` varchar(1) DEFAULT NULL,
  `access_order` varchar(45) DEFAULT NULL,
  `htpasswd` int(11) DEFAULT NULL,
  `purgedir` varchar(1) DEFAULT 'N',
  PRIMARY KEY (`id`),
  UNIQUE KEY `id_UNIQUE` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `www_access`
--

DROP TABLE IF EXISTS `www_access`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `www_access` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `fromhost` varchar(45) DEFAULT NULL,
  `access_permission` int(11) DEFAULT NULL,
  `vhost_id` varchar(45) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `www_opts`
--

DROP TABLE IF EXISTS `www_opts`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `www_opts` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `vhostopt` varchar(45) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=5 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `www_opts_selected`
--

DROP TABLE IF EXISTS `www_opts_selected`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `www_opts_selected` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `vhost_id` int(11) DEFAULT NULL,
  `opt_id` int(11) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `www_users`
--

DROP TABLE IF EXISTS `www_users`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `www_users` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `login` varchar(45) DEFAULT NULL,
  `password` varchar(128) DEFAULT NULL,
  `system_id` int(11) DEFAULT NULL,
  `status` varchar(1) DEFAULT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `id_UNIQUE` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `www_users_access`
--

DROP TABLE IF EXISTS `www_users_access`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `www_users_access` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `user_id` int(11) DEFAULT NULL,
  `vhost_id` int(11) DEFAULT NULL,
  `server_id` int(11) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2017-04-04 21:53:11
