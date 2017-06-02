<?php

#DKA:xpelan03
/*
 * Soubor: parameters.php
 * Datum: 2015/03/13
 * Autor: Lukas Pelanek, xpelan03@stud.fit.vutbr.cz
 * Projekt: Determinizace konecneho automatu
 * Popis: Program nacte zdrojovy soubor obsahujici konecny automat a provede jeho determinizaci
 */

function getParams(&$caseInsensitive, &$noEps, &$determinization, &$doWSA, &$str, &$inFile, &$outFile, &$argcount){
    $shortopts = "edi";
    $longopts = array("help", "input:", "output:", "wsfa", "analyze-string:", "help", "determinization", "case-insensitive", "no-epsilon-rules");
    $counter = 1;       // pocitadlo zpracovanych parametru

    foreach (getopt($shortopts, $longopts) as $opt => $value)
    {
        switch ($opt)
        {
            case "e":
                if(!$noEps){
                    $noEps = true;
                    $counter++;
                    break;
                }
                else{
                    exit(1);
                }

            case "d":
                if (!$determinization){
                    $determinization = true;
                    $counter++;
                    break;
                }
                else{
                    exit(1);
                }

            case "i":
                if (!$caseInsensitive){
                    $caseInsensitive = true;
                    $counter++;
                    break;
                }
                else{
                    exit(1);
                }

            case "input":
                if(empty($inFile)){
                    $inFile = $value;
                    $counter++;
                    break;
                }
                else{
                    exit(1);
                }

            case "output":
                if(empty($outFile)){
                    $outFile = $value;
                    $counter++;
                    break;
                }
                else{
                    exit(1);
                }

            case "wsfa":
                if(!$doWSA){
                    $doWSA = true;
                    $counter++;
                    break;
                }
                else{
                    exit(1);
                }

            case "analyze-string":
                if(empty($str)){
                    $str = $value;
                    $counter++;
                    break;
                }
                else{
                    exit(1);
                }
            case "help":
                if($argcount == 2){
                    printHelp();
                    exit(0);
                }
                else{
                    exit(1);
                }
                break;
            case "determinization":
                if (!$determinization){
                    $determinization = true;
                    $counter++;
                    break;
                }
                else{
                    exit(1);
                }
            case "no-epsilon-rules":
                if(!$noEps){
                    $noEps = true;
                    $counter++;
                    break;
                }
                else{
                    exit(1);
                }
            case "case-insensitive":
                if (!$caseInsensitive){
                    $caseInsensitive = true;
                    $counter++;
                    break;
                }
                else{
                    exit(1);
                }
        }
    }
    if(empty($inFile)){
        $inFile = "php://stdin";
    }
    if(empty($outFile)){
        $outFile = "php://stdout";
    }
    // osetreni kombinace parametru
    if(($noEps and $determinization) or ($doWSA and $noEps) or ($doWSA and $determinization) or ($doWSA and !empty($str)) or
        (!empty($str) and $determinization) or (!empty($str) and $noEps)){
        exit(1);
    }

    if ($counter != $argcount){
        // pokud se pocet zpracovanych parametru lisi od poctu zadanych parametru -> chyba
        exit(1);
    }

}