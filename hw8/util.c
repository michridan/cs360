/************ util.c **********
  This is starting as a collection
  of prelab functions, and I will
  be adding the other utility functions
  here as well, as long as they make
  sense to be here.
 ********************************/

void ls_file(char *filename)
{
  int i;
  char mods[3] = { 'x', 'w', 'r' }, *temp, buf[1024];
  struct stat st;

  lstat(filename, &st);

  // print mode
  if (S_ISREG(st.st_mode))
	  putchar('-');
  else if(S_ISDIR(st.st_mode))
	  putchar('d');
  else if(S_ISLNK(st.st_mode))
	  putchar('l');

  for(i = 8; i >= 0; i--)
  {
	  if(st.st_mode & (1 << i))
		  putchar(mods[i % 3]);
	  else
		  putchar('-');
  }

  // links, owners, size, and access time
  strcpy(temp, ctime(&st.st_atime));
  temp[strlen(temp) - 1] = 0;
  printf(" %4d %4d %4d %4d %s ", st.st_nlink, st.st_uid, st.st_gid, st.st_size, temp);

  // print name
  strcpy(temp, filename);
  printf("%s", basename(temp));
  if(S_ISLNK(st.st_mode))
  {
	  i = readlink(filename, buf, 1024);
	  buf[i] = 0;
	  printf(" -> %s", buf);
  }
}

void ls_dir(char *dirname)
{
  struct dirent *ep;
  DIR *dp = opendir(dirname);

  while(ep = readdir(dp))
  {
  	ls_file(ep->d_name);
	printf("<p>");
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
	  ls_file(name);
}

int find_command(char *cmd)
{
  if(!strcmp(cmd, "mkdir"))
	  return 2;
  if(!strcmp(cmd, "rmdir"))
	  return 3;
  if(!strcmp(cmd, "rm"))
	  return 4;
  if(!strcmp(cmd, "ls"))
	  return 6;
}

void execute_command(int cmd, char *path)
{
  switch(cmd)
  {
    case 2: if(!mkdir(path, 0777))
		    printf("%s created successfully", path1);
	    else
		    printf("mkdir failed");
	    break;
    case 3: if(!rmdir(path))
		   printf("%s removed successfully", path1);
	    else
		   printf("rmdir failed");
	    break;
    case 4: if(!unlink(path))
		    printf("%s removed successfully", path1);
	    else
		    printf("rm failed");
	    break;
    case 6: ls(path);
	    break;
  }
}

int upload(int sock, char *path)
{
}

int download(int sock, char *name, int size)
{
}
