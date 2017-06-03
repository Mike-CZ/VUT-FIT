-- phpMyAdmin SQL Dump
-- version 3.5.8.2
-- http://www.phpmyadmin.net
--
-- Počítač: wm89.wedos.net:3306
-- Vygenerováno: Ned 13. pro 2015, 17:59
-- Verze serveru: 5.6.23
-- Verze PHP: 5.4.23

SET SQL_MODE="NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;

--
-- Databáze: `d110166_itu`
--

-- --------------------------------------------------------

--
-- Struktura tabulky `otazky`
--

CREATE TABLE IF NOT EXISTS `otazky` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `otazka` varchar(255) CHARACTER SET utf8 NOT NULL,
  `odpoved1` varchar(255) CHARACTER SET utf8 NOT NULL,
  `odpoved2` varchar(255) CHARACTER SET utf8 NOT NULL,
  `odpoved3` varchar(255) CHARACTER SET utf8 NOT NULL,
  `odpoved4` varchar(255) CHARACTER SET utf8 NOT NULL,
  `obrazek` varchar(255) CHARACTER SET utf8 NOT NULL,
  `obtiznost` tinyint(4) NOT NULL,
  `spravna_odpoved` tinyint(4) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB  DEFAULT CHARSET=latin1 AUTO_INCREMENT=47 ;

-- --------------------------------------------------------

--
-- Struktura tabulky `score`
--

CREATE TABLE IF NOT EXISTS `score` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `login` varchar(255) NOT NULL,
  `score` int(11) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB  DEFAULT CHARSET=latin1 AUTO_INCREMENT=15 ;

-- --------------------------------------------------------

--
-- Struktura tabulky `user`
--

CREATE TABLE IF NOT EXISTS `user` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `login` varchar(255) CHARACTER SET utf8 NOT NULL,
  `password` varchar(255) CHARACTER SET utf8 NOT NULL,
  `hash_id` varchar(255) CHARACTER SET utf8 NOT NULL,
  `ip_address` varchar(100) CHARACTER SET utf8 NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB  DEFAULT CHARSET=latin1 AUTO_INCREMENT=13 ;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
