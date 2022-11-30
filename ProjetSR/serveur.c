#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <netinet/in.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include <netinet/ip.h>
#include <stdlib.h>

#define noPort 2121
#define tailleMessage 255

    // Ce fichier est un serveur d'application client/serveur
    // dont le but est de modéliser une bibliothèque
    // nous retrouvons les fonctionnalités suivantes :

void end_child()
{
    wait(NULL);
}

int main(int argc, char **argv)
{

    // Création de la socket d'écoute du serveur.
    int desSockEcoute = socket(AF_INET, SOCK_STREAM, 0);
    // gestion d'erreur de socket d'écoute
    if (desSockEcoute == -1)
    {
        perror("Socket");
        exit(-1);
    }

    // Création de la structure infoEcoute pour accépter la connexion
    struct sockaddr_in *infoEcoute = (struct sockaddr_in *)malloc(sizeof(struct sockaddr_in));
    infoEcoute->sin_family = AF_INET;
    infoEcoute->sin_addr.s_addr = htonl(INADDR_ANY);
    infoEcoute->sin_port = htons(noPort);

    // Attachement de la socket d'écoute en utilisant la structure définie dans infoEcoute
    int resBind = bind(desSockEcoute, (struct sockaddr *)infoEcoute, sizeof(struct sockaddr));
    // gestion d'erreur
    if (resBind == -1)
    {
        perror("bind");
        exit(-1);
    }

    // Ouverture de service sur la socket d'écoute
    int resLis = listen(desSockEcoute, 10);
    if (resLis == -1)
    {
        perror("listen");
        exit(-1);
    }

    // variable longeur de struct infoEcoute
    unsigned int *lenAccept = malloc(sizeof(unsigned int));
    int descServ;

    // création du socket Client (stocker les infos qu'on connait)
    struct sockaddr_in *sClient = malloc(sizeof(struct sockaddr_in));
    sClient->sin_family = AF_INET;
    sClient->sin_addr.s_addr = htonl(INADDR_ANY);
    sClient->sin_port = htonl(noPort);

    // création de la socket serveur
    descServ = socket(AF_INET, SOCK_STREAM, 0);
    
    // gestion des sigchld
    struct sigaction ac;
    ac.sa_handler = end_child;
    // Permet d'éviter l'interruption de "accept"
    ac.sa_flags = SA_RESTART;
    sigaction(SIGCHLD, &ac, NULL);

    close(descServ);
    
    // import du tableau de données



    // Création de la boucle infinie
    
    while (1)
    {

       

        // Acceptation d'une connexion sur la socket d'écoute, récupération des informations sur le client et création de la socket de service.
        printf("Attente d'une connexion...\n");
        int desSockSer = accept(desSockEcoute, (struct sockaddr *)infoEcoute, lenAccept);
        // LA PRIMITIVE ACCEPT EST BLOQUANTE
        printf("Acceptation d'une connexion sur la socket d'écoute..\n");
        if (desSockSer == -1)
        {
            perror("Erreur accept");
            exit(-1);
        }

        // Création du processus fils chargé de satisfaire les requêtes du client.
        switch (fork())
        {
        case -1:
            perror("Erreur fork");
            exit(-1);
        case 0:
            // Comportement du processus fils (pour chaque client)
            close(desSockEcoute);

            // en attente de recevoir un message
            int *mess= malloc(sizeof(int));
            read(desSockSer, mess, sizeof(int));
            printf("MESS = %d\n", *mess);
            if(*mess == 0){
                exit(0);
            }
            // envoie du message bien reçu
            int val;
            val = 0;
            write(desSockSer, &val, sizeof(int));
            printf("Message : %d\n", val);

            // attente du deuxième message (aka la requete)
            char requete[tailleMessage];
            read(desSockSer, requete, sizeof(char)*tailleMessage);
            printf("j'ai reçu ça : %s\n", requete);

            // REPONSE A LA REQUETE KYKS
            char maReponse[tailleMessage] = "Réponse a ta requete cher client\n";
            printf("J'envoie ça :%s\n", maReponse);
            write(desSockSer, &maReponse, sizeof(char)*tailleMessage);
            printf("fin\n");
            exit(0);

        default:
            // Comportement processus père
            close(desSockSer);
            break;
        }
    }
    return 0;
}
