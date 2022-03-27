#include <iostream>

#include "File.hpp"

int main()
{
    File file("tests/test.txt");

    for (int i = 0; i < file.fileLength; i++)
    {
        std::cout << file.filePointer[i];
    }
    std::cout << '\n' << file.fileLength << std::endl;

    file.Tokenize();
    file.PrintTokens();

    return 0;
}