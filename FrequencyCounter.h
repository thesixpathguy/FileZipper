/*
*
* FrequencyCounter module is to read input file and count frequency for each character.
*
*/
#include <string.h>
#include <iostream>
#include <unordered_map>
#include <fstream>

using namespace std;

#ifndef FREQUENCY_COUNTER_H_
#define FREQUENCY_COUNTER_H_


class FrequencyCounter 
{
private:
    unordered_map<char,int> frequencyMap;

public:
    const unordered_map<char, int> &getFrequencyMap() const
    {
        return frequencyMap;
    }

    void readFile(string fileName)
    {
            char character;

            ifstream inputFile;
            inputFile.open(fileName, ios::in);

            while(inputFile.get(character))
                this->frequencyMap[character]++;

            inputFile.close();
    }
};

#endif