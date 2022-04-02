#include <cstring>
#include <fstream>
#include <iostream>
#include <vector>
#include <string.h>

#include "File.hpp"

File::File(std::string FilePath)
{
    OpenFile(FilePath);
}

File::~File()
{
    delete[] filePointer;
}

void File::Tokenize()
{
    TokenInfo tokenInfo;

    for (int i = GetIndexOfNextToken(0, 0); i < fileLength && i != -1;)
    {
        tokenInfo = GetTokenInfoFromIndex(i);
        AddToken(tokenInfo.tokenType, i, tokenInfo.tokenLength);
        i = GetIndexOfNextToken(i, tokenInfo.tokenLength);
    }
}

void File::PrintTokens()
{
    for (int i = 0; i < tokens.size(); i++)
    {
        // print token type
        switch ((int)tokens[i][0])
        {
            // weird formatting because of using the preprocessor to automatically get token type enum names
#define TOKENTYPE_DEF(x)                                                                                                                                       \
    case x:                                                                                                                                                    \
        std::cout << #x;                                                                                                                                       \
        break;
#include "TokenType.txt"
#undef TOKENTYPE_DEF
        }

        std::cout << ' ';

        int tokenValueSize = 0;
        while (tokens[i][tokenValueSize + sizeof(TokenType)] != '\0') tokenValueSize++;

        for (int j = 0; j < tokenValueSize; j++)
        {
            std::cout << tokens[i][sizeof(TokenType) + j];
        }
        std::cout << std::endl;
    }
}

void File::OpenFile(std::string FilePath)
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

void File::AddToken(TokenType tokenType, int tokenIndex, int tokenValueSize)
{
    // A token's memory is layed out in the form,
    // TokenType, TokenValue
    //
    // The token type is an enum, so it consists of 4 bytes
    // The token value is a string that has the relevant data
    //
    // Example,
    // The token, "String Token", onced tokenized would be,
    // SEPARATOR"
    // CONSTANT_STRINGString Token
    // SEPARATOR"

    // allocate memory big enough for the TokenType, tokenValue, and an escape character
    const int totalTokenSize = sizeof(TokenType) + sizeof(char) * tokenValueSize + 1;
    tokens.push_back((char*)malloc(totalTokenSize));

    tokens[tokens.size() - 1][0] = tokenType;
    for (int i = 0; i < tokenValueSize; i++)
    {
        tokens[tokens.size() - 1][sizeof(TokenType) + i] = filePointer[tokenIndex + i];
    }
    tokens[tokens.size() - 1][sizeof(TokenType) + tokenValueSize] = '\0';
    tokenCount++;
}

TokenInfo File::GetTokenInfoFromIndex(int index)
{
    TokenInfo tokenInfo;

    // check for single character tokens
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
        tokenInfo.tokenType = SEPARATOR;
        tokenInfo.tokenLength = 1;
        return tokenInfo;
        break;
    }

    return tokenInfo;
}

// returns -1 if at the end of the file
int File::GetIndexOfNextToken(int indexOfCurrentToken, int lengthOfCurrentToken)
{
    for (int i = 0; indexOfCurrentToken + lengthOfCurrentToken + i < fileLength; i++)
    {
        switch (filePointer[indexOfCurrentToken + lengthOfCurrentToken + i])
        {
        case ' ':
        case '\n':
        case '\t':
        case '\v':
            continue;
            break;
        }

        return indexOfCurrentToken + lengthOfCurrentToken + i;
        break;
    }

    return -1;
}