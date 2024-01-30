#include <stdio.h> // librairie de base
#include <stdlib.h> // fonction exit
#include <unistd.h> // fonction pause / exit
#include <signal.h> // fonction signal 
#include <string.h> // autorisé

// Fonction pour convertir une chaîne binaire en texte
char* BinaryToString(const char* binaire) {
    int longueur = strlen(binaire);

    // Vérifie si la longueur de la chaîne binaire est un multiple de 8
    if (longueur % 8 != 0) {
        printf("Erreur : La longueur de la chaîne binaire doit être un multiple de 8.\n");
        return NULL;
    }
    // Allouer de la mémoire pour la chaîne texte (plus un caractère nul '\0')
    int tailleTexte = longueur / 8;
    char* texte = (char*)malloc((tailleTexte + 1) * sizeof(char));

    // Vérifier l'allocation de mémoire
    if (texte == NULL) {
        printf("Erreur : Échec de l'allocation de mémoire.\n");
        return NULL;
    }

    // Convertie la chaîne binaire en texte
    for (int i = 0; i < tailleTexte; i++) {
        char octet[9];  // Stocker un octet (8 bits) + le caractère nul
        strncpy(octet, binaire + i * 8, 8);
        octet[8] = '\0';

        // Convertir l'octet en décimal et puis en caractère ASCII
        texte[i] = (char)strtol(octet, NULL, 2);
    }
    texte[tailleTexte] = '\0';

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
    else if (signo == 5) // signal 5 = SIGTRAP - renvoie la liste texte/letter_word avec l'appel en une fois de BinaryToString
    {
        char* letter_word = BinaryToString(letter);
        printf("%s\n",letter_word);
        FILE* fichier = NULL;
        fichier = fopen("conversation.log","a");
        if (fichier != NULL)
        {
            // On l'écrit dans le fichier
            fprintf(fichier, letter_word,"\n");
            fclose(fichier);
        }



        // Reset la lettre
        for (int j = 0; j < 256; j++) {
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
    signal(SIGTRAP, signal_handler_o);

    // Boucle
    while (1)
    {
        pause();
    }
	return 0;
}
