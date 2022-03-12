#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <string.h>
#include <pthread.h>
#include <logger.h>

#define MAX_NAME_LEN 1000


void deamon_proc();


int run_deamon(){
    int pid = fork();
    if (pid == -1){
        fprintf(stderr, "Can't fork.");
        exit(1);
    } else if (pid>0)
       return pid;
    else{

        log_mesg("Set new default file mask");
        umask(0);

        log_mesg("Deatach from session");
        setsid();

        log_mesg("Reset PWS");
        chdir("/");

        log_mesg("Close IO descriptors");
        close(STDIN_FILENO);
        close(STDOUT_FILENO);
        close(STDERR_FILENO);

        log_mesg("Start signal control");

        deamon_proc();
        return 0;
    }
}

int* signals = NULL;
char** names = NULL;
int sig_count;

void free_configs(){

    if (signals != NULL)
        free(signals);
    if (names != NULL){
        for (int i=0; i<sig_count; i++)
            free(names[i]);
        free(names);
    }
}


void read_signals(){
    FILE* fconf = fopen("/home/alex/osis/lab5/config.txt", "r");
    if (!fconf)
        return;

    free_configs();

    sig_count = 0;
    while (!feof(fconf)){
        int signal;
        char name[MAX_NAME_LEN];
        if (fscanf(fconf, "%d %s\n", &signal, name) == 2)
            sig_count++;
    }

    signals = (int*)malloc(sizeof(int) * sig_count);
    names = (char**)malloc(sizeof(char*) * sig_count);
    for (int i=0; i<sig_count; i++)
        names[i] = (char*)malloc(sizeof(char)*MAX_NAME_LEN);

    rewind(fconf);
    sig_count = 0;
    while (!feof(fconf)){
        int signal;
        char name[MAX_NAME_LEN];
        if (fscanf(fconf, "%d %s\n", &signal, name) == 2){
            strcpy(names[sig_count], name);
            signals[sig_count] = signal;
            sig_count++;
        }
    }

    fclose(fconf);
}


void fill_sig_set(sigset_t* psigset){
    sigemptyset(psigset);
    sigaddset(psigset, SIGHUP);
    sigaddset(psigset, SIGTERM);
    for (int i=0; i<sig_count; i++)
        sigaddset(psigset, signals[i]);
}

void try_log_signal(int sig){
    int sig_num=0;
    while (sig_num<sig_count && signals[sig_num] != sig)
        sig_num++;

    if (sig_num != sig_count){
        char mesg[1000];
        sprintf(mesg, "GET SIGNAL %d (%s)", sig, names[sig_num]);
        log_mesg(mesg);
    }
}


void deamon_proc(){
    log_mesg("Read configuration");
    read_signals();

    sigset_t sigset;
    log_mesg("Set signals");
    fill_sig_set(&sigset);
    sigprocmask(SIG_BLOCK, &sigset, NULL);

    int run = 1;
    log_mesg("Start signal listen");
    while (run){
        int sig;

        sigwait(&sigset, &sig);

        try_log_signal(sig);
        

        if (sig == SIGHUP){
            log_mesg("Update configuration");
            read_signals();
            log_mesg("Update signals");
            sigprocmask(SIG_UNBLOCK, &sigset, NULL);
            fill_sig_set(&sigset);
            sigprocmask(SIG_BLOCK, &sigset, NULL);
        }

        if (sig == SIGTERM)
            run = 0;
    }

    log_mesg("Free configuration");
    free_configs();

    log_mesg("Exit from deamon");
}