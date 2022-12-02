#include "../headers/requeteServeur.h"
#include "../headers/lecture.h"
#include <stdlib.h>

// Ce fichier est le serveur de la phase 4
// auteurs : Charre Kyllian, Papeau Willem, Delagrange Alex
//
// Paramètres d'appel : ./serveur numPort nomFichierTxt

// Fonction end_child - sert à la gestion eds signaux SIGCHLD
void end_child()
{
    wait(NULL);
}

// Fonction main - paramètres : taille tab, tab=[noPort, nomFichierTxt]
void serveur(int noPort, char *nomFichierTxt)
{ 

    // Création de la socket d'écoute du serveur.
    int bOptVal = 1;
    // Création de la socket d'écoute du serveur.
    int socketEcoute = socket(AF_INET, SOCK_STREAM, 0);
    
    // Permet un redemarrage rapide du serveur
    // Résout "Adresse already in use"
    setsockopt(socketEcoute, SOL_SOCKET, SO_REUSEADDR, &bOptVal, sizeof(bOptVal));

    // Création du livre par l'appel au parseur de l'annexe
    ts_tableau *livre = (ts_tableau *)malloc(sizeof(ts_tableau) * MAX_NombreDeLigneDansAnnexe);
    int valideParse = parseDocument(livre, nomFichierTxt);
    if (valideParse == -1)
    {
        exit(-1);
    }

    // Gestion d'erreur de la socket d'écoute
    if (socketEcoute == -1)
    {
        perror("Erreur socket ");
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
        perror("Erreur bind");
        exit(-1);
    }

    // Ouverture de service sur la socket d'écoute
    int valideListen = listen(socketEcoute, 10);
    if (valideListen == -1)
    {
        perror("Erreur listen");
        exit(-1);
    }

    // Sestion des sigchld
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
        printf("En attente d'une connexion...\n");
        int socketService = accept(socketEcoute, (struct sockaddr *)&infoClient, &longueurInfoClient);
        // LA PRIMITIVE ACCEPT EST BLOQUANTE
        // gestion d'erreur accept ()
        if (socketService == -1)
        {
            perror("Erreur lors du Accept");
            exit(-1);
        }
        printf("\nConnexion établie avec un client !\n\n");

        // Création du processus fils chargé de satisfaire les requêtes du client.
        switch (fork())
        {
        case -1:
            perror("Erreur Fork ");
            exit(-1);

        case 0:
            /// On ferme la socket non utilisée (socket d'écoute)
            close(socketEcoute);

            // Contriendra la variable de retour du serveur
            //  0 : erreur
            //  1 : valide
            int messageValide;

            // Contient la valeur de retour du type de la recherche
            int typeRecherche = 1;

            // Permet la discussion serveur vers client
            int continuerConnection = 1;
            int finDeConnection = 0;
            

            // Boucle de communication avec le client
            while (1)
            {

                // Réception du type de la recherche du client
                read(socketService, &typeRecherche, sizeof(int));
                printf("Le type de la recherche du client est %d\n", typeRecherche);

                if (typeRecherche == 1)
                {
                    printf("Fin de communication avec le client\n");
                    break;
                }

                // Envoie de la bonne réception du message
                write(socketService, &continuerConnection, sizeof(int));

                switch (typeRecherche)
                {
                    //Réponse à une requête lié à une référence
                case 2:
                    //Attente de la taille de la requete
                    int longueurRequete;
                    read(socketService, &longueurRequete, sizeof(int));


                    //Envoie de  la bonne réception de la taille
                    write(socketService, &continuerConnection, sizeof(int));

                    //Variable qui contiendra la requete du client
                    char *requeteClient = malloc(sizeof(char) * longueurRequete);

                    //Lecture de la requete
                    printf("Lecture de la requête client ...\n");
                    read(socketService, requeteClient, longueurRequete);
                    
                    //Conversion de String à int pour la référence
                    int resultatReference = atoi(requeteClient);
                    printf("La référence choisit est %d\n", resultatReference);
                    
                    free(requeteClient);
                    //Permet de supprimer les caractères inconnus 
                    char messageString[255];
                    sprintf(messageString,"%d",resultatReference);

                    // Envoie de la bonne réception du message
                    write(socketService, &continuerConnection, sizeof(int));

                    int trouveOuNon = 0;
                    int colonne = 0;
                    char reponse[255];
                    char erreur[255] = "L'auteur n'a pas été trouvé car la reference n'est pas présente dans le fichier\n\nVeuillez réésayer :\n";

                    for (int i = 0; i < MAX_NombreDeLigneDansAnnexe; i++)
                    {
                        
                        if (strcmp(messageString, livre[i][0]->token) == 0)
                        {
                            //On trouve un message
                            trouveOuNon = 1;
                            colonne = i;
                        }
                    }

                    if (trouveOuNon == 0)
                    {
                        // Envoie de la taille de la reponse
                        int tailleMessage = strlen(erreur);
                        write(socketService, &tailleMessage, sizeof(int));

                        
                        // Vérifie que le client a bien recu la taille
                        read(socketService, &messageValide, sizeof(int));
                        if (messageValide == finDeConnection)
                        {
                            printf("Une erreur est survenue lors de la reception de la taille de la réponse chez le client\n");
                            break;
                        }

                        // Envoie du message
                        write(socketService, erreur, sizeof(erreur));

                        
                        // Vérifie que le client a bien recu le message
                        read(socketService, &messageValide, sizeof(int));
                        if (messageValide == finDeConnection)
                        {
                            printf("Une erreur est survenue lors de la reception de la reponse\n");
                            break;
                        }
                    }
                    else
                    {
                        //initialisation de la variable reponse
                        memset(reponse, 0, strlen(livre[colonne][1]->token));

                        //Composition de la reponse
                        strcat(strcpy(reponse, livre[colonne][1]->token), ",");
                        strcat(reponse, livre[colonne][2]->token);
                        strcat(reponse, ",");
                        strcat(reponse, livre[colonne][3]->token);
                        strcat(reponse, ",");

                        if (atoi(livre[colonne][4]->token) >= 300)
                        {
                            strcat(reponse, "Ce livre a plus de 300 pages\n");
                        }
                        else
                        {
                            strcat(reponse, "Ce livre a moins de 300 pages\n");
                        }

                        //Envoie de la taille du message
                        int tailleMessage = strlen(reponse);
                        write(socketService, &tailleMessage, sizeof(int));

                        // Vérifie que le client a bien recu la taille
                        read(socketService, &messageValide, sizeof(int));
                        if (messageValide == finDeConnection)
                        {
                            printf("Une erreur est survenue lors de la reception de la taille de la réponse chez le client\n");
                            break;
                        }

                        //Envoie de la reponse
                        write(socketService, reponse, tailleMessage);

                        
                        // Vérifie que le client a bien recu le message
                        read(socketService, &messageValide, sizeof(int));
                        if (messageValide == finDeConnection)
                        {
                            printf("Une erreur est survenue lors de la reception de la reponse\n");
                            break;
                        }
                    }
                    printf("Le client a recu une réponse à sa requête\n\n\n");

                case 3:
                    // MOT CLES A FAIRE
                    break;

                case 4:
                    // Nom auteur et genre littéraire A FAIRE
                    break;
                case 5:
                    // Nom d'auteur A FAIRE
                    break;

                default:
                    break;
                }

                // Le client souhaite faire une nouvelle requête ?
                int nouvelleRecherche;
                read(socketService, &nouvelleRecherche, sizeof(int));

                // Gestion du cas ou le client entre 0 = ne plus continuer
                // Fin de service
                if (!nouvelleRecherche)
                {
                    printf("Le client ne souhaite pas faire une nouvelle recherche\n");
                    break;
                }
                printf("Le client souhaite continuer à utiliser ce service ...\n");
            }
            printf("\n---------MORT DU FILS---------\n");
            close(socketService);
            exit(0);

        default:
            // Comportement processus père
            // On ferme la socket non utilisée
            close(socketService);
            break;
        }
    }
}
