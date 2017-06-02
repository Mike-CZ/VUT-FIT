

#CSV:xpelan03

"""
" Soubor:  csv.py
" Datum:   2015/04/18
" Autor:  Lukas Pelanek, xpelan03@stud.fit.vutbr.cz
" Projekt: CSV2XML
" Popis:   Program nacte zdrojovy soubor zapsany ve formatu CSV a prevede jej do formatu XML
"""

import sys
import argparse
import re

# napoveda
def printHelp():
    print("Autor:\t Lukas Pelanek")
    print("E-mail:\t xpelan03@stud.fit.vutbr.cz")
    print("Popis:\t Skript pro prevod formatu csv do xml\n")
    print("--help\t\t\t Zobrazeni napovedy")
    print("--input=filename\t Vstupni soubor. Neni-li zadan tento parametr, je cteno ze stdin")
    print("--output=filename\t Vystupni soubor. Neni-li zadan tento parametr, je zapisovano na stdout")
    print("-n\t\t\t Nebude se generovat XML hlavicka")
    print("-r=root-element\t\t Obaleni korenovym elementem root-element")
    print("-s=separator\t\t Nastaveni separatoru bunek")
    print("-h=subst\t\t Prvni radek souboru slouzi jako hlavicka a podle nej odvodi nazvy elementu")
    print("-c=column-element\t Prefix jmena elementu, ktery bude obalovat nepojmenovany bunky(sloupce)")
    print("-l=line-element\t\t Jmeno elementu, ktery obaluje zvlast kazdy radek souboru")
    print("-i\t\t\t Vlozeni atributu index s ciselnou hodnotou do elementu line-element, musi byt kombinovano s -l")
    print("--start=n\t\t Inicializace citace pro parametr -i. Pouze v kombinaci s -l a -i")
    print("-e, --error-recovery\t Zotaveni z chybneho poctu sloupcu na neprvnim radku")
    print("--missing-field=val\t Pokud nejaka vstupni bunka chybi, je doplnena hodnotou val. Pouze s -e")
    print("--all-columns\t\t Sloupce, ktere jsou navic nejsou ignorovany, ale take vypsany. Pouze s -e")

# funkce zkontroluje, zda zadany element je validni
def checkName(name):
    if re.search("^[:_A-Za-z\u00C0-\u00D6\u00D8-\u00F6\u00F8-\u02FF\u0370-\u037D\u037F-\u1FFF\u200C-\u200D\u2070-\u218F\u2C00-\u2FEF\u3001-\uD7FF\uF900-\uFDCF\uFDF0-\uFFFD][:_A-Za-z\u00C0-\u00D6\u00D8-\u00F6\u00F8-\u02FF\u0370-\u037D\u037F-\u1FFF\u200C-\u200D\u2070-\u218F\u2C00-\u2FEF\u3001-\uD7FF\uF900-\uFDCF\uFDF0-\uFFFD\.\-0-9\u00B7\u0300-\u036F\u203F-\u2040}]*$", name):
        return True;
    return False;

# funkce zpracujici parametry        
def getParams(parameters):
    counter = 1
    parser = argparse.ArgumentParser(add_help=False)
    
    parser.add_argument("-r", dest = "root")
    parser.add_argument("-h", dest = "subst", nargs = "?", const = "-")
    parser.add_argument("-n", action = "store_true")
    parser.add_argument("-s", dest = "separator")
    parser.add_argument("-c", dest = "column")
    parser.add_argument("-l", dest = "line")
    parser.add_argument("-i", action = "store_true")
    parser.add_argument("--start", dest = "start")
    parser.add_argument("-e", "--error-recovery" , dest = "error", action = "store_true")
    parser.add_argument("--missing-field", dest = "val")
    parser.add_argument("--all-columns", dest = "all", action = "store_true")
    parser.add_argument("--help", action = "store_true") 
    parser.add_argument("--input", dest = "inpFile")
    parser.add_argument("--output", dest = "oFile")
    parser.add_argument("--padding", action = "store_true")
        
    try:
        args = parser.parse_args()
    except:
        exit(1)
        
    if args.root:
        counter += 1
        parameters["r"] = args.root
        
    if args.subst:
        counter += 1
        parameters["h"] = args.subst
    
    if args.n:
        counter += 1
        parameters["n"] = True   
    
    if args.separator:
        counter += 1
        parameters["s"] = args.separator 
    
    if args.column:
        counter += 1
        parameters["c"] = args.column
    
    if args.line:
        counter += 1
        parameters["l"] = args.line
        
    if args.i:
        counter += 1
        parameters["i"] = True
    
    if args.inpFile:
        counter += 1
        parameters["input"] = args.inpFile
        
    if args.oFile:
        counter += 1
        parameters["output"] = args.oFile
        
    if args.start:
        counter += 1
        parameters["start"] = args.start
    
    if args.error:
        counter += 1
        parameters["e"] = True
    
    if args.val:
        counter += 1
        parameters["missing"] = args.val
        
    if args.all:
        counter += 1
        parameters["all"] = args.all
    if args.padding:
        counter += 1
        parameters["padding"] = True
        
    if args.help:
        if len(sys.argv) != 2:      # parametr --help musi byt zadan samostatne
            exit(1)
        printHelp()
        exit(0)
        
    # pocet zpracovanych parametru musi byt roven poctu zadanych parametru   
    if len(sys.argv) != counter:
        exit(1)
      
# funkce zkontroluje zadane parametry        
def paramsCheck(parameters):
    # parametr --all-columns musi byt pouzit s -e nebo --error-recovery
    if parameters["all"] == True and parameters["e"] != True:
        exit(1)
    
    # parametr --missing-field="val" musi byt pouzit s -e nebo --error-recovery
    if parameters["missing"] != False and parameters["e"] != True:
        exit(1)
        
    # parametr --start="val" je povolen pouze v kombinaci s parametry -i a -l="val"
    if parameters["start"] != False and (parameters["i"] != True or parameters["l"] == False):
        exit(1)
        
    # parametr -i je povolen pouze v kombinaci s parametrem -l="val"
    if parameters["i"] == True and parameters["l"] == False:
        exit(1)
                
    #osetreni parametru -s
    if parameters["s"] != False:
        if parameters["s"] == "TAB":
            parameters["s"] = "\t"
        
        if len(parameters["s"]) != 1:
            exit(1)   
    else:
        parameters["s"] = ","       # pokud nebyl zadan zadny separator -> defaultne znak ","
    
    if parameters["l"] == False:
        parameters["l"] = "row"
        
    if parameters["c"] == False:
        parameters["c"] = "col"
    
    if parameters["start"] == False:
        parameters["start"] = 1
    else:
        if not re.match("^[0-9]+$", parameters["start"]):
            exit(1)
    
    # pokud je hodnota prepinace -r nevalidni xml znacka -> chyba
    if parameters["r"] != False:
        if not checkName(parameters["r"]):
            exit(30)
    
    # pokud je hodnota prepinace -c nevalidni xml znacka -> chyba
    if parameters["c"] != False:
        if not checkName(parameters["c"]):
            exit(30)
    
    # pokud je hodnota prepinacce -l nevalidni xml znacka -> chyba
    if parameters["l"] != False:
        if not checkName(parameters["l"]):
            exit(30)   