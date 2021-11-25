#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <string.h>
#include "read.h"
#define BUFFERSIZE 16


//fast write to stdout
void fprint(const char* s){
    write(STDOUT_FILENO,s,strlen(s));
}

void print_prefix(const char* visible_directory){
    fprint("[csci-1730] ");
    fprint(visible_directory);
    fprint(" % "); //fancy close
}

void change_dir(const char* new_path, char* working_directory, char* visible_directory){
    for(int i = 0; i<strlen(working_directory)&&i<strlen(new_path); i++){
        working_directory[i] = new_path[i];
    }//update current directory
    getvwd(working_directory,visible_directory); //update visible directory
    return;
}

//get visible working directory
void getvwd(const char* current_path, char* visible_directory){
    unsigned short index;
    for(index = strlen(current_path); index>0; index--){
        if(current_path[index] == '/')
            break;
    }
    index++; //remove '/' from path
    int i; //index to track for null termination
    for(i = 0; i<strlen(current_path); i++)
        visible_directory[i] = current_path[index++];
    visible_directory[++i] = '\0'; //terminate string
    return;
}

//add to read.h
//skips spaces in string, returns index of next nonspace
unsigned int skip_space(const char* s, unsigned int index){
    while(isspace(s[index])){
        if(s[index]=='\\') //handle \ to allow spaces in paths
            index++;
        index++;
    } //traverse spaces
    return index;
}

//given current index, return dynamic string of next word. index is set to end of word
//add to read.h
char* next_word(const char* s, unsigned int index){
    unsigned int begin=skip_space(s,index); //beginning of next word
    index = begin;
    while(!isspace(s[index])){
        index++;
    } //increment index to end of word
    char* word = malloc((index-begin)*sizeof(char));
    for(int i = begin; i<index; i++)
        word[i] = s[i]; //store word
    return word;
}

//start parsing from index
//add to read.h
void parse(const char* s, unsigned int index){
    char* current_word = next_word(s,index);  
    if (strcmp(current_word,"cd")){
        char* possible_path = next_word(current_word,index); 
        fprint(possible_path);

    } // change directory function
    free(current_word);
    return;

}

char* resize(char* s, unsigned int size){
    if(size<1){
        return "";
    }
    s = realloc(s,size);
    if (s)
        return s;
    fprint("\n REALLOC FAILURE \n");
    return "";
}

//TODO
// Will only handle n<buffersize. need to fix buffer handling
//add error handling
char* read_stdin(int buffer_size){
    char *s = malloc(buffer_size);
    char buff[buffer_size];
    unsigned int index = 0;
    int n;
    char linebreak = 0; //bool, used for EOL
    while ((n = read(STDIN_FILENO, &buff, buffer_size))>0&&linebreak==0){ //buffer this!
        if(n>buffer_size) // n should be amount of characters in stdin
        {
            for(int i = 1; buffer_size*i<n; i++){
                s = resize(s,buffer_size);
            }
        } //resize s with buffer until it can fit n characters
        for(unsigned short i = 0; i<n; i++){
            s[index++] = buff[i];
            if(buff[i] == '\n'){
                linebreak = 1;
                break;
            } // EOL test
        }//assigned buffer to s
        if(linebreak==1) //lazy test for EOL
            break;
    } 
    s[index] = '\0'; //close string
    fprint("end of read");
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
