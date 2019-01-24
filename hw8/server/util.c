/************ util.c **********
  This is starting as a collection
  of prelab functions, and I will
  be adding the other utility functions
  here as well, as long as they make
  sense to be here.
 ********************************/
#include "util.h"

int upload(int sock, char *path)
{
	char data[MAX], temp[MAX];
	int n, fd, total = 0;
	// find file (-1 if not found)
	struct stat st;

	if(stat(path, &st))
	{
		sprintf(data, "%d %s", -1, "file not found");
		write(sock, data, MAX);
		printf("upload failed, file not found");
		return -1;
	}

	// check if REG (-1 if not)
	if(!S_ISREG(st.st_mode))
	{
		sprintf(data, "%d %s", -1, "not a regular file");
		write(sock, data, MAX);
		printf("upload failed, not a regular file");
	}

	// send file size and name
	strcpy(temp, path);
	sprintf(data, "%d %s", (int)st.st_size, basename(temp));
	write(sock, data, MAX);

	// Wait for go-ahead
	read(sock, data, MAX);

	if(strcmp(data, "GO"))
	{
		printf("Upload failed, %s\n", data);
		return -1;
	}

	// Send file permissions
	sprintf(data, "%d", st.st_mode);
	write(sock, data, MAX);

	// send 256 bits at a time, until size is reached
	printf("---Uploading file, size: %d bytes---\n", (int)st.st_size);
	fd = open(path, O_RDONLY);
	while(total < st.st_size)
	{
		n = read(fd, data, MAX);
		write(sock, data, n);
		total += n;
		printf("%d bytes sent, %d left\n", n, (int)st.st_size - total);
	}

	printf("Upload complete, %d bytes sent total\n", total);
	
	return 0;
}

int download(int sock, char *name, int size)
{
	char data[MAX];
	struct stat st;
	int n, fd, total = 0, mode;

	// Check if file exists and if so, if it is not REG
	if(!stat(name, &st) && !S_ISREG(st.st_mode))
	{
		printf("Dowlnload failed, file exists and is not a regular file\n");
		write(sock, "can't overwrite file", MAX);
		return -1;
	}

	write(sock, "GO", MAX);

	// Receive permissions and open file
	read(sock, data, MAX);
	sscanf(data, "%d", &mode);
	fd = open(name, O_WRONLY|O_CREAT, mode);

	// Read 256 bytes at a time, then write to a file until size is reached
	printf("---Downloading file, size: %d bytes---\n", size);
	while(total < size)
	{
		n = read(sock, data, MAX);
		write(fd, data, n);
		total += n;
		printf("%d bytes received, %d left\n", n, size - total);
	}

	printf("Download complete, %d bytes received total\n", total);
}
