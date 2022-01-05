#ifndef READ_H
#define READ_H


//print out the [user]$~ prefix
void print_prefix(const char* visible_directory);

//store the last part of the directory in a string
void getvwd(const char* current_path,char* visible_directory);


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

//parse borrowed from brennen.io
char **lsh_split_line(char *line, int *position);

//count all words in s
unsigned int count_words(const char* s);

//free 2d array
void free_2d(char** s,const unsigned short argc);

void change_dir(const char *s,char *working_directory,char *visible_directory,unsigned int *index_of_next_word);

//fork, look for bin, exec, return 1 for success, 0 no file, -1 failed exec
short fork_exec(const char* s);

char** shorten_arg(char** argv, int arg_amount);

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


//DEBUG FUNCTIONS
//#############################################################################

void print2d(char** argv, const int argc);

//easy write to stdout
void fprint(const char* s);
void fdprint(const char* s);
#endif
