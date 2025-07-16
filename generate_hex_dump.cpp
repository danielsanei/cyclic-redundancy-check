// imports
#include <iostream>
#include <fstream>
#include <ctime>
using namespace std;

// generate binary file
void gen_bin_file() {

    // create binary file
    ofstream binFile;
    
    // write to binary file, generate 10,000 random characters (file, mode flags)
    binFile.open("bin_file.dat", ios::out | ios::binary );  // OR sets both bit flags to 1
    if ( binFile ) {
        
        // Generate random number
        srand(time(0)); // set seed to current time (seconds)
        int fileSize = 10000;   // 1 char = 1 byte => 10 KB file
        for ( int i = 0; i < fileSize; i++ ) {
            int randomNum = 97 + rand() % 26;
            char randomChar = char(randomNum);
            binFile.write(&randomChar, sizeof(char));
            //cout << randomChar << endl;
        }
        binFile.close();

    } else {
        cout << "Error opening binary file.\n" << endl;
    }
}

// driver code
int main() {

    char test = int('a');
    cout << char(99) << endl;
    
    // generate binary file with random characters (10 KB)
    gen_bin_file();

    // return
    return 0;
}

/*
Notes
- 10 KB size will show as 10,000 / 1,024 = 9.76 KB given file explorer conventions
*/