-- Soubor:    xpelan03.hs
-- Datum:    2017/03/06
-- Autor:    Bc. Lukas Pelanek, xpelan03@stud.fit.vutbr.cz
-- Projekt:    FLP, rka-2-dka
-- Popis:    Prevod rozsireneho konecneho automatu na deterministicky konecny automat

import System.IO
import System.Environment
import System.Exit
import Data.Char
import Data.List

-- vlastni typy
type MState = String
type MSymbol = String

data Machine = FSM
    { states :: [MState]
    , alphabet :: [MSymbol]
    , trans :: [Transition]
    , start :: MState
    , end :: [MState]
    } deriving (Eq)

data Transition = Trans
    { fromState :: MState
    , fromSym :: MSymbol
    , toState :: MState
    } deriving (Eq)

instance Show Transition where
    show (Trans f c t) = show f ++ "," ++ show c ++ "," ++ show t ++ "\n"

instance Show Machine where
    show (FSM q a t s f) = show q ++ "\n" ++ show a ++ "\n" ++ show t ++ "\n" ++ show s ++ "\n" ++ show f

----------------------------------------------------- ZACATEK APLIKACE ----------------------------------
main :: IO ()
main = do
    args <- getArgs
    let (simulate, inFile) = procArgs args
    fileContent <- if inFile == "printStdin" then hGetContents stdin else readFile inFile
    fsm <- if simulate then getDFSM fileContent else getFSM fileContent
    printFSM fsm
    return ()
    where
        readFile inFile = do
            hInFile <- openFile inFile ReadMode
            fileContent' <- hGetContents hInFile
            return fileContent'

-- zpracovani parametru prikazove radky (z cviceni FLP)
procArgs :: [String] -> (Bool,String)
procArgs [x]
    | x=="-i" = (False, "printStdin")
    | x=="-t" = (True, "printStdin")
    | otherwise = error "You have to run app like this: ./rka-2-dka (-i|-t) [file_path]"
procArgs [x,y]
    | x=="-i" = (False, y)
    | x=="-t" = (True, y)
    | otherwise = error "You have to run app like this: ./rka-2-dka (-i|-t) [file_path]"
procArgs _ = error "You have to run app like this: ./rka-2-dka (-i|-t) [file_path]"

-- provede determinizaci automatu
getDFSM :: String -> IO Machine
getDFSM input = do
    let lns = lines input
    let fsm = procLns lns
    let noeps = getNoEpsFSM fsm
    let dfsm = getDeterministicFSM noeps
    return dfsm

-- pouze nacte vstupni automat do pameti
getFSM :: String -> IO Machine
getFSM input = do
    let lns = lines input
    let fsm = procLns lns
    return fsm

-- zpracuje vstupni soubor a vrati konecny automat
procLns :: [String] -> Machine
procLns (states:start:finalStates:transitions) = 
    if null transitions
        then error "No transitions"
    else if checkStatesError
        then error "Wrong states"
    else if checkFinalStatesError
        then error "Wrong final states"
    else if checkStartStateError
        then error "Wrong start state"
    else if checkTransitionsError
        then error "Wrong transitions"
    else FSM getStates getAlphabet getTransitions start getFinalStates
    where    
        getStates = split states
        getTransitions =  (map getRule transitions)
        getAlphabet = sort (nub [c | (Trans f c t) <- getTransitions, c /= ""])
        getFinalStates = split finalStates
        getRule rule = getRule' (split rule)

        -- kontrola vstupnich hodnot
        checkFinalStatesError = ((intersect getFinalStates getStates) /= getFinalStates) || (getFinalStates == [])
        checkStartStateError = ((length (split start) /= 1) || ((intersect (split start) getStates) == [])) 
        checkTransitionsError = checkTransitionsError' getTransitions
        checkStatesError = checkStatesError' getStates || (length getStates < 1)
        checkTransitionsError' :: [Transition] -> Bool
        checkTransitionsError' [] = False
        checkTransitionsError' (x:xs) = if transFail x then True else checkTransitionsError' xs
            where
                transFail (Trans a b c)
                    | notElem a getStates = True
                    | (intersect b ['a'..'z']) /= b || (length b > 1) = True
                    | notElem c getStates = True
                    | otherwise = False
        checkStatesError' :: [MState] -> Bool
        checkStatesError' [] = False
        checkStatesError' (x:xs) = if checkFail x then True else checkStatesError' xs
            where
                checkFail x
                    | (intersect x ['0'..'9']) /= x || (length x < 1) = True
                    | otherwise = False


        -- extrahovani prechodu
        getRule' :: [String] -> Transition
        getRule' [q1, sym, q2] = Trans q1 sym q2
        getRule' _ = error "Bad transition syntax"

        -- rozdeli retezec podle znaku ',' a retezce ulozi do seznamu
        split :: String -> [String]
        split [] = []
        split s = x : split (drop 1 y) where (x,y) = span (/= ',') s
procLns _ = error "Bad syntax"

-- Vytiskne automat v pozadovanem formatu na stdout
printFSM :: Machine -> IO ()
printFSM (FSM states alphabet trans start end) =
    putStrLn $ printStates states ++ "\n" ++ start ++ "\n" ++ printStates end ++ "\n" ++ printTransitions trans
    where
        printStates :: [String] -> String
        printStates [] = ""
        printStates (x:[]) = x
        printStates (x:xs) = x ++ "," ++ printStates xs
        printTransitions :: [Transition] -> String
        printTransitions [] = ""
        printTransitions (x:[]) = printTransition' x
        printTransitions (x:xs) = printTransition' x ++ "\n" ++ printTransitions xs
        printTransition' :: Transition -> String
        printTransition' (Trans f s t) = f ++ "," ++ s ++ "," ++ t

