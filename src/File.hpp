#pragma once

#include <iostream>
#include <vector>

enum TokenType { SEPARATOR, IDENTIFIER, KEYWORD, OPERATOR, TYPE, COMMENT, CONSTANT_STRING, CONSTANAT_INTEGER, CONSTANT_BOOLEAN };

class File
{
public:
    char* filePointer;
    int length;
    std::vector<char*> tokens;

    File(std::string FilePath);
    ~File();

    void Tokenize();

    void PrintTokens();

private:
    // Opens file, and saves data to filePointer, and saves length of file in length
    void OpenFile(std::string FilePath);

    // Add a token
    void AddToken(std::string tokenType, std::string tokenValue, int tokenValueSize);

    // Add a single char of a token
    int partialTokenCallCount = 0;
    void AddPartialTokenValue(std::string tokenType, char tokenValue, int tokenValueSize);

    bool insideQuotes = false;
    int tokenTypeSize = 0;
};