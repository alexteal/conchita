#ifndef READ_H
#define READ_H


//print out the [user]$~ prefix
void print_prefix(const char* visible_directory);

//store the last part of the directory in a string
void getvwd(const char* current_path,char* visible_directory);

//easy write to stdout
void fprint(const char* s);

// return the index of the next nonspace in a string
//  
unsigned int skip_space(const char* s, unsigned int index);

// return a dynamic char* of the next word in a string
char* next_word(const char* s, unsigned int* index);

//parse and execute inbuilt commands
short parse_builtin(const char* s, char* working_directory, char* visible_directory);

//TODO
//what does this func do
void parse(const char* s, char* working_directory, char* visible_directory);

void change_dir(const char *s,char *working_directory,char *visible_directory,unsigned int *index_of_next_word);


//print out everything in the current directory
void ls(const char *working_directory);

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
