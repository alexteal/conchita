#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "read.h"
#include <limits.h>
#define BUFFERSIZE 16

//TODO
//Merge to writes only, no printf
int main(){
    setvbuf(stdout, NULL, _IONBF, 0); //disable printf() buffer
    char working_directory[PATH_MAX];
    char visible_directory[FILENAME_MAX];
    getcwd(working_directory,PATH_MAX);
    getvwd(working_directory,visible_directory);
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
