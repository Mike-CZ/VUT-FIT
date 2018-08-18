-- phpMyAdmin SQL Dump
-- version 4.5.0.2
-- http://www.phpmyadmin.net
--
-- Počítač: 127.0.0.1
-- Vytvořeno: Úte 02. kvě 2017, 23:26
-- Verze serveru: 10.0.17-MariaDB
-- Verze PHP: 5.6.14

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Databáze: `pis`
--

-- --------------------------------------------------------

--
-- Struktura tabulky `evidence_techniky`
--

CREATE TABLE `evidence_techniky` (
  `EvidID` int(10) UNSIGNED NOT NULL,
  `fkTypeID` int(10) UNSIGNED NOT NULL,
  `SerialID` varchar(50) DEFAULT NULL,
  `fkUserID_vlastnik` int(10) UNSIGNED DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Vypisuji data pro tabulku `evidence_techniky`
--

INSERT INTO `evidence_techniky` (`EvidID`, `fkTypeID`, `SerialID`, `fkUserID_vlastnik`) VALUES
(1, 1, 'abc-123', 1),
(4, 3, 'klm-456', 3),
(7, 8, '505SA-505015PS', 2),
(8, 9, '98090560-510505', NULL),
(9, 10, '5465465-1545AS', 4),
(10, 2, '811984-51618', NULL),
(12, 11, '648465-518484', NULL),
(13, 12, '8461533-31848', NULL),
(14, 13, '8465135-1484631', NULL),
(15, 14, '15486-3846846', NULL),
(16, 16, '98465-16548', NULL),
(17, 15, '45313-626294', NULL);

-- --------------------------------------------------------

--
-- Struktura tabulky `evidence_techniky_cvt`
--

CREATE TABLE `evidence_techniky_cvt` (
  `fkIDZarizeni` int(10) UNSIGNED NOT NULL,
  `fkIDMistnosti` int(10) UNSIGNED NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Vypisuji data pro tabulku `evidence_techniky_cvt`
--

INSERT INTO `evidence_techniky_cvt` (`fkIDZarizeni`, `fkIDMistnosti`) VALUES
(13, 9),
(14, 8),
(12, 7),
(15, 8),
(16, 7),
(17, 9);

-- --------------------------------------------------------

--
-- Struktura tabulky `mistnost`
--

CREATE TABLE `mistnost` (
  `MistnostID` int(10) UNSIGNED NOT NULL,
  `Nazev` varchar(10) NOT NULL,
  `jeCvt` int(10) UNSIGNED NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Vypisuji data pro tabulku `mistnost`
--

INSERT INTO `mistnost` (`MistnostID`, `Nazev`, `jeCvt`) VALUES
(1, 'A-01', 0),
(2, 'A-02', 0),
(3, 'B-01', 0),
(4, 'D-01', 0),
(7, 'C412', 1),
(8, 'O142', 1),
(9, 'B312', 1);

-- --------------------------------------------------------

--
-- Struktura tabulky `typ_techniky`
--

CREATE TABLE `typ_techniky` (
  `TypeID` int(10) UNSIGNED NOT NULL,
  `Typ_zarizeni` varchar(255) NOT NULL,
  `Nazev` varchar(100) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Vypisuji data pro tabulku `typ_techniky`
--

INSERT INTO `typ_techniky` (`TypeID`, `Typ_zarizeni`, `Nazev`) VALUES
(1, 'Elektromotor', '56-2353-A'),
(2, 'Katalyzátor', '563-5352-C'),
(3, 'Vrtačka', 'GBM-6-RE-Professional'),
(8, 'Tablet', 'Asus 50e'),
(9, 'Notebook', 'Lenovo s4000'),
(10, 'Rentgen', 'Rentgen 56S'),
(11, 'Počítačová myš', 'A4tech Bloody A60'),
(12, 'Monitor', 'Dell U2412M'),
(13, 'Pájka', 'Asist AE6P20DN'),
(14, 'Projektor', 'Optoma DH1017'),
(15, 'Klávesnice', 'Logitech Keyboard K120 CZ'),
(16, 'Repráky', 'Creative GigaWorks T40 ');

-- --------------------------------------------------------

--
-- Struktura tabulky `uzivatel`
--

CREATE TABLE `uzivatel` (
  `UserID` int(10) UNSIGNED NOT NULL,
  `Login` varchar(50) NOT NULL,
  `Heslo` varchar(60) NOT NULL,
  `Jmeno` varchar(100) NOT NULL,
  `Prijmeni` varchar(100) NOT NULL,
  `Rodne_cislo` varchar(100) NOT NULL,
  `Mesto` varchar(100) DEFAULT NULL,
  `Ulice` varchar(100) DEFAULT NULL,
  `Cislo_domu` int(10) UNSIGNED DEFAULT NULL,
  `PSC` int(10) UNSIGNED DEFAULT NULL,
  `fkKancelar` int(10) UNSIGNED NOT NULL,
  `jeAdmin` int(1) UNSIGNED NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Vypisuji data pro tabulku `uzivatel`
--

INSERT INTO `uzivatel` (`UserID`, `Login`, `Heslo`, `Jmeno`, `Prijmeni`, `Rodne_cislo`, `Mesto`, `Ulice`, `Cislo_domu`, `PSC`, `fkKancelar`, `jeAdmin`) VALUES
(1, 'xkola00', 'q1SaP5ORPAc=$tmsNQmBxmOI6vN4DfncdcA==', 'Jitka', 'Kolaříková', '915901/5437', 'Pelhřimov', 'Hrnčířská', 123, 39301, 1, 1),
(2, 'xdvor00', 'yFK+Ad33CZI=$erVh2yif6uB3PHzSAPOjvg==', 'Josef', 'Průša', '951227/9491', 'Praha', 'Táborská', 12, 10010, 3, 0),
(3, 'xhrub00', '2+LOSCLvXCE=$/tvAXZA5+FpjeH8r5v6QoA==', 'Petr', 'Hrubý', '876209/5067', 'Jihlava', 'Dlouhá', 785, 39301, 3, 0),
(4, 'xhaja00', 'hnS31x+t8N8=$0Q4KwTC/JgXVfvrBZtQ3pQ==', 'Jan', 'Hajátko', '915706/8679', 'Pelhřimov', 'Krátká', 855, 78274, 4, 0);

--
-- Spouště `uzivatel`
--
DELIMITER $$
CREATE TRIGGER `rodneCislo_TriggerInsert` BEFORE INSERT ON `uzivatel` FOR EACH ROW BEGIN
DECLARE msg VARCHAR(255);
IF NEW.Rodne_cislo NOT REGEXP '^[[:digit:]]{6}/[[:digit:]]{4}$' THEN
      set msg = "Toto neni platne rodne cislo";
      SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = msg;
END IF; 
END
$$
DELIMITER ;
DELIMITER $$
CREATE TRIGGER `rodneCislo_TriggerUpdate` BEFORE UPDATE ON `uzivatel` FOR EACH ROW BEGIN
DECLARE msg VARCHAR(255);
IF NEW.Rodne_cislo NOT REGEXP '^[[:digit:]]{6}/[[:digit:]]{4}$' THEN
      set msg = "Toto neni platne rodne cislo";
      SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = msg;
END IF; 
END
$$
DELIMITER ;

--
-- Klíče pro exportované tabulky
--

--
-- Klíče pro tabulku `evidence_techniky`
--
ALTER TABLE `evidence_techniky`
  ADD PRIMARY KEY (`EvidID`),
  ADD KEY `fkey_TypeID` (`fkTypeID`) USING BTREE,
  ADD KEY `fkey_UserIDVlastnik` (`fkUserID_vlastnik`) USING BTREE;

--
-- Klíče pro tabulku `evidence_techniky_cvt`
--
ALTER TABLE `evidence_techniky_cvt`
  ADD KEY `fkey_IDZarizeni` (`fkIDZarizeni`) USING BTREE,
  ADD KEY `fkey_IDMistnosti` (`fkIDMistnosti`) USING BTREE;

--
-- Klíče pro tabulku `mistnost`
--
ALTER TABLE `mistnost`
  ADD PRIMARY KEY (`MistnostID`);

--
-- Klíče pro tabulku `typ_techniky`
--
ALTER TABLE `typ_techniky`
  ADD PRIMARY KEY (`TypeID`);

--
-- Klíče pro tabulku `uzivatel`
--
ALTER TABLE `uzivatel`
  ADD PRIMARY KEY (`UserID`),
  ADD UNIQUE KEY `uniq_login` (`Login`) USING BTREE,
  ADD UNIQUE KEY `uniq_rodneCislo` (`Rodne_cislo`) USING BTREE,
  ADD KEY `fkey_Kancelar` (`fkKancelar`) USING BTREE;

--
-- AUTO_INCREMENT pro tabulky
--

--
-- AUTO_INCREMENT pro tabulku `evidence_techniky`
--
ALTER TABLE `evidence_techniky`
  MODIFY `EvidID` int(10) UNSIGNED NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=18;
--
-- AUTO_INCREMENT pro tabulku `mistnost`
--
ALTER TABLE `mistnost`
  MODIFY `MistnostID` int(10) UNSIGNED NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=10;
--
-- AUTO_INCREMENT pro tabulku `typ_techniky`
--
ALTER TABLE `typ_techniky`
  MODIFY `TypeID` int(10) UNSIGNED NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=17;
--
-- AUTO_INCREMENT pro tabulku `uzivatel`
--
ALTER TABLE `uzivatel`
  MODIFY `UserID` int(10) UNSIGNED NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=21;
--
-- Omezení pro exportované tabulky
--

--
-- Omezení pro tabulku `evidence_techniky`
--
ALTER TABLE `evidence_techniky`
  ADD CONSTRAINT `evidence_techniky_ibfk_1` FOREIGN KEY (`fkTypeID`) REFERENCES `typ_techniky` (`TypeID`) ON DELETE CASCADE ON UPDATE CASCADE,
  ADD CONSTRAINT `evidence_techniky_ibfk_2` FOREIGN KEY (`fkUserID_vlastnik`) REFERENCES `uzivatel` (`UserID`) ON DELETE CASCADE ON UPDATE CASCADE;

--
-- Omezení pro tabulku `evidence_techniky_cvt`
--
ALTER TABLE `evidence_techniky_cvt`
  ADD CONSTRAINT `evidence_techniky_cvt_ibfk_1` FOREIGN KEY (`fkIDMistnosti`) REFERENCES `mistnost` (`MistnostID`) ON DELETE CASCADE ON UPDATE CASCADE,
  ADD CONSTRAINT `evidence_techniky_cvt_ibfk_3` FOREIGN KEY (`fkIDZarizeni`) REFERENCES `evidence_techniky` (`EvidID`) ON DELETE CASCADE ON UPDATE CASCADE;

--
-- Omezení pro tabulku `uzivatel`
--
ALTER TABLE `uzivatel`
  ADD CONSTRAINT `uzivatel_ibfk_1` FOREIGN KEY (`fkKancelar`) REFERENCES `mistnost` (`MistnostID`) ON UPDATE CASCADE;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
