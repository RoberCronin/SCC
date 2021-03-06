#include <fstream>
#include <iostream>
#include <vector>

#include "Tokenizer.hpp"
#include "Tokens.hpp"

Tokenizer::Tokenizer(const char* FilePath)
{
    OpenFile(FilePath);
}

Tokenizer::~Tokenizer()
{
    delete[] fileChars;
}

void Tokenizer::Tokenize()
{
    for (int i = 0; i < fileLength; i++)
    {
        SearchForIndividualTokens(i);
        if (CharAtIndexIsSeparator(i)) AddToken(i, 1, GetSeparatorTokenTypeFromIndex(i));
    }

    ChangeKeywordsToCorrectTokenType();
    ChangeIntegersToCorrectTokenType();
    ChangeIdentifiersToCorrectTokenType();
}

void Tokenizer::PrintTokens()
{
    for (int i = 0; i < tokens.size(); i++)
    {
        // print token type
        switch (tokens[i].tokenType)
        {
            // weird formatting because of using the preprocessor to automatically get token type enum names
#define TOKENTYPE_DEF(x)                                                                                                                                       \
    case x:                                                                                                                                                    \
        std::cout << #x;                                                                                                                                       \
        break;
#include "TokenType.txt"
#undef TOKENTYPE_DEF
        }

        // print token value
        std::cout << " \"";
        for (int j = 0; j < tokens[i].tokenLength; j++)
        {
            std::cout << fileChars[tokens[i].tokenIndex + j];
        }

        // print token attributes
        std::cout << "\"    Token Index: " << tokens[i].tokenIndex << "    Token Length: " << tokens[i].tokenLength;
        std::cout << std::endl;
    }
}

// private methods
void Tokenizer::OpenFile(const char* FilePath)
{
    std::ifstream t;
    t.open(FilePath);                    // open input file
    t.seekg(0, std::ios::end);           // go to the end
    fileLength = t.tellg();              // report location (this is the length)
    t.seekg(0, std::ios::beg);           // go back to the beginning
    char* buffer = new char[fileLength]; // allocate memory for a buffer of appropriate dimension
    t.read(buffer, fileLength);          // read the whole file into the buffer
    t.close();                           // close file handle
    fileChars = buffer;
}

void Tokenizer::SearchForIndividualTokens(const int& index)
{
    if (index > 0 && index < fileLength + 1)
    {
        // if previous character is a paranthesis
        if (fileChars[index - 1] == '"' && endParenthesis != index - 1) insideParanthesis = true;

        // if next character is a parenthesis and index is inside parenthesis
        if (fileChars[index + 1] == '"' && insideParanthesis)
        {
            insideParanthesis = false;
            endParenthesis = index + 1;
        }
    }

    if (!(CharAtIndexIsSeparator(index) || CharAtIndexIsWhitespace(index)) || insideParanthesis)
    {
        currentTokenLength++;
        return;
    }

    // if the token length is 0 then the current character is a separator
    if (currentTokenLength != 0)
    {
        if (endParenthesis == index)
            AddToken(index - currentTokenLength, currentTokenLength, CONSTANT_STRING);
        else
            AddToken(index - currentTokenLength, currentTokenLength, NO_TOKEN);
    }

    currentTokenLength = 0;
}

