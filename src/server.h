#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <errno.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#define NBCLI 3
#define PORT 3550
#define ADDRESS "127.0.0.1"

#define TRUE 1
#define FALSE 0
//==============================================================
//  SERVEUR MULTICLIENT (CHAQUE CLIENT EST DS UNE MACHINE      =
//           DISTINCTE DU SERVEUR)                             =
//  L'adresse IP du serveur est dans ADRESSE le port dans PORT =
//==============================================================

int startServer(int port);
void charToSymbol(char c, char *buffer);

void renderMap(const char map[10][10], size_t width, size_t height);
void resetMap(char map[10][10], size_t width, size_t height);