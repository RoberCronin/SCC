#include <fstream>
#include <iostream>
#include <vector>

#include "File.hpp"

File::File(const char* FilePath)
{
    OpenFile(FilePath);
}

File::~File()
{
    delete[] filePointer;
}

void File::Tokenize()
{
    for (int i = 0; i < fileLength; i++)
    {
        SearchForConsecutiveChars(i);
        if (CharAtIndexIsSeparator(i)) AddToken(i, 1, GetSeparatorTokenTypeFromIndex(i));
    }

    ChangeTokenTypeOfKeyword("return", KEYWORD_RETURN);
}

void File::PrintTokens()
{
    for (int i = 0; i < token.size(); i++)
    {
        // print token type
        switch (token[i].tokenType)
        {
            // weird formatting because of using the preprocessor to automatically get token type enum names
#define TOKENTYPE_DEF(x)                                                                                                                                       \
    case x:                                                                                                                                                    \
        std::cout << #x;                                                                                                                                       \
        break;
#include "TokenType.txt"
#undef TOKENTYPE_DEF
        }

        std::cout << " \"";

        for (int j = 0; j < token[i].tokenLength; j++)
        {
            std::cout << filePointer[token[i].tokenIndex + j];
        }
        std::cout << "\"    Token Index: " << token[i].tokenIndex << "    Token Length: " << token[i].tokenLength;
        std::cout << std::endl;
    }
}

void File::OpenFile(const char* FilePath)
{
    std::ifstream t;
    t.open(FilePath);                    // open input file
    t.seekg(0, std::ios::end);           // go to the end
    fileLength = t.tellg();              // report location (this is the length)
    t.seekg(0, std::ios::beg);           // go back to the beginning
    char* buffer = new char[fileLength]; // allocate memory for a buffer of appropriate dimension
    t.read(buffer, fileLength);          // read the whole file into the buffer
    t.close();                           // close file handle
    filePointer = buffer;
}

void File::SearchForConsecutiveChars(const int& index)
{
    if (!(CharAtIndexIsSeparator(index) || CharAtIndexIsWhitespace(index)))
    {
        currentTokenLength++;
        return;
    }

    if (currentTokenLength != 0) AddToken(index - currentTokenLength, currentTokenLength, NO_TOKEN);
    currentTokenLength = 0;
}

bool File::CharAtIndexIsSeparator(const int& index)
{
    switch (filePointer[index])
    {
    case '#':
    case '(':
    case ')':
    case '{':
    case '}':
    case '[':
    case ']':
    case ';':
    case '<':
    case '>':
    case ',':
    case '\"':
        return true;
        break;
    default:
        return false;
        break;
    }
}

bool File::CharAtIndexIsWhitespace(const int& index)
{
    switch (filePointer[index])
    {
    case ' ':
    case '\n':
    case '\t':
    case '\v':
        return true;
        break;
    default:
        return false;
        break;
    }
}

void File::ChangeTokenTypeOfKeyword(const char* searchString, const TokenType& tokenType)
{
    for (int i = 0; i < token.size(); i++)
    {
        int counter = 0;
        int searchStringLength = 0;
        while (searchString[searchStringLength] != '\0') searchStringLength++;

        if (CompareStringToToken(searchString, searchStringLength, token[i].tokenIndex, token[i].tokenLength))
        {
            token[i].tokenType = tokenType;
        }
    }
}

TokenType File::GetSeparatorTokenTypeFromIndex(const int& index)
{
    switch (filePointer[index])
    {
    case '#':
        return SEPARATOR_HASH;
        break;
    case '(':
        return SEPARATOR_OPEN_PARENTHESIS;
        break;
    case ')':
        return SEPARATOR_CLOSE_PARENTHESIS;
        break;
    case '{':
        return SEPARATOR_OPEN_CURLY_BRACKET;
        break;
    case '}':
        return SEPARATOR_CLOSE_CURLY_BRACKET;
        break;
    case '[':
        return SEPARATOR_OPEN_SQUARE_BRACKET;
        break;
    case ']':
        return SEPARATOR_CLOSE_SQUARE_BRACKET;
        break;
    case ';':
        return SEPARATOR_SEMI_COLON;
        break;
    case '<':
        return SEPARATOR_ARROW_LEFT;
        break;
    case '>':
        return SEPARATOR_ARROW_RIGHT;
        break;
    case ',':
        return SEPARATOR_COMMA;
        break;
    case '\"':
        return SEPARATOR_CLOSE_PARENTHESIS;
        break;
    default:
        return NO_TOKEN;
        break;
    }
}

bool File::CompareStringToToken(const char* str1, const int& str1Length, const int& indexOfToken, const int& tokenLength)
{
    if (str1Length != tokenLength) return false;
    for (int i = 0; i < str1Length; i++)
    {
        if (str1[i] != filePointer[i + indexOfToken]) return false;
    }

    return true;
}

void File::AddToken(const int& tokenIndex, const int& tokenLength, const TokenType& tokenType)
{
    token.push_back({tokenIndex, tokenLength, tokenType});
}
