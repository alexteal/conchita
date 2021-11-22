#include <unistd.h>
#include <cstdlib>
#include <iostream>

using std::cout;
using std::endl;

int main(const int argc, const char * argv[], const char * envp[]){
    setvbuf(stdout, NULL, _IONBF, 128);
    // loops through each pointer until you reach null
    for (int i = 0; envp[i] != nullptr; i++) {
        cout << envp[i] << endl;
    } // for
    return EXIT_SUCCESS;
} // main