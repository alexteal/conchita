#include <stdio.h>
#include <stdbool.h> //booleans
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>  
#include <sys/types.h> //wait()
#include <sys/wait.h> // wait()
#include <string.h>
#include <limits.h>  //path limits
#include <errno.h>   //error codes
#include "read.h"    //Header file for this thing
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
    unsigned short num_commands = 2;
    char* commands[num_commands];
    commands[0] = "cd";
    commands[1] = "exit";
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
        case 1: //exit
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
    //exit code from parse_builtin
    short code = parse_builtin(s,working_directory, visible_directory);
    if(code>-1){
        if(code==1)
            return;
    } //command was at least recognized 
    else{
        fprint("builtin parse failure\n");
    } //error code handling

    if(fork_exec(s) == 1){
        return;
    }
    fprint("command not found: ");
    return;

}

//###################################################################
// BLOCK BORROWED FROM https://brennan.io/2015/01/16/write-a-shell-in-c/
//###################################################################
#define LSH_TOK_BUFSIZE 64
#define LSH_TOK_DELIM " \t\r\n\a"
char **lsh_split_line(char *line, int *position)
{
    *position = 0;
    int bufsize = LSH_TOK_BUFSIZE;
    char **tokens = malloc(bufsize * sizeof(char*));
    char *token;

    if (!tokens) {
        fprintf(stderr, "lsh: allocation error\n");
        exit(EXIT_FAILURE);
    }

    token = strtok(line, LSH_TOK_DELIM);
    while (token != NULL) {
        tokens[*position] = token;
        (*position)++; // WHY ARE POINTERS SO WIERD

        if (*position >= bufsize) {
            bufsize += LSH_TOK_BUFSIZE;
            tokens = realloc(tokens, bufsize * sizeof(char*));
            if (!tokens) {
                fprintf(stderr, "lsh: allocation error\n");
                exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, LSH_TOK_DELIM);
    }
    tokens[*position] = NULL;
    return tokens;
}
//####################################################################

//free a 2d array easily
void free_2d(char** s,const unsigned short argc){
    for(int i = 0; i<argc; i++){
        free(s[i]);
    }
    free(s);
}

short fork_exec(const char* s){
    short pid = fork();
    if(pid==-1){
        fprint(strerror(errno));
        return -1;
    } //fork failure
    if(pid==0){ //don't change dir acutally, this doesn't work as well as you'd like
        int argc = count_words(s);
        char* copy = malloc(strlen(s)*sizeof(char));
        strcpy(copy,s);
        char** argv = lsh_split_line(copy,&argc);
        print2d(argv,argc); //prints entire array to stdout
        if(execvp(argv[0],argv)==-1){ //replaces current thread and execs argv
            fprint(strerror(errno));
            fprint("\n");
        }
        fprint("thread check FREE \n");
        //free_2d(argv,argc); //will not execute unless invalid command
        free(argv); // w h a t
        exit(0);
        fprint("should've exited");
    } //child process exec
    else{
        wait(NULL); //wait for process to fail/finish
        return 1;
    } //parent process exec
    return 0;
}

//count all words in a string
//currently just counts spaces, which probs isn't best
unsigned int count_words(const char* s){
    unsigned int wordcount = 0;
    for(int i = 0; i < strlen(s); i++){
        while(!isspace(s[i]))
            i++;
        wordcount++;
        while(isspace(s[i]))
            i++;
    }
    if(s[0] == ' ')
        wordcount--;
    return wordcount;
}

//fast write to stdout with linebreak;
void fprint(const char* s){ 
    write(STDOUT_FILENO,s,strlen(s));
}

void print_prefix(const char* visible_directory){
    fprint("[csci-1730] ");
    fprint(visible_directory);
    fprint(" $ "); //fancy close
}

//If refactor this,
//  here's an idea
//  instead of hardcoded chdir to home if ~
//
//  modify original string to replace ~ as home path and continue. 
//
void change_dir(const char *s,char *work,char *vis, unsigned int *index)
{
    char* word = next_word(s,index);
    if(word[0]=='~'||strlen(word)<1){ //change dir to home with 'cd ~' or  'cd'
        if(chdir(getenv("HOME"))!=0){
            fprint(strerror(errno));
            fprint("\n");
        } //attempt dir change
        getcwd(work,PATH_MAX);
        getvwd(work,vis);
        free(word);
        return;
    }
    //attempt dir change
    if(chdir(word)!=0){
        fprint(strerror(errno));
        fprint("\n");
    } //dir change and error handling
    getcwd(work,PATH_MAX);
    getvwd(work,vis);
    free(word);
    return;
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
    while(!isspace(s[*index])&&*index<strlen(s)){
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


//######################################################################
// DEBUG FUNCTIONS
// #####################################################################


void print2d(char** argv, const int argc){
    for(int i = 0; i<argc; i++){
        printf("%d\t",i);
        fprint(argv[i]);
        fprint("\n");
    }
    return;
}
