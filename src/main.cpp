#include <iostream>
#include "../include/Dapp_string.hpp"
#include "../include/Dapp_algorithm.hpp"
#include "../include/Dapp_vector.hpp"

int main(int argc, char* argv[])
{
    dapp::String str  = "Hello there";
    dapp::String str2 = str.substr(0, 5);
    
    std::cout << str2;

    return 0;
}
