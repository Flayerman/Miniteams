#include <stdio.h> /* librairie de base*/
#include <stdlib.h> /* fonction exit*/
#include <unistd.h> /*fonction pause / exit*/
#include <signal.h> /* fonction signal */
#include <string.h> /* autorisé*/




int main(int argc, char *argv[])
{
	// Récupération du PID Server
	pid_t server_pid = atoi(argv[1]);
	printf("Sendind to %d\n",server_pid );
	// Envoi de l'instruction SIGUSR1
	//kill(server_pid,SIGUSR1);
	// Envoie de la chaine de caractères
	union sigval value;
    value.sival_ptr = argv[2]; //stock la variable dans un adresse mémoire
    sigqueue(server_pid, SIGUSR1, value); // Il ne semble pas etre cable d'envoyer une chaine de caractère directement. Je dois convertir en binaire ?
    printf("Message envoyé au serveur : %s\n", argv[2]);
	printf("Done.\n");

	return 0;
}
