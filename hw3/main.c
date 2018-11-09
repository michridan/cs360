/* 
 * MiSH: Shell simulator by Michael Dandrea
 * v0.01
 * Last Updated: 9/26/18
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>	// for O_RDONLY, O_WRONLY, O_APPEND...
#include <unistd.h> // for chdir()
#include <errno.h>

char bins[20][128];
char line[128];
char cwd[128];
char HOME[128];

void cd(char *path);
int newProcess(char *line, char *env[]);

int main(int argc, char *argv[], char *env[])
{
	char *myargv[64], temp[128], cmd[128], args[128], *ptemp;
	int i = 0, j = 0, pid, status;

	//	Get Path from env
	while(env[i])
	{
		printf("env[%d]: %s\n", i, env[i]);

		if(!strncmp(env[i], "PATH=", 5))
		{
			printf("LOCATING BINARIES...\n");
			strcpy(line, env[i]);

			strtok(line, "=");

			while(ptemp = strtok(0, ":"))
			{
				strcpy(bins[j], ptemp);
				printf("%d: %s\n", j, bins[j]);
				j++;
			}
			strcpy(bins[j], ""); // Null out end of array to get a stopping point
		}
		else if(!strncmp(env[i], "HOME=", 5))
		{
			strcpy(line, env[i]);

			strtok(line, "=");

			strcpy(HOME, strtok(0, "="));
		}

		i++;
	}

	
	while(1)
	{
		//	Prompt for input
		getcwd(cwd, 128);
		printf("MiSH @ %s: ", cwd);
		fgets(line, 128, stdin);
		line[strlen(line) - 1] = 0;

		strcpy(args, "");
		//	Parse Command
		strcpy(temp, line);

		//	Handle "cd" and "exit"
		if(!strncmp(temp, "cd", 2))
		{
			sscanf(temp, "%s %s", cmd, args);
			cd(args);
		}
		else if(!strncmp(temp, "exit", 4))
			exit(0);
		else
		{
			//	Send it off
			newProcess(temp, env);
		}

	}
	return 0;
}

void cd(char *path)
{
	int i;

	if(!strcmp(path, ""))
		i = chdir(HOME);
	else
		i = chdir(path);

	if(i == -1)
	{
		printf("ERROR: %s\n", strerror(errno));
	}
}

int newProcess(char *line, char *env[])
{
	char *argv[64], temp[128], *templ, *tempr, cpy[128];
	int i, redirect = 0, pid = 0, pidch = 0, status, pd[2];

	strcpy(cpy, line);
	//	Look for a pipe
	templ = strtok(cpy, "|");
	tempr = strtok(0, "\0");

	//	Fork a child, let it find out what to do, get the exit code
	pid = fork();

	if(pid < 0)
		fprintf(stderr, "ERROR: Process failed to fork");
	else
	{
		
		if(strcmp(templ, line)) //	meaning there was a pipe
		{
			if(pid)
			{
				pid = waitpid(pid, &status, 0);
			}
			else
			{
				pipe(pd);

				//	child forks another child to act as reader
				pidch = fork();

				if(!pidch) //	the new child needs to do the later part
				{
					//	redirect to write pipe
					close(pd[0]); // WRITER MUST close pd[0]

					close(1);     // close 1
					dup(pd[1]);   // replace 1 with pd[1]
					close(pd[1]); // close pd[1] since it has replaced 1
					//	find and execute args
					argv[0] = strtok(templ, " ");
					i = 1;
	
					while(argv[i] = strtok(0, " "))
					{
						i++;
					}
					argv[++i] = 0;
					//	Look for redirection
					for(i = 0; !redirect && argv[i]; i++)
					{
						if(!strcmp(argv[i], ">")) //	Write
						{
							close(1);
							open(argv[i + 1], O_WRONLY | O_CREAT, 0666);
							argv[i] = 0;
							redirect = 1;
						}
						else if(!strcmp(argv[i], "<")) //	Read
						{
							close(0);
							open(argv[i + 1], O_RDONLY);
							argv[i] = 0;
							redirect = 1;
						}
						else if(!strcmp(argv[i], ">>")) //	Append
						{
							close(1);
							open(argv[i + 1], O_WRONLY | O_APPEND | O_CREAT, 0666);
							argv[i] = 0;
							redirect = 1;
						}
					}
		
					fprintf(stderr, "Searching for command \"%s\"\n", argv[0]);
					for(i = 0; strcmp(bins[i], ""); i++)
					{
						strcpy(temp, bins[i]);
						strcat(temp, "/");
						strcat(temp, argv[0]);

						fprintf(stderr, "\ttesting %s\n", temp);
						execve(temp, argv, env);				
					}
					fprintf(stderr, "\"%s\" not found\n", argv[0]);
					exit(100);
				}
				else
				{
					//	redirect to read pipe
					close(pd[1]); // READER MUST close pd[1]

					close(0);  
					dup(pd[0]);   // replace 0 with pd[0]
					close(pd[0]); // close pd[0] since it has replaced 0
					//	recursively execute the rest of the command
					newProcess(tempr, env);
					// Kill the child
					exit(100);
				}
			}
		}
		else //	Meaning no pipe
		{
			if(pid)
			{
				pid = waitpid(pid, &status, 0);
			}
			else
			{
				argv[0] = strtok(templ, " ");
				i = 1;

				while(argv[i] = strtok(0, " "))
				{
					i++;
				}
				argv[++i] = 0;
				//	Look for redirection
				for(i = 0; !redirect && argv[i]; i++)
				{
					if(!strcmp(argv[i], ">")) //	Write
					{
						close(1);
						open(argv[i + 1], O_WRONLY | O_CREAT, 0666);
						argv[i] = 0;
						redirect = 1;
					}
					else if(!strcmp(argv[i], "<")) //	Read
					{
						close(0);
						open(argv[i + 1], O_RDONLY);
						argv[i] = 0;
						redirect = 1;
					}
					else if(!strcmp(argv[i], ">>")) //	Append
					{
						close(1);
						open(argv[i + 1], O_WRONLY | O_APPEND | O_CREAT, 0666);
						argv[i] = 0;
						redirect = 1;
					}
				}
		
				fprintf(stderr, "Searching for command \"%s\"\n", argv[0]);
				for(i = 0; strcmp(bins[i], ""); i++)
				{
					strcpy(temp, bins[i]);
					strcat(temp, "/");
					strcat(temp, argv[0]);

					fprintf(stderr, "\ttesting %s\n", temp);
					execve(temp, argv, env);				
				}
				fprintf(stderr, "\"%s\" not found\n", argv[0]);
				exit(100);
				}
		}
	}
}
