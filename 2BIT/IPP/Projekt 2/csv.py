#!/usr/bin/python
# -*- coding: utf-8 -*-
#CSV:xpelan03

"""
" Soubor:  csv.py
" Datum:   2015/04/18
" Autor:  Lukas Pelanek, xpelan03@stud.fit.vutbr.cz
" Projekt: CSV2XML
" Popis:   Program nacte zdrojovy soubor zapsany ve formatu CSV a prevede jej do formatu XML
"""

import params
import sys
path=sys.path[:]
sys.path=path[1:]
import csv as csvLib
sys.path=path[:]
import re

# parametry prikazove radky
parameters = {"help":False, "input":False, "output":False, "n":False, "r":False, "s":False, "h":False, 
              "c":False, "l":False, "i":False, "start":False, "e":False, "missing":False, "all":False, "padding":False}

params.getParams(parameters)
params.paramsCheck(parameters)

cols = 0;        # promenna ve ktere uchovavam cislo radku
spaces = 0;      # promenna ve ktere uchovavam pocet tabulatoru
head = []        # promenna ve ktere je ulozen prvni radek pokud je aktivni prepinac -h
x = 0            # pocitadlo prvku v hlavicce
rowCount = int(parameters["start"])

#funkce nahradi problematicke znaky
def replace(st):
    st = st.replace("&", "&amp;").replace("'", "&apos;").replace(">", "&gt;").replace('"', "&quot;").replace("<", "&lt;")
    return st

# funkce nahradi neplatne znaky ve jmene elementu
def nameReplace (st):
    if len(st) > 1:
        tmpStr = re.sub("[^:_A-Za-z\u00C0-\u00D6\u00D8-\u00F6\u00F8-\u02FF\u0370-\u037D\u037F-\u1FFF\u200C-\u200D\u2070-\u218F\u2C00-\u2FEF\u3001-\uD7FF\uF900-\uFDCF\uFDF0-\uFFFD]",parameters["h"],re.sub("\n",parameters['h']*2,st[0]))
        tmpStr += re.sub("[^:_A-Za-z\u00C0-\u00D6\u00D8-\u00F6\u00F8-\u02FF\u0370-\u037D\u037F-\u1FFF\u200C-\u200D\u2070-\u218F\u2C00-\u2FEF\u3001-\uD7FF\uF900-\uFDCF\uFDF0-\uFFFD\.\-0-9\u00B7\u0300-\u036F\u203F-\u2040}]",parameters["h"],re.sub("\n",parameters['h']*2,st[1:]))
    else:
        tmpStr = re.sub("[^:_A-Za-z\u00C0-\u00D6\u00D8-\u00F6\u00F8-\u02FF\u0370-\u037D\u037F-\u1FFF\u200C-\u200D\u2070-\u218F\u2C00-\u2FEF\u3001-\uD7FF\uF900-\uFDCF\uFDF0-\uFFFD]",parameters["h"],re.sub("\n",parameters['h']*2,st))
    return tmpStr

# funkce pro rozsireni padding, doplni odpovidajici pocet nul u jednotlivych sloupcu
def getColIndex(colLen, number):
    if parameters["padding"] == True:
        tmpNum = ""
        for i in range(len(str(number)), len(str(colLen))):
            tmpNum += "0"
        tmpNum += str(number)
        return tmpNum
    return number

# funkce pro rozsireni padding, doplni odpovidajici pocet nul u jednotlivych radku
def getRowIndex(rowLen, number):
    if parameters["padding"] == True:
        maxIndex = int(parameters["start"])-1 + rowLen
        tmpN = ""
        for e in range(len(str(number)), len(str(maxIndex))):
            tmpN += "0"
        tmpN += str(number)
        return tmpN
    return number

# pokusim se otevrit vstupni soubor
if parameters["input"] != False:
    try:
        inputFile = open(parameters["input"], encoding = "utf-8", mode = "r")
    except:
        sys.exit(2)
else:
    inputFile = sys.stdin

