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

    // Ce fichier est le serveur de la phase 1
    // auteurs : Charre Kyllian, Papeau Willem, Delagrange Alex
    // 
    // Paramètres d'appel : ./serveur numPort nomFichierTxt


// Fonction end_child - sert à la gestion eds signaux SIGCHLD
void end_child()
{
    wait(NULL);
}

// Fonction main - paramètres : taille tab, tab=[noPort, nomFichierTxt]
int main(int argc, char **argv)
{   // gestion des paramètres

    if (argc != 3){
        printf("Option non valide\n");
        return -1;
    }
    // On attribut le numéro de port (en int)
    int noPort = atoi(argv[1]);

    // Création de la socket d'écoute du serveur.
    int socketEcoute = socket(AF_INET, SOCK_STREAM, 0);
    // Gestion d'erreur de la socket d'écoute
    if (socketEcoute == -1)
    {
        perror("Socket");
        exit(-1);
    }

    // Création de la structure infoEcoute

    struct sockaddr_in infoEcoute;
    infoEcoute.sin_family = AF_INET;
    infoEcoute.sin_addr.s_addr = htonl(INADDR_ANY);
    infoEcoute.sin_port = htons(noPort);

    // Attachement de la socket d'écoute en utilisant la structure définie dans infoEcoute
    int valideBind = bind(socketEcoute, (struct sockaddr *)&infoEcoute, sizeof(struct sockaddr));
    // gestion d'erreur
    if (valideBind == -1)
    {
        perror("bind");
        exit(-1);
    }

    // Ouverture de service sur la socket d'écoute
    int valideListen = listen(socketEcoute, 10);
    if (valideListen == -1)
    {
        perror("listen");
        exit(-1);
    }
    
    // gestion des sigchld
    struct sigaction ac;
    ac.sa_handler = end_child;
    // Permet d'éviter l'interruption de "accept"
    ac.sa_flags = SA_RESTART;
    sigaction(SIGCHLD, &ac, NULL);

    // Création de la boucle infinie

    while (1)
    {   
        // Création de la structure infoClient pour stocker les données client
        unsigned int longueurInfoClient;
        struct sockaddr_in infoClient;
        
        // Acceptation d'une connexion sur la socket d'écoute, 
        // récupération des informations sur le client dans struct infoClient
        // et création de la socket de service.
        printf("En attente de connexion...\n");
        int socketService = accept(socketEcoute, (struct sockaddr *)&infoClient, &longueurInfoClient);
        // LA PRIMITIVE ACCEPT EST BLOQUANTE
        // gestion d'erreur accept ()
        if (socketService == -1)
        {
            perror("Erreur accept");
            exit(-1);
        }
        printf("Connexion établie\n");

        // Création du processus fils chargé de satisfaire les requêtes du client.
        switch (fork())
        {
            case -1:
                perror("Erreur fork");
                exit(-1);

            case 0:
                // Comportement du processus fils

                // on ferme la socket non utilisée
                close(socketEcoute);

                int message = 1;
                // boucle de communication avec le client
                while(message != 0){
                    
                    read(socketService, &message, sizeof(int));
                    write(socketService, &message, sizeof(int));
                    printf("messageage reçu : %d\n", message);

                    if (message == 0){
                        printf("Mort du fils\n");
                        break;
                    }
                }
                close(socketService);
                exit(0);

            default:
                // Comportement processus père

                // on ferme la socket non utilisée
                close(socketService);
                break;
        }
    }
    return 0;
}
