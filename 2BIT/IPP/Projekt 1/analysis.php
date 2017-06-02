<?php

#DKA:xpelan03
/*
 * Soubor: analysis.php
 * Datum: 2015/03/13
 * Autor: Lukas Pelanek, xpelan03@stud.fit.vutbr.cz
 * Projekt: Determinizace konecneho automatu
 * Popis: Program nacte zdrojovy soubor obsahujici konecny automat a provede jeho determinizaci
 */

//trida ze ktere vytvarim objekty reprezentujici pravidla
class RulesClass{
    var $startingState;             // vychozi stav
    var $inputSymbol;               // cteny vstupni symbol
    var $finalState;                // cilovy stav
}

/* Stavy lexikalniho analyzatoru
 * 0 - pocatecni stav
 * 1 - zacatek definice stavu automatu
 * 2 - definice stavu automatu
 * 3 - identifikator stavu
 * 4 - dalsi identifikator
 * 5 - definice abecedy
 * 6 - zpracovani dvou apostrofu v rade
 * 7 - pocatecni stav
 * 8 - definice pravidel
 * 9 - definice konecnych stavu automatu
 * 10 - kontrola znaku za definici automatu
 */

function getFSM(&$states, &$alphabet, &$rules, &$defaultState, &$finalStates, &$caseInsensitive, &$file){
    $symbol = "";         // promenna do ktere ukladam jednotlive symboly, ktere zpracovavam v lexikalni analyze
    $lexState = 0;           // promenna ve ktere uchovavam stav konecneho automatu lexikalni analyzy
    $tmp = "";              // pomocna promenna do ktere ukladam nacitane znaky

    // lexikalni analyza implementovana pomoci konecneho automatu, zaroven plni funkci syntakticke a semanticke analyzy
    while(mb_strlen($file)){
        getChar($symbol, $file, $caseInsensitive);
        switch($lexState){
            case 0:
                skip($symbol, $file, $caseInsensitive);

                if(strcmp($symbol, "(")){             // definice automatu musi zacinat zavorkou, jinak syntakticka chyba
                    exit(40);
                }
                $lexState = 1;
                break;

            case 1:
                skip($symbol, $file, $caseInsensitive);

                if(strcmp($symbol, "{")){             // definice stavu automatu musi zacinat slozenou zavorkou
                    exit(40);
                }
                $lexState = 2;
                break;

            case 2:
                skip($symbol, $file, $caseInsensitive);
                if (ctype_alpha($symbol)){      // pokud narazim na pismeno, nactu string a ulozim do seznamu stavu
                    $tmp .= $symbol;
                    $lexState = 3;
                    break;
                }
                if (!strcmp($symbol, "}")) {     // pokud je mnozina stavu prazdna -> prejdu k nacitani abecedy
                    getChar($symbol, $file, $caseInsensitive);
                    skip($symbol, $file, $caseInsensitive);
                    if (strcmp($symbol, ",")) {              // pokud dalsi znak neni carka, chyba
                        exit(40);
                    }
                    getChar($symbol, $file, $caseInsensitive);
                    skip($symbol, $file, $caseInsensitive);
                    if (!strcmp($symbol, "{")) {              // pokud dalsi znak je slozena zavorka, nacitam abecedu
                        $lexState = 5;
                        break;
                    }
                }
                                  // pokud se dostaneme az sem, chyba
                exit(40);

            case 3:
                if((ctype_alnum($symbol) or !strcmp($symbol, "_"))){  // pokud je to platny znak identifikatoru, stale ukladam do pomocne promenne
                    $tmp .= $symbol;
                    break;
                }
                if(!strcmp(mb_substr($tmp, -1), "_")){         // pokud indentifikator konci na podrtrzitko -> lexikalni chyba
                    exit(40);
                }
                addState($states, $tmp);      // vlozim stav do seznamu
                skip($symbol, $file, $caseInsensitive);
                if(!strcmp($symbol, ",")){              // pokud narazim na carku, nacitam dalsi identifikator
                    $lexState = 4;
                    break;
                }
                elseif(!strcmp($symbol, "}")){          // pokud narazim na slozenou zavorku, prechazim k nacitani abecedy
                    getChar($symbol, $file, $caseInsensitive);
                    skip($symbol, $file, $caseInsensitive);
                    if(strcmp($symbol, ",")){              // pokud dalsi znak neni carka, chyba
                        exit(40);
                    }
                    getChar($symbol, $file, $caseInsensitive);
                    skip($symbol, $file, $caseInsensitive);
                    if(!strcmp($symbol, "{")){              // pokud dalsi znak je slozena zavorka, nacitam abecedu
                        $lexState = 5;
                        break;
                    }
                                            // jinak chyba
                    exit(40);
                }
                else{
                    exit(40);
                }

            case 4:
                skip($symbol, $file, $caseInsensitive);

                if (ctype_alpha($symbol)){      // pokud narazim na pismeno, nactu string a ulozim do seznamu stavu
                    $tmp .= $symbol;
                    $lexState = 3;
                    break;
                }
                exit(40);

            case 5:
                skip($symbol, $file, $caseInsensitive);
                if(empty($alphabet) and !strcmp($symbol, "}")){  //pokud prijde slozena zavorka a vstupni abeceda je prazdna -> chyba
                    exit(41);
                }
                if(strcmp($symbol, "'")){              //musi nasledovat apostrof
                    exit(40);
                }
                getChar($symbol, $file, $caseInsensitive);
                if(!strcmp($symbol, "'")){              //pokud jsem nacetl apostrof, prejdu do dalsiho stavu, kde ho zpracuji
                    $lexState = 6;
                    break;
                }
                addState($alphabet, $symbol);           // zkontroluji, zda prvek abecedy jiz neni v seznamu, pripadne vlozim
                getChar($symbol, $file, $caseInsensitive);
                if(strcmp($symbol, "'")){              //musi nasledovat apostrof
                    exit(40);
                }
                getChar($symbol, $file, $caseInsensitive);
                skip($symbol, $file, $caseInsensitive);
                if(!strcmp($symbol, ",")){              //pokud dalsi symbol je carka, nacitam dalsi prvek abecesy
                    break;
                }
                if(!strcmp($symbol, "}")){              //pokud dalsi symbol je slozena zavorka, konec abecedy
                    getChar($symbol, $file, $caseInsensitive);
                    skip($symbol, $file, $caseInsensitive);
                    if(strcmp($symbol, ",")) {              // pokud dalsi znak neni carka, chyba
                        
                        exit(40);
                    }
                    getChar($symbol, $file, $caseInsensitive);
                    skip($symbol, $file, $caseInsensitive);
                    if(!strcmp($symbol, "{")){              // pokud dalsi znak je slozena zavorka, nacitam pravidla
                        $lexState = 7;
                        break;
                    }
                    exit(40);
                }
                exit(40);
            case 6:
                if(!strcmp($symbol, "'")) {              //narazil jsem na treti apostrof
                    getChar($symbol, $file, $caseInsensitive);
                    if(!strcmp($symbol, "'")) {              //narazil jsem na ctvrty apostrof
                        $symbol .= "'";
                        addState($alphabet, $symbol);
                        getChar($symbol, $file, $caseInsensitive);
                        skip($symbol, $file, $caseInsensitive);
                        if(!strcmp($symbol, ",")){              //pokud dalsi symbol je carka, nacitam dalsi prvek abecedy
                            $lexState = 5;
                            break;
                        }
                        if(!strcmp($symbol, "}")){              //pokud dalsi symbol je slozena zavorka, konec abecedy
                            getChar($symbol, $file, $caseInsensitive);
                            skip($symbol, $file, $caseInsensitive);
                            if(strcmp($symbol, ",")) {              // pokud dalsi znak neni carka, chyba
                                exit(40);
                            }
                            getChar($symbol, $file, $caseInsensitive);
                            skip($symbol, $file, $caseInsensitive);
                            if(!strcmp($symbol, "{")){              // pokud dalsi znak je slozena zavorka, nacitam pravidla
                                $lexState = 7;
                                break;
                            }
                            exit(40);
                        }
                        exit(40);
                    }
                }
                // chyba -> prazdny retezec
                exit(40);

            case 7:
                skip($symbol, $file, $caseInsensitive);
                if (empty($rules) and !strcmp($symbol, "}")){   // pokud je mnozina pravidel prazdna, prejdu k nacitani pocatecniho stavu
                    getChar($symbol, $file, $caseInsensitive);
                    skip($symbol, $file, $caseInsensitive);
                    if(strcmp($symbol, ",")) {              // pokud dalsi znak neni carka, chyba
                        exit(40);
                    }

                    $lexState = 8;
                    break;
                }
                $object = new RulesClass();
                if(!ctype_alpha($symbol)) {              // pokud dalsi znak neni pismeno -> chyba
                    
                    exit(40);
                }
                while (ctype_alnum($symbol) or !strcmp($symbol, "_")){  //dokud je to platny znak, nacitam indentifikator
                    $tmp .= $symbol;
                    getChar($symbol, $file, $caseInsensitive);
                }
                if(!strcmp(mb_substr($tmp, -1), "_")){         // pokud indentifikator konci na podrtrzitko -> lexikalni chyba
                    
                    exit(40);
                }
                checkDeclaration($states, $tmp);      // kontrola, zda takovy stav existuje
                $object->startingState = $tmp;
                $tmp = "";

                skip($symbol, $file, $caseInsensitive);
                if(strcmp($symbol, "'")){              //musi nasledovat apostrof
                    exit(40);
                }
                getChar($symbol, $file, $caseInsensitive);
                if(!strcmp($symbol, "'")){           // pokud jsou dva apostrofy za sebou
                    getChar($symbol, $file, $caseInsensitive);
                    if(!strcmp($symbol, "'")){        // treti apostrof
                        getChar($symbol, $file, $caseInsensitive);
                        if(strcmp($symbol, "'")) {        // musi nasledovat ctvrty apostrof
                            exit(40);
                        }
                        $tmpVar = "''";
                        checkDeclaration($alphabet, $tmpVar);
                        $object->inputSymbol = $tmpVar;
                        getChar($symbol, $file, $caseInsensitive);
                    }
                    else{
                        $object->inputSymbol = "eps";     // jinak eps prechod
                    }
                }
                else{
                    checkDeclaration($alphabet, $symbol);
                    $object->inputSymbol = $symbol;
                    getChar($symbol, $file, $caseInsensitive);
                    if(strcmp($symbol, "'")){              //musi nasledovat apostrof
                        exit(40);
                    }
                    getChar($symbol, $file, $caseInsensitive);
                }
                skip($symbol, $file, $caseInsensitive);
                if(strcmp($symbol, "-")){              //musi nasledovat pomlcka
                    exit(40);
                }
                getChar($symbol, $file, $caseInsensitive);
                if(strcmp($symbol, ">")){              //musi nasledovat vetsitko
                    exit(40);
                }
                getChar($symbol, $file, $caseInsensitive);
                skip($symbol, $file, $caseInsensitive);

                if(!ctype_alpha($symbol)) {              // pokud dalsi znak neni pismeno -> chyba
                    exit(40);
                }
                while (ctype_alnum($symbol) or !strcmp($symbol, "_")){  //dokud je to platny znak, nacitam indentifikator
                    $tmp .= $symbol;
                    getChar($symbol, $file, $caseInsensitive);
                }
                if(!strcmp(mb_substr($tmp, -1), "_")){         // pokud indentifikator konci na podrtrzitko -> lexikalni chyba
                    exit(40);
                }
                checkDeclaration($states, $tmp);
                $object->finalState = $tmp;
                $tmp = "";
                checkSameRules($rules, $object);
                skip($symbol, $file, $caseInsensitive);
                if(!strcmp($symbol, ",")){              //pokud prisla carka, nacitam dalsi pravidlo
                    break;
                }
                if(!strcmp($symbol, "}")){              //pokud dalsi symbol je slozena zavorka, konec abecedy
                    getChar($symbol, $file, $caseInsensitive);
                    skip($symbol, $file, $caseInsensitive);
                    if(strcmp($symbol, ",")) {              // pokud dalsi znak neni carka, chyba
                        exit(40);
                    }
                    $lexState = 8;
                    break;
                }
                
                exit(40);

            case 8:
                skip($symbol, $file, $caseInsensitive);
                if(!ctype_alpha($symbol)) {              // pokud dalsi znak neni pismeno -> chyba
                    exit(40);
                }
                while (ctype_alnum($symbol) or !strcmp($symbol, "_")){  //dokud je to platny znak, nacitam indentifikator
                    $tmp .= $symbol;
                    getChar($symbol, $file, $caseInsensitive);
                }
                if(!strcmp(mb_substr($tmp, -1), "_")){         // pokud indentifikator konci na podrtrzitko -> lexikalni chyba
                    exit(40);
                }
                checkDeclaration($states, $tmp);
                $defaultState = $tmp;
                $tmp = "";
                skip($symbol, $file, $caseInsensitive);
                if(strcmp($symbol, ",")){              //pokud dalsi symbol neni carka, chyba
                    exit(40);
                }
                getChar($symbol, $file, $caseInsensitive);
                skip($symbol, $file, $caseInsensitive);
                if(strcmp($symbol, "{")){              //pokud dalsi symbol neni slozena zavorka, chyba
                    exit(40);
                }

                $lexState = 9;
                break;

            case 9:
                skip($symbol, $file, $caseInsensitive);
                if (empty($finalStates) and !strcmp($symbol, "}")){   // pokud je mnozina pravidel prazdna, prejdu ke kontrole znaku za definici automatu
                    getChar($symbol, $file, $caseInsensitive);
                    skip($symbol, $file, $caseInsensitive);
                    if(strcmp($symbol, ")")) {              // pokud dalsi znak neni zavorka, chyba
                        exit(40);
                    }
                    $lexState = 10;
                    break;
                }

                if(!ctype_alpha($symbol)) {              // pokud dalsi znak neni pismeno -> chyba
                    exit(40);
                }
                while (ctype_alnum($symbol) or !strcmp($symbol, "_")){  //dokud je to platny znak, nacitam indentifikator
                    $tmp .= $symbol;
                    getChar($symbol, $file, $caseInsensitive);
                }
                if(!strcmp(mb_substr($tmp, -1), "_")){         // pokud indentifikator konci na podrtrzitko -> lexikalni chyba
                    exit(40);
                }
                checkDeclaration($states, $tmp);
                addState($finalStates, $tmp);
                skip($symbol, $file, $caseInsensitive);
                if (!strcmp($symbol, ",")){             // pokud nasleduje carka, nacitam dalsi koncovy stav
                    break;
                }
                if(!strcmp($symbol, "}")){
                    getChar($symbol, $file, $caseInsensitive);
                    skip($symbol, $file, $caseInsensitive);
                    if(strcmp($symbol, ")")) {              // pokud dalsi znak neni zavorka, chyba
                        exit(40);
                    }
                    $lexState = 10;
                    break;
                }
                exit(40);

            case 10:
                // odstraneni bilych znaku a komentaru
                while(ctype_space($symbol) or !strcmp($symbol, "#")){
                    if(!strcmp($symbol, "#")) {
                        while (strcmp($symbol, "\n") and mb_strlen($file)) {    // pokud narazim na komentar, pokracuji dokud nenarazim na konec radku nebo eof
                            getChar($symbol, $file, $caseInsensitive);
                        }
                    }
                    getChar($symbol, $file, $caseInsensitive);
                }
                if(mb_strlen($file)){           // pokud po odstraneni komentaru a bilych znaku neni eof, chyba -> bordel za definici automatu
                    exit(40);
                }
        }
    }
}