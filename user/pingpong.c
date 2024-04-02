#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main()
{
    int p1[2];//parent_pipe
    int p2[2];//children_pipe
    
    pipe(p1);
    pipe(p2);

    if (fork()==0)
    {
        close(p2[1]);
        close(p1[0]);

        char byte;

        read(p2[0], &byte, 1);
        printf("%d: received ping\n", getpid);
        write(p1[1], &byte, 1);

        close(p2[0]);
        close(p1[1]);
    }else{

        char byte = 'T';

        close(p2[0]);
        close(p1[1]);

        write(p2[1], &byte, 1);
        read(p1[0], &byte, 1);
        printf("%d: received pong\n", getpid);

        close(p2[1]);
        close(p1[0]);
    }

    exit(0);
    
}