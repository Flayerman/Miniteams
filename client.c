#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

// char* charToBinary(char c)
// {
//     char* binaryString = (char*)malloc(9);
//     for (int i = 7; i >= 0; --i) {
//         binaryString[7 - i] = ((c >> i) & 1) + '0';
//     }

//     binaryString[8] = '\0';
//     return binaryString;
// }

void sendSignal(int bit, char* targetPID)
{
    int pid = atoi(targetPID);

    if (bit == 2) {
        kill(pid, SIGTRAP);
    }

    else if (bit == 1) {
        kill(pid, SIGUSR1);
    } 
    
    else if (bit == 0) {
        kill(pid, SIGUSR2);
    }
}

int main(int argc, char* argv[])
{
    if (argc != 3){
        printf("Please only provide target PID and message to send.");
        return 1;
    }

    char *targetPID = argv[1];
    char *message = argv[2];
    

    printf("Input : %s\n", message);
    printf("Converted: \n");

    for (int i = 0; message[i] != '\0'; ++i) {
        for (int j = 7; j >= 0; --j) {
            printf("%d", (message[i] >> j) & 1);
            sendSignal((message[i] >> j) & 1, targetPID);;
            usleep(1000);
        }
        printf(" ");
            printf("%s\n", targetPID);
    }

    sendSignal(2, targetPID);
    printf("PID du processus : %d\n", getpid());
}
