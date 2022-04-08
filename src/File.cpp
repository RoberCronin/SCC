#include <fstream>
#include <iostream>
#include <vector>

#include "File.hpp"

File::File(std::string FilePath)
{
    OpenFile(FilePath);
    for (int i = 0; i < 31; i++)
    {
        keywordCharMatchCounter[i] = 0;
    }
}

File::~File()
{
    delete[] filePointer;
}

void File::Tokenize()
{
    for (int i = 0; i < fileLength; i++)
    {
        SearchForSeparatorTokens(i);
        SearchForKeywordTokens(i);
    }
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

void File::SearchForSeparatorTokens(const int& index)
{
    switch (filePointer[index])
    {
    case '#':
        AddToken(index, 1, SEPARATOR);
        break;
    case '(':
        AddToken(index, 1, SEPARATOR);
        break;
    case ')':
        AddToken(index, 1, SEPARATOR);
        break;
    case '{':
        AddToken(index, 1, SEPARATOR);
        break;
    case '}':
        AddToken(index, 1, SEPARATOR);
        break;
    case '[':
        AddToken(index, 1, SEPARATOR);
        break;
    case ']':
        AddToken(index, 1, SEPARATOR);
        break;
    case ';':
        AddToken(index, 1, SEPARATOR);
        break;
    case '<':
        AddToken(index, 1, SEPARATOR);
        break;
    case '>':
        AddToken(index, 1, SEPARATOR);
        break;
    case ',':
        AddToken(index, 1, SEPARATOR);
        break;
    case '\"':
        AddToken(index, 1, SEPARATOR);
        break;
    default:
        break;
    }
}

void File::SearchForKeywordTokens(const int& index)
{
    SearchForSingleKeyword("auto", KEYWORD_AUTO, index);
    SearchForSingleKeyword("double", KEYWORD_DOUBLE, index);
    SearchForSingleKeyword("int", KEYWORD_INT, index);
    SearchForSingleKeyword("struct", KEYWORD_STRUCT, index);
    SearchForSingleKeyword("break", KEYWORD_BREAK, index);
    SearchForSingleKeyword("else", KEYWORD_ELSE, index);
    SearchForSingleKeyword("longswitch", KEYWORD_LONGSWITCH, index);
    SearchForSingleKeyword("case", KEYWORD_CASE, index);
    SearchForSingleKeyword("enum", KEYWORD_ENUM, index);
    SearchForSingleKeyword("register", KEYWORD_REGISTER, index);
    SearchForSingleKeyword("typedef", KEYWORD_TYPEDEF, index);
    SearchForSingleKeyword("char", KEYWORD_CHAR, index);
    SearchForSingleKeyword("extern", KEYWORD_EXTERN, index);
    SearchForSingleKeyword("return", KEYWORD_RETURN, index);
    SearchForSingleKeyword("union", KEYWORD_UNION, index);
    SearchForSingleKeyword("continue", KEYWORD_CONTINUE, index);
    SearchForSingleKeyword("for", KEYWORD_FOR, index);
    SearchForSingleKeyword("signed", KEYWORD_SIGNED, index);
    SearchForSingleKeyword("void", KEYWORD_VOID, index);
    SearchForSingleKeyword("do", KEYWORD_DO, index);
    SearchForSingleKeyword("if", KEYWORD_IF, index);
    SearchForSingleKeyword("static", KEYWORD_STATIC, index);
    SearchForSingleKeyword("while", KEYWORD_WHILE, index);
    SearchForSingleKeyword("default", KEYWORD_DEFAULT, index);
    SearchForSingleKeyword("goto", KEYWORD_GOTO, index);
    SearchForSingleKeyword("sizeof", KEYWORD_SIZEOF, index);
    SearchForSingleKeyword("volatile", KEYWORD_VOLATILE, index);
    SearchForSingleKeyword("const", KEYWORD_CONST, index);
    SearchForSingleKeyword("float", KEYWORD_FLOAT, index);
    SearchForSingleKeyword("short", KEYWORD_SHORT, index);
    SearchForSingleKeyword("unsigned", KEYWORD_UNSIGNED, index);
}

void File::SearchForSingleKeyword(const char* searchString, const KeywordToken& tokenType, const int& index)
{
    int searchStringLength = 0;
    while (searchString[searchStringLength] != '\0') searchStringLength++;

    if (keywordCharMatchCounter[(int)tokenType] == searchStringLength)
    {
        AddToken(index - keywordCharMatchCounter[(int)tokenType], searchStringLength, KEYWORD);
        keywordCharMatchCounter[(int)tokenType] = 0;
    }

    if (searchString[keywordCharMatchCounter[(int)tokenType]] == filePointer[index])
    {
        keywordCharMatchCounter[(int)tokenType]++;
    }
    else
    {
        keywordCharMatchCounter[(int)tokenType] = 0;
    }
}

void File::AddToken(const int& tokenIndex, const int& tokenLength, const TokenType& tokenType)
{
    token.push_back({tokenIndex, tokenLength, tokenType});
}