bool Tokenizer::CharAtIndexIsSeparator(const int& index)
{
    switch (fileChars[index])
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

bool Tokenizer::CharAtIndexIsWhitespace(const int& index)
{
    switch (fileChars[index])
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

void Tokenizer::ChangeIntegersToCorrectTokenType()
{
    for (int i = 0; i < tokens.size(); i++)
    {
        if (IsTokenInteger(i)) tokens[i].tokenType = CONSTANAT_INTEGER;
    }
}

bool Tokenizer::IsTokenInteger(const int& indexOfToken)
{
    for (int i = 0; i < tokens[indexOfToken].tokenLength; i++)
    {
        switch (fileChars[i + tokens[indexOfToken].tokenIndex])
        {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            break;
        default:
            return false;
            break;
        }
    }
    return true;
}

void Tokenizer::ChangeKeywordsToCorrectTokenType()
{
    ChangeTokenTypeOfKeyword("auto", KEYWORD_AUTO);
    ChangeTokenTypeOfKeyword("double", KEYWORD_DOUBLE);
    ChangeTokenTypeOfKeyword("int", KEYWORD_INT);
    ChangeTokenTypeOfKeyword("struct", KEYWORD_STRUCT);
    ChangeTokenTypeOfKeyword("break", KEYWORD_BREAK);
    ChangeTokenTypeOfKeyword("else", KEYWORD_ELSE);
    ChangeTokenTypeOfKeyword("longswitch", KEYWORD_LONGSWITCH);
    ChangeTokenTypeOfKeyword("case", KEYWORD_CASE);
    ChangeTokenTypeOfKeyword("enum", KEYWORD_ENUM);
    ChangeTokenTypeOfKeyword("register", KEYWORD_REGISTER);
    ChangeTokenTypeOfKeyword("typedef", KEYWORD_TYPEDEF);
    ChangeTokenTypeOfKeyword("char", KEYWORD_CHAR);
    ChangeTokenTypeOfKeyword("extern", KEYWORD_EXTERN);
    ChangeTokenTypeOfKeyword("return", KEYWORD_RETURN);
    ChangeTokenTypeOfKeyword("union", KEYWORD_UNION);
    ChangeTokenTypeOfKeyword("continue", KEYWORD_CONTINUE);
    ChangeTokenTypeOfKeyword("for", KEYWORD_FOR);
    ChangeTokenTypeOfKeyword("signed", KEYWORD_SIGNED);
    ChangeTokenTypeOfKeyword("void", KEYWORD_VOID);
    ChangeTokenTypeOfKeyword("do", KEYWORD_DO);
    ChangeTokenTypeOfKeyword("if", KEYWORD_IF);
    ChangeTokenTypeOfKeyword("static", KEYWORD_STATIC);
    ChangeTokenTypeOfKeyword("while", KEYWORD_WHILE);
    ChangeTokenTypeOfKeyword("default", KEYWORD_DEFAULT);
    ChangeTokenTypeOfKeyword("goto", KEYWORD_GOTO);
    ChangeTokenTypeOfKeyword("sizeof", KEYWORD_SIZEOF);
    ChangeTokenTypeOfKeyword("volatile", KEYWORD_VOLATILE);
    ChangeTokenTypeOfKeyword("const", KEYWORD_CONST);
    ChangeTokenTypeOfKeyword("float", KEYWORD_FLOAT);
    ChangeTokenTypeOfKeyword("short", KEYWORD_SHORT);
    ChangeTokenTypeOfKeyword("unsigned", KEYWORD_UNSIGNED);
}

void Tokenizer::ChangeTokenTypeOfKeyword(const char* searchString, const TokenType& tokenType)
{
    for (int i = 0; i < tokens.size(); i++)
    {
        int counter = 0;
        int searchStringLength = 0;
        while (searchString[searchStringLength] != '\0') searchStringLength++;

        if (CompareStringToToken(searchString, searchStringLength, i))
        {
            tokens[i].tokenType = tokenType;
        }
    }
}

TokenType Tokenizer::GetSeparatorTokenTypeFromIndex(const int& index)
{
    switch (fileChars[index])
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
        return SEPARATOR_QUOTE;
        break;
    default:
        return NO_TOKEN;
        break;
    }
}

bool Tokenizer::CompareStringToToken(const char* str1, const int& str1Length, const int& indexOfToken)
{
    if (str1Length != tokens[indexOfToken].tokenLength) return false;
    for (int i = 0; i < str1Length; i++)
    {
        if (str1[i] != fileChars[i + tokens[indexOfToken].tokenIndex]) return false;
    }

    return true;
}

void Tokenizer::ChangeIdentifiersToCorrectTokenType()
{
    for (int i = 0; i < tokens.size(); i++)
    {
        if (IsTokenIdentifier(i) && tokens[i].tokenType == NO_TOKEN) tokens[i].tokenType = IDENTIFIER;
    }
}

bool Tokenizer::IsTokenIdentifier(const int& indexOfToken)
{
    for (int i = 0; i < tokens[indexOfToken].tokenLength; i++)
    {
        if (IsSpecialCharacter(fileChars[tokens[indexOfToken].tokenIndex + i])) return false;
    }

    return true;
}

bool Tokenizer::IsSpecialCharacter(const char& character)
{
    switch (character)
    {
    case 'a':
    case 'b':
    case 'c':
    case 'd':
    case 'e':
    case 'f':
    case 'g':
    case 'h':
    case 'i':
    case 'j':
    case 'k':
    case 'l':
    case 'm':
    case 'n':
    case 'o':
    case 'p':
    case 'q':
    case 'r':
    case 's':
    case 't':
    case 'u':
    case 'v':
    case 'w':
    case 'x':
    case 'y':
    case 'z':
    case 'A':
    case 'B':
    case 'C':
    case 'D':
    case 'E':
    case 'F':
    case 'G':
    case 'H':
    case 'I':
    case 'J':
    case 'K':
    case 'L':
    case 'M':
    case 'N':
    case 'O':
    case 'P':
    case 'Q':
    case 'R':
    case 'S':
    case 'T':
    case 'U':
    case 'V':
    case 'W':
    case 'X':
    case 'Y':
    case 'Z':
    case '_':
        return false;
    }
    return true;
}

void Tokenizer::AddToken(const int& tokenIndex, const int& tokenLength, const TokenType& tokenType)
{
    tokens.push_back({tokenIndex, tokenLength, tokenType});
}
