-- phpMyAdmin SQL Dump
-- version 3.5.8.2
-- http://www.phpmyadmin.net
--
-- Počítač: wm89.wedos.net:3306
-- Vygenerováno: Pon 07. pro 2015, 21:28
-- Verze serveru: 5.6.23
-- Verze PHP: 5.4.23

SET SQL_MODE="NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;

--
-- Databáze: `d110166_letiste`
--

-- --------------------------------------------------------

--
-- Struktura tabulky `gate`
--

CREATE TABLE IF NOT EXISTS `gate` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `nazev` varchar(255) CHARACTER SET utf8 NOT NULL,
  `terminal_id` int(11) unsigned NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB  DEFAULT CHARSET=latin1 AUTO_INCREMENT=16 ;

--
-- Vypisuji data pro tabulku `gate`
--

INSERT INTO `gate` (`id`, `nazev`, `terminal_id`) VALUES
(1, 'G1', 1),
(2, 'G2', 1),
(3, 'G3', 1),
(4, 'G4', 1),
(5, 'G5', 1),
(6, 'G1', 2),
(7, 'G2', 2),
(8, 'G3', 2),
(9, 'G4', 2),
(10, 'G5', 2),
(11, 'G1', 3),
(12, 'G2', 3),
(13, 'G3', 3),
(14, 'G4', 3),
(15, 'G5', 3);

-- --------------------------------------------------------

--
-- Struktura tabulky `let`
--

CREATE TABLE IF NOT EXISTS `let` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `cil` varchar(255) NOT NULL,
  `odlet` date NOT NULL,
  `cas_odletu` varchar(100) NOT NULL,
  `cena` varchar(100) NOT NULL,
  `doba_letu` int(11) NOT NULL,
  `gate_id` int(11) NOT NULL,
  `letadlo_id` int(11) NOT NULL,
  `vydanych_vstupenek` int(11) NOT NULL DEFAULT '1',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 AUTO_INCREMENT=5 ;

--
-- Vypisuji data pro tabulku `let`
--

INSERT INTO `let` (`id`, `cil`, `odlet`, `cas_odletu`, `cena`, `doba_letu`, `gate_id`, `letadlo_id`, `vydanych_vstupenek`) VALUES
(1, 'Praha', '2015-12-17', '09:30', '10000', 120, 1, 5, 1),
(2, 'Londýn', '2015-12-11', '09:30', '50000', 300, 3, 3, 1),
(3, 'Moskva', '2015-12-16', '13:00', '15000', 380, 15, 4, 1),
(4, 'Bratislava', '2015-12-01', '13:00', '5555', 60, 7, 5, 1);

-- --------------------------------------------------------

--
-- Struktura tabulky `letadlo`
--

CREATE TABLE IF NOT EXISTS `letadlo` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `revize` date NOT NULL,
  `vyroba` date NOT NULL,
  `pocet_clenu` int(5) NOT NULL,
  `vyrobce` varchar(100) NOT NULL,
  `typ_id` int(11) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB  DEFAULT CHARSET=latin1 AUTO_INCREMENT=6 ;

--
-- Vypisuji data pro tabulku `letadlo`
--

INSERT INTO `letadlo` (`id`, `revize`, `vyroba`, `pocet_clenu`, `vyrobce`, `typ_id`) VALUES
(1, '2015-09-02', '2013-06-07', 5, 'Boeing', 1),
(2, '2015-12-01', '2012-11-08', 5, 'Boeing', 4),
(3, '2015-11-04', '2013-07-12', 6, 'Boeing', 5),
(4, '2015-12-11', '2012-06-06', 7, 'Airbus', 2),
(5, '2015-12-11', '2008-11-05', 5, 'Convair', 3);

-- --------------------------------------------------------

--
-- Struktura tabulky `letadlo_typ`
--

CREATE TABLE IF NOT EXISTS `letadlo_typ` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `nazev` varchar(255) CHARACTER SET utf8 NOT NULL,
  `pocet_sedadel` int(10) NOT NULL,
  `typ_rozmisteni` tinyint(4) unsigned NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB  DEFAULT CHARSET=latin1 AUTO_INCREMENT=6 ;

