#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>

#define TAILLE_BUFFER 256
#define READ_END 0
#define WRITE_END 1

int main(int argc, char **argv)
{

	char cmd1[] = "ps";  char *args1[] = {"ps", "-eaux", NULL};
	char cmd2[] = "grep";  char *args2[] = {"grep", "^root", NULL};

	int status;

	int pipe_desc[2];

	pipe(pipe_desc);
	
	int pid_fork = fork();

	if(pid_fork == 0){ // Fils

	    dup2(pipe_desc[WRITE_END],1); // On redirige la sortie standard vers l'écriture du pipe

    	close(pipe_desc[READ_END]); // On ferme la sortie du pipe

	    execvp(cmd1,args1); // ps

	} else { //Père

		//wait(&status);

		dup2(pipe_desc[READ_END],0); // On redirige la sortie du pipe vers l'entrée standard

		close(pipe_desc[WRITE_END]); // On ferme l'entrée du pipe


		printf("Statut du fils %d\n", status);

	    execvp(cmd2,args2); //grep
	}


	return 0;
}