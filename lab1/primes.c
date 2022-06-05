#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"


int
main(int argc, char *argv[]) {
    if (argc >= 2) {
        fprintf(2, "usage: primes\n");
        exit(1);
    }

    int p[40][2];
    int pos = 0;
    pipe(p[pos]);

    for (int i = 2; i <= 35; i++) {
        write(p[pos][1], &i, 4);
    }
    close(p[pos][1]);

    while (1) {
        int a[40];
        //从左侧读入
        int n;
        for (n = 0;; n++) {
            read(p[pos][0], &a[n], 4);
            if (a[n] == 0)break;
        }
        //child close read
        close(p[pos][0]);

        printf("prime %d\n", a[0]);
        if (a[0] == 31)break;
        pipe(p[++pos]);
        if (fork() == 0) {
            //向右侧写入 //parnt close read
            close(p[pos][0]);
            for (int i = 1; i < n; ++i) {
                if (a[i] % a[0] == 0)continue;
                write(p[pos][1], &a[i], 4);
            }
            //parnt close write
            close(p[pos][1]);
            exit(0);
        } else {
            //child close write
            close(p[pos][1]);
        }
    }
    exit(0);
}
