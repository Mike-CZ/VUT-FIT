<?php

#DKA:xpelan03
/*
 * Soubor: determinization.php
 * Datum: 2015/03/13
 * Autor: Lukas Pelanek, xpelan03@stud.fit.vutbr.cz
 * Projekt: Determinizace konecneho automatu
 * Popis: Program nacte zdrojovy soubor obsahujici konecny automat a provede jeho determinizaci
 */

///////////////////////////////////// Algoritmy prevzaty ze 4. prednasky do predmetu IFJ /////////////////////////////////////

// odstraneni epsilon prechodu
function removeEps(&$states, &$rules, &$finalStates){
    $newRules = array();    // pole ve kterem budou ulozeny nova pravidla po odstraneni eps prechodu
    $newFinalStates = array();  // pole ve kterem budou ulozeny nove koncove stavy po odstraneni eps prehodu

    foreach($states as $state){                 // musim projit kazdym stavem a ostranit pripadne eps prechody
        // prvne vytvorim epsilon uzaver
        $newResults = array($state);
        do{
            $previousResults = $newResults;
            foreach($previousResults as $tmpState){
                foreach($rules as $rule){       // projdu vsechna pravidla, pokud existuje eps prechod z daneho stavu, vlozim koncovy stav do eps uzaveru
                    if (!strcmp($tmpState, $rule->startingState) and !strcmp($rule->inputSymbol, "eps")){
                        addEpsState($newResults, $rule->finalState);
                    }
                }
            }

        }while(array_diff($newResults, $previousResults));          // cyklus provadim do te doby, dokud mohu pridat dalsi eps prechod

        // odstraneni eps prechodu
        foreach($newResults as $epsResult){                // pro kazdy prvek z eps uzaveru
            foreach($rules as $epsRule){                    // musim projit vsechna pravidla
                if(!strcmp($epsResult, $epsRule->startingState) and strcmp($epsRule->inputSymbol, "eps")){
                    // pokud pravidlo zacina stejnym stavem jako je v eps uzaveru a neni eps, pridam jej do novych pravidel
                    $epsRuleObject = new RulesClass();
                    $epsRuleObject->startingState = $state;
                    $epsRuleObject->inputSymbol = $epsRule->inputSymbol;
                    $epsRuleObject->finalState = $epsRule->finalState;
                    checkSameRules($newRules, $epsRuleObject);    // zkontroluji zda takove pravidlo neexistuje, pripadne jej pridam
                }
            }
        }
        // pridani koncovych stavu
        foreach($newResults as $eps){
            foreach($finalStates as $final){
                if(!strcmp($eps, $final)){
                    addEpsState($newFinalStates, $state);
                }
            }
        }

    }
    // priradim nove hodnoty
    $rules = $newRules;
    $finalStates = $newFinalStates;
}

// determinizace
function determinize(&$states, &$alphabet, &$rules, &$defaultState, &$finalStates){
    removeEps($states, $rules, $finalStates);

    $newDetStates = array(array($defaultState));
    $index = 0;
    $detRules = array();
    $detStates = array();
    $detFinal = array();

    do{
        $detTmp = $newDetStates[intval($index)];
        unset($newDetStates[intval($index)]);
        $index++;
        uniteStates($detStates, $detTmp);
        foreach($alphabet as $alpha){
            $detTmpTmp = array();
            foreach($detTmp as $dTmp){
                foreach($rules as $rTmp){
                    if(!strcmp($rTmp->startingState, $dTmp) and !strcmp($rTmp->inputSymbol, $alpha)){
                        addEpsState($detTmpTmp, $rTmp->finalState);
                    }
                }
            }
            if (count($detTmpTmp)){
                addFinalRule($detRules, $alpha, $detTmp, $detTmpTmp);
            }
            if (!isIncluded($detStates, $detTmpTmp)){
                uniteStates($newDetStates, $detTmpTmp);
            }
        }
        addFinalState($detTmp, $finalStates, $detFinal);
    }while(count($newDetStates));
    uniteAll($detStates, $detFinal);
    //predani novych vysledku odkazem
    $states = $detStates;
    $rules = $detRules;
    $finalStates = $detFinal;
}

/////////////////////////// ROZSIRENI ///////////////////////////

