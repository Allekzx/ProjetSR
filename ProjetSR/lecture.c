#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N 255
#define MAX_NombreDeLigneDansAnnexe 19

//Structure à 2 dimension où chaques cases pointe vers une chaine de caractère
//Elle est de taille [Nombre de livre dans l'annexe] par [nombre d'informations de chaques livre]
typedef struct s_data{
	char *token;
}ts_data[MAX_NombreDeLigneDansAnnexe][6];

int main(int argc, char *argv[])
{
    //Ouverture du fichier "Annexe"en lecture
    FILE* fichier = NULL;
    fichier = fopen("Annexe.txt", "r");
    
    //Compteur du livre courant
    int ligneCourante =0;

    //Allocation dynamique de l'espace nécessaire pour chaque ligne
    char * buffer = (char *)malloc(sizeof(char)*N);
    
    //Déclaration et initialisation du caractère de séparation entre chaques caractéristiques
    char * separateur = "#";

    /*Déclaration du tableau à 2 dimensions qui contiendra tous les livres
      ainsi que chaques caractéristiques de tous les livres
      
      Exemple : 
        Annexe :
            -10#Jack London#Aventures en mer#recits#1024#B
            -20#Moliere#Dom Juan#theatre#192#B
        
        livre[0][2].token = "Aventures en mer"
        livre[1][5].token = "B"
    */
    ts_data livre;
    
    //On entre dans le fichier ouvert en lecture
    if (fichier != NULL){

        //Tant que on a pas atteint la fin du fichier
        while (!feof(fichier)){

            //On récupère la 1er ligne qui deviendra la chaine de caractère courtante à traiter
            char * ligne =fgets(buffer, N, fichier);
            
            //Définition du caractère de séparation
            char * separator = "#";

            //On récupère tout ce qui se trouve entre chaques séparateurs
            char* strToken = strtok(ligne, separateur);

            //Compteur de la caractéristique
            int caracteristique =0;

            //Tant que la ligne n'est pas fini
            while(strToken !=NULL){

                livre[ligneCourante][caracteristique].token = (char*)calloc( N, sizeof(char) );
                if(  livre[ligneCourante][caracteristique].token == NULL ){
				perror("_ERREUR_ (ALLOCATION)\n");
				exit( EXIT_FAILURE );
			    }
                
                //affichage 
                printf("%s\n", strToken);
                strcpy(livre[ligneCourante][caracteristique].token, strToken);

                //on demande le token suivant
                strToken = strtok(NULL, separator);
                //incrémente le compteur pour positionner chaque valeur au bon endroit
                caracteristique++;
            }
            ligneCourante++;
        }
        
        printf("\n\n%s\n", livre[4][3].token);
        printf("%s\n", livre[1][2].token);
        printf("%s\n", livre[0][1].token);
       
    } else{
        perror("Problème ouverture fichier ");
    }

    fclose(fichier);
 
    return 0;


}