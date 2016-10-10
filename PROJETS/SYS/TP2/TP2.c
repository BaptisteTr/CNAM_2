#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    printf("--beginning of program\n");

    pid_t pid = fork();

    int status;

    int print_pid = getpid();
    int print_ppid = getppid();

    if (pid == 0)
    {
        printf("CHILD \nPID : %d\nPPID : %d\n\n",print_pid,print_ppid);
    }
    else if (pid > 0)
    {
        printf("FATHER\nPID : %d\n\n",pid);
        waitpid(pid,&status,0);
        printf("Return code : %d\n",status);

    }

    return 0;
}
