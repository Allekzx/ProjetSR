#include "requeteServeur.h"
#include "lecture.h"

int main(int argc, char **argv)
{   // gestion des paramètres

    if (argc != 3){
        printf("Option non valide\n");
        return -1;
    }
    // On attribut le numéro de port (en int)
    int noPort = atoi(argv[1]);
    char *nomFichierTxt = argv[2];

    // appel du serveur
    serveur(noPort, nomFichierTxt);

}