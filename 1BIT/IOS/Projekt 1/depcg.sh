#!/bin/bash

#Prvni projekt do predmetu IOS 2013/2014
#Sobor: depcg.sh
#Autor: Lukas Pelanek, xpelan03

#prepinace
g=false
p=false
r=false
d=false
argumenty="$#"

#ktere prepinace jsou aktivni
while getopts "gpr:d:" prepinac; do
    case $prepinac in
    g)
        g=true
        ;;
    p)
        p=true
        ;;
    r)
        r=$OPTARG
        ;;
    d)
        d=$OPTARG
        ;;
    *)
        echo "Wrong arguments!"
        exit 1
        ;;
    esac
done

((OPTIND--))
shift $OPTIND

#Pokud jsou aktivni prepinace -r a -d -> chybove hlaseni
if [ $r != false ] && [ $d != false ] ; then
	echo "Wrong arguments!"
	exit 1
fi

#Test zda dany soubor existuje
test -f "$1"
if [ $? == 1 ] ; then
	echo "No such file!"
	exit 1
fi

#vytrideni funkci a volani
vstup=`objdump -d -j .text $1 | grep -e '>:' -e 'call'`

smycka1=`for a in $vstup ; do
	if [[ "$a" =~ ">" ]] ; then
		echo "$a" 
	fi
done`

#seznam funkci
vstup2=`echo "$smycka1" | sed '/>:/!d' | sed 's/<//' | sed 's/>://' | sort -u`

#vypis vsech funkci a volani
smycka2=`for b in $vstup2 ; do
	echo "$smycka1" | sed '/+/d'| sed "0,/<$b>:/d" | tac | sed 's/^<.*>://' | tac | sed '0,/^\s*$/!d' | sort -u | sed '/^\s*$/d' | sed "s/</$b -> /" | sed 's/>$//' 
done`

#podle aktivnich prepinacu vybiram z vypisu vsech funkci pozadovane zavislosti
case $argumenty in
	"1")
		echo "$smycka2" | sed '/@plt/d' 
		;;

	"2")	if [ $p == "true" ] ; then echo "$smycka2" ; fi
		if [ $g == "true" ] ; then echo "$smycka2" | sed '/@plt/d'  | sed 's/@plt/_PLT/' | sed 's/$/;/' | sed  '1idigraph CG {' | sed '$a\}'; fi
		;;	

	"3")
		if [ $d != "false" ] ; then echo "$smycka2" | sed '/@plt/d' | grep "^$d " ; fi
		if [ $r != "false" ] ; then echo "$smycka2" | sed '/@plt/d' | grep " $r$" ; fi
		if [ $p == "true" ] && [ $g == "true" ] ; then echo "$smycka2"  | sed 's/@plt/_PLT/' | sed 's/$/;/' | sed  '1idigraph CG {'  | sed '$a\}'; fi
		;;
	"4")
		if [ $d != "false" ] && [ $p == "true" ] ; then echo "$smycka2" | grep "^$d " ; fi
		if [ $r != "false" ] && [ $p == "true" ] ; then echo "$smycka2" | grep " $r$" ; fi
		if [ $d != "false" ] && [ $g == "true" ] ; then echo "$smycka2" | sed '/@plt/d'  | sed 's/@plt/_PLT/' | sed 's/$/;/' | sed  '1idigraph CG {'  | sed '$a\}' | grep -e "^$d " -e '{' -e '}' ; fi 	
		if [ $r != "false" ] && [ $g == "true" ] ; then echo "$smycka2" | sed '/@plt/d'  | sed 's/$/;/' | sed  '1idigraph CG {' | sed '$a\}' | grep -e " $r;$" -e '{' -e '}' ; fi
		;;

	"5")	
		if [ $g == "true" ] && [ $p == "true" ] && [ $d != "false" ] ; then  echo "$smycka2" | sed 's/@plt/_PLT/' | sed 's/$/;/' | sed '1idigraph CG {' | sed '$a\}' | grep -e "^$d " -e '{' -e '}'; fi 	
		if [ $g == "true" ] && [ $p == "true" ] && [ $r != "false" ] ; then  echo "$smycka2" | sed 's/$/;/' | sed  '1idigraph CG {' | sed '$a\}' | grep -e " $r;$" -e'{' -e '}' | sed 's/@plt/_PLT/' ; fi 	

		;;

	*)
		echo "Wrong arguments!"
		;;
esac




