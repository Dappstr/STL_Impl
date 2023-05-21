#include <iostream>
#include "../include/Dapp_string.h"

int main(int argc, char* argv[])
{

    String str = "Hello";
    str.append(" test");
    std::cout << str;

    return 0;
}
