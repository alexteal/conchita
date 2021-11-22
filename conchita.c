#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "read.h"
#define BUFFERSIZE 16

//TODO
//Merge to writes only, no printf
int main(){
    setvbuf(stdout, NULL, _IONBF, 0); //disable printf() buffer
    // So need to while loop stdin
    char* s = read_stdin(BUFFERSIZE); //segfault :(
    write(1,s,strlen(s));
    free(s);
    return 0;
}
