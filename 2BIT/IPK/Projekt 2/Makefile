# Autor: Lukas Pelanek, xpelan03@stud.fit.vutbr.cz

LOGIN = xpelan03
FILES = ipkhttpclient ipkhttpserver Makefile README test.sh www

all: client server

client: ipkhttpclient
	chmod +x ipkhttpclient

server: ipkhttpserver
	chmod +x ipkhttpserver

pack:
	tar -cvzf $(LOGIN).tgz $(FILES)
	
test:
	chmod +x test.sh
	./test.sh

clean:
	rm -f *.log *.payload *.header *.tgz