#include <iostream>
#include "../include/Dapp_string.hpp"
#include "../include/Dapp_for_each.hpp"

int main(int argc, char* argv[])
{

    String str = "Hello";
    for_each(str.begin(), str.end(), [](char c) {std::cout << c << ' ';});
    return 0;
}
