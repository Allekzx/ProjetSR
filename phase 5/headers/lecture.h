#ifndef LECTURE_H
#define LECTURE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N 255
#define MAX_NombreDeLigneDansAnnexe 19
#define NB_CARACTERISTIQUE 6

typedef struct tableau{
	char *token;
}ts_tableau[MAX_NombreDeLigneDansAnnexe][NB_CARACTERISTIQUE];

int parseDocument( ts_tableau *livre, char* annexe);

#endif