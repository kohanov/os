#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

void sigusr_handler(int signumber, siginfo_t *info, void *ucontext_t) {
    if (signumber == SIGUSR1) {
        printf("SIGUSR1");
    } else if (signumber == SIGUSR2) {
         printf("SIGUSR2");
    } else {
        printf("%d", signumber);
    }
    printf(" from %d\n", info->si_pid);
    exit(0);
}

int main(int argc, char* argv[]) {
    struct sigaction action;
    action.sa_sigaction = &sigusr_handler;
    sigaction(SIGUSR1, &action, NULL);
    sigaction(SIGUSR2, &action, NULL);
    sleep(10);
    printf("No signals were caught\n");
    return 0;
}
