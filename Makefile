.PHONY= help clean build

colore="\e[33;1m"

help: 
	@printf $(colore)
	@echo ""
	@echo "Laboratorio Sistemi Operativi - Anno 2017-2018 "
	@echo "Shell custom"
	@echo ""
	@echo "Gruppo composto da:"
	@echo "Sara Callaioli 	- 171807"
	@echo "Vanes Carrer 	- 180035"
	@echo "Asia Salvaterra  - 180778"
	@echo ""
	@echo "\nComandi:\n"
	@echo "build \t Rimuove i file e cartelle create precedentemente e compila"
	@echo "clean \t Cancella gli eseguibili e cartelle e file non necessari "
	@echo "help  \t Mostra questo messaggio (default)\n"


clean:
	rm -rf ./log/
	rm -rf bin/

build: clean
	mkdir ./log
	mkdir bin
	gcc  -o bin/shell  ./src/main.c  ./src/shell.c 