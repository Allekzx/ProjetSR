#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N 255
#define MAX_NombreDeLigneDansAnnexe 19
#define NB_CARACTERISTIQUE 6

//Structure à 2 dimension où chaques cases pointe vers une chaine de caractère
//Elle est de taille [Nombre de livre dans l'annexe] par [nombre d'informations de chaques livre]
typedef struct tableau{
	char *token;
}ts_tableau[MAX_NombreDeLigneDansAnnexe][NB_CARACTERISTIQUE];




void parseDocument( ts_tableau *livre, char* annexe){
    //Ouverture du fichier "Annexe"en lecture
    FILE* fichier = NULL;
    fichier = fopen(annexe, "r");
    
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
    //On entre dans le fichier ouvert en lecture
    if (fichier != NULL){

        //Tant que on a pas atteint la fin du fichier
        while (!feof(fichier)){
   
            //On récupère la 1er ligne qui deviendra la chaine de caractère courtante à traiter
            char * ligne =fgets(buffer, N, fichier);
            //On récupère tout ce qui se trouve entre chaques séparateurs
            char* strToken = strtok(ligne, separateur);

            //Compteur de la caractéristique
            int caracteristique =0;
            
            //Tant que la ligne n'est pas fini
            while(strToken !=NULL){

                //calloc met tous les octets du bloc à la valeur 0 alors que malloc ne modifie pas la zone de mémoire
                //Permet donc d'initialiser chaque cases du tableau à 2D
                livre[ligneCourante][caracteristique]->token = (char*)calloc(N, sizeof(char));
                
                //On vérifie que chaques cases à bien été initialisée
                if(&livre[ligneCourante][caracteristique]->token == NULL ){
				    perror("_ERREUR_ (ALLOCATION)\n");
				    exit(EXIT_FAILURE);
			    }

                //Affecter à chaque valeur le token (caractéristique correspondante)
                strcpy(livre[ligneCourante][caracteristique]->token, strToken);
                
                //On prend la caractéristique suivante
                strToken = strtok(NULL, separateur);
        
                //Incrémente le compteur pour positionner chaque caractéristiques au bon endroit
                caracteristique++;
                
            }

            //On passe à la ligne suivante
            ligneCourante++;
        }
       
    } else{
        perror("Problème d'ouverture fichier ");
    }

    fclose(fichier);
 
    


}


int main(int argc, char *argv[])
{
    ts_tableau * livre =(ts_tableau *) malloc(sizeof(ts_tableau)*MAX_NombreDeLigneDansAnnexe);
    parseDocument(livre,"stock.txt");
    char*ref = malloc(sizeof(char*));
    printf("Pour quelle reference voulez vous les infos ?\n");
    scanf("%s",ref);
    printf("Vous avez demandé les infos pour le livre de référence %s :\n", ref);
    for(int i = 0; i < MAX_NombreDeLigneDansAnnexe;i++) {
        if(strcmp(ref,livre[i][0]->token)==0) {

            printf("Auteur : %s\n",livre[i][1]->token);
            printf("Titre : %s\n",livre[i][2]->token);
            printf("Genre : %s\n",livre[i][3]->token);
            
            if(atoi(livre[i][4]->token) >= 300) {
                printf("Ce livre a plus de 300 pages\n");
            } else {
                printf("Ce livre a moins de 300 pages\n");
            }
        }


    }

    /*Test de lecture et d'accecibilité par une fonction
    printf("%s", livre[0][0]->token);
    printf(" %s", livre[0][1]->token);
    printf(" %s", livre[0][2]->token);
    printf(" %s", livre[0][3]->token);
    printf(" %s", livre[0][4]->token);
    printf(" %s", livre[0][5]->token);*/

   

   return 0;
}