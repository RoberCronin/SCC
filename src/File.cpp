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
    for (int i = 0; i < length; i++)
    {
        // if text is inside quotes
        if (filePointer[i] == '\"')
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
        }

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
            AddToken("Token", std::string(1, filePointer[i]), 1);
            break;
        }
    }
}

void File::PrintTokens()
{
    for (int i = 0; i < tokens.size(); i++)
    {
        int tokenSize = 0;
        while (tokens[i][tokenSize] != '\0') tokenSize++;

        for (int j = 0; j < tokenSize; j++)
        {
            std::cout << tokens[i][j];
        }
        std::cout << std::endl;
    }
}

void File::OpenFile(std::string FilePath)
{
    std::ifstream t;
    t.open(FilePath);                // open input file
    t.seekg(0, std::ios::end);       // go to the end
    length = t.tellg();              // report location (this is the length)
    t.seekg(0, std::ios::beg);       // go back to the beginning
    char* buffer = new char[length]; // allocate memory for a buffer of appropriate dimension
    t.read(buffer, length);          // read the whole file into the buffer
    t.close();                       // close file handle
    filePointer = buffer;
}

void File::AddToken(std::string tokenType, std::string tokenValue, int tokenValueSize)
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

    for (int i = 0; i < tokenValueSize; i++)
    {
        tokens[tokens.size() - 1][tokenTypeSize + i + 1] = tokenValue[i];
    }

    tokens[tokens.size() - 1][totalTokenSize - 2] = ')';
    tokens[tokens.size() - 1][totalTokenSize - 1] = '\0';
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
