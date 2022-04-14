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

class Tokenizer
{
public:
    char* filePointer;
    int fileLength;
    int tokenCount = 0;
    std::vector<TokenInfo> tokens;

    Tokenizer(const char* FilePath);
    ~Tokenizer();

    void Tokenize();

    void PrintTokens();

private:
    int currentTokenLength = 0;
    void SearchForConsecutiveChars(const int& index);

    void AddToken(const int& tokenIndex, const int& tokenLength, const TokenType& tokenType);

    void ChangeKeywordsToCorrectTokenType();

    void ChangeTokenTypeOfKeyword(const char* searchString, const TokenType& tokenType);

    void ChangeIntegersToCorrectTokenType();

    bool IsTokenInteger(const int& indexOfToken);

    bool CompareStringToToken(const char* str1, const int& str1Length, const int& indexOfToken, const int& tokenLength);

    bool CharAtIndexIsSeparator(const int& index);

    bool CharAtIndexIsWhitespace(const int& index);

    TokenType GetSeparatorTokenTypeFromIndex(const int& index);

    // Opens file, and saves data to filePointer, and saves length of file in length
    void OpenFile(const char* FilePath);
};
