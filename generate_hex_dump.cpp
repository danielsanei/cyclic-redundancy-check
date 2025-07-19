// imports
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <vector>
#include <unordered_map>
using namespace std;

// generate binary file
void gen_bin_file() {

    // create binary file
    ofstream binFile;
    
    // write to binary file, generate 10,000 random characters (file, mode flags)
    binFile.open("bin_file.bin", ios::out | ios::binary );  // OR sets both bit flags to 1
    if ( binFile ) {    // .dat is general data file (can be text or binary)
        
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

// extract data fields from hex file
vector<pair<string, vector<string>>> parse_hex_file() {

    // initialize input stream
    ifstream binFile;
    vector<string> hex_values;
    
    // retrieve file contents
    binFile.open("sample_hex.hex");
    if ( binFile ) {
        
        // store each line of hex file
        while ( !binFile.eof() ) {  // continue until end of file
            string line;
            getline(binFile, line);
            hex_values.push_back(line);
        }
        binFile.close();

        // check hex values
        for ( int i = 0; i < hex_values.size(); i++ ) {
            cout << hex_values[i] << endl;
        }

    } else {
        cout << "Error opening binary file.\n" << endl;
    }

    // extract data fields
    //unordered_map<string, vector<string>> dataFields;     // DON'T USE --> does not keep sequential order 
    vector<pair<string, vector<string>>> dataFields;        // declare map of data (hex, data fields)
    for ( int i = 0; i < hex_values.size(); i++ ) {

        // get indices for variable-length data
        int start = 9;
        int end = (hex_values[i].size() - 2) - start;

        // initialize extracted fields (ignore first semicolon)W
        vector<string> currFields;
        string byte_count = hex_values[i].substr(1, 2);
        currFields.push_back(byte_count);
        string address = hex_values[i].substr(4, 4);
        currFields.push_back(address);
        string record_type = hex_values[i].substr(7, 2);
        currFields.push_back(record_type);
        string data = hex_values[i].substr(start, end);
        currFields.push_back(data);
        string checksum = hex_values[i].substr(hex_values[i].size() - 2, 2);
        currFields.push_back(checksum);
        
        // initialize map of data (hex, data fields)
        dataFields.push_back(make_pair(hex_values[i], currFields));
        dataFields[i].first = hex_values[i];
    }

    // check extracted data
    cout << endl << "Extracted Data" << endl << endl;
    vector<string> fields = {"Byte Count: ", "Address: ", "Record Type: ", "Data: ", "Checksum: "};
    for ( auto hex_value : dataFields ) {
        cout << hex_value.first << endl;
        for ( int i = 0; i < hex_value.second.size(); i++ ) {
            cout << fields[i] << hex_value.second[i] << endl;
        }
        cout << endl;
    }

    // return
    return dataFields;
}

// driver code
int main() {

    char test = int('a');
    cout << char(99) << endl;
    
    // generate binary file with random characters (10 KB)
    //gen_bin_file();

    // parse hex file
    vector<pair<string, vector<string>>> dataFields;
    dataFields = parse_hex_file();

    // return
    return 0;
}

/*
Notes
- 10 KB size will show as 10,000 / 1,024 = 9.76 KB given file explorer conventions
*/