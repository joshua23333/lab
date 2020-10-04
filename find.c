#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void
find(char *path, char *file){ 
    char buf[512], *p;
    int fd;//file directory
    struct dirent de;//struct preserve diretor
    struct stat sa;//judge is file or directory

    if((fd = open(path, 0))<0){
        fprintf(2, "File can not open %s\n", path);
        return;

    }
    if(fstat(fd, &sa)<0 || T_DIR!=sa.type){
        fprintf(2, "path %s must be dir\n", path);
        close(fd);
        return;
    }
    //diama renwei yiding shidir,yinci kaishi split
    while(read(fd, &de, sizeof(de))==sizeof(de)){
        if(de.inum==0){
            continue;
        }
        strcpy(buf, path);
        p=buf+strlen(buf);
        *p++ = '/';

        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ]=0;
        if(stat(buf, &sa)<0){
            printf("can not stat %s\n", buf);
            continue;
        }

        //buf limian yonglebaocunzhenggestring zheli yong de.name
        // compare de.name with file
        switch(sa.type){
            case T_FILE:
                if(strcmp(de.name, file)==0){
                    printf("%s\n", buf);

                }
                break;
            case T_DIR:
                if(strcmp(de.name, ".")!=0 && strcmp(de.name, "..")!=0){
                    find(buf, file);
                }
                break;
        }
    }
    close(fd);

}

int
main(int argc, char *argv[]){
    if(argc<3){
        fprintf(2, "Usage find need two params\n");
        exit(-1);
    }
    find(argv[1], argv[2]);
    exit(0);
}



