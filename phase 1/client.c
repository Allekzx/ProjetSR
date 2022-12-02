#include "stdio.h"
#include "stdlib.h"
#include <sys/types.h>          
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>

	// Ce fichier est le code de la phase 1
	// auteurs : Charre Kyllian, Papeau Willem, Delagrange Alex
    // 
	// Paramètres d'appel : numPort nomServeur
    // 

// Paramètres : taille tab, tab=[noPort,nomServeur]
int main(int argc, char **argv) {
    // gestion des paramètres
    if (argc != 3){
        printf("Option non valide");
        return -1;
    }
    int noPort =atoi(argv[1]);
    char *nomServeur = argv[2];

    // création de la socket
    int resSocket;
    resSocket = socket(AF_INET, SOCK_STREAM, 0);

    // gestion d'erreur sur socket
    if (resSocket == -1 ){
        perror("socket");
        exit(-1);
    }  

    // création des types struct pour stocker les informations
    struct hostent* infMachine = malloc(sizeof(struct hostent));
    struct sockaddr_in adserv;
    
    // récupération des informations server dans infMachine
    infMachine = gethostbyname(nomServeur);

    adserv.sin_family = AF_INET;
    adserv.sin_port = htons(noPort);
    // copie de l'information h_addr de infMachine dans s_addr de adserv
    memcpy(&adserv.sin_addr.s_addr ,infMachine->h_addr, sizeof(__u_long)); 

    // connexion au serveur
    int valideConnect;
    valideConnect = connect(resSocket, (struct sockaddr *)&adserv, sizeof(struct sockaddr)); 
    // Gestion d'erreur de connexion
    if(valideConnect ==-1){
        perror("Connect");
        exit(-1);
    }
    printf("Connexion établie\n");

    // Boucle d'écriture de messages
    int message = 1;
    while(message != 0){
        
        printf("Entrez un entier pour le serveur (0 pour se déconnecter)\n");
        scanf("%d", &message);
        write(resSocket, &message, sizeof(int));
    }
    // fermeture du socket
    close(resSocket);
    return 0;
}
