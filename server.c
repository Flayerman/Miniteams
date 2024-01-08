#include <stdio.h> /* librairie de base*/
#include <stdlib.h> /* fonction exit*/
#include <unistd.h> /*fonction pause / exit*/
#include <signal.h> /* fonction signal */
#include <string.h> /* autorisé*/

/* Compilation avec gcc server.c -o server */

// Fonction pour convertir une chaîne binaire en texte
char* BinaryToString(const char* binaire) {

    // Allouer de la mémoire pour la chaîne texte (plus un caractère nul '\0')
    char* texte = (char*)malloc((8 + 1) * sizeof(char));

    // Convertir la chaîne binaire en texte
    for (int i = 0; i < 8; i++) {
        char octet[9];  // Stocker un octet (8 bits) + le caractère nul
        strncpy(octet, binaire + i * 8, 8);
        octet[8] = '\0';

        // Convertir l'octet en décimal et puis en caractère ASCII
        texte[i] = (char)strtol(octet, NULL, 2);
    }

    // Ajouter le caractère nul à la fin de la chaîne texte
    texte[8] = '\0';

    return texte;
}

void signal_handler_o(int signo) 
{
	static int i = 0;
	static char letter[9] = ""; // 8bits + \0
	if (signo == 10) // signal 10 = USR1 x86/ARM - man 7 signal
	{
		char bit[] = "1";
		strcat(letter,bit);
	}
	else if (signo == 12) // signal 12 = USR2 x86/ARM - man 7 signal
	{
		char bit[] = "0";
		strcat(letter,bit);
	}
	i++;
	if (i >= 8)
	{
		char* letter_inv = BinaryToString(letter);
		i = 0;
        printf("%s\n",letter_inv );
        // Reset la lettre
		for (int j = 0; j < 9; j++) {
            letter[j] = '\0';
        }
	}
}



int main(int argc, char const *argv[])
{
	// Récupération PID
	pid_t pid = getpid();
    printf("Miniteams starting...\n");
    printf("My PID is %d\n", pid );
    printf("Waiting for new messages\n");
    printf("========================\n");

    // Création fonction handler
    struct sigaction sa;
    sa.sa_handler = signal_handler_o; // fonction à 1 arg =/= sa_sigaction à 3 arg
    sa.sa_flags = 0; // pas besoin de fonction à 3 arguments !! En binaire 0 = USR1 et 1=USR2 !!
    

    // Écoute les signaux USR1 et USR2
    signal(SIGUSR1, signal_handler_o);
    signal(SIGUSR2, signal_handler_o);

    // Boucle
    while (1)
    {
        pause();
    }
	return 0;
}
