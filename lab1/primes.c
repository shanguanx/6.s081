#include "kernel/types.h"
#include "user/user.h"

void is_prime(int pipe_read) {
    int prime;
    if(read(pipe_read, &prime, sizeof(int)) <= 0) {
        fprintf(1, "termination!\n");
        exit(0);
    }

    int p[2];
    if(pipe(p) < 0) {
        fprintf(2, "create pipe failed\n");
        exit(1);
    }

    if(fork() == 0) {
        close(p[1]);
        is_prime(p[0]);
    }

    close(p[0]);
    fprintf(1, "prime %d\n", prime);

    int judge;
    while(read(pipe_read, &judge, sizeof(int)) > 0) {
        if(judge % prime == 0) 
            continue;
        write(p[1], &judge, sizeof(int));
    }
    close(p[1]);
    wait((int*)0);
    exit(0);
}

int main(int argc, char *argv[]) {
    int p[2];
    int start = 2, end = 35;

    if(pipe(p) < 0) {
        fprintf(2, "create pipe failed\n");
        exit(1);
    }

    if(fork() == 0) {
        close(p[1]);
        is_prime(p[0]);
    }else {
        close(p[0]);
        for(int i = start; i <= end; ++i) {
            write(p[1], &i, sizeof(int));
        }
        close(p[1]);
        wait((int*)0);
    }
    exit(0);
}
