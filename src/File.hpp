#pragma once

#include <iostream>
#include <vector>

enum TokenType {
#define TOKENTYPE_DEF(x) x,
#include "TokenType.txt"
#undef TOKENTYPE_DEF
};

struct TokenInfo
{
    int tokenLength;
    TokenType tokenType;
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

    void AddToken(TokenType tokenType, int tokenIndex, int tokenValueSize);

    TokenInfo GetTokenInfoFromIndex(int index);

    int GetIndexOfNextToken(int indexOfLastToken, int lengthOfLastToken);

    bool insideQuotes = false;
};
