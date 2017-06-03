/*
* Soubor:  xpelan03.pl
* Datum:   2017/4/29
* Autor:   Bc. Lukas Pelanek, xpelan03@stud.fit.vutbr.cz
* Projekt: Nedeterministicky turinguv stroj, projekt c. 2 pro predmet FLP
* Popis:   Simulator nedeterministickeho Turingova stroje
*/

/*
 * Funkce pro cteni vstupu
 * Prevzato z cviceni FLP
*/
read_line(L,C) :-
     get_char(C),
     (isEOFEOL(C), L = [], !;
          read_line(LL,_),% atom_codes(C,[Cd]),
          [C|LL] = L).

%Tests if character is EOF or LF.
isEOFEOL(C) :-
     C == end_of_file;
     (char_code(C,Code), Code==10).

read_lines(Ls) :-
     read_line(L,C),
     ( C == end_of_file, Ls = [] ;
       read_lines(LLs), Ls = [L|LLs]
     ).

%%%%%%%%%%%%%%% Konec ctecich funkci %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


% funkce pro vypis radku
writeLine([]) :- write('\n').
writeLine([Item|NextItems]) :- write(Item),writeLine(NextItems).

writeLines([]).
writeLines([Item|NextItems]) :- writeLine(Item),writeLines(NextItems).

% odstrani posledni radek
removeLastLine(L1, L2):-
    append(L2, [_], L1).

% funkce vrati nasledujici symbol
getSymbol([], ' ').
getSymbol([H|_], H).

% ziska aktualni stav a znak pod hlavou
getConfiguration([UnderHead|Tape], State, Symbol) :-
     % test, zda se jedna o stav 
     char_type(UnderHead, upper),
          (
               % nacteme symbol a vratime aktualni stav
               getSymbol(Tape, Symbol),
               State = UnderHead
          )
     ;
     % jinak hledame dale
     getConfiguration(Tape, State, Symbol)
.

% funkce nacte prechodava pravidla ze souboru
getRulesFromFile([],[]).
getRulesFromFile([H|T], Res) :-
     [Start, _, Symbol, _, NextState, _, Action] = H,
     (
          Rule = [Start, Symbol, NextState, Action],
          getRulesFromFile(T, Res1),
          Res = [Rule|Res1]
     )
.

% funkce vrati nasledujici stav a akci na pasce
getRule([Rule|List], State, Symbol, NextState, Action) :-
     [From, Read, To, Do] = Rule,
          (
               % hledane pravidlo
               From == State, Read == Symbol, 
               (
                    % predame nasledujici stav a akci
                    NextState = To,
                    Action = Do
               )
               ;
               getRule(List, State, Symbol, NextState, Action)
          )
.

% funkce posune pasku doprava
shiftRight([Item|Tape], State, NextState, NewTape) :-
     Item == State,
          (
               [NextInList|MoreTape] = Tape,
                    (
                         % jsme na konci pasky, pridame blank
                         MoreTape == [],
                         (
                              append([NextState], [' '], NewTmp),
                              append([NextInList], NewTmp, NewTape)
                         )
                         ;
                         append([NextInList], [NextState], NewTmp),
                         append(NewTmp, MoreTape, NewTape)
                    )
          )
          ;
          shiftRight(Tape, State, NextState, NewTmp2),
          NewTape = [Item|NewTmp2]
.


% funkce posune pasku doleva
shiftLeft([Item|Tape], State, NextState, NewTape) :-
     % pokud neni paska na okraji, tak posuneme
     Item \= State,
     (
          shiftLeft_([Item|Tape], State, NextState, NewTape)
     )     
.

% pomocna funkce pro posuv
shiftLeft_([Item|Tape], State, NextState, NewTape) :-
     [NextInList|MoreTape] = Tape,
          (
               State == NextInList,
               (
                    append([NextState], [NextInList], NewTmp),
                    append(NewTmp, MoreTape, NewTape)
               )
               ;
               shiftLeft_(Tape, State, NextState, NewTmp2),
               NewTape = [Item|NewTmp2]
          )
.

writeSymbol([Item|Tape], State, NextState, Symbol, NewTape) :-
     Item == State,
     (
          [_|MoreTape] = Tape,
          (
               append([NextState], [Symbol], NewTmp),
               append(NewTmp, MoreTape, NewTape)
          )
     )
     ;
     writeSymbol(Tape, State, NextState, Symbol, NewTmp2),
     NewTape = [Item|NewTmp2]
.    

% simulace NTS
runNTM(StartTape, Rules, Tapes) :-

     % ziskame aktualni konfiguraci
     getConfiguration(StartTape, ActualState, ActualSymbol),
     (
          % pokud aktualni stav je koncovy, tak koncime
          ActualState == 'F', !
          ;
          % jinak ziskame pravidlo
          getRule(Rules, ActualState, ActualSymbol, NextState, Action),
               (
                    % posuv vpravo
                    Action == 'R',
                    (
                         shiftRight(StartTape, ActualState, NextState, NewTape)
                    )
                    ;
                    % postuv vlevo
                    Action == 'L',
                    (
                         shiftLeft(StartTape, ActualState, NextState, NewTape)
                    )
                    ;
                    Action \= 'L', Action \= 'R',
                    (
                         % zapis symbolu
                         writeSymbol(StartTape, ActualState, NextState, Action, NewTape)
                    )
               ),
               runNTM(NewTape, Rules, TmpTape),
               Tapes = [NewTape|TmpTape]
     )
. 

start :-
     prompt(_, ''),

     % nacteme vstup
     read_lines(LL),

     % odstrani posledni radek s paskou
     removeLastLine(LL, A_Rules),

     % nacteme pravidla
     getRulesFromFile(A_Rules, Rules),

     % ziskame posledni radek s paskou
     last(LL, A_Tape),
     
     % pridame pocatecni stav na zacatek pasky
     append(['S'],A_Tape, Tape),

     % spustime simulaci
     runNTM(Tape, Rules, Tapes),

     % vytiskne pocatecni konfiguraci
     writeLine(Tape),

     % vypiseme obsah pasky
     writeLines(Tapes),

     halt
.

