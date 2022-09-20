//
// Main file to handle encoding and decoding of input file.
//
#include <string.h>
#include <iostream>
#include <unordered_map>
#include <fstream>
#include <bits/stdc++.h>
#include "Node.h"

using namespace std;

#ifndef HUFFMANCODING_HUFFER_H
#define HUFFMANCODING_HUFFER_H

#define INTERNAL_NODE_CHARACTER char(188)
#define PSEUDO_EOF char(129)
#define CHARACTER_CODE_SEPERATOR char(189)
#define HEADER_ENTRY_SEPERATOR char(190)
#define HEADER_TEXT_SEPERATOR char(191)


class Huffman 
{
private:
    unordered_map<char,string> codeMap;

    void encodeCharacters(Node *rootNode, string codeString)
    {
        if (!rootNode)
        {
            return;
        }
        if (rootNode->getCharacter() != INTERNAL_NODE_CHARACTER) 
        {
            codeMap[rootNode->getCharacter()] = codeString;
        }
        encodeCharacters(rootNode->getLeft(), codeString + "0");
        encodeCharacters(rootNode->getRight(), codeString + "1");
    }

    void writeHeader(ofstream &outputStream)
    {
        for (const auto &item : codeMap)
            outputStream << item.first << CHARACTER_CODE_SEPERATOR << item.second << HEADER_ENTRY_SEPERATOR;
        outputStream << HEADER_TEXT_SEPERATOR;
    }

    void readHeader(ifstream &inputStream)
    {
        codeMap.clear();
        char character;
        inputStream.get(character);
        char key = character;
        while (character != HEADER_TEXT_SEPERATOR) 
        {
            if (character == CHARACTER_CODE_SEPERATOR) 
            {
                inputStream.get(character);
                while (character != HEADER_ENTRY_SEPERATOR) 
                {
                    codeMap[key] += character;
                    inputStream.get(character);
                }
            } 
            else
                key = character;
            inputStream.get(character);
        }
        
    }

    Node* buildDecodingTree(unordered_map<char,string> encodingMap)
    {
        Node *rootNode = new Node(INTERNAL_NODE_CHARACTER);
        Node *previousNode;

        for (const auto &item : encodingMap) 
        {
            previousNode = rootNode;
            Node *newNode = new Node(item.first);
            string characterCode = item.second;
            for (int i = 0; i < characterCode.size(); ++i) 
            {
                if (characterCode[i] == '0') 
                {
                    if (i == characterCode.size() - 1)
                        previousNode->setLeft(newNode);
                    else 
                    {
                        if (!previousNode->getLeft()) 
                        {
                            previousNode->setLeft(new Node(INTERNAL_NODE_CHARACTER));
                            previousNode = previousNode->getLeft();
                        } 
                        else previousNode = previousNode->getLeft();
                    }
                }
                else 
                {
                    if (i == characterCode.size() - 1)
                        previousNode->setRight(newNode);
                    else 
                    {
                        if (!previousNode->getRight()) 
                        {
                            previousNode->setRight(new Node(INTERNAL_NODE_CHARACTER));
                            previousNode = previousNode->getRight();
                        } 
                        else previousNode = previousNode->getRight();
                    }
                }
            }

        }
        return rootNode;
    }

    class myCompartor
    {
    public :
        int operator() (Node* node1, Node* node2)
        {
            return node1->getFrequency() > node2->getFrequency();
        }
    };

public :
    void huffer(unordered_map<char, int> frequencyMap)
    {
        priority_queue <Node *, vector<Node *>, myCompartor> HufferQueue;
        string tempString;
        Node *leftNode, *rightNode, *newNode;
        for (const auto &item : frequencyMap)
            HufferQueue.push(new Node(item.first, item.second));
        HufferQueue.push(new Node(PSEUDO_EOF, 1));
        while (HufferQueue.size() != 1) 
        {
            leftNode = HufferQueue.top();
            HufferQueue.pop();
            rightNode = HufferQueue.top();
            HufferQueue.pop();
            newNode = new Node(INTERNAL_NODE_CHARACTER, leftNode->getFrequency() + rightNode->getFrequency());
            HufferQueue.push(newNode);
            newNode->setLeft(leftNode);
            newNode->setRight(rightNode);
        }
        encodeCharacters(HufferQueue.top(), tempString);
        return;
    }

    void deHuffer(string compressedFileName,string decompressedFileName)
    {
        char character;
        string codeString;
        ifstream inputStream;
        inputStream.open(compressedFileName, ios_base::in|ios::binary);
        readHeader(inputStream);
        do {
            character = inputStream.get();
            if(character==char_traits<wchar_t>::eof())
                break;
            bitset<8> bits(character);
            codeString += bits.to_string();
        } while(!inputStream.fail());
        Node *rootNode = buildDecodingTree(codeMap);
        decompressToFile(codeString, rootNode, decompressedFileName);
        return;
    }

    void compressTofile(string InputfileName ,string OutputfileName)
    {
        char character;
        string file;
        ifstream inputStream;
        ofstream outputStream;
        outputStream.open(OutputfileName, ios::out);
        inputStream.open(InputfileName, ios::in);
        writeHeader(outputStream);
        while (inputStream.get(character))
            file += codeMap[character];
        inputStream.close();
        file += codeMap[PSEUDO_EOF];
        unsigned long remainder = (file.size() - 1) % 8;
        for (int i = 0; i < 8 - remainder; ++i)
            file += '0';
        stringstream stringStream(file);

        while (stringStream.good()) {
            bitset<8> bits;
            stringStream >> bits;
            char c = char(bits.to_ulong());
            outputStream << c;
        }


        outputStream.flush();
        outputStream.close();
    }

    void decompressToFile(string codeString,Node *rootNode,string decompressedFileName)
    {
        ofstream outputStream;
        outputStream.open(decompressedFileName, ios::out);
        Node *traversingPointer = rootNode;
        for (int i = 0; i < codeString.size() + 1; ++i) {
            if (codeString[i] == '0')
                traversingPointer = traversingPointer->getLeft();
            else
                traversingPointer = traversingPointer->getRight();

            if (traversingPointer->getCharacter() != INTERNAL_NODE_CHARACTER) {
                if (traversingPointer->getCharacter() == PSEUDO_EOF)
                    break;
                outputStream << traversingPointer->getCharacter();
                traversingPointer = rootNode;
            }
        }
        outputStream.flush();
        outputStream.close();
    }
};

#endif
