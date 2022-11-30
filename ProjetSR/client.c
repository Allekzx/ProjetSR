#include "stdio.h"
#include "stdlib.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

#define nbPort 2121
#define tailleMessage 255

    // Ce fichier est un client d'application client/serveur
    // dont le but est de modéliser une bibliothèque
    // nous retrouvons les fonctionnalités suivantes :

int main(int argc, char **argv)
{

    // création de la socket
    int resSocket;
    resSocket = socket(AF_INET, SOCK_STREAM, 0);

    // gestion d'erreur sur socket
    if (resSocket == -1)
    {
        perror("socket");
        exit(-1);
    }

    // création des types struct pour stocker les informations
    struct hostent *infServeur = malloc(sizeof(struct hostent));
    struct sockaddr_in *adserv = malloc(sizeof(struct sockaddr_in));

    // récupération des informations server
    infServeur = gethostbyname("f203-01");

    adserv->sin_family = AF_INET;
    adserv->sin_port = htons(nbPort);
    // copie de l'information dans adserv
    memcpy(&adserv->sin_addr.s_addr, infServeur->h_addr, sizeof(__u_long));
    adserv->sin_port = htons(nbPort);
    // on créer la variable struct sockaddr pour la connexion
    struct sockaddr *p = (struct sockaddr *)adserv;

    int resConnect;
    // connexion au serveur et tests
    resConnect = connect(resSocket, p, sizeof(struct sockaddr));
    if (resConnect == -1)
    {
        perror("\nErreur :");
        exit(-1);
    }

    // variable de stockage de la requête
    char *requete[sizeof(char)][sizeof(char)*tailleMessage];

    //interface d'accueil
    printf("Bonjour et bienvenue danns notre bibliothèque !\n");
    printf("Quelle recherche souhaitez-vous faire ? \n(0. exit 1. Une référence, 2. Des mots clés, 3. Un nom d'auteur et un genre littéraire, 4. Un nom d'auteur)\n");
    int typeRecherche;
    scanf("%d", &typeRecherche);
    
    // controle de saisie
    // si c'est un chiffre accepté : on sort, sinon on redemande 3 fois avant de sortir
    // 0 est un chiffre qui ferme la connexion au serveur
    bool valide = typeRecherche >0 && typeRecherche<5;
    int compteur = 0;
    while (compteur < 4 && !valide){
        // lorsqu'on rentre 0 -> exit 
        if(typeRecherche == 0 || compteur == 3){
            exit(0);
        }
        printf("Attention, nous attendons un chiffre compris entre 0 et 4\n");
        printf("Que souhaitiez-vous rentrer ?\n");
        scanf("%d", &typeRecherche);

        valide = typeRecherche >0 && typeRecherche<5;
    }

    // écriture de message du type de recherche
    int *type;
    type = &typeRecherche;
    printf("j'envoie : %d\n",typeRecherche);
    write(resSocket, type, sizeof(int));
   
    // Message de validation du serveur
    int messOk;
    read(resSocket, &messOk, sizeof(int));
    if(messOk != 0){
        printf("Une erreur s'est produite\n");
        exit(-1);
    }

    printf("j'ai recu ça du serveur : %d\n", messOk);


    // mots recherchés
    char *mots = malloc(sizeof(char)*tailleMessage);
    printf("Que souhaitez-vous rechercher ?\n");
    scanf("%s", mots);

    // envoie de la requete au serveur
    write(resSocket, mots, sizeof(char)*tailleMessage);

    char messReponse[tailleMessage];
    read(resSocket, &messReponse, sizeof(char)*tailleMessage);
    printf("message : %s\n", messReponse);

    printf("fin\n");
    return 0;
}