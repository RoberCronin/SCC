#include <cstring>
#include <fstream>
#include <iostream>
#include <string.h>
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
    TokenInfo tokenInfo;

    GetAllSeparatorTokens();
    GetAllKeywordTokens();
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

        std::cout << ' ';

        for (int j = 0; j < token[i].tokenLength; j++)
        {
            std::cout << filePointer[token[i].tokenIndex + j];
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

void File::GetAllSeparatorTokens()
{
    for (int i = 0; i < fileLength; i++)
    {
        switch (filePointer[i])
        {
        case '#':
            token.push_back({i, 1, SEPARATOR_HASH});
            break;
        case '(':
            token.push_back({i, 1, SEPARATOR_OPEN_PARENTHESIS});
            break;
        case ')':
            token.push_back({i, 1, SEPARATOR_CLOSE_PARENTHESIS});
            break;
        case '{':
            token.push_back({i, 1, SEPARATOR_OPEN_CURLY_BRACKET});
            break;
        case '}':
            token.push_back({i, 1, SEPARATOR_CLOSE_CURLY_BRACKET});
            break;
        case '[':
            token.push_back({i, 1, SEPARATOR_OPEN_SQUARE_BRACKET});
            break;
        case ']':
            token.push_back({i, 1, SEPARATOR_CLOSE_SQUARE_BRACKET});
            break;
        case ';':
            token.push_back({i, 1, SEPARATOR_SEMI_COLON});
            break;
        case '<':
            token.push_back({i, 1, SEPARATOR_ARROW_LEFT});
            break;
        case '>':
            token.push_back({i, 1, SEPARATOR_ARROW_RIGHT});
            break;
        case ',':
            token.push_back({i, 1, SEPARATOR_COMMA});
            break;
        case '\"':
            token.push_back({i, 1, SEPARATOR_QUOTE});
            break;
        default:
            break;
        }
    }
}

void File::GetAllKeywordTokens()
{
    GetAllOfSingleKeyword("auto", KEYWORD_AUTO);
    GetAllOfSingleKeyword("double", KEYWORD_DOUBLE);
    GetAllOfSingleKeyword("int", KEYWORD_INT);
    GetAllOfSingleKeyword("struct", KEYWORD_STRUCT);
    GetAllOfSingleKeyword("break", KEYWORD_BREAK);
    GetAllOfSingleKeyword("else", KEYWORD_ELSE);
    GetAllOfSingleKeyword("longswitch", KEYWORD_LONGSWITCH);
    GetAllOfSingleKeyword("case", KEYWORD_CASE);
    GetAllOfSingleKeyword("enum", KEYWORD_ENUM);
    GetAllOfSingleKeyword("register", KEYWORD_REGISTER);
    GetAllOfSingleKeyword("typedef", KEYWORD_TYPEDEF);
    GetAllOfSingleKeyword("char", KEYWORD_CHAR);
    GetAllOfSingleKeyword("extern", KEYWORD_EXTERN);
    GetAllOfSingleKeyword("return", KEYWORD_RETURN);
    GetAllOfSingleKeyword("union", KEYWORD_UNION);
    GetAllOfSingleKeyword("continue", KEYWORD_CONTINUE);
    GetAllOfSingleKeyword("for", KEYWORD_FOR);
    GetAllOfSingleKeyword("signed", KEYWORD_SIGNED);
    GetAllOfSingleKeyword("void", KEYWORD_VOID);
    GetAllOfSingleKeyword("do", KEYWORD_DO);
    GetAllOfSingleKeyword("if", KEYWORD_IF);
    GetAllOfSingleKeyword("static", KEYWORD_STATIC);
    GetAllOfSingleKeyword("while", KEYWORD_WHILE);
    GetAllOfSingleKeyword("default", KEYWORD_DEFAULT);
    GetAllOfSingleKeyword("goto", KEYWORD_GOTO);
    GetAllOfSingleKeyword("sizeof", KEYWORD_SIZEOF);
    GetAllOfSingleKeyword("volatile", KEYWORD_VOLATILE);
    GetAllOfSingleKeyword("const", KEYWORD_CONST);
    GetAllOfSingleKeyword("float", KEYWORD_FLOAT);
    GetAllOfSingleKeyword("short", KEYWORD_SHORT);
    GetAllOfSingleKeyword("unsigned", KEYWORD_UNSIGNED);
}

void File::GetAllOfSingleKeyword(const char* searchString, TokenType tokenType)
{
    int counter = 0;

    int searchStringLength = 0;
    while (searchString[searchStringLength] != '\0') searchStringLength++;

    for (int i = 0; i < fileLength; i++)
    {
        if (counter == searchStringLength)
        {
            token.push_back({i - counter, searchStringLength, tokenType});
            counter = 0;
        }

        if (searchString[counter] == filePointer[i])
        {
            counter++;
        }
        else
        {
            // Special case where character preceding the i'th character is duplicate
            if (counter > 0)
            {
                i -= counter;
            }
            counter = 0;
        }
    }
}
