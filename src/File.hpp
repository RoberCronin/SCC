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
    int tokenIndex;
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
    std::vector<TokenInfo> token;

    File(std::string FilePath);
    ~File();

    void Tokenize();

    void PrintTokens();

private:
    // Opens file, and saves data to filePointer, and saves length of file in length
    void OpenFile(std::string FilePath);

    void AddToken(TokenType tokenType, int tokenIndex, int tokenValueSize);

    void GetAllSeparatorTokens();

    void GetAllKeywordTokens();

    void GetAllOfSingleKeyword(const char* searchString, TokenType tokenType);

    void AddToken(int tokenIndex, int tokenLength, TokenType tokenType);
};
