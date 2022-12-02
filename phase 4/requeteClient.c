#include "requeteClient.h"

// Ce fichier est le code de la phase 3
// auteurs : Charre Kyllian, Papeau Willem, Delagrange Alex
//
// Paramètres d'appel : numPort nomServeur
//

// Paramètres : taille tab, tab=[noPort,nomServeur]
void client(int noPort, char *nomServeur)
{

    // création de la socket
    int resSocket;
    resSocket = socket(AF_INET, SOCK_STREAM, 0);

    // Gestion d'erreur sur socket
    if (resSocket == -1)
    {
        perror("Erreur socket");
        exit(-1);
    }

    // Création des types struct pour stocker les informations
    struct hostent *informationServeur = malloc(sizeof(struct hostent));
    struct sockaddr_in adserv;

    // Récupération des informations server dans informationServeur
    informationServeur = gethostbyname(nomServeur);

    adserv.sin_family = AF_INET;
    adserv.sin_port = htons(noPort);
    // copie de l'information h_addr de informationServeur dans s_addr de adserv
    memcpy(&adserv.sin_addr.s_addr, informationServeur->h_addr, sizeof(__u_long));

    // connexion au serveur
    int valideConnect;
    valideConnect = connect(resSocket, (struct sockaddr *)&adserv, sizeof(struct sockaddr));
    // Gestion d'erreur de connexion
    if (valideConnect == -1)
    {
        perror("Erreur connect");
        exit(-1);
    }
    printf("Connexion établie avec le serveur\n");

    // Boucle d'écriture de messages
    printf("\n");
    printf("Bonjour et bienvenue dans notre bibliothèque !\n");
    while (1)
    {

        // Initialise le type de la recherche
        int typeRecherche = 0;
        // Initialise la variable de la vérification
        int conditionBoucle = 1;

        // Contriendra la variable de retour du serveur
        //  0 : erreur
        //  1 : valide
        int messageValide;

        // Permet la discussion client vers le serveur
        int continuerConnection = 1;
        int finDeConnection = 0;

        do
        {
            printf("Quel type de recherche souhaitez-vous faire ?\n");
            printf("\n\t\t-1. Se déconnecter \n\t\t-2. Recherche par référence \n\t\t-3. Recherche par mots clés \n\t\t-4. Recherche un nom d'auteur et un genre littéraire \n\t\t-5. Recherche un nom d'auteur\n");
            char saisi;
            printf("Votre choix : ");
            scanf("%c", &saisi);
            // Vérification qu'il s'agit d'un nombre
            if (isdigit(saisi))
            {

                // On convertie le chiffre de char à int
                typeRecherche = atoi(&saisi);

                // On vérifie la cohérence avec le menu
                if ((typeRecherche > 0) && (typeRecherche < 6))
                {

                    // Permet d'acceder à la suite de la discussion avec le serveur
                    conditionBoucle = 0;
                }
                else
                {
                    printf("ATTENTION Le chiffre n'est pas valide\n");
                }
            }
            else
            {
                printf("ATTENTION Vous n'avez pas rentré un chiffree\n");
            }
        } while (conditionBoucle == 1);

        // Envoie au serveur quel type de recherche on souhaite
        write(resSocket, &typeRecherche, sizeof(int));

        // Si le client souhaite quitter le service
        if (typeRecherche == 1)
        {
            // Deconnexion avec le serveur
            printf("Fin de communication avec le serveur\n");
            break;
        }

        // Attend la réponse du serveur pour continuer (0 ou 1)
        read(resSocket, &messageValide, sizeof(int));
        if (messageValide == 0)
        {
            printf("Une erreur est survenue\n");
            break;
        }

        switch (typeRecherche)
        {
            // Réponse à une requête lié à une référence
        case 2:
            char *requete = malloc(sizeof(char) * tailleMaxRequete);
            printf("Quelle reference cherchez vous ?\n>>>>> ");
            scanf("%s", requete);

            // Envoie la taille de la requete
            int longueurRequete = strlen(requete);
            write(resSocket, &longueurRequete, sizeof(int));

            // Attente de validation du serveur
            read(resSocket, &messageValide, sizeof(int));
            if (messageValide == 0)
            {
                printf("une erreur est surevenue sur l'envoie de la taille de la requete\n");
            }

            // Envoie de la requete au serveur
            write(resSocket, requete, sizeof(char) * longueurRequete);

            // Attend la réponse du serveur pour continuer (0 ou 1)
            messageValide = 0;
            read(resSocket, &messageValide, sizeof(int));
            if (messageValide == 0)
            {
                printf("Une erreur est survenue lors de la réponse du serveur\n");
                printf("Suite à la l'envoie de la reference choisi\n");
                break;
            }

            // Reception de la taille de la réponse attendue
            int tailleReponse;
            read(resSocket, &tailleReponse, sizeof(int));

            // On vérifie si le méssage n'est pas vide
            if (tailleReponse == 0)
            {

                printf("Une erreur est survenue lors de la reception de la taille de la réponse\n");
                write(resSocket, &finDeConnection, sizeof(int));
                break;
            }
            else
            {

                // On envoie au serveur que tout s'est bien passé
                write(resSocket, &continuerConnection, sizeof(int));
                printf("check bien passé apres taille reponse\n");
            }

            // Création de la variable contenant la réponse du serveur
            char *reponse = malloc(sizeof(char) * tailleReponse);

            printf("on attend la reponse du serveur ...\n");
            // Reception de la réponse du serveur
            read(resSocket, reponse, tailleReponse);

            // On informe le serveur de la bonne réception de la requête
            write(resSocket, &continuerConnection, sizeof(int));

            char *separateur = ",";

            char *result[1000] = {"Auteur : ", "Titre : ", "Genre : ", "Nombre de pages : "};
            int i = 0;
            char *strToken = strtok(reponse, separateur);
            printf("\n\n------------ REPONSE DU SERVEUR ------------\n\n");

            // Affichage de la réponse
            while (strToken != NULL)
            {

                printf("%s", result[i]);
                i++;
                printf("%s\n", strToken);

                // On demande le token suivant.
                strToken = strtok(NULL, separateur);
            }

            break;

        case 3:
            printf("\n\nLe cas 3 n'as pas été géré\n\n");
            // MOT CLES A FAIRE
            break;

        case 4:
            printf("\n\nLe cas 4 n'as pas été géré\n\n");
            // Nom auteur et genre littéraire A FAIRE
            break;
        case 5:
            printf("\n\nLe cas 5 n'as pas été géré\n\n");
            // Nom d'auteur A FAIRE
            break;

        default:
            break;
        }

        // Le client choisi s'il veut effectuer une nouvelle recherche

        int nouvelleRecherche = 0;
        do
        {
            printf("Souhaitez-vous refaire une recherche ? (0 pour non / 1 pour oui) : ");
            scanf("%d", &nouvelleRecherche);
            if (nouvelleRecherche != 1 || nouvelleRecherche != 0)
            {
                printf("\nLa réponse n'est pas valide\n");
            }

        } while (nouvelleRecherche != 1 && nouvelleRecherche != 0);

        // Le client avertis le serveur s'il veux continuer ou non
        write(resSocket, &nouvelleRecherche, sizeof(int));
        if (nouvelleRecherche == 0)
        {
            printf("Pas de nouvelle recherche\n");
            break;
        }
    }
    printf("\n------------Fin de la communication avec le serveur------------\n");
    // Fermeture du socket avec le serveur
    close(resSocket);
    exit(0);
}
