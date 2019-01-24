// The echo client client.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "util.h"


// Define variables
struct hostent *hp;              
struct sockaddr_in  server_addr; 

int server_sock, r;
struct in_addr SERVER_IP;
int SERVER_PORT; 


// clinet initialization code

int client_init(char *argv[])
{
  printf("======= client init ==========\n");

  printf("1 : get server info\n");
  hp = gethostbyname(argv[1]);
  if (hp==0){
     printf("unknown host %s\n", argv[1]);
     exit(1);
  }

  SERVER_IP   = *((struct in_addr *)hp->h_addr_list[0]);
  SERVER_PORT = atoi(argv[2]);

  printf("2 : create a TCP socket\n");
  server_sock = socket(AF_INET, SOCK_STREAM, 0);
  if (server_sock<0){
     printf("socket call failed\n");
     exit(2);
  }

  printf("3 : fill server_addr with server's IP and PORT#\n");
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = SERVER_IP.s_addr;
  server_addr.sin_port = htons(SERVER_PORT);

  // Connect to server
  printf("4 : connecting to server ....\n");
  r = connect(server_sock,(struct sockaddr *)&server_addr, sizeof(server_addr));
  if (r < 0){
     printf("connect failed\n");
     exit(1);
  }

  printf("5 : connected OK to \007\n"); 
  printf("---------------------------------------------------------\n");
  printf("hostname=%s  IP=%s  PORT=%d\n", 
          hp->h_name, inet_ntoa(SERVER_IP), SERVER_PORT);
  printf("---------------------------------------------------------\n");

  printf("========= init done ==========\n");
}

int main(int argc, char *argv[ ])
{
  int n, size;
  char line[MAX], temp[MAX], cmd[64], fullpath[MAX], name[MAX];
  cmd[0] = 0;
  fullpath[0] = 0;

  if (argc < 3){
     printf("Usage : client ServerName SeverPort\n");
     exit(1);
  }

  client_init(argv);
  // sock <---> server
  printf("********  processing loop  *********\n");
  while (1){
    printf("Input command: ");
    bzero(line, MAX);                // zero out line[ ]
    fgets(line, MAX, stdin);         // get a line (end with \n) from stdin

    line[strlen(line)-1] = 0;        // kill \n at end
    if (line[0]==0)                  // exit if NULL line
       exit(0);

	if(!strncmp(line, "get", 3))
	{
		// Send command to server and await feedback in the form of "#bytes filename"
		write(server_sock, line, MAX);
		read(server_sock, line, MAX);

		// Parse file information
		sscanf(line, "%d %s", &size, name);

		// Download file
		if(size >= 0)
			download(server_sock, name, size);
	}
	else if(!strncmp(line, "put", 3))
	{
		strcpy(temp, line);
		sscanf(temp, "%s %s", cmd, fullpath);
		
		// Send command to prep server
		write(server_sock, cmd, 64);

		// Upload file
		upload(server_sock, fullpath);
	}
	else if(!strncmp(line, "ls", 2))
	{
		// Send command to server
		write(server_sock, line, MAX);

		strcpy(line, "Perfoming ls");

		// Keep reading lines until a stop message is given
		while(strcmp(line, "STOP"))
		{
			printf("%s\n", line);

			read(server_sock, line, MAX);
		}
	}
	else
	{
		// Send command to server and receive feedback
		write(server_sock, line, MAX);
		read(server_sock, line, MAX);
		printf("%s\n", line);
	}
  }
}
