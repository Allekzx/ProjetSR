CC=gcc -Wall
SRCCLIENT=sources/requeteClient.c sources/client.c
SRCSERVEUR=sources/requeteServeur.c sources/lecture.c sources/serveur.c
OBJCLIENT=$(SRCCLIENT:.c=.o)
OBJSERVEUR=$(SRCSERVEUR:.c=.o)
EXEC=bin/serveur bin/client
all:$(EXEC)

bin/client: $(OBJCLIENT)
	$(CC) $^ -o $@

bin/serveur: $(OBJSERVEUR)
	$(CC) $^ -o $@

clean :
	@echo "tous les fichiers du projet ont été supprimé"
	rm $(OBJCLIENT) $(OBJSERVEUR) $(EXEC)