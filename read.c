#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "read.h"
#define BUFFERSIZE 16

void parse_args(int argc, char **argv){
   for(int i = 0; i<argc; i++){
        
   }
}

char* resize(char* s, unsigned int size){
    if(size<1){
        return "";
    }
    s = realloc(s,size);
    if (s)
        return s;
    return "";
}

//TODO
//add error handling
char* read_stdin(int buffer_size){
    char* s = malloc(buffer_size);
    char* buff[buffer_size];
    unsigned int index = 0;
    while(read(STDIN_FILENO, &buff, buffer_size)>0){ //buffer this!
        for(unsigned short i = 0; i<buffer_size; i++){
            s[index++] = *buff[i];
        }
        s = resize(s,buffer_size); //increment s size out of fear
    } 
    s[index] = '\0'; //close string
    return s;
}


char* read_file(char* path){
    int fd;
    if((fd = open(path,O_RDONLY)) == -1){
        printf("Something went wrong reading the file\n");
        close(fd);
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
    close(fd);
    return file;
}//return string with plaintext of file

char* read_file_bytes(char* path,int numbytes){ //setting numbytes as unsgined causes a multiversal collision. 
    int fd;
    if((fd = open(path,O_RDONLY)) == -1){
        printf("Something went wrong reading the file\n");
        close(fd);
        return "";
    }
    char buffer[BUFFERSIZE];
    //reposition to read only certain amount of bytes
    lseek(fd,((numbytes)*-1), SEEK_END);
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
    close(fd);
    return file;
}//return string with plaintext of file
