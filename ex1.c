#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/stat.h>



int main(int argc, char **argv)
{
	struct stat file;
	if(argc != 2)
	{
		printf("Usage ./a.out filename\n");
		return 1;
	}
	else
	{
		if(strcmp(argv[1], "--help") == 0)
		{
			printf("Usage ./a.out filename\n");
			return 0;
		}
		else if(stat(argv[1], &file) == 0)
		{
			printf("Error: %s already exists\n", argv[1]);
			return 1;
		}
	}
	int fd = open(argv[1], O_CREAT | O_APPEND | O_WRONLY | O_RDONLY, 0644);
	if(fd == -1)
	{
		perror("open");
		return 1;
	}
	int status;
	pid_t child;
	child = fork();
	if(child < 0)
	{
		printf("Error in generating child program");
		return 1;
	}
	if(child==0)
	{
		pid_t child_childid = getpid();
		pid_t child_parentid = getppid();
		char printchild[50];
		sprintf(printchild, "[CHILD] getpid()= %d, getppid()= %d\n",child_childid, child_parentid);

		if(write(fd, printchild,strlen(printchild)) < strlen(printchild))
		{
			perror("write");
			return 1;
		}
		exit(0);
	}
	else
	{
		pid_t parent_childid = getpid();
		pid_t parent_parentid = getppid();
		char printparent[50];
		sprintf(printparent, "[PARENT] getpid()= %d, getppid()= %d\n", parent_childid, parent_parentid);
		if(write(fd, printparent, strlen(printparent)) < strlen(printparent))
		{
			perror("write");
			return 1;
		}
		close(fd);
		if(close < 0)
		{
			perror("close error");
			return 1;
		}
		wait(&status);
		exit(0);
	}
	return 0;
}


