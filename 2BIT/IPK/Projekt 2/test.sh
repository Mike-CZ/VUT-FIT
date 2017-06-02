#!/bin/bash

# Autor: Lukas Pelanek, xpelan03@stud.fit.vutbr.cz
# Testovaci sada k druhemu projektu do predmetu IPK 2014/2015

Yellow='\e[0;33m'
Default='\e[0m' 

# Zkusime vypsat napovedu
echo -e "${Yellow}Vypiseme napovedu u klienta${Default}"
./ipkhttpclient -h

# Nespravna kombinace parametru
echo -e "${Yellow}Nelze kombinovat parametr -h s jinymi parametry u serveru${Default}"
./ipkhttpserver -p 8080 -h

echo -e "${Yellow}Rovnez u klienta nelze kombinovat parametr -h${Default}"
./ipkhttpclient -h www.seznam.cz

echo -e "${Yellow}Prepinac -t bez prepinace -c u serveru${Default}"
./ipkhttpserver -p 8080 -t 200

echo -e "${Yellow}Spusteni klienta bez URI${Default}"
./ipkhttpclient

# spustime server na pozadi a ziskame id jeho procesu
echo -e "${Yellow}Spoustim server bez chunked-encoding...${Default}"
./ipkhttpserver -p 8080 &
processID=`echo $!`

# chvili pockame nez se server spusti
sleep 0.5

# Test funkcnosti.. vypisuji pouze hlavicku, protoze pri dlouhe odpovedi by zaplavil cely terminal, celou odpoved lze najit v logovacich souborech

# Pripojime se na adresu http://localhost:8080/nonchunked.txt
echo -e "${Yellow}Zkusime se pripojit k serveru a ziskat soubor nonchunked.txt (pouze hlavicka)${Default}"
./ipkhttpclient -d http://localhost:8080/nonchunked.txt

# Pripojime se na adresu http://localhost:8080/docxformat.docx
echo -e "${Yellow}Zkusime se pripojit k serveru a ziskat soubor docxformat.docx (pouze hlavicka)${Default}"
./ipkhttpclient -d http://localhost:8080/docxformat.docx

# Pripojime se na adresu http://localhost:8080/docxformat.xml
echo -e "${Yellow}Zkusime se pripojit k serveru a ziskat neexistujici soubor hype.xml (pouze hlavicka)${Default}"
./ipkhttpclient -d http://localhost:8080/docxformat.xml

# Ukoncime server
echo -e "${Yellow}Ukoncime server.${Default}"
kill -9 "${processID}"

# spustime server na pozadi a ziskame id jeho procesu
echo -e "${Yellow}Spoustim server s chunked-encoding (20) a simulujeme provoz na lince (100ms)...${Default}"
./ipkhttpserver -p 8080 -c 20 -t 100 &
processID=`echo $!`

# chvili pockame nez se server spusti
sleep 0.5


# Pripojime se na adresu http://localhost:8080/chunked.txt
echo -e "${Yellow}Zkusime se pripojit k serveru a ziskat soubor chunked.txt + utf-8 (pouze hlavicka)${Default}"
./ipkhttpclient -d http://localhost:8080/chunked.txt

# Pripojime se na adresu http://localhost:8080/docxformat.docx
echo -e "${Yellow}Znovu se pokusime ziskat soubor docxformat.docx (pouze hlavicka)${Default}"
./ipkhttpclient -d http://localhost:8080/docxformat.docx

# Pripojime se na adresu http://www.seznam.cz/index.html
echo -e "${Yellow}Pokusime se pripojit k www.seznam.cz (pouze hlavicka)${Default}"
./ipkhttpclient -d http://www.seznam.cz/index.html

# Pripojime se na adresu http://www.gamepark.cz
echo -e "${Yellow}Pokusime se pripojit k www.gamepark.cz (pouze hlavicka)${Default}"
./ipkhttpclient -d http://www.gamepark.cz

# Ukoncime server
echo -e "${Yellow}Ukoncime server.${Default}"
kill -9 "${processID}"

exit 0








