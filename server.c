#include <stdio.h> /* librairie de base*/
#include <stdlib.h> /* fonction exit*/
#include <unistd.h> /*fonction pause / exit*/
#include <signal.h> /* fonction signal */
#include <string.h> /* autorisé*/

/* Compilation avec gcc server.c -o server */


/*void sigUSR1_handler(int sig)
{
    printf("Signal reçu %d\n",sig );
}*/

void sigusr1_handler(int signo, siginfo_t *info, void *context)
{
    printf("USR2\n");
    //printf("Signal reçu de %d : %s\n",info->si_pid,(char *)info->si_value.sival_ptr ); // write better ?
    char * c;
    c = (char *)info->si_value.sival_ptr;
    write(1,c,1);
}



int main(int argc, char const *argv[])
{
    pid_t pid = getpid();
    printf("Miniteams starting...\n");
    printf("My PID is %d\n", pid );
    printf("Waiting for new messages\n");
    struct sigaction sa_usr1;
    sa_usr1.sa_flags = SA_SIGINFO; // permets d'utiliser signo,info,context
    sa_usr1.sa_sigaction = sigusr1_handler; // pointe vers une fonction à trois arguments
    //sa_usr1.sa_handler = sigusr1_handler; pour une fonction a un argument
    sigaction(SIGUSR1, &sa_usr1, NULL);

    while (1)
    {
        pause();
    }
    return 0;
}
