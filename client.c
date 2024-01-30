#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <time.h>


void sendSignal(int bit, char* targetPID)
{
    int pid = atoi(targetPID);

    if (bit == 2) {
        kill(pid, SIGTRAP); // Fin de message
    }

    else if (bit == 1) {
        kill(pid, SIGUSR1);
    } 
    
    else if (bit == 0) {
        kill(pid, SIGUSR2);
    }
}

char* timeNow(void)
{
    static char s_now[sizeof "HH:MM:SS"];

    /* lire l'heure courante */
   time_t now = time (NULL);

   /* la convertir en heure locale */
   struct tm *tm_now = localtime (&now);

   strftime (s_now, sizeof s_now, "%H:%M:%S", tm_now);
   return s_now;
}

int main(int argc, char* argv[])
{
    // Erreur 1
    if (argc != 3){
        printf("Please only provide target PID and message to send.");
        return 1;
    }

    // Initialisation des variables
    char *targetPID = argv[1];
    char *message = argv[2];
    pid_t pid = getpid();
    char user[128]; // 128 caractères

    sprintf(user, "[From client %d] [%s] ", pid, timeNow()); // Utilisation de sprintf pour formater la chaîne 29caractères
    strcat(user,message);
    // Erreur 2
    if (strlen(user) > 128)
    {
        printf("WARNING, Please write a sentence of less than 100 characters!\n");
        return 1;
    }

    printf("Sending to : %s\n", targetPID);

    // Convertie message en séquence binaire
    for (int i = 0; user[i] != '\0'; ++i) {
        for (int j = 7; j >= 0; --j) {
            sendSignal((user[i] >> j) & 1, targetPID);;
            usleep(100); // pause 10ms
        }
    }
    // Fin de séquence
    sendSignal(2, targetPID);
    printf("Done\n");
}
