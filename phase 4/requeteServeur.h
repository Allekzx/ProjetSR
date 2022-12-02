#ifndef SERVEUR_H
#define SERVEUR_H

#define tailleMaxReponse 1000
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
#include "lecture.h"

void end_child();

void serveur(int noPort, char *nomFichierTxt);

#endif