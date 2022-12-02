CC=gcc -Wall
all : client serveur

client.o : client.c
	$(CC) -c client.c

lecture.o : lecture.c
	$(CC) -c lecture.c

requeteServeur.o : requeteServeur.c lecture.c
	$(CC) -c requeteServeur.c lecture.c

serveur : serveur.o requeteServeur.o lecture.o
	$(CC) serveur.o requeteServeur.o lecture.o -o serveur

client : client.o requeteClient.o
	$(CC) client.o requeteClient.o -o client

clean :
	@echo "tous les fichiers présent sur ton ordinateur ont été supprimé"
	-rm *.o