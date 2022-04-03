#include "server.h"

int startServer(int port)
{
  int opt = TRUE;
  int master_socket, addrlen, new_socket, client_socket[30],
      max_clients = 30, activity, i, valread, sd, client_count = 0, min_client_count = 0;
  int max_sd;
  struct sockaddr_in address;

  int numberOfBoats = 5;

  char buffer[1025]; // data buffer of 1K
  char map[10][10] = {
      "OOOOOOOOOO",
      "OOOOOOOOOO",
      "OOOOOOOOOO",
      "OOOOOOOOOO",
      "OOOOOOOOOO",
      "OOOOOOOOOO",
      "OOOOOOOOOO",
      "OOOOOOOOOO",
      "OOOOOOOOOO",
      "OOOOOOOOOO"};

  // set of socket descriptors
  fd_set readfds;
  int bombs[10][10] = {0};

  key_t key = ftok("shmfile", 65);

  // shmget returns an identifier in shmid
  int shmid = shmget(key, 1024, 0666 | IPC_CREAT);

  renderMap(map, 10, 10);

  printf("Nombre de participants minimum: ");
  scanf("%d", &min_client_count);
  fflush(stdin);

  printf("Nombre de bateaux a placer: ");
  scanf("%d", &numberOfBoats);
  fflush(stdin);
  printf("Veuillez placer vos bateaux\n");

  int x, y;

  for (size_t i = 0; i < numberOfBoats; i++)
  {
    printf("Position du bateau: %d\n", i + 1);
    printf("x: ");
    scanf("%d", &x);
    fflush(stdin);
    printf("y: ");
    scanf("%d", &y);
    fflush(stdin);

    map[y][x] = 'B';
    bombs[y][x] = 1;
    renderMap(map, 10, 10);
  }

  int choice = 0;

  printf("\nVoici votre carte final: \n");
  renderMap(map, 10, 10);
  printf("\n\t0 - Quitter\n\t1 - Lancer la partie\n\t2 - Editer la carte\n\t3 - Voir la liste des participants\n\t4 - Configurer le serveur\n\n: ");
  scanf("%d", &choice);

  resetMap(map, 10, 10);

  switch (choice)
  {
  case 0:
    printf("\nFermeture du serveur");
    for (size_t i = 0; i < 6; i++)
    {
      printf(".");
      usleep(500000);
      fflush(stdout);
    }
    printf("\nA bientot !\n\n");
    return EXIT_SUCCESS;

    break;

  default:
    renderMap(map, 10, 10);

    break;
  }

  // a message
  char *message = "Welcome to the server !! \r\n";

  // initialise all client_socket[] to 0 so not checked
  for (i = 0; i < max_clients; i++)
  {
    client_socket[i] = 0;
  }

  // create a master socket
  if ((master_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0)
  {
    perror("socket failed");
    exit(EXIT_FAILURE);
  }

  // set master socket to allow multiple connections ,
  // this is just a good habit, it will work without this
  if (setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt,
                 sizeof(opt)) < 0)
  {
    perror("setsockopt");
    exit(EXIT_FAILURE);
  }

  // type of socket created
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(port);

  // bind the socket to localhost port 8888
  if (bind(master_socket, (struct sockaddr *)&address, sizeof(address)) < 0)
  {
    perror("bind failed");
    exit(EXIT_FAILURE);
  }
  printf("Listener on port %d \n", PORT);

  // try to specify maximum of 3 pending connections for the master socket
  if (listen(master_socket, 3) < 0)
  {
    perror("listen");
    exit(EXIT_FAILURE);
  }

  // accept the incoming connection
  addrlen = sizeof(address);
  puts("Waiting for connections ...");

  while (TRUE)
  {
    // clear the socket set
    FD_ZERO(&readfds);

    // add master socket to set
    FD_SET(master_socket, &readfds);
    max_sd = master_socket;

    // add child sockets to set
    for (i = 0; i < max_clients; i++)
    {
      // socket descriptor
      sd = client_socket[i];

      // if valid socket descriptor then add to read list
      if (sd > 0)
        FD_SET(sd, &readfds);

      // highest file descriptor number, need it for the select function
      if (sd > max_sd)
        max_sd = sd;
    }

    // wait for an activity on one of the sockets , timeout is NULL ,
    // so wait indefinitely
    activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);

    if ((activity < 0) && (errno != EINTR))
    {
      printf("select error");
    }

    // If something happened on the master socket ,
    // then its an incoming connection
    if (FD_ISSET(master_socket, &readfds))
    {
      if ((new_socket = accept(master_socket,
                               (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
      {
        perror("accept");
        exit(EXIT_FAILURE);
      }

      // inform user of socket number - used in send and receive commands
      printf("Nouvelle connexion , socket fd: %d , ip: %s , port: %d\n ", new_socket, inet_ntoa(address.sin_addr), ntohs(address.sin_port));
      client_count++;
      if (client_count < min_client_count)
      {
        printf("%d connexion(s) restante afin de pouvoir demarrer la partie\n", min_client_count - client_count);
      }

      // send new connection greeting message
      if (send(new_socket, map, strlen(map), 0) != strlen(map))
      {
        perror("send");
      }

      puts("Welcome message sent successfully");

      // add new socket to array of sockets
      for (i = 0; i < max_clients; i++)
      {
        // if position is empty
        if (client_socket[i] == 0)
        {
          client_socket[i] = new_socket;
          printf("Adding to list of sockets as %d\n", i);

          break;
        }
      }
    }

    // else its some IO operation on some other socket
    for (i = 0; i < max_clients; i++)
    {
      sd = client_socket[i];

      if (FD_ISSET(sd, &readfds))
      {
        // Check if it was for closing , and also read the
        // incoming message
        if ((valread = read(sd, buffer, 1024)) == 0)
        {
          // Somebody disconnected , get his details and print
          getpeername(sd, (struct sockaddr *)&address,
                      (socklen_t *)&addrlen);
          printf("Host disconnected , ip %s , port %d \n",
                 inet_ntoa(address.sin_addr), ntohs(address.sin_port));
          client_count--;

          // Close the socket and mark as 0 in list for reuse
          close(sd);
          client_socket[i] = 0;
        }
        else if (client_count >= min_client_count)
        {
          int x = buffer[0] - '0';
          int y = buffer[1] - '0';
          if (bombs[y][x] == 1)
          {
            map[y][x] = 'B';
            bombs[y][x] = 2;
            printf("Bomb discovered at position (%d, %d)\n", x, y);
          }
          else if (bombs[y][x] == 2)
          {
            printf("This bomb has already been discovered...\nTry again\n");
          }
          else
          {

            map[y][x] = 'X';
          }

          // send(sd, "A votre tour de jouer: \n\0", 26, 0);
          send(sd, map, 1024, 0);
        }
        else
        {
          // send(sd, "La partie n'a pas commencer, en attente de joueur.\n\0", 53, 0);
          send(sd, map, 1024, 0);
        }
        memset(buffer, 0, sizeof(buffer));
      }
    }
  }

  return 0;
}

void charToSymbol(char c, char *buffer)
{
  if (buffer == NULL)
    return;

  switch (c)
  {
  case 'O':
    strncpy(buffer, "🔲", 10);
    break;
  case 'X':
    strncpy(buffer, "🔴", 10);
    break;
  case 'B':
    strncpy(buffer, "💥", 10);
    break;
  default:
    break;
  }
}

void renderMap(const char map[10][10], size_t width, size_t height)
{
  if (map == NULL)
  {
    printf("There is no map to render.\n");
    return;
  }

  char s[10] = {0};
  for (size_t i = 0; i < width; i++)
  {
    for (size_t j = 0; j < height; j++)
    {
      charToSymbol(map[i][j], s);
      printf("%s", s);
    }
    printf("\n");
  }
}

void resetMap(char map[10][10], size_t width, size_t height)
{
  if (map == NULL)
  {
    printf("There is no map to reset.\n");
    return;
  }

  for (size_t i = 0; i < width; i++)
  {
    for (size_t j = 0; j < height; j++)
    {
      map[i][j] = 'O';
    }
  }
}