// This is the echo SERVER server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "util.h"

// Define variables:
struct sockaddr_in  server_addr, client_addr, name_addr;
struct hostent *hp;

int  mysock, client_sock;              // socket descriptors
int  serverPort;                     // server port number
int  r, length, n;                   // help variables
char root[MAX];

void add_root(char *path)
{
	char temp[MAX];
	if(path[0] == '/')
	{
		strcpy(temp, root);
		strcat(temp, path);
		strcpy(path, temp);
	}
}

void ls_file(char *file)
{
  int i = 0, n;
  char mods[3] = { 'x', 'w', 'r' }, temp[MAX], buf[MAX], name[MAX], pathname[MAX];
  struct stat st;

  lstat(file, &st);
  strcpy(pathname, file);

  // print mode
  if (S_ISREG(st.st_mode))
	  file[i++] = '-';
  else if(S_ISDIR(st.st_mode))
	  file[i++] = 'd';
  else if(S_ISLNK(st.st_mode))
	  file[i++] = 'l';

  for(n = 8; n >= 0; n--)
  {
	  if(st.st_mode & (1 << n))
		  file[i++] = mods[i % 3];
	  else
		  file[i++] = '-';
  }

  file[i] = 0;

  // links, owners, size, and access time
  strcpy(temp, ctime(&st.st_atime));
  temp[strlen(temp) - 1] = 0;
  sprintf(buf, " %4lu %4d %4d %4ld %s ", st.st_nlink, st.st_uid, st.st_gid, st.st_size, temp);

  strcat(file, buf);
  // print name
  strcpy(temp, pathname);
  strcat(file, basename(temp));

  if(S_ISLNK(st.st_mode))
  {
	  i = readlink(pathname, temp, MAX);
	  temp[i] = 0;
	  sprintf(buf, " -> %s", temp);
	  strcat(file, buf);
  }
}

void ls_dir(char *dirname)
{
  struct dirent *ep;
  DIR *dp = opendir(dirname);
  char temp[MAX];

  while(ep = readdir(dp))
  {
	strcpy(temp, ep->d_name);
  	ls_file(temp);
	write(client_sock, temp, MAX);
  }
}

void ls(char *name)
{
  struct stat st;

  if(!strcmp(name, ""))
	  strcpy(name, ".");

  stat(name, &st);

  if(S_ISDIR(st.st_mode))
	  ls_dir(name);
  else
  {
	  ls_file(name);
	  write(client_sock, name, MAX);
  }

  write(client_sock, "STOP", MAX);
}

int find_command(char *cmd)
{
  if(!strcmp(cmd, "mkdir"))
	  return 1;
  if(!strcmp(cmd, "rmdir"))
	  return 2;
  if(!strcmp(cmd, "rm"))
	  return 3;
  if(!strcmp(cmd, "ls"))
	  return 4;
  if(!strcmp(cmd, "get"))
	  return 5;
  if(!strcmp(cmd, "put"))
	  return 6;
}

void execute_command(int cmd, char *path)
{
	char line[MAX], name[MAX];
	int size;

  switch(cmd)
  {
    case 1: if(!mkdir(path, 0777))
		    write(client_sock, "mkdir successful", MAX);
	    else
		    write(client_sock, "mkdir failed", MAX);
	    break;
    case 2: if(!rmdir(path))
		    write(client_sock, "rmdir successful", MAX);
	    else
		    write(client_sock, "rmdir failed", MAX);
	    break;
    case 3: if(!unlink(path))
		    write(client_sock, "rm successful", MAX);
	    else
		    write(client_sock, "rm failed", MAX);
	    break;
    case 4: printf("running ls...\n");
			ls(path);
	    break;
	case 5: upload(client_sock, path);
		break;
	case 6: read(client_sock, line, MAX);

			sscanf(line, "%d %s", &size, name);

			if(size >= 0)
				download(client_sock, name, size);
		break;
	default: write(client_sock, "command not implemented", MAX);
			 break;

  }
}

// Server initialization code:

int server_init(char *name)
{
   printf("==================== server init ======================\n");   
   // get DOT name and IP address of this host

   printf("1 : get and show server host info\n");
   hp = gethostbyname(name);
   if (hp == 0){
      printf("unknown host\n");
      exit(1);
   }
   printf("    hostname=%s  IP=%s\n",
               hp->h_name,  inet_ntoa(*((struct in_addr*)hp->h_addr_list[0])));
  
   //  create a TCP socket by socket() syscall
   printf("2 : create a socket\n");
   mysock = socket(AF_INET, SOCK_STREAM, 0);
   if (mysock < 0){
      printf("socket call failed\n");
      exit(2);
   }

   printf("3 : fill server_addr with host IP and PORT# info\n");
   // initialize the server_addr structure
   server_addr.sin_family = AF_INET;                  // for TCP/IP
   server_addr.sin_addr.s_addr = htonl(INADDR_ANY);   // THIS HOST IP address  
   server_addr.sin_port = 0;   // let kernel assign port

   printf("4 : bind socket to host info\n");
   // bind syscall: bind the socket to server_addr info
   r = bind(mysock,(struct sockaddr *)&server_addr, sizeof(server_addr));
   if (r < 0){
       printf("bind failed\n");
       exit(3);
   }

   printf("5 : find out Kernel assigned PORT# and show it\n");
   // find out socket port number (assigned by kernel)
   length = sizeof(name_addr);
   r = getsockname(mysock, (struct sockaddr *)&name_addr, &length);
   if (r < 0){
      printf("get socketname error\n");
      exit(4);
   }

   // show port number
   serverPort = ntohs(name_addr.sin_port);   // convert to host ushort
   printf("    Port=%d\n", serverPort);

   // listen at port with a max. queue of 5 (waiting clients) 
   printf("5 : server is listening ....\n");
   listen(mysock, 5);
   printf("===================== init done =======================\n");
}


int main(int argc, char *argv[])
{
   char *hostname;
   char line[MAX];
   char cmd[64], pathname[MAX];


   if (argc < 2)
      hostname = "localhost";
   else
      hostname = argv[1];
 
   server_init(hostname); 

   // Try to accept a client request
   while(1){
     printf("server: accepting new connection ....\n"); 

     // Try to accept a client connection as descriptor newsock
     length = sizeof(client_addr);
     client_sock = accept(mysock, (struct sockaddr *)&client_addr, &length);
     if (client_sock < 0){
        printf("server: accept error\n");
        exit(1);
     }
     printf("server: accepted a client connection from\n");
     printf("-----------------------------------------------\n");
     printf("        IP=%s  port=%d\n", inet_ntoa(client_addr.sin_addr),
                                        ntohs(client_addr.sin_port));
     printf("-----------------------------------------------\n");

     // Processing loop: newsock <----> client
     while(1){
       n = read(client_sock, line, MAX);
       if (n==0){
           printf("server: client died, server loops\n");
           close(client_sock);
           break;
      }
      
      // show the line string
      printf("server: read  n=%d bytes; line=[%s]\n", n, line);
      
	  cmd[0] = 0;
      pathname[0] = 0;

	  sscanf(line, "%s %s", cmd, pathname);
	  add_root(pathname);

	  execute_command(find_command(cmd), pathname);

    }
 }
}
