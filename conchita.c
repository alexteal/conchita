#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include "read.h"
#include <limits.h>
#define BUFFERSIZE 16

//TODO
//Merge to writes only, no printf
int main(){
    setvbuf(stdout, NULL, _IONBF, 0); //disable printf() buffer
    char working_directory[PATH_MAX];
    char visible_directory[FILENAME_MAX];
    if(1){
        char* home_dir = getenv("HOME");
        unsigned int faux_index = 0; //segfault on static value. Really should've written this better. 
        if(home_dir!=NULL)
            change_dir(home_dir, working_directory, visible_directory, &faux_index); //set to home dir, if dir is known

    } //move to home dir automatically. disabled for debug
    /*
       char* home_dir = getenv("HOME");
       unsigned int faux_index = 0; //segfault on static value. Really should've written this better. 
       if(home_dir!=NULL)
       change_dir(home_dir, working_directory, visible_directory, &faux_index); //set to home dir, if dir is known
       */

    // while loop stdin
    //
    // TODO
    // current working directory
    // enable directory changing
    while(1){
        print_prefix(visible_directory);
        char* s = read_stdin(BUFFERSIZE);
        parse(s,working_directory,visible_directory);
        free(s);
    } //main input loop

    // Close stdin on exit
    // close(STDIN_FILENO); //close input


    close(1); //close stdout
    //free(s);
    return 0;
}
