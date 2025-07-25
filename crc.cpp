// imports
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <vector>
#include <unordered_map>
#include <bits/stdc++.h>
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
vector<string> parse_hex_file() {

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
        cout << endl << "Original Intel HEX file" << endl;
        cout << "------------------------" << endl;
        for ( int i = 0; i < hex_values.size(); i++ ) {
            cout << hex_values[i] << endl;
        }

    } else {
        cout << "Error opening binary file.\n" << endl;
    }

    // extract data fields
    //unordered_map<string, vector<string>> dataFields;     // DON'T USE --> does not keep sequential order 
    vector<pair<string, vector<string>>> dataFields;        // declare map of data (hex, data fields)
    vector<string> hex_lines;                               // extracted hex data
    for ( int i = 0; i < hex_values.size(); i++ ) {

        // get indices for variable-length data
        int start = 9;
        int end = (hex_values[i].size() - 2) - start;

        // initialize extracted fields (ignore first semicolon)
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
        
        // initialize map of data (hex, data fields), consider only data bytes (record type == 00)
        if ( record_type == "00" ) {
            dataFields.push_back(make_pair(hex_values[i], currFields));
            hex_lines.push_back(data);
        }
    }

    // check extracted data
    cout << endl << "Extracted Data" << endl;
    vector<string> fields = {"Byte Count: ", "Address: ", "Record Type: ", "Data: ", "Checksum: "};
    for ( auto hex_value : dataFields ) {
        cout << "---------------------------------------------" << endl;
        cout << hex_value.first << endl;
        for ( int i = 0; i < hex_value.second.size(); i++ ) {
            cout << fields[i] << hex_value.second[i] << endl;
        }
    }

    // check hex lines
    cout << endl << "Hex Lines" << endl;
    cout << "----------" << endl;
    for ( int i = 0; i < hex_lines.size(); i++ ) {
        cout << hex_lines[i] << endl;
    }

    // return
    return hex_lines;
}

// convert hex data to binary
vector<uint8_t> hex_to_binary(vector<string> hex_lines) {

    // declare variables
    vector<string> binary_lines;
    vector<uint8_t> binary_bytes;

    // convert hexadecimal to binary
    cout << endl << "Convert hex to binary";
    for ( int i = 0; i < hex_lines.size(); i++ ) {
        cout << endl << "----------------------------------------------------------------" << endl;
        string curr_hex = hex_lines[i];
        cout << curr_hex << endl;
        // convert hex line to binary (split up instead of converting entire line at once)
        for ( int j = 0; j < curr_hex.size(); j += 2 ) {
            string byte_str = curr_hex.substr(j, 2);
            uint8_t curr_byte = static_cast<uint8_t>(stoul(byte_str, nullptr, 16));
            binary_bytes.push_back(curr_byte);
            cout << (int)curr_byte << " ";
        }
    }

    // return
    return binary_bytes;
}

// perform CRC-8 on hex lines to compute checksum value
uint8_t crc_8(vector<uint8_t> binary_bytes) {

    // define polynomial (use standard), initial value
    const uint8_t polynomial = 0x07;
    uint8_t crc = 0x00;

    // CRC algorithm
    for ( auto byte : binary_bytes ) {
        crc = crc ^ byte;                       // XOR
        for ( int i = 0; i < 8; i++ ) {         // for each bit in current byte
            if ( crc & 0x80 ) {                // 0x80 --> 128 --> 10000000
                crc = (crc << 1) ^ polynomial;
            } else {
                crc = crc << 1;
            }
            crc = crc & 0xFF;                   // keep within 8 bits
        }
    }

    // return
    cout << endl;
    cout << endl << "CRC Checksum: " << (int)crc << endl;
    return crc;
}

// stamp binary data with checksum byte at the end
    // (line breaks for clarity, though standard procedure is all data bytes concatenated together)
void stamp_file(vector<uint8_t> binary_bytes, uint8_t checksum) {

    // create binary file
    ofstream outFile;
    
    // write to binary file, generate 10,000 random characters (file, mode flags)
    outFile.open("stamped_file.bin", ios::out | ios::binary );  // OR sets both bit flags to 1
    if ( outFile ) {    // .dat is general data file (can be text or binary)
        
        // print all binary bytes to file
        for ( int i = 0; i < binary_bytes.size(); i++ ) {
            outFile.write(reinterpret_cast<const char*>(&binary_bytes[i]), sizeof(uint8_t));
        }
        // checksum stamp
        char checksum_char = char(checksum);
        outFile.write(reinterpret_cast<const char*>(&checksum), sizeof(uint8_t));
        outFile.close();

    } else {
        cout << "Error opening binary file.\n" << endl;
    }
}

// recompute CRC-8 and verify checksum
uint8_t verify_crc() {

    // create binary file
    ifstream stamped_file;
    
    // open stamped file
    stamped_file.open("stamped_file.bin", ios::in | ios::binary);
    if ( stamped_file ) {

        // read all binary bytes inside stamped file
        vector<uint8_t> binary_bytes((istreambuf_iterator<char>(stamped_file)), istreambuf_iterator<char>());
        stamped_file.close();

        // extract data, exclude CRC checksum
        uint8_t stamped_crc = binary_bytes.back();
        binary_bytes.pop_back();  // remove checksum

        // recompute CRC-8
        const uint8_t polynomial = 0x07;
        uint8_t crc = 0x00;
        for ( auto byte : binary_bytes ) {
            crc = crc ^ byte;                       // XOR
            for ( int i = 0; i < 8; i++ ) {         // for each bit in current byte
                if ( crc & 0x80 ) {                // 0x80 --> 128 --> 10000000
                    crc = (crc << 1) ^ polynomial;
                } else {
                    crc = crc << 1;
                }
                crc = crc & 0xFF;                   // keep within 8 bits
            }
        }

        // return resulting checksum
        return crc;

    } else {
        cout << "Error opening binary file.\n" << endl;
        return -1;
    }
}

// driver code
int main() {

    // quick check
    char test = int('a');
    cout << char(99) << endl;
    
    // generate binary file with random characters (10 KB)
    //gen_bin_file();

    // parse hex file
    vector<string> hex_lines = parse_hex_file();
    vector<uint8_t> binary_bytes = hex_to_binary(hex_lines);

    // run CRC-8 algorithm to get checksum
    uint8_t checksum = crc_8(binary_bytes);

    // stamp file with checksum
    stamp_file(binary_bytes, checksum);

    // verify CRC-8 checksum
    uint8_t recomputed_checksum = verify_crc();
    cout << endl;
    if (checksum == recomputed_checksum) {
        cout << "CRC verification passed!" << endl;
    } else {
        cout << "CRC verification failed" << endl;
        cout << "Expected: " << (int)recomputed_checksum << ", Found: " << (int)checksum << endl;
    }

    // return
    return 0;
}

/*
Notes
- 10 KB size will show as 10,000 / 1,024 = 9.76 KB given file explorer conventions

----------------------
Intel HEX File Format
----------------------
Record Type                 Code	Purpose
Data	                    00	    Actual data to be written to memory
EOF	                        01	    End of file marker
Extended Segment Address	02	    Used for addressing beyond 64KB
Extended Linear Address	    04	    Used for 32-bit addressing
Start Linear Address	    05	    Optional, for specifying execution start address
*/