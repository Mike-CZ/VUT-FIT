<?php

#DKA:xpelan03
/*
 * Soubor: functions.php
 * Datum: 2015/03/13
 * Autor: Lukas Pelanek, xpelan03@stud.fit.vutbr.cz
 * Projekt: Determinizace konecneho automatu
 * Popis: Program nacte zdrojovy soubor obsahujici konecny automat a provede jeho determinizaci
 */

// funkce preskoci vsechny bile znaky a komentare
function skip (&$varS, &$varF, &$isLower){
    while(ctype_space($varS) or !strcmp($varS, "#")){    // ignoruji vsechny  bile znaky a komentare az po dalsi platny symbol
        if(!strcmp($varS, "#")) {
            while (strcmp($varS, "\n")) {
                $varS = mb_substr($varF, 0, 1);
                $varF = mb_substr($varF, 1);
            }
        }
        $varS = mb_substr($varF, 0, 1);
        $varF = mb_substr($varF, 1);
        if ($isLower){                           // pokud je aktivni prepinac -i -> znak prevedu na maly
            $varS = mb_strtolower($varS);
        }
    }
}
// funkce prohleda seznam stavu automatu, pokud takovy stav neexistuje, prida jej do seznamu
function addState(&$varSt, &$varSy){
    $isDefined = false;
    foreach($varSt as $tmp){
        if (!strcmp($tmp, $varSy)) {
            $isDefined = true;
        }
    }
    if(!$isDefined){
        array_push($varSt, $varSy);
    }
    $varSy = "";
}
//funkce zkontroluje, zda existuje takove pravidlo, prida jej do seznamu
function checkSameRules(&$rules, RulesClass &$rule){
    foreach($rules as $tmp){
        if(!strcmp($tmp->startingState, $rule->startingState) and !strcmp($tmp->inputSymbol, $rule->inputSymbol) and !strcmp($tmp->finalState, $rule->finalState)){
            return;
        }
    }
    array_push($rules, $rule);
}
// funkce zkontroluje, zda byl stav/symbol deklarovan
function checkDeclaration(&$states, &$state){
    $isIncluded = false;
    foreach($states as $tmp){
        if (!strcmp($tmp, $state)){
            $isIncluded = true;
            break;
        }
    }
    if(!$isIncluded){
        exit(41);
    }
}
// funkce pro nacitani znaku ze vstupniho souboru
function getChar(&$symbol, &$file, &$isLower){
    $symbol = mb_substr($file, 0, 1);
    $file = mb_substr($file, 1);
    if ($isLower){                           // pokud je aktivni prepinac -i -> vsechny znaky prevedu na male
        $symbol = mb_strtolower($symbol);
    }
}
// funkce pridava stavy pri praci s eps uzaverem
function addEpsState(&$arr, &$sym){
    $isDefined = false;
    foreach($arr as $tmp){
        if (!strcmp($tmp, $sym)){
            $isDefined = true;
        }
    }
    if (!$isDefined){
        array_push($arr, $sym);
    }
}
// funkce porovnava prvky v dvojrozmernem poli, pokud v poli neexistuje takova mnozina, vlozi ji
function uniteStates(&$arr1, &$arr2){
    $isThere = false;
    foreach($arr1 as $arr1Tmp){
        if (!array_diff($arr1Tmp, $arr2) && !array_diff($arr2, $arr1Tmp)){
            $isThere = true;
        }
    }
    if(!$isThere ){
        array_push($arr1, $arr2);
    }
}
// pomocna funkce spojujici stavy v mnozine do jednoho
function myUnit(&$arr1, &$state){
    $state = $arr1[0];
    $length = count($arr1);
    for($x = 1; $x < $length; $x++){
        $state .= "_" . $arr1[intval($x)];
    }
}

