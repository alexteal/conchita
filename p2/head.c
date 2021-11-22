#include <ctype.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#define BUFFERSIZE 32
// can not be stacked. Must be invoked seperately as -n or -c, not -cn or -nc
unsigned int numlines; //denoted by -n
unsigned int numbytes; //denoted by -c

//TODO
// take stdin when no file described
//
char* readfile(char* path, bool read_stdin){
    int fd;
    if(read_stdin)
        fd = STDIN_FILENO;
    else if((fd = open(path,O_RDONLY)) == -1){
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
            }
        }
    } // malloc succesful
    return file;
}//return string with plaintext of file

char* read_bytes_from_file(char* path,int numbytes,bool read_stdin){// fuck c
    int fd;
    if(!read_stdin){
        if((fd = open(path,O_RDONLY)) == -1){
            printf("Something went wrong reading the file\n");
            return "";
        }
    }
    else{
        fd = STDIN_FILENO;
    }
    char buffer[BUFFERSIZE];
    char *file = malloc(numbytes*sizeof(char));
    //char file[numbytes];
    int n;
    if(file){
        unsigned short index = 0;
        while((n = read(fd,buffer,BUFFERSIZE)) > 0||index<numbytes){
            for(int i = 0; index<numbytes&&i<n; i++){
                file[index++] = buffer[i];
            }
        }
    } // malloc succesful
    return file;
}//return string with plaintext of file

//extern int errno;
// fuck finishing this tonight
// TODO
// rewrite stdin part to a setup like https://stackoverflow.com/questions/4358728/end-of-file-eof-in-c
// Read each character individually watching for a '\n'
int main(int argc, char **argv){
    setvbuf(stdout, NULL, _IONBF, 0); //disable printf() buffer
    //int errnum;
    numbytes = 0;
    int index;
    int c;

    opterr = 0;

    while ((c = getopt (argc, argv, "c:n:")) != -1){
        switch (c)
        {
            case 'c':
                numbytes = atoi(optarg);
                break;
            case 'n':
                numlines = atoi(optarg);
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

    unsigned short amount_of_paths = argc-optind;
    if(amount_of_paths<1){
        if(numlines != 0){
            char ch;
            unsigned short index = 0;
            unsigned short breaks = 0;
            int size = lseek(0,(size_t)0,SEEK_END); //fd 0 is stdin
            char* s = malloc(size*sizeof(char));
            while(read(STDIN_FILENO, &ch, 1) > 0&&breaks<numlines)
            {
                s[index] = ch;
                index++; //tracks amount of characters in s[]
                if(ch=='\n'){
                    breaks++; 
                    printf("%s",s);
                    memset(s,0,strlen(s)); //clear buffer
                    index = 0; // reset counter
                }
                if(breaks == numlines){
                    break;
                }
            }
            close(STDIN_FILENO);
            //printf("%s",s);
        }
        if(numbytes !=0){
            char ch;
            unsigned short counter = 0;
            char* s = malloc(numbytes*sizeof(char));
            while(read(STDIN_FILENO, &ch, 1) > 0)
            {
                s[counter] = ch;
                counter++;
                if(counter == numbytes){
                    s[counter] = '\0';
                    printf("%s",s);
                    break;
                }
            }
            close(STDIN_FILENO);
        }// take stdin
    } //take stdin
    else{
        char *path[amount_of_paths]; // fix path length
        int count = 0;
        for (index = optind; index < argc; index++){
            path[count++] = argv[index];
        } //grab paths from args
        if(numbytes>0 && numlines>0){
            fprintf(stderr,"head: can't combine line and byte counts\n");
            return 1;
        }
        if(numbytes<1&&numlines<1){
            numlines = 10;
        }
        for(int i = 0; i < amount_of_paths; i++){
            if(numlines != 0){
                unsigned short countedlines=0;
                char* s = readfile(path[i],0); //error prone statement
                if(strlen(s)<1){
                    return 1;
                }
                for(int i = 0; countedlines<numlines&&i < strlen(s); i++){
                    if(s[i]=='\n'){
                        countedlines++;
                    }
                    printf("%c",s[i]);
                }
                free(s);
            }
            if(numbytes !=0){
                char* s = read_bytes_from_file(path[i], numbytes,0);
                if(strlen(s)<1){
                    return 1;
                }
                printf("%s",s);
                printf("\n");
                free(s);
            }
        }
    }
    return 0;
}
