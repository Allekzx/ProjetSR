#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    char chaine[] = "Auteur,Livre,Page,Bibliothèque";

    // Le caractère qui sera considéré comme un séparateur
    char * separateur = ",";


    printf("Voici la forme que prendra notre résultat : %s\n\n",chaine);
    // On divise la chaîne en récupérant tout ce qui se trouve entre les séparateurs
    char * strToken = strtok( chaine, separateur );
    

    
    printf("Et voici comment sera l'affichage dans le client : \n\n");

    while(strToken!=NULL) {
        printf("%s",strToken);
        printf("\n");
        strToken = strtok(NULL, separateur);
    }

}