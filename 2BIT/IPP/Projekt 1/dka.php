#!/usr/bin/php
<?php
#DKA:xpelan03
/*
 * Soubor: dka.php
 * Datum: 2015/03/13
 * Autor: Lukas Pelanek, xpelan03@stud.fit.vutbr.cz
 * Projekt: Determinizace konecneho automatu
 * Popis: Program nacte zdrojovy soubor obsahujici konecny automat a provede jeho determinizaci
 */

include('functions.php');
include('analysis.php');
include('determinization.php');
include('parameters.php');
mb_internal_encoding("UTF-8");

$states = array();      // pole ve kterem jsou ulozeny stavy automatu
$alphabet = array();    // vstupni abeceda
$rules = array();       // mnozina pravidel
$defaultState = "";      // startovaci stav
$finalStates = array();  // pole ve kterem jsou ulozeny koncove stavy
///////////////////////////////////////////////////////////////////////////

$caseInsensitive = false;  // pokud je aktini prepinac -i, vsechny nactene znaky prevedu na male
$noEps = false;            // prepinac pro pouhe odstraneni epsilon prechodu
$determinization = false;   // prepinac pro provedeni determinizace
$doWSA = false;             // prepinac pro prevod automatu na dobre specifikovanz konecny automat [rozsireni]
$string = "";
////////////////////////////////////////////////////////////////////////////
$outFile = "";
$inFile = "";
getParams($caseInsensitive, $noEps, $determinization, $doWSA, $string, $inFile, $outFile, $argc);   // zpracovani vstupnich parametru
$file = file_get_contents($inFile, "r") or exit(2);         // pokud se nepodari otevrit soubor, skoncim s prislusnym chybovym kodem

// zpracovani vstupu
getFSM($states, $alphabet, $rules, $defaultState, $finalStates, $caseInsensitive, $file);

//vykonani prislusne akce na zaklade parametru
if($noEps){
    removeEps($states, $rules, $finalStates);
}
elseif($determinization){
    determinize($states, $alphabet, $rules, $defaultState, $finalStates);
}
elseif($doWSA){
    wsa($states, $alphabet, $rules, $defaultState, $finalStates, $caseInsensitive);
}
elseif(!empty($string)){
    stringAnalysis($states, $alphabet, $rules, $defaultState, $finalStates, $string, $outFile);
}

printFSM($states, $alphabet, $rules, $defaultState, $finalStates, $outFile);