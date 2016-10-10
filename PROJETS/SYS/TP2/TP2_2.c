#include <stdio.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, char **argv)
{


	pid_t pid = fork();

	if(pid == 0){
		
		printf("PID (%d)\n",getpid());
		close(1);

		int desc = mkstemp("/tmp/proc-exercise");
		int desc_new;

		desc_new = dup2(desc, 56);

		execv("/home/CNAM_2A/PROJETS/SYS/TP2/TP2.c", argv);
	} else {

		if(argc != 2){
			printf("Mauvais nombre de paramètres.");
			return 1;
		}

		printf("PID : %d\n",getpid());
		
		wait(pid);

		printf("J'aime les gauffres au %s\n",argv[1]);
		fprintf(stderr,"Message quelconque\n");


	}


	return 0;
}