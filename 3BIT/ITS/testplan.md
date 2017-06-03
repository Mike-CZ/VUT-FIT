## Identifikátor testovacího plánu:

xpelan03  

## Úvod:

Tento testovací plán (master test plan) vznikl jako školní projekt do předmětu ITS v akademickém roce 2015/2016 na Fakultě informačních technologíí VUT v Brně.

Cílem testování je webové grafické uživatelské rozhraní systému freeIPA, jenž je integrované bezpečnostní řešení pro správu informací. Testovací plán je zaměřen na stránku s profilem uživatele a její funkcionalitu.

## Reference:

Seznam souvisejících dokumentů:
* IEEE Standard for Software Test Documentation, ANSI/IEEE Std. 829-1983 -- norma pro softwarovou i systémovou dokumentaci testů
* [freeIPA](http://www.freeipa.org/page/Main_Page) -- projekt freeIPA
* [Selenium](http://www.seleniumhq.org/) -- testovací nástroj Selenium

## Předmět testování:

Stránka s profilem uživatele a její funkcionalita.

## Testované vlastnosti:

Testovány budou následující vlastnosti:

* Navigace
  * Ovládání myší a klávesnicí
  * Přístup k prvkům pomocí klávesy TAB
  * Chování tlačítek vpřed/zpět
* Povinná data
  * Ověření, že povinná data jsou vyžadována
* Typ dat
  * Oveření, že do formulářů vyžadující datum, číslo aj. lze vložit pouze validní data
* Integrita dat
  * Ověření, že vložená data se správně uložila
* Krajní hodnoty
  * Oveření vložení dlouhé sekvence znaků do formuláře
  * Ověření vložení velkého čísla do formuláře, kde se očekává číselný typ
  * Ověření vložení záporného čísla do foruláře, kde se očekává kladné číslo

## Vlastnosti, které nejsou předmetem testování:

Testovat se nebudou zejména vizuální vlastnosti (způsob zobrazení, pozice formulářů, ...), jelikož takové testy nelze zahrnout mezi automatické testy.

## Přístup:

Testování bude probíhat automaticky pomocí nástroje Selenium. Bude spuštěna sada několika testovacích případů, které se automaticky vyhodnotí. Testování proběhne metodou bílé skříňky (white box), jelikož projekt freeIPA je projekt s otevřeným zdrojovým kódem, a tak nám je implementace známa.

## Kritéria selhání/úspěchu:

Celkové hodnocení se odvíjí od jednotlivých testovacích případů. Kritériem pro úspěch je, že všechny testovací případy budou úspěšné.

## Kritéria na pozastavení a požadavky na obnovení:

Testování bude pozastaveno pouze v případě, že testovaná webová stránka bude nedostupná. Pokud tento případ nastane, testování bude obnoveno jakmile webová stránka bude opět dostupná.

## Dokumenty:

Seznam dokumentů, které náleží k testovacímu plánu:
* Master Test Plan (tento dokument)
* Příloha 1 - Přehled ukázkových testovacích případů (na konci tohoto dokumentu)

## Testovací prostředí:

Projekt freeIPA je spuštěn na serveru, který běží na platformě OpenStack, jenž provozuje firma RedHat. To vše běží na poslední stabilní linuxové distribuci Fedora. Název serveru je *ipa.demo1.freeipa.org*.

## Časový plán:

* Návrh testovacího plánu a testovacích případů proběhne od 29. 3. 2016 do 11. 4. 2016.
* Implementace testů proběhne v době od 11. 4. 2016 do 1. 5. 2016.
* Samotné testování bude provedeno v období mezi 1. 5. 2016 až 9. 5. 2016.

## Požadavky na personál a vzdělání:

* Všeobecné znalosti testování software
* Znalost systému freeIPA
* Znalost nástroje Selenium

## Zodpovědnost:

* Lukáš Pelánek -- vypracování testovacího plánu, testovacích případů a implementace testů
* Ing. Aleš Smrčka, Ph.D. -- kontrola testovacího plánu a spuštění testů

## Rizika:

* Webová stránka bude nedostupná. V takovém případě bude testování odloženo.
* Nefunkční testovací nástroj. Následuje odhalení příčiny, případně manuální testování.

## Schválení:

Testovací plán musí odsouhlasit a schválit Ing. Aleš Smrčka, Ph.D.

# Příloha 1 - Přehled ukázkových testovacích případů

* T1 - Ověření povinných dat:
  * klik dole pole "Last name"
  * vyprázdnění formuláře
  * klik na tlačítko "Save"
  * ověření, že položka "Last name" je vyžadována
* T2 - Ověření formuláře vyžadující číselnou hodnotu
  * klik do pole "UID"
  * vyplnění formuláře daty: 123abc456
  * klik na tlačítko "Save"
  * ověření, že položka "UID" nebyla změněna
* T3 - Ověření vložení dat
  * klik do pole "First name"
  * vyplnění formuláře daty: abc
  * klik do pole "Last name"
  * vyplnění formuláře daty: bcd
  * klik do pole "Full name"
  * vyplnění formuláře daty: cde
  * klik na tlačítko "Save"
  * ověření, že data byla uložena
* T4 - Ověření vložení dlouhé sekvence znaků
  * klik do pole "Last name"
  * vyplnění formuláře daty: Administratorbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbdddddddddddddddddddddddddddddddddddddddddddddddgsdgsdgddddddddddddddddddddddddddsggdfgdfg
  * klik na tlačítko "Save"
  * ověření vložených dat
* T5 - Ověření vložení velkého čísla
  * klik do pole "UID"
  * vyplnění formuláře daty: 11200000001
  * klik na tlačítko "Save"
  * ověření vložených dat