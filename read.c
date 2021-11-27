#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <string.h>
#include <limits.h>  //path limits
#include <errno.h>
#include "read.h"
#include <dirent.h>  // ls command
#define BUFFERSIZE 16


//feature list
//  cd works fine
//  exit ends program
//  next_word functions as expected
//  home directory is default now, and denoted by ~
//

//TODO
// build ls
//  may require new library, and struct
//
//

//returns are 
// -1   failure
//  0   no command found
//  1   command found and executed properly
short parse_builtin(const char* s,char* working_directory, char* visible_directory){
    unsigned int index_of_next_word = 0;
    char *cmd = next_word(s,&index_of_next_word);
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
    free(cmd);
    switch(index){
        case 0: //cd
            change_dir(s,working_directory,visible_directory,&index_of_next_word);
            return 1;

            break;
        case 1: //ls
            /*
            DIR *d;
            struct dirent *dir;
            d = opendir(".");
            if (d) {
                while ((dir = readdir(d)) != NULL) {
                    printf("%s\n", dir->d_name);
                }
                closedir(d);
            }
            return(0);
            */
            break;
        case 2: //h
            break;
        case 3: //exit
            fprint("Closing conchita...\n");
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
//
void parse(const char* s,char* working_directory, char* visible_directory){
    //char* current_word = next_word(s,index);  
    //free(current_word);
    short code = parse_builtin(s,working_directory, visible_directory);
    if(code>-1){
        if(code==1)
            return;
        //fprint("worked like a charm");

    }
    else{
        fprint("builtin parse failure\n");
    }
    fprint("command not found: ");
    fprint(s);
    return;

}
//fast write to stdout with linebreak;
void fprint(const char* s){
    write(STDOUT_FILENO,s,strlen(s));
}

void print_prefix(const char* visible_directory){
    fprint("[csci-1730] ");
    fprint(visible_directory);
    fprint(" % "); //fancy close
}

void ls(const char *working_directory){
    
}

void change_dir(const char *s,char *working_directory,char *visible_directory, unsigned int *index_of_next_word)
{
    char* word = next_word(s,index_of_next_word);
    //attempt dir change
    if(chdir(word)!=0){
        fprint(strerror(errno));
        fprint("\n");
    } //dir change and error handling
    getcwd(working_directory,PATH_MAX);
    getvwd(working_directory,visible_directory);
    free(word);
}

//get visible working directory
void getvwd(const char* current_path, char* visible_directory){
    char* home_dir = getenv("HOME");
    if(home_dir!=NULL){
        if(strcmp(home_dir,current_path)==0){
            visible_directory[0] = '~';
            visible_directory[1] = '\0';
            return;
        } // is inside home directory
    } //home directory is testable
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
    unsigned int difference = *index-begin; // a world of difference here,, idk
    char* word = malloc((difference)*sizeof(char));
    for(int i = 0; i<difference; i++){
        word[i] = s[begin++]; //store word
    } //assign buffer to word
    word[*index] = '\0'; //close string
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