# pokusim se otevrit vystupni soubor    
if parameters["output"] != False:
    try:
        outputFile = open(parameters["output"], encoding = "utf-8", mode = "w")
    except:
        sys.exit(3)
else:
    outputFile = sys.stdout
    
reader = csvLib.reader(inputFile,  delimiter=parameters["s"])

# ulozim vstup do promenne, protoze budu potrebovat projit vstup vicekrat
file = list(reader)

# pokud je vstupni soubor prazdny, pridam do souboru prazdny retezec
if len(file) == 0:
    file.append("")
# ulozim si pocet sloupcu    
colsLength = len(file[0])
rowsLength = len(file)  

#pokud neni zadan prepinac -e a neodpovida pocet sloupcu -> chyba
if parameters["e"] == False:
    for tmp in file:
        if colsLength != len(tmp):
            exit(32)

# pokud je aktivni prepinac -h, projdu hlavicku, nahradim neplatne znaky a znakontroluji validitu
if parameters["h"] != False:
    head = file[0]
    for b, i in enumerate(head):
        head[b] = nameReplace(i);
        if not params.checkName(head[b]):
            exit(31)
    
    file = file[1:]

# pokud neni aktivni prepinac -n, generuji XML hlavicku
if parameters["n"] == False:
    outputFile.write("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n")

# pokud je aktivni prepinac -r, generuji root element
if parameters["r"] != False:
    spaces += 1
    outputFile.write("<%s>\n" % parameters["r"])   

actualCol = colsLength

for row in file:
    # pokud je aktivni prepinac -i, vlozim atribut index
    if parameters["i"] == True:
        outputFile.write("\t"*spaces + "<%s index=\"" % parameters["l"])
        outputFile.write("%s\">\n" % getRowIndex(rowsLength, rowCount))
        rowCount += 1
    else:
        outputFile.write("\t"*spaces + "<%s>\n" % parameters["l"])
        
    spaces += 1   
    # zotaveni z chybneho poctu sloupcu
    if len(row) < colsLength:
        for a in range(0, (colsLength - len(row))):
            # chybejici sloupce vyplnim znakem "<" pro identifikaci, tento symbol se mi na vstupu nikdy nemuze objevit
            row.append("<")
            
    for col in row:
        if parameters["all"]:
            actualCol = len(row)
        cols += 1
        
        # pokud neni aktivni prepinac --all-columns, prebyvajici sloupce ignoruji
        if cols > colsLength:
            if parameters["all"] == False:
                break
        
        # pokud je aktivni prepinac -h, tisknu elementy podle hlavicky, jinak podle parametru -c
        if parameters["h"] != False and x < len(head):
            outputFile.write("\t"*spaces + "<%s>\n" % head[x])
        else:   
            outputFile.write("\t"*spaces + "<%s" % parameters["c"])
            outputFile.write("%s>\n" % getColIndex(actualCol, cols))
            
        spaces +=1
        # pokud je pocet sloupcu mensi nebo roven nez na prvnim radku a sloupec nema hodnotu a zaroven je aktivni prepinac --missing-value, doplnim hodnotu, jinak prazdne pole
        if cols <= colsLength and col == "<":
            if parameters["missing"] != False:
                outputFile.write("\t"*spaces + replace(parameters["missing"]) + "\n")
        else:
            outputFile.write("\t"*spaces + replace(col) + "\n")
        spaces -=1
        
        # pokud je aktivni prepinac -h, tisknu elementy podle hlavicky, jinak podle parametru -c
        if parameters["h"] != False and x < colsLength:
            outputFile.write("\t"*spaces + "</%s>\n" % head[x])
            x += 1
        else:   
            outputFile.write("\t"*spaces + "</%s" % parameters["c"])
            outputFile.write("%s>\n" % getColIndex(actualCol, cols))
         
    x = 0
    cols = 0
    spaces -= 1
    outputFile.write("\t"*spaces + "</%s>\n" % parameters["l"])

# pokud je aktivni prepinac -r, tisknu root element
if parameters["r"] != False:
    spaces -= 1
    outputFile.write("</%s>\n" % parameters["r"])     
    
    