#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define PIPE_READ 0
#define PIPE_WRITE 1

void primes(int left_pipe[2])
{
    int prime,num;
    
    if (read(left_pipe[PIPE_READ], &prime, sizeof(int))==0)
    {
        exit(0);
    }
    printf("prime %d\n", prime);

    int right_pipe[2];
    pipe(right_pipe);

    close(left_pipe[PIPE_WRITE]);
    if (fork()==0)
    {
        primes(right_pipe);
    }else{
        close(right_pipe[PIPE_READ]);
        while (read(left_pipe[PIPE_READ], &num, sizeof(int))==sizeof(int))
        {
            if (num%prime!=0)
            {
                write(right_pipe[PIPE_WRITE], &num, sizeof(int));
            }
        }
        close(left_pipe[PIPE_READ]);
        close(right_pipe[PIPE_WRITE]);
        wait(0);
        exit(0);
    }
    
}

int main(int argc, char *argv[])
{
    int left_pipe[2];
    pipe(left_pipe);

    if (argc>1)
    {
        fprintf(2, "usage: primes");
        exit(1);
    }
    
    if (fork()==0)
    {
        primes(left_pipe);
    }else{

        close(left_pipe[PIPE_READ]);
        for (int i = 2; i <=35; i++)
        {
            write(left_pipe[PIPE_WRITE], &i, sizeof(int));
        }
        close(left_pipe[PIPE_WRITE]);
        wait(0);
        
    }
    exit(0);
}