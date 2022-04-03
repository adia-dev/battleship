#include "client.h"

int connectToServer(const char *address, int port)

{
   int sd;
   char buffer[1024] = {0};
   char message[1024] = {0};

   struct sockaddr_in dest_addr;

   if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
   {
      perror("\n Erreur socket : ");
      exit(1);
   }

   dest_addr.sin_family = AF_INET;
   dest_addr.sin_port = htons(port);
   dest_addr.sin_addr.s_addr = inet_addr(address);
   bzero(&(dest_addr.sin_zero), 8);
   if ((connect(sd, (struct sockaddr *)&dest_addr, sizeof(struct sockaddr))) == -1)
   {
      perror("\n Erreur connect : ");
      exit(2);
   }

   if (read(sd, buffer, 1024) == -1)
   {
      perror("\n Erreur read : ");
      exit(3);
   }

   for (size_t i = 0; i < 10; i++)
   {
      for (size_t j = 0; j < 10; j++)
      {
         char s[10] = {0};
         charToSymbol(buffer[i * 10 + j], s);
         printf("%s", s);
      }
      printf("\n");
   }
   memset(buffer, 0, sizeof(buffer));

   while (1)
   {

      memset(message, 0, sizeof(message));

      printf("message: ");
      fgets(message, 1024, stdin);
      size_t length = strlen(message);

      // if (message[length - 1] == '\n')
      message[2] = '\0';
      fflush(stdin);

      // send(sock, username, strlen(username), 0);
      // send(sock, ": ", strlen(": "), 0);
      send(sd, message, strlen(message), 0);
      printf("Message sent to the server !\n");

      // if (read(sd, buffer, 1024) == -1)
      // {
      //    perror("\n Erreur recv : ");
      //    exit(3);
      // }
      // printf("%s\n", buffer);

      // memset(buffer, 0, sizeof(buffer));
      // // buffer[0] = '\0';

      if (read(sd, buffer, 1024) == -1)
      {
         perror("\n Erreur recv : ");
         exit(3);
      }

      for (size_t i = 0; i < 10; i++)
      {
         for (size_t j = 0; j < 10; j++)
         {
            char s[10] = {0};
            charToSymbol(buffer[i * 10 + j], s);
            printf("%s", s);
         }
         printf("\n");
      }
      memset(buffer, 0, sizeof(buffer));

      // buffer[0] = '\0';
   }

   // on pourrait utiliser read(sd,buffer,6) au lieu de recv

   printf("\n %d %s\n", getpid(), buffer);
   // sleep(15);
   exit(4);
}
