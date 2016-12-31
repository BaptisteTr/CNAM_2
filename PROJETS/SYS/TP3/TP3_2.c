#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>

int r; 

int pointsJ1 = 0;
int pointsJ2 = 0;

void print_scores(){
	printf("Joueur 1 : %d\tJoueur 2 : %d\n\n",pointsJ1,pointsJ2);
}

void sig_hand_sigusr1(int sig){
	r = rand()%2;
	if(r==0){
		printf("Le joueur 1 rattrape la balle! Il la renvoie au joueur 2!\n");

	} else {
		printf("Le joueur 1 rate la balle... Quel échec... Il la relance au joueur 2\n");
		pointsJ2++;
	}
	printf("Joueur 2 : %d\n\n",pointsJ2);
	sleep(1);
}

void sig_hand_sigusr2(int sig){
	r = rand()%2;

	if(r==0){
		printf("Le joueur 2 rattrape la balle! Il la renvoie au joueur 1!\n");
	} else {
		printf("Le joueur 2 rate la balle... Quel échec... Il la relance au joueur 1\n");
		pointsJ1++;
	}
	printf("Joueur 1 : %d\n\n",pointsJ1);
	sleep(1);
}



int main(int argc, char **argv){

	int status;

	sigset_t sig_proc;
	struct sigaction action;
	struct sigaction action2;

	sigemptyset(&sig_proc);
	action.sa_mask=sig_proc;
	action.sa_flags=0;
	action.sa_handler=sig_hand_sigusr1;
	sigaction(SIGUSR1, &action,0);

	action2.sa_mask=sig_proc;
	action2.sa_flags=0;
	action2.sa_handler=sig_hand_sigusr2;
	sigaction(SIGUSR2,&action2,0);

	sigaddset(&sig_proc, SIGUSR1);
	sigaddset(&sig_proc, SIGUSR2);
	sigprocmask(SIG_SETMASK, &sig_proc, NULL);

	sigfillset(&sig_proc);
	sigdelset(&sig_proc, SIGUSR1);
	sigdelset(&sig_proc, SIGUSR2);

	int pid_fork = fork();

	if(pid_fork == 0){ // Fils

		srand(3);

		do{

			sigsuspend(&sig_proc);
			kill(getppid(), SIGUSR2);			

		}while(pointsJ2<13);

		return (0);

	} else { // Père

		srand(8);

		do{

			kill(pid_fork, SIGUSR1); // Envoi d'un signal SIGUSR1 au proc fils
			sigsuspend(&sig_proc);


		}while(pointsJ1<13);


		if(pointsJ2>13){
			printf("VICTOIRE DU JOUEUR2 !");
		} else {
			printf("VICTOIRE DU JOUEUR1 !");			
		}

	}
}