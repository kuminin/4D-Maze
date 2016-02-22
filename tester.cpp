//
//  tester.cpp
//  MazesBetter
//
//  Created by Kumin In on 2/22/16.
//  Copyright © 2016 Kumin In. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main(int argc, const char* argv[]) {
    // initialize memblock for the size of the file and inFileSize in order
    // to obtain the size of the file.
    char *memblock;
    streampos inFileSize;
    
    // open the file for binary
    ifstream inFile (argv[1], ios::binary | ios::in);
    ofstream outFile ("output");
    
    // tell the file to unset function for skipping white space.
    inFile.unsetf(ios::skipws);
    
    // Gets the size of the binary file...
    inFile.seekg(0, ios::end);
    inFileSize = inFile.tellg();
    inFile.seekg(0, ios::beg);
    
    // request the allocation of a memory block large enough to hold the file
    memblock = new char[inFileSize];
    
    // read the file into memblock of size inFileSize
    inFile.read(memblock, inFileSize);
    
    // output the given input BYTE file into a string 
    for (int i = 0; i < inFileSize; i++) {
        string wall = "";
        for (char bit = 7; bit >= 0; --bit) {
            wall += ((memblock[i] >> bit) & 1);
        }
        for (int j = 0; j < 8; j++) {
            outFile << (int)wall[j];
        }
    }
    inFile.close();
    outFile.close();
}