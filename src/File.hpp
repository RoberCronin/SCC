#pragma once

#include <iostream>
#include <vector>

enum TokenType {
#define TOKENTYPE_DEF(x) x,
#include "TokenType.txt"
#undef TOKENTYPE_DEF
};

enum SeparatorToken {
#define TOKENTYPE_DEF(x) x,
#include "SeparatorTokens.txt"
#undef TOKENTYPE_DEF
};

enum KeywordToken {
#define TOKENTYPE_DEF(x) x,
#include "KeywordTokens.txt"
#undef TOKENTYPE_DEF
};

enum ConstantTokens {
#define TOKENTYPE_DEF(x) x,
#include "ConstantTokens.txt"
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

    void SearchForKeywordTokens(int index);

    void SearchForSeparatorTokens(int index);

    void SearchForSingleKeyword(const char* searchString, KeywordToken tokenType, int index);

    void SearchSeparatorTokens(const int& index);

    int keywordCharMatchCounter[31];

    void AddToken(int tokenIndex, int tokenLength, TokenType tokenType);
};
