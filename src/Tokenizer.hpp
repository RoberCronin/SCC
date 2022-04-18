#pragma once

#include <vector>

#include "Tokens.hpp"

class Tokenizer
{
public:
    // Pointer to the beginning of the file, can be accsessed like an array
    char* fileChars;
    // Length of the file, in bytes/chars
    int fileLength;
    std::vector<TokenInfo> tokens;

    Tokenizer(const char* FilePath);
    ~Tokenizer();

    void Tokenize();

    void PrintTokens();

private:
    // Searches for substring that are separated by whitespace, and or non-separator tokens
    // then adds that substring to the 'tokens' vector
    void SearchForIndividualTokens(const int& index);
    int currentTokenLength = 0;

    // Changes tokens to correct types
    void ChangeKeywordsToCorrectTokenType();
    void ChangeIntegersToCorrectTokenType();
    void ChangeIdentifiersToCorrectTokenType();

    // Checks if a string matches a token
    bool CompareStringToToken(const char* str1, const int& str1Length, const int& indexOfToken);

    // Changes the type of token for all keywords matching 'searchString'
    void ChangeTokenTypeOfKeyword(const char* searchString, const TokenType& tokenType);

    bool IsTokenInteger(const int& indexOfToken);
    bool CharAtIndexIsSeparator(const int& index);
    bool CharAtIndexIsWhitespace(const int& index);
    bool IsTokenIdentifier(const int& indexOfToken);
    bool IsSpecialCharacter(const char& character);


    // Gets the type of token for a separator
    TokenType GetSeparatorTokenTypeFromIndex(const int& index);

    // Opens file, and saves data to filePointer, and saves length of file in length
    void OpenFile(const char* FilePath);
    void AddToken(const int& tokenIndex, const int& tokenLength, const TokenType& tokenType);
};
