#include <ctype.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#define BUFFERSIZE 32

char* readfile(char* path){
    int fd;
    if((fd = open(path,O_RDONLY)) == -1){
        printf("Something went wrong reading the file\n");
        return "";
    }
    int size = lseek(fd,(size_t)0, SEEK_END);
    lseek(fd,(size_t)0,0); //reposition to beginning
    char buffer[BUFFERSIZE];
    char *file = malloc(size * sizeof *file);
    int n;
    if(file){
        unsigned short index = 0;
        while((n = read(fd,buffer,BUFFERSIZE)) > 0){
            for(int i = 0; i<n; i++){
                file[index++] = buffer[i];
            }//write buffer to string output
        }//while
    } // malloc succesful
    return file;
}//return string with plaintext of file

extern int errno;
int main(int argc, char **argv){
    setvbuf(stdout, NULL, _IONBF, 0); //disable printf() buffer
    //int errnum;
    int index;
    int flag;
    int numwords,numbytes,numlines;
    bool c = false;
    bool l = false;
    bool w = false;
    opterr = 0;
    // count words
    // count bytes
    // count lines
    while ((flag = getopt (argc, argv, "clw")) != -1){
        switch (flag)
        {
            case 'c': //numbytes
                c = true;   
                break;
            case 'l': //numlines
                l = true;
                break;
            case 'w': //numwords
                w = true;
                break;
            case '?':
                if (optopt == 'c')
                    fprintf (stderr, "Option -%c requires an argument.\n", optopt);
                else if (isprint (optopt))
                    fprintf (stderr, "Unknown option `-%c'.\n", optopt);
                else
                    fprintf (stderr,
                            "Unknown option character `\\x%x'.\n",
                            optopt);
                return 1;
            default:
                abort ();
        }
    }
    if(c==0&&l==0&&w==0){
        c=1;
        l=1;
        w=1;
    }

    unsigned short amount_of_paths = argc-optind;
    if(amount_of_paths < 1){
        char ch;
        int size = lseek(0,(size_t)0,SEEK_END); //fd 0 is stdin
        numlines = 0;
        numbytes = 0;
        numwords = 0;
        char* s = malloc(size*sizeof(char));
        while(read(STDIN_FILENO, &ch, 1) > 0)
        {
            s[numbytes] = ch;
            numbytes++; //tracks amount of characters in s[]
            if(ch=='\n'){
                numlines++;
            }//count lines
            if(isspace(ch)){
                numwords++;
            }//count words
        }
        printf("\t%d ",numlines);
        printf("\t%d ",numwords);
        printf("\t%d ",numbytes);
        printf("\n");
        close(STDIN_FILENO);
        //printf("%s",s);
    }
    else{
        char *path[amount_of_paths]; // fix path length
        int count = 0;
        for (index = optind; index < argc; index++){
            path[count++] = argv[index];
        } //grab paths from args
        for(int i =0; i < amount_of_paths; i++){
            char *s = readfile(path[i]);
            if (strlen(s) < 1){
                return 1;
            }

            if(l){
                numlines = 0;
                for(int i = 0; i < strlen(s); i++){
                    if(s[i]== '\n')
                        numlines++;
                }
                printf("\t%d ",numlines);
            }
            if(w){
                numwords = 0;
                for(int i = 0; i<strlen(s); i++){
                    if(isspace(s[i])){
                        numwords++;
                    }
                    while(isspace(s[i])&&i<strlen(s))
                        i++;
                    /*
                       if(s[i]==' '){
                       numwords++;
                       }
                       */
                }
                printf("\t%d ",numwords);
            }
            if(c){
                int fd;
                if((fd = open(path[i],O_RDONLY)) == -1){
                    printf("Something went wrong reading the file\n");
                }
                numbytes = lseek(fd,(size_t)0, SEEK_END);
                close(fd);
                printf("\t%d ",numbytes);
            }
            printf("\t%s\n",path[i]);
            free(s);
        }
    }
    return 0;
}
