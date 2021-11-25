#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <string.h>
#include <limits.h>
#include "read.h"
#define BUFFERSIZE 16


//returns are 
// -1   failure
//  0   no command found
//  1   command found and executed properly
short parse_builtin(const char* s,char* working_directory, char* visible_directory){
    unsigned int index_of_next_word = 0;
    char *cmd = next_word(s,&index_of_next_word);
    printf("\nIndex of word is %d \n",index_of_next_word);
    unsigned short num_commands = 4;
    char* commands[num_commands];
    commands[0] = "cd";
    commands[1] = "ls"; 
    commands[2] = "h"; //throwaway
    commands[3] = "exit";
    short index = -1; 
    for(int i = 0; i<num_commands; i++){
        if(strcmp(cmd,commands[i])==0){
            index = i;
            break;
        } // if s is in commands
    }
    switch(index){
        case 0: //cd
            fprint("changing directory\n");
            char* word = next_word(s,&index_of_next_word);
            printf("\nindex is : %d",index_of_next_word);
            //fprint("\n NEXT WORD IS : ");
            //
            //
            //
            // lets break it down
            // current issue is that 
            // index_of_next_word is too far
            //
            // so maybe next_word function is broken?
            // if we can get proper next word, then i think it'll work
            //
            //fprint(word);
            //fprint("\n");
            //attempt dir change
            chdir(word);
            getcwd(working_directory,PATH_MAX);
            //fprint(working_directory);
            //fprint("\n");
            getvwd(working_directory,visible_directory);
            //fprint(visible_directory);
            //fprint("\n");
            return 1;
            
            break;
        case 1: //ls
            break;
        case 2: //h
            break;
        case 3: //exit
            fprint("\n Closing shell...\n");
            exit(0);
            break;
        default:
            return 0; //no command found
            break;
    }

    return -1;
}

//parse from index
//refactor my guy
//  do multi func parsing
//  one to do system commands
//  one to do inbuilt commands
//
void parse(const char* s,char* working_directory, char* visible_directory){
    //char* current_word = next_word(s,index);  
    //free(current_word);
    short code = parse_builtin(s,working_directory, visible_directory);
    if(code>-1){
        if(code==1)
            fprint("worked like a charm");

    }
    else{
        fprint("builtin parse failure");
    }
    return;

}
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
char* next_word(const char* s, unsigned int* index){
    unsigned int begin=skip_space(s,*index); //beginning of next word
    *index = begin;
    while(!isspace(s[*index])){
        (*index)++; //pointers are black magic and i hate them
    } //increment *index to end of word
    char* word = malloc((*index-begin)*sizeof(char));
    for(int i = begin; i<*index; i++){
        word[i] = s[i]; //store word
        printf("%c%c",word[i],s[i]);
    }
    printf("\n next word is: %s \n",word);
    return word;
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