// odstraneni neukoncujicich stavu
function removeNonEndingStates(&$states, &$alphabet, &$rules, &$finalStates){
    $nextTmpStates = $finalStates;
    do{
        $tmpStates = $nextTmpStates;
        foreach($rules as $rule){
            foreach($alphabet as $alpha){
                foreach($tmpStates as $state){
                    if (!strcmp($rule->inputSymbol, $alpha) and !strcmp($rule->finalState, $state)){
                        addEpsState($nextTmpStates, $rule->startingState);
                    }
                }
            }
        }
    }while(array_diff($nextTmpStates, $tmpStates));     // cyklus probiha do te doby, dokud nemuzu pridat dalsi stav
    $newRules = array();
    foreach($rules as $rule){
        foreach($nextTmpStates as $state){
            if(!strcmp($rule->startingState, $state)){
                foreach($nextTmpStates as $state2){
                    if(!strcmp($rule->finalState, $state2)){
                        foreach($alphabet as $al){
                            if(!strcmp($rule->inputSymbol, $al)){
                                array_push($newRules, $rule);
                            }
                        }
                    }
                }
            }

        }
    }
    $states = $nextTmpStates;
    $rules = $newRules;
}

// prevod na dobre specifikovany konecny automat
function wsa(&$states, &$alphabet, &$rules, &$defaultState, &$finalStates, $isLower){
    determinize($states, $alphabet, $rules, $defaultState, $finalStates);
    removeNonEndingStates($states, $alphabet, $rules, $finalStates);

    $qState = ($isLower)? "qfalse" : "qFALSE";                  // musim rozlisovat pokud je aktivni prepinac -i
    $newStates = $states;
    $newRules = $rules;
    array_push($newStates, $qState);

    foreach($alphabet as $alpha){
        foreach($newStates as $state) {
            // pokud neexistuje takove pravidlo, tak jej vlozim
            if(!wsaCheckRules($rules, $state, $alpha, $states)){
                $rule = new RulesClass();
                $rule->startingState = $state;
                $rule->inputSymbol = $alpha;
                $rule->finalState = $qState;
                array_push($newRules, $rule);
            }
        }
    }
    // pokud pocatecni stav patri mezi neukoncujici, novy pocatecni stav je qFALSE
    $isNonEnding = false;
    foreach($states as $state){
        if(!strcmp($state, $defaultState)){
            $isNonEnding = true;
            break;
        }
    }
    if(!$isNonEnding){
        $defaultState = $qState;
    }
    $states = $newStates;
    $rules = $newRules;
}
// funkce zjisti, zda zadany retezec je retezcem prijimanym konecnym automatem
function stringAnalysis(&$states, &$alphabet, &$rules, &$defaultState, &$finalStates, &$string, &$outFile){
    determinize($states, $alphabet, $rules, $defaultState, $finalStates);
    $actState = $defaultState;
    $found = false;
    $string = str_split($string);   // udelam ze stringu pole a budu prochazet kade pismeno zvlast

    foreach($string as $ch){        // prvne zkontroluji, zda vstupni string patri do jazyka prijimaneho konecnym automatem
        $found = false;
        foreach($alphabet as $alpha){
            if (!strcmp($ch, $alpha)){
                $found = true;
                break;
            }
        }
        if (!$found){
            exit(1);
        }
    }

    foreach($string as $ch){
        foreach($rules as $rule){
            $found = false;
            if (!strcmp($rule->startingState, $actState) and !strcmp($rule->inputSymbol, $ch)){
                $actState = $rule->finalState;  // pokud existuje pravidlo, vylezu z cyklu a kontroluji dalsi vstup
                $found = true;
                break;
            }
        }
        if(!$found) {
            if(!file_put_contents($outFile, "0")){
                exit(2);
            }
            exit(0);
        }
    }
    $isInFinal = false;
    foreach($finalStates as $finState){
        if(!strcmp($finState, $actState)){
            $isInFinal = true;
            break;
        }
    }
    if(!$isInFinal){
        if(!file_put_contents($outFile, "0")){
            exit(2);
        }
    }
    else{
        if(!file_put_contents($outFile, "1")){
            exit(2);
        }
    }
    exit(0);

}