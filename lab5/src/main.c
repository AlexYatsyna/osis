#include <stdio.h>
#include <daemon.h>

int main(int argc, char* argv[]){

    int pid=run_deamon();
    if (!pid)
        return 0;

    printf("%d\n", pid);
    return 0;
}