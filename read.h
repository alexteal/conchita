#ifndef READ_H
#define READ_H

// Copy file contents into a string
//output is dynamic string, don't forget to free(s)
// @param   char*   path    relative string path to read
// @return  char*           string containing file contents
char* read_file(char* path);

//Copy file contents to a string, limited by amount of bytes.
//output is dynamic string, don't forget to free(s)
//Reads from index 0 to numbytes
// @param   char*   path        relative string path
// @param   uint    numbytes    amount of bytes to read
// @return  char*               string containing file contents
char* read_file_bytes(char* path, int numbytes);

// resize an array with defined amount of bytes
//output is dynamic string, don't forget to free(s)
//@param    s       string to increment
//@param    size    amount of bytes to add
char* resize(char* s, unsigned int size);

//read standard input into a string
//output is dynamic string, don't forget to free(s)
//@param    BUFFERSIZE     size for buffer
//@return   char*           string  containing standard in.
char* read_stdin(int buffer_size);

#endif
