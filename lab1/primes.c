#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"


int
main(int argc, char *argv[]) {
    if (argc >= 2) {
        fprintf(2, "usage: primes\n");
        exit(1);
    }

    int p[100][2];
    int pos = 0;
    pipe(p[pos]);

    for (int i = 2; i <= 100; i++) {
        write(p[pos][1], &i, 4);
    }
    close(p[pos][1]);


    while (1) {
        int a[100];
        //从左侧读入
        int n;
        pipe(p[++pos]);
        for (n = 0;; n++) {
            read(p[pos-1][0], &a[n], 4);
            if (a[n] == 0)break;
            if (a[n] % a[0] == 0)continue;
            write(p[pos][1], &a[n], 4);
        }
        //close read
        close(p[pos-1][0]);
        //close Write
        close(p[pos][1]);
        printf("prime %d\n", a[0]);
        if (a[0] == 97)break;
        if (fork() == 0) {
            exit(0);
        }
    }
    exit(0);
}