-- vypocte eps uzaver pro danou mnozinu stavu
epsUzaver :: [MState] -> [Transition] -> [MState]
epsUzaver s tr 
    | s == [] = []
    | tr == [] = s
    | s == eps' s = s
    | otherwise = epsUzaver (eps' s) tr
    where
        -- vypocte eps prechod pro dane stavy
        eps' :: [MState] -> [MState]
        eps' [] = []
        eps' s = sort (nub (s ++ [t | (Trans f c t) <- tr, elem f s, c == ""]))

-- odstraneni epsilon prechodu. Vstup: rozsireny konecny automat, vystup: konecny automat bez eps prechodu
getNoEpsFSM :: Machine -> Machine
getNoEpsFSM (FSM q al t s e) = 
    FSM q al transitions s finalStates
    where
        transitions = getTransitions q
        finalStates = [q' | q' <- q, (intersect (epsUzaver [q'] t) e) /= [] ]
        getTransitions :: [MState] -> [Transition]
        getTransitions [] = []
        getTransitions (x:xs) = nub ([Trans x b c | (Trans a b c) <- t, elem a (epsUzaver [x] t), elem c q, elem b al] ++ getTransitions xs)

-- Determinizace konecneho automatu. Vstup: Automat bez eps pravidel, vystup: deterministicky automat
getDeterministicFSM :: Machine -> Machine
getDeterministicFSM (FSM q al t s e) =
    -- vrati deterministicky automat s prejmenovanymi stavy
    renameStates (FSM (sort(nub $ myConcatenate newStates)) al (nub newTransitions) s (sort(nub $ myConcatenate newFinalStates)))
    where
        (_, newTransitions, newStates, newFinalStates) = determinize ([[s]], [], [], [])

        -- odstraneni nedeterminismu bez nedostupnych stavu, viz prednaska ifj
        determinize :: ([[MState]], [Transition], [[MState]], [[MState]]) -> ([[MState]], [Transition], [[MState]], [[MState]])
        determinize (qNew, rd, qd, fd)
            | qNew == [] = (qNew, rd, qd, fd)
            | otherwise = determinize (qNew', rd', qd', fd')
            where
                q' = head qNew
                tmp_qNew' = tail qNew
                qd' = nub (qd ++ [q'])
                (rd', qNew') = loop al (rd, tmp_qNew')
                fd' = if (intersect q' e) /= [] then nub (fd ++ [q']) else fd
                -- for each smycka nad abecedou, vrati seznam prechodu Rd a stavu Qnew
                loop :: [MSymbol] -> ([Transition], [[MState]]) -> ([Transition], [[MState]])
                loop [] (nRules, nStates) = (nRules, nStates)
                loop (fi:li) (nRules, nStates)
                    | q'' /= [] && notElem q'' (qd' ++ [[]]) = loop li ((nub (nRules ++ [Trans (concat (sort q')) fi (concat (sort q''))])), (nub (nStates ++ [q''])))
                    | q'' /= [] = loop li ((nub (nRules ++ [Trans (concat (sort q')) fi (concat (sort q''))])), nStates) -- serazeni stavu u prechodu a nasledne konkatenace, jednotne se stavy, nasledne pro prejmenovani stavu
                    | notElem q'' (qd' ++ [[]]) = loop li (nRules, (nub (nStates ++ [q''])))
                    | otherwise = loop li (nRules, nStates)
                    where
                        q'' = nub [c | (Trans a _ c) <- t, elem a q', elem (Trans a fi c) t]

        -- konkatenace makro stavu do samostatneho stavu
        myConcatenate :: [[MState]] -> [MState]
        myConcatenate list = [concat (sort x)| x <- list]

        -- vrati automat s prejmenovanymi stavy dle zadani
        renameStates :: Machine -> Machine
        renameStates (FSM states alphabet transitions startState finalStates) = 
            FSM renameNewStates alphabet renameTransitions "0" renameFinalStates
            where
                -- pocatecni stav jako prvni, aby byl pojmenovan jako "0"
                startStateFirst = startState:(delete startState states)
                (renameNewStates, renameTransitions, renameFinalStates) = renameStates' startStateFirst 0 (states, transitions, finalStates)

                -- prejmenuje stavy na cisla v rozmezi 0 - (n | n > 0). Vyzaduje, aby pocatecni stav byl na zacatku seznamu daneho prvnim parametrem
                renameStates' :: [MState] -> Int -> ([MState], [Transition], [MState]) -> ([MState], [Transition], [MState])
                renameStates' statesList index (st', tr', fSt')
                    | statesList == [] = (st', tr', fSt')
                    | otherwise = renameStates' (tail statesList) (index + 1) (nSt', nTr', nFST')
                    where
                        actState = head statesList
                        replaceWith = show index
                        nSt' = [(replaceFnc item replaceWith) | item <- st']
                        nTr' = [Trans (replaceFnc a replaceWith) b (replaceFnc c replaceWith) | (Trans a b c) <- tr']
                        nFST' = [(replaceFnc item replaceWith) | item <- fSt']
                        replaceFnc :: MState -> [Char] -> MState
                        replaceFnc r i 
                            | actState == r = i
                            | otherwise = r