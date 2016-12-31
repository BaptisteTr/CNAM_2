#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <signal.h>




int compteur = 0;

void sig_hand_sigint(int sig){
	compteur++;
	printf("Compteur : %d\n",compteur);
}

void sig_hand_term(int sig){
	printf("SIGTERM reçu, adieu\n");
	exit(0);
}

int main(int argc, char **argv)
{
	sigset_t sig_proc;
	struct sigaction action;
	struct sigaction action2;

	sigemptyset(&sig_proc);

	//On change le traitement de SIGTERM
	action2.sa_mask=sig_proc;
	action2.sa_flags=0;
	action2.sa_handler=sig_hand_term;
	sigaction(SIGTERM,&action2,0);

	//On change le traitement de SIGINT
	action.sa_mask=sig_proc;
	action.sa_flags=0;
	action.sa_handler=sig_hand_sigint;
	sigaction(SIGINT, &action,0);

	//On masque SIGINT et SIGTERM
	sigaddset(&sig_proc, SIGINT);
	sigaddset(&sig_proc, SIGTERM);
	sigprocmask(SIG_SETMASK, &sig_proc, NULL);

	sigfillset(&sig_proc);
	sigdelset(&sig_proc, SIGTERM);
	sigdelset(&sig_proc, SIGINT);

	while(1){
		//On attend le signal SIGINT
		sigsuspend(&sig_proc);	
	}

	return 0;
}