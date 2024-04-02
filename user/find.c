#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void find(char *path, char *fileName)
{
    int fd;
    struct stat st;
    char buf[512], *p;
    struct dirent de;

    if ((fd=open(path, 0))<0)
    {
        fprintf(2, "find: cannot open %s\n", path);
        exit(1);
    }
    
    if ((fstat(fd, &st))<0)
    {
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        exit(1);
    }

    if (st.type!=T_DIR)
    {
        fprintf(2, "usage: find <dir> <file>\n");
        exit(1);
    }
    
    if ((strlen(path)+1+DIRSIZ+1)>sizeof(buf))
    {
        fprintf(2, "find: path too long\n");
        exit(1);
    }

    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';

    while (read(fd, &de, sizeof(de))==sizeof(de))
    {
        if (de.inum==0)
        {
            continue;
        }

        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ]=0;
        if (stat(buf, &st)<0)
        {
            printf("find: cannot stat %s\n", buf);
            continue;
        }
        
        if (st.type==T_DIR && strcmp(p, ".")!=0 && strcmp(p, "..")!=0)
        {
            find(buf, fileName);
        }else if (strcmp(p, fileName)==0)
        {
            printf("%s\n",buf);
        }
        
    }
    close(fd);
}

int main(int argc, char *argv[])
{
    if (argc==2)
    {
        find(".", argv[1]);
    }else if (argc==3)
    {
        find(argv[1], argv[2]);
    }else{
        printf("usage: find <path> <fileName>");
        exit(1);
    }

    exit(0);
}