--
-- Vypisuji data pro tabulku `letadlo_typ`
--

INSERT INTO `letadlo_typ` (`id`, `nazev`, `pocet_sedadel`, `typ_rozmisteni`) VALUES
(1, '737', 300, 3),
(2, 'A320', 250, 1),
(3, 'CV-580', 350, 4),
(4, '757', 150, 1),
(5, '767', 180, 2);

-- --------------------------------------------------------

--
-- Struktura tabulky `letenka`
--

CREATE TABLE IF NOT EXISTS `letenka` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `misto` int(11) unsigned NOT NULL,
  `trida` tinyint(4) unsigned NOT NULL,
  `cena` varchar(200) NOT NULL,
  `cestujici` int(11) unsigned NOT NULL,
  `let` int(11) unsigned NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 AUTO_INCREMENT=2 ;

--
-- Vypisuji data pro tabulku `letenka`
--

INSERT INTO `letenka` (`id`, `misto`, `trida`, `cena`, `cestujici`, `let`) VALUES
(1, 80, 1, '17500', 1, 3);

-- --------------------------------------------------------

--
-- Struktura tabulky `terminal`
--

CREATE TABLE IF NOT EXISTS `terminal` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `nazev` varchar(255) CHARACTER SET utf8 NOT NULL,
  `kapacita` int(11) NOT NULL,
  `parkovacich_mist` int(11) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB  DEFAULT CHARSET=latin1 AUTO_INCREMENT=4 ;

--
-- Vypisuji data pro tabulku `terminal`
--

INSERT INTO `terminal` (`id`, `nazev`, `kapacita`, `parkovacich_mist`) VALUES
(1, 'T1', 500, 100),
(2, 'T2', 200, 50),
(3, 'T3', 400, 200);

-- --------------------------------------------------------

--
-- Struktura tabulky `typ_gate`
--

CREATE TABLE IF NOT EXISTS `typ_gate` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `gate_id` int(11) NOT NULL,
  `typ_letadla_id` int(11) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 AUTO_INCREMENT=17 ;

--
-- Vypisuji data pro tabulku `typ_gate`
--

INSERT INTO `typ_gate` (`id`, `gate_id`, `typ_letadla_id`) VALUES
(1, 6, 1),
(2, 11, 4),
(3, 6, 5),
(4, 2, 5),
(5, 7, 3),
(6, 12, 2),
(7, 3, 5),
(8, 8, 5),
(9, 13, 5),
(10, 4, 2),
(11, 9, 2),
(12, 14, 3),
(13, 5, 5),
(14, 10, 5),
(15, 15, 2),
(16, 1, 3);

-- --------------------------------------------------------

--
-- Struktura tabulky `user`
--

CREATE TABLE IF NOT EXISTS `user` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `login` varchar(255) CHARACTER SET utf8 NOT NULL,
  `password` varchar(255) CHARACTER SET utf8 NOT NULL,
  `jmeno` varchar(255) CHARACTER SET utf8 NOT NULL,
  `rodne_cislo` varchar(255) CHARACTER SET utf8 NOT NULL,
  `narozen` date NOT NULL,
  `hash_id` varchar(255) CHARACTER SET utf8 NOT NULL,
  `ip_address` varchar(100) CHARACTER SET utf8 NOT NULL,
  `datum_registrace` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB  DEFAULT CHARSET=latin1 AUTO_INCREMENT=2 ;

--
-- Vypisuji data pro tabulku `user`
--

INSERT INTO `user` (`id`, `login`, `password`, `jmeno`, `rodne_cislo`, `narozen`, `hash_id`, `ip_address`, `datum_registrace`) VALUES
(1, 'uzivatel', 'ae6fe677d7842de43bbce44e0550c059', 'Test1 Test2', '9212010016', '1992-12-01', '7de3f285226a8d5d9170ea90687faf3c', '147.229.93.38', '2015-12-07 17:56:24');

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
