#include "requeteClient.h"

int main(int argc, char **argv){
    // gestion des paramètres
    if (argc != 3){
        printf("Option non valide");
        return -1;
    }
    int noPort =atoi(argv[1]);
    char *nomServeur = argv[2];
    
    // appel du client
    client(noPort, nomServeur);

}