#!/bin/bash

#Prvni projekt do predmetu IOS 2013/2014
#Sobor: depsym.sh
#Autor: Lukas Pelanek, xpelan03

#prepinace
r=false
d=false
g=false

#ktere prepinace jsou aktivni
while getopts "r:d:g" prepinac; do
    case "$prepinac" in
    r)
        r=true
	hledany=$OPTARG
        ;;
    d)
        d=true
	hledany=$OPTARG
        ;;
    g)
        g=true
        ;;
    *)
        echo "Wrong arguments!"
        exit 1
        ;;
    esac
done

((OPTIND--))
shift $OPTIND

#prepinac r a d nemohou byt aktivni soucasne
if [ $r == true ] && [ $d == true ] ; then
	echo "Wrong arguments!"	
	exit 1
fi

#Pokud neni zadan zadny argument -> chybove hlaseni
if [ $# == 0 ] ; then
echo "Wrong arguments!"
exit 1
fi


seznam11=`for a in $@ ; do
	echo $a
done`

#prideleni seznamu ve kterych se bude hledat + vyber souboru koncicich .o
if [ $d == true ] ; then
	seznam1=`echo "$seznam11" | grep '\.o'`
	seznam0=`echo "$hledany" | grep '\.o'`
fi

if [ $r == true ] ; then
	seznam0=`echo "$seznam11" | grep '\.o'`
	seznam1=`echo "$hledany" | grep '\.o'`
fi

if [ $d == false ] && [ $r == false ] ; then
	seznam1=`echo "$seznam11" | grep '\.o'`
	seznam0=`echo "$seznam11" | grep '\.o'`
fi

#test zda nektery seznam neni prazdny
test -z "$seznam0"
if [ $? == 0 ] ; then
	echo "Wrong file(s)!"
	exit 1
fi
test -z "$seznam1"
if [ $? == 0 ] ; then
	echo "Wrong file(s)!"
	exit 1
fi

#v pripade aktivniho prepinace g vytisknu hlavicku
if [ $g = true ] ; then
 echo "digraph GSYM {"
fi

#prochazeni seznamu a porovnavani zavislosti
najdi=`for a in $seznam0 ; do

	for b in $seznam1 ; do

		for c in $( nm $a | grep '\sU\s' | sed 's/^.*U\s//' ) ; do
			
			for d in $( nm $b | grep '\s[TBDCG]\s' | sed 's/^.*[TBDCG]\s//' ) ; do
				if [ $c = $d ] && [ $a != $b ] && [ $g = true ] ; then
					echo -n "$a" | sed 's/^.*\///' | sed 's/-/_/' | sed 's/\./D/' | sed 's/+/P/'
					echo -n " -> "
					echo -n "$b" | sed 's/^.*\///' | sed 's/-/_/' | sed 's/\./D/' | sed 's/+/P/'
					echo " ($d)" | sed 's/(/[label="/' | sed 's/)/"];/'				
					echo -n "$a" | sed 's/^.*\///' | sed 's/-/_/' | sed 's/\./D/' | sed 's/+/P/'
					echo -n " [label=\""
					echo -n "$a" 
					echo "\"];"
					echo -n "$b" | sed 's/^.*\///' | sed 's/-/_/' | sed 's/\./D/' | sed 's/+/P/'
					echo -n " [label=\""
					echo -n "$b" 
					echo "\"];"
				fi 

				if [ $c = $d ] && [ $a != $b ] && [ $g = false ] ; then
					echo "$a -> $b ($d)"
				fi

			done
		done	
	done
done`

#vypis podle aktivnich prepinacu
if [ $r == true ] && [ $g != true ] ; then
	echo "$najdi" | grep " $hledany"
fi
if [ $d == true ] && [ $g != true ] ; then
	echo "$najdi" | grep "^$hledany"
fi
if [ $r = false ] && [ $d = false ] && [ $g != true ] ; then
	echo "$najdi"
fi

if [ $g = true ] ; then
	echo "$najdi" | sort -u | sed '/^\s*$/d'
	echo "}"
fi





