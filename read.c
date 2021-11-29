#include <stdio.h>
#include <stdbool.h> //booleans
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
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
            ls(working_directory,0); //change second value to 1 for "ls -a"
            return 1;
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
    //
    //exit code from parse_builtin
    short code = parse_builtin(s,working_directory, visible_directory);
    if(code>-1){
        if(code==1)
            return;
    } //command was at least recognized 
    else{
        fprint("builtin parse failure\n");
    } //error code handling

   // add forks/ exec here. 
    unsigned int argc = count_words(s);
    char** argv = string_to_args(s,argc); //so i can do getopt() now

    for(int i = 0; i<argc; i++){
        fprint(argv[i]);
        fprint("\n");
    }
    for(int i = 0; i<argc; i++)
        free(argv[i]);
    free(argv);
    fprint("command not found: ");
    fprint(s);
    return;

}

//take a string, break it into individual strings and remove - if it's at the 
//beginning of a word. Skip first word if its a commanda
char** string_to_args(const char* s,const unsigned short argc){
    unsigned int index = 0;
    char** argv; //may need to malloc?
    argv = malloc(sizeof(char*)*argc);
    for(int i = 0; i<argc; i++){
        argv[i] = malloc(sizeof(char)*FILENAME_MAX);
    } //now need to free this fancilly
    for(int i = 0; i<strlen(s)&&(i<argc); i++){ //don't add random +1 unless you know what you're doing doofus
        argv[i] = next_word(s,&index);
    }
    return argv;
}

short what_the_fork(const char* s){
    char* cmd = malloc(strlen(s)*sizeof(char));
    unsigned int index = 0;
    cmd = next_word(s,&index);
    short pid = fork();
    if(pid==-1){
        fprint(strerror(errno));
    } //fork failure
    if(pid==0){

    } //child process exec
    else{

    } //parent process exec
    return -1;
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

void ls(const char *working_directory, bool list_all){
    // BEGIN PARTIALLY BORROWED BLOCK
    //##########################################################################
      struct dirent *de;  // Pointer for directory entry
    // opendir() returns a pointer of DIR type. 
    DIR *dr = opendir(working_directory);
    if (dr == NULL)  // opendir returns NULL if couldn't open directory
    {
        fprint("Could not open current directory" );
        return;
    }
    // Refer http://pubs.opengroup.org/onlinepubs/7990989775/xsh/readdir.html
    // for readdir()
    while ((de = readdir(dr)) != NULL){ //prints all in dir
        for(unsigned short i = 0; i<4&&(de = readdir(dr)) != NULL; i++){
            char* filename = de->d_name;
            if(!list_all){
                while(filename[0]=='.'&&(de = readdir(dr)) != NULL){ //skip '.' files
                    filename = de->d_name;
                    //fprint(filename);
                }
                if(filename[0]=='.') //catch sneaky . files
                    continue;
                fprint(filename);
            }
            //fprint("\t");
            fprint("\n");
        }
        //fprint("\n");
    }
    closedir(dr);   
    //##########################################################################
    // END PARTIALLY BORROWED BLOCK
    // https://www.geeksforgeeks.org/c-program-list-files-sub-directories-directory/
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


//######################################################################
// DEBUG FUNCTIONS
// #####################################################################



void print_2d_array(const char** argv, const int argc){
    for(int i = 0; i<argc; i++){
        fprint(argv[i]);
        fprint("\n");
    }
    return;
}
