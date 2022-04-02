#include <iostream>

#include "File.hpp"

int main()
{
    File file("tests/singlechartokens.txt");

    // print out file
    for (int i = 0; i < file.fileLength; i++)
    {
        std::cout << file.filePointer[i];
    }
    std::cout << '\n' << file.fileLength << " characters long" << std::endl;

    file.Tokenize();
    file.PrintTokens();
    std::cout << file.tokenCount << " tokens" << std::endl;

    return 0;
}