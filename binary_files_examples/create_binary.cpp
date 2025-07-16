// Imports
#include <iostream>
#include <fstream>
using namespace std;

void number_example() {

    // create files
    ofstream text;  // defaults to text file (write to file only)
    fstream fout;   // (both read and write to file)

    // write to file (verify if file opened successfully)
    fout.open("file.dat", ios::out | ios::binary);  // specify binary file
    if ( fout ) {

        // declare variables
        unsigned short x = 8675;

        // write to file --> memory address (as char), how many bytes
        fout.write(reinterpret_cast<char*>(&x), sizeof(unsigned short));
        //fout << x;      // write as text file
        fout.close();
        
    } else {
        cout << "Error opening file\n";
    }

    // read from file (verify if file opened successfully)
    fout.open("file.dat", ios::in | ios::binary);  // specify binary file
    if ( fout ) {

        // declare variables
        unsigned short y = 0;

        // read from file --> memory address (as char), how many bytes
        fout.read(reinterpret_cast<char*>(&y), sizeof(unsigned short));
        fout.close();
        cout << "Reading from file.dat --> " << y << endl;

    } else {
        cout << "Error opening file\n";
    }
}

void char_example() {

    // create files
    ofstream text;  // defaults to text file (write to file only)
    fstream fout;   // (both read and write to file)

    // write to file (verify if file opened successfully)
    fout.open("file.dat", ios::out | ios::binary);  // specify binary file
    if ( fout ) {

        // declare variables
        char x = 'a';

        // write to file --> memory address (as char), how many bytes
        fout.write(&x, sizeof(unsigned short));
        //fout << x;      // write as text file
        fout.close();

    } else {
        cout << "Error opening file\n";
    }

    // read from file (verify if file opened successfully)
    fout.open("file.dat", ios::in | ios::binary);  // specify binary file
    if ( fout ) {

        // declare variables
        char y = 'z';

        // read from file --> memory address (as char), how many bytes
        fout.read(&y, sizeof(unsigned short));
        fout.close();
        cout << "Reading from file.dat --> " << y << endl;

    } else {
        cout << "Error opening file\n";
    }
}

void read_array_example() {

    fstream fout;   // use this for binary (to write)
    ofstream f;     // use this with for loop
    fout.open("file.dat", ios::binary | ios::out);
    if ( fout ) {
        int a[] = {8, 6, 7, 5, 3, 0, 9};
        // requires 7 writes
        // for ( auto c : a ) {
        //     f << c;
        // }
        fout.write(reinterpret_cast<char*>(a), sizeof(a));
        fout.close();

    } else {
        cout << "Error opening file\n";
    }

    int b[7];
    fout.open("file.dat", ios::in | ios::binary);
    if ( fout ) {
        fout.read(reinterpret_cast<char*>(b), sizeof(b));
        for ( int i : b ) {
            cout << i << " ";
        }
        cout << endl;
    } else {
        cout << "Error opening file\n";
    }
}

void read_array_example_2(int* a, int size) {
    fstream fin("file.dat", ios::in | ios::binary);
    if (fin) {
        fin.read(reinterpret_cast<char*>(a), size * sizeof(int));
        fin.close();
    } else {
        cout << "Error opening file for reading!\n";
    }
}

void write_array_example(int* a, int size)
{
    fstream fout("file.dat", ios::out | ios::binary);
    if (fout)
    {
        fout.write(reinterpret_cast<char*>(a), size * sizeof(int));
        fout.close();
    } else {
        cout << "Error opening file for writing!\n";
    }
}

int main() {

    // number example
    number_example();

    // character example
    char_example();

    // read array example
    read_array_example();

    // write array example
    int SIZE = 10;
    int c[SIZE] = {5, 1, 0, 8, 6, 7, 5, 3, 0, 9};
    int d[SIZE];
    write_array_example(c, SIZE);
    read_array_example_2(d, SIZE);
    for (auto i : d)
        cout << i << " ";
    cout << endl;

    // return
    return 0;
}

/*
Binary Numbers
- Take number 8675 in text file
- Each character --> 1 byte
- File needs 4 byte stotal to store
- However, binary files store raw data
- No need to consider ASCII conversion
- Saves space
*/