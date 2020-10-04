//only use fork() pipe()
#include "kernel/types.h"
#include "user/user.h"

void
primes(void){
    int n, cur_n, len;
    int p[2];

    if((len=read(0, &n, sizeof(int)))<=0 || n<=0){
        //none to write
        close(1);
        exit(-1);
    }
    printf("prime %d\n", n);
    pipe(p);
    if(fork()==0){
        close(0);
        dup(p[0]);
        close(p[0]);
        close(p[1]);
        primes();
    }
    else{
        close(1);
        dup(p[1]);
        close(p[0]);
        close(p[1]);
        // read next 
        while((len=read(0, &cur_n, sizeof(int)))>0 && p>0){
            if(cur_n%n!=0){
                write(1, &cur_n, sizeof(int));
            }

        }
        if(len<=0 || p<=0){
            close(1);
            exit(-1);
        }

    }
}




int
main(void){
    int i;
    int p[2];

    pipe(p);
    if(fork()==0){
        // child read
        close(0);
        dup(p[0]);
        close(p[0]);
        close(p[1]);
        primes();
    }
    else{
        // parent write
        close(1);
        dup(p[1]);
        close(p[0]);
        close(p[1]);
        for(i=2;i<=35;i++){
            // int == 4 bytes,using &
            write(1, &i, sizeof(int));

        }
        // after write closse
        close(1);
        wait(0);
    }
    exit(0);
}
