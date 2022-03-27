#include <fstream>
#include <iostream>
#include <vector>

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
    for (int i = 0; i < fileLength; i++)
    {
        // if text is inside quotes
        /*if (filePointer[i] == '\"')
        {
            insideQuotes = true;
            for (int j = 1; insideQuotes; j++)
            {
                if (filePointer[i + j] == '\"')
                {
                    for (int stringIndex = 0; stringIndex < j; stringIndex++)
                    {
                        AddPartialTokenValue("String", filePointer[stringIndex + i + 1], j - 1);
                    }
                    // AddToken("String", std::string((j - 1), filePointer[i]), 1);
                    insideQuotes = false;
                    i = i + j + 1;
                }
            }
        }*/

        // test for single character, non-operator tokens
        switch (filePointer[i])
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
            AddToken(SEPARATOR, &filePointer[i], 1);
            break;
        }
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
    t.open(FilePath);                // open input file
    t.seekg(0, std::ios::end);       // go to the end
    fileLength = t.tellg();              // report location (this is the length)
    t.seekg(0, std::ios::beg);       // go back to the beginning
    char* buffer = new char[fileLength]; // allocate memory for a buffer of appropriate dimension
    t.read(buffer, fileLength);          // read the whole file into the buffer
    t.close();                       // close file handle
    filePointer = buffer;
}

void File::AddToken(TokenType tokenType, char* tokenValue, int tokenValueSize)
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
        tokens[tokens.size() - 1][sizeof(TokenType) + i] = tokenValue[i];
    }
    tokens[tokens.size() - 1][sizeof(TokenType) + tokenValueSize] = '\0';
    tokenCount++;
}

void File::AddPartialTokenValue(std::string tokenType, char tokenValue, int tokenValueSize)
{
    // If partialTokenCallCount is 0 then memory needs to be allocated for the token
    if (partialTokenCallCount == 0)
    {
        tokenTypeSize = 0;
        while (tokenType[tokenTypeSize] != '\0') tokenTypeSize++;

        // allocate memory big enough for the tokenType, tokenValue, and two extra parenthesis, and an escape character
        int totalTokenSize = tokenTypeSize + tokenValueSize + 3;
        tokens.push_back((char*)malloc(sizeof(char) * totalTokenSize));

        for (int i = 0; i < tokenTypeSize; i++)
        {
            tokens[tokens.size() - 1][i] = tokenType[i];
        }

        tokens[tokens.size() - 1][tokenTypeSize] = '(';
    }

    // Adding partial token to token vector
    tokens[tokens.size() - 1][tokenTypeSize + partialTokenCallCount + 1] = tokenValue;
    partialTokenCallCount++;

    // if
    if (partialTokenCallCount - 1 == tokenValueSize)
    {
        tokens[tokens.size() - 1][(tokenTypeSize + tokenValueSize + 3) - 2] = ')';
        tokens[tokens.size() - 1][(tokenTypeSize + tokenValueSize + 3) - 1] = '\0';
        partialTokenCallCount = 0;
    }
}
