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
vector<vector<uint8_t>> hex_to_binary(vector<string> hex_lines) {

    // declare variables
    vector<string> binary_lines;
    vector<uint8_t> line_bytes;
    vector<vector<uint8_t>> binary_bytes;

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
            line_bytes.push_back(curr_byte);
            cout << (int)curr_byte << " ";
        }
        // push back bytes of current line
        binary_bytes.push_back(line_bytes);
    }

    // return
    return binary_bytes;
}

// perform CRC-8 on hex lines to compute checksum value
vector<uint8_t> crc_8(vector<vector<uint8_t>> binary_bytes) {

    // define polynomial (use standard) and vector of checksums
    const uint8_t polynomial = 0x07;
    vector<uint8_t> checksums;

    // CRC algorithm
    for ( auto line_bytes : binary_bytes ) {

        // reset initial CRC value
        uint8_t crc = 0x00;
        for ( auto byte : line_bytes ) {
            // perform algorithm
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
        // add new line's checksum to vector
        checksums.push_back(crc);
    }

    // return
    cout << endl << "\nCRC Checksums (per line)";
    cout << "\n------------------------" << endl;
    for ( int i = 0; i < checksums.size(); i++ ) {
        cout << endl << i << " " << (int)checksums[i] << endl;
    }
    
    return checksums;
}

// stamp binary data with checksum byte at the end
    // (line breaks for clarity, though standard procedure is all data bytes concatenated together)
void stamp_file(vector<vector<uint8_t>> binary_bytes, vector<uint8_t> checksums) {

    // create binary file
    ofstream outFile;
    
    // write to binary file, generate 10,000 random characters (file, mode flags)
    outFile.open("stamped_file.bin", ios::out | ios::binary );  // OR sets both bit flags to 1
    if ( outFile ) {    // .dat is general data file (can be text or binary)
        
        // print all binary bytes to file and stamp each line with checksum
        int i = 0;
        for ( auto line_bytes : binary_bytes ) {

            // get all bytes in current line
            for ( auto byte : line_bytes ) {
                // write bytes to file
                outFile.write(reinterpret_cast<const char*>(&byte), sizeof(uint8_t));
            }

            // checksum stamp for current line
            char checksum_char = char(checksums[i]);
            outFile.write(reinterpret_cast<const char*>(&checksum_char), sizeof(uint8_t));
            i++;
        }
        outFile.close();
        
    } else {
        cout << "Error opening binary file.\n" << endl;
    }
}

// recompute CRC-8 and verify checksum
bool verify_crc(const vector<vector<uint8_t>>& binary_bytes, const vector<uint8_t>& checksums) {

    // open stamped file
    ifstream stamped_file("stamped_file.bin", ios::in | ios::binary);
    if (stamped_file) {

        // read all bytes in the stamped file
        vector<uint8_t> stamped_data((istreambuf_iterator<char>(stamped_file)), istreambuf_iterator<char>());
        stamped_file.close();

        // define polynomial
        const uint8_t polynomial = 0x07;

        // initialize variables
        size_t pos = 0;
        bool all_passed = true;

        // iterate through original binary lines
        cout << endl << "Verify CRC Checksums" << endl;
        cout << "------------------------" << endl;
        for (size_t i = 0; i < binary_bytes.size(); ++i) {

            // get entire line of data (including checksum byte at the end)
            const vector<uint8_t>& line_data = binary_bytes[i];
            size_t line_length = line_data.size();

            // extract data and stamped checksum
            vector<uint8_t> stamped_line_data(stamped_data.begin() + pos, stamped_data.begin() + pos + line_length);
            uint8_t stamped_crc = stamped_data[pos + line_length];

            // recompute CRC
            uint8_t crc = 0x00;
            for (auto byte : stamped_line_data) {
                crc ^= byte;
                for (int b = 0; b < 8; b++) {
                    if (crc & 0x80)
                        crc = (crc << 1) ^ polynomial;
                    else
                        crc <<= 1;
                    crc &= 0xFF;
                }
            }

            // display comparison results
            cout << "\nOriginal Checksum: " << (int)stamped_crc << "\nRecomputed Checksum: " << (int)crc << endl;

            // compare with stored checksum
            cout << "Line " << i << ": ";
            if (crc == stamped_crc) {
                cout << "PASS" << endl;
            } else {
                cout << "FAIL" << endl;
                all_passed = false;
            }

            // move position
            pos += line_length + 1;
        }

        // return result
        return all_passed;

    } else {
        cout << "Error opening binary file.\n" << endl;
        return false;
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
    vector<vector<uint8_t>> binary_bytes = hex_to_binary(hex_lines);

    // run CRC-8 algorithm to get checksum
    vector<uint8_t> checksums = crc_8(binary_bytes);

    // stamp file with checksum
    stamp_file(binary_bytes, checksums);

    // verify CRC-8 checksum
    bool result = verify_crc(binary_bytes, checksums);

    // display final result
    cout << endl;
    if ( result ) {
        cout << "Final result: PASS" << endl;
    } else {
        cout << "Final result: FAIL" << endl;
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