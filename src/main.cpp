#include <iostream>

#include "File.hpp"

int main(int argc, char** argv)
{
    for (int i = 0; i < argc; i++)
    {
        std::cout << argv[i] << std::endl;
    }

    File file(argv[1]);
    // File file("tests/simple.txt");

    // print out file
    for (int i = 0; i < file.fileLength; i++)
    {
        std::cout << file.filePointer[i];
    }
    std::cout << '\n' << file.fileLength << " characters long" << std::endl;

    file.Tokenize();
    file.PrintTokens();
    std::cout << file.token.size() << " tokens" << std::endl;

    return 0;
}
