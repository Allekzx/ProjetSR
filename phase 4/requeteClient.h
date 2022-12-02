#ifndef CLIENT_H
#define CLIENT_H

#define tailleMaxRequete 1000

#include "stdio.h"
#include "stdlib.h"
#include <sys/types.h>          
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <ctype.h>

void client(int noPort, char *nomServeur);

#endif
