#pragma once

#include <iostream>
#include <vector>

enum TokenType {
#define TOKENTYPE_DEF(x) x,
#include "TokenType.txt"
#undef TOKENTYPE_DEF
};

class File
{
public:
    char* filePointer;
    int fileLength;
    int tokenCount = 0;
    std::vector<char*> tokens;

    File(std::string FilePath);
    ~File();

    void Tokenize();

    void PrintTokens();

private:
    // Opens file, and saves data to filePointer, and saves length of file in length
    void OpenFile(std::string FilePath);

    // Add a token
    void AddToken(TokenType tokenType, char* tokenValue, int tokenValueSize);

    // Add a single char of a token
    int partialTokenCallCount = 0;
    void AddPartialTokenValue(std::string tokenType, char tokenValue, int tokenValueSize);

    bool insideQuotes = false;
    int tokenTypeSize = 0;
};