// funkce spoji mnozinu stavu do jednoho stavu a vlozi do mnoziny pravidel
function addFinalRule(&$rules, &$input, &$defaultState, &$finalState){
    $isThere = false;
    $defState = "";
    $finState = "";
    sort($defaultState);
    sort($finalState);
    if (count($defaultState)){
        myUnit($defaultState, $defState);
    }
    if (count($finalState)){
        myUnit($finalState, $finState);
    }
    foreach($rules as $rule){
        if(!strcmp($rule->startingState, $defState) and !strcmp($rule->inputSymbol, $input) and !strcmp($rule->finalState, $finState)){
            $isThere = true;
        }
    }
    if (!$isThere){
        $object = new RulesClass();
        $object->startingState = $defState;
        $object->inputSymbol = $input;
        $object->finalState = $finState;
        array_push($rules, $object);
    }
}
// funkce zjisti, zda dana podmnozina je prvkem mnoziny sjodnocena s prazdnou mnozinou
function isIncluded(&$arr1, &$arr2){
    // prazdna mnozina
    if(!count($arr2)){
        return true;
    }
    foreach($arr1 as $tmp){
        if (!array_diff($tmp, $arr2) && !array_diff($arr2, $tmp)){
            return true;
        }
    }
    return false;
}
// funkce zkontroluje, zda prvek mnoziny stavu patri mezi koncove, pripadne jej vlozi do nove mnoziny koncovych stavu
function addFinalState(&$arr1, &$arr2, &$arr3){
    $isThere = false;
    foreach($arr1 as $tmpState){
        foreach($arr2 as $finalState){
            if(!strcmp($tmpState, $finalState)){
                $isThere = true;
            }
        }
    }
    // pokud patri mezi koncove stavy, vlozim stav do mnoziny koncovych stavu
    if($isThere){
        uniteStates($arr3, $arr1);
    }
}
// funkce spoji podmnoziny stavu do jednotnych stavu
function uniteAll(&$startStates, &$endStates){
    $newStartStates = array();
    $newEndStates = array();
    foreach($startStates as $tmpStart){
        sort($tmpStart);
        $tmp = "";
        myUnit($tmpStart, $tmp);
        array_push($newStartStates, $tmp);
    }
    foreach($endStates as $tmpEnd){
        sort($tmpEnd);
        $tmp = "";
        myUnit($tmpEnd, $tmp);
        array_push($newEndStates, $tmp);
    }
    $startStates = $newStartStates;
    $endStates = $newEndStates;
}
// funkce setridi vsechny prvky
function sortAll(&$states, &$alphabet, &$rules, &$finalStates){
    sort($states);
    sort($alphabet);
    sort($finalStates);
    my_sort($rules);
}

// funkce vypise automat podle pravidel
function printFSM(&$states, &$alphabet, &$rules, &$defaultState, &$finalStates, $outFile){
    $out = "";      // vypisy budu ukladat do jednoho retezce
    sortAll($states, $alphabet, $rules, $finalStates);
    $out .= "(\n{";
    $length = count($states);
    for ($x = 0; $x < $length; $x++){
        if($x < $length - 1){
            $out .= $states[intval($x)] . ", ";
        }
        else{
            $out .= $states[intval($x)];
        }
    }
    $out .= "},\n{";

    $length = count($alphabet);
    for ($x = 0; $x < $length; $x++){
        if($x < $length - 1){
            $out .= "'" . $alphabet[intval($x)] . "'" . ", ";
        }
        else{
            $out .= "'" . $alphabet[intval($x)] . "'";
        }
    }
    $out .= "},\n{\n";

    $length = count($rules);
    for ($x = 0; $x < $length; $x++){
        // pokud jde o eps pravidlo, musim pomocny string "eps" nahradit za prazdny retezec
        if($x < $length - 1){
            $out .= $rules[intval($x)]->startingState . " " . "'" . $rules[intval($x)]->inputSymbol . "' -> " . $rules[intval($x)]->finalState . ",\n" ;
        }
        else{
            $out .= $rules[intval($x)]->startingState . " " . "'" . $rules[intval($x)]->inputSymbol . "' -> " . $rules[intval($x)]->finalState . "\n" ;
        }
    }
    $out .= "},\n";

    $out .= $defaultState . ",\n{";

    $length = count($finalStates);
    for ($x = 0; $x < $length; $x++){
        if($x < $length - 1){
            $out .= $finalStates[intval($x)] . ", ";
        }
        else{
            $out .= $finalStates[intval($x)];
        }
    }
    $out .= "}\n)";

    // vytisknu automat, pokud se operace nezdarila, ukoncim script
    if(!file_put_contents($outFile, $out)){
        exit(2);
    }

}

// pomocna funkce ktera kontroluje, zda dane pravidlo patri do mnoziny pravidel
function wsaCheckRules(&$rules, &$start, &$input, &$states){
    foreach ($rules as $tmpRule) {
        foreach($states as $state){
            if (!strcmp($tmpRule->startingState, $start) and !strcmp($tmpRule->inputSymbol, $input) and !strcmp($tmpRule->finalState, $state)) {
                return true;
            }
        }

    }
    return false;
}

function printHelp(){
    echo "----Napoveda----\n".
         "Skript pro zpracovani a determinizaci konecneho automatu\n".
         "Pouziti:\n".
         "--help \t vypise napovedu\n".
         "--input=filename\t cesta ke vstupnimu souboru s konecnym automatem\n".
         "--output=filename\t cesta k vystupnimu souboru\n" .
         "-e, --no-epsilon-rules\t odstraneni epsilon pravidel\n".
         "-d, --determinization\t determinizace konecneho automatu\n".
         "-i, --case-insensitive\t nebude bran ohled na velikost znaku pri porovnavani stavu ci symbolu\n".
         "--wsfa\t prevede automat na dobre specifikovany konecny automat\n".
         "--analyze-string=\"retezec\"\t analyza, zda je retezec retezcem jazyka prijimaneho zadanym konecnym automatem.";
}
// funkce pro serazeni pravidel kvuli "eps"
function my_sort(&$rules){
    foreach($rules as $rule){
        if(!strcmp($rule->inputSymbol, "eps")){
            $rule->inputSymbol = "";
        }
    }
    sort($rules);
}

?>