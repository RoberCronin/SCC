#pragma once

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
    // Searches for substring that are separated by whitespace, and or non-separator tokens
    // then adds that substring to the 'tokens' vector
    void SearchForConsecutiveChars(const int& index);
    int currentTokenLength = 0;

    // Changes tokens to correct types
    void ChangeKeywordsToCorrectTokenType();
    void ChangeIntegersToCorrectTokenType();

    // Checks if a string matches a token
    bool CompareStringToToken(const char* str1, const int& str1Length, const int& indexOfToken);

    // Changes the type of token for all keywords matching 'searchString'
    void ChangeTokenTypeOfKeyword(const char* searchString, const TokenType& tokenType);

    bool IsTokenInteger(const int& indexOfToken);
    bool CharAtIndexIsSeparator(const int& index);
    bool CharAtIndexIsWhitespace(const int& index);

    TokenType GetSeparatorTokenTypeFromIndex(const int& index);

    // Opens file, and saves data to filePointer, and saves length of file in length
    void OpenFile(const char* FilePath);
    void AddToken(const int& tokenIndex, const int& tokenLength, const TokenType& tokenType);
};
