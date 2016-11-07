#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

int main(int argc, char **argv)
{

	if(argc != 2){
		printf("Mauvais nombre de paramètres.");
		return 1;
	}

	int descripteurPipe[2];
	pid_t pid = fork();

	if(pid == 0){
		
		printf("PID (%d)\n",getpid());
		close(1);

		int out = open("/tmp/proc-exercise", O_RDWR, 0600);

		if(errno == EEXIST){

			printf("%s",strerror(errno));
			exit(0);
		}

		dup2(1, out);


		printf("Numéro du descripteur de fichier : %d\n",out);

	} else {

		printf("PID : %d\n",getpid());
		
		wait(pid);

		printf("J'aime les gauffres au %s\n",argv[1]);
		fprintf(stderr,"Message quelconque\n");


	}


	return 0;
}