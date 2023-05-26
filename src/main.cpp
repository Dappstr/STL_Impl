#include <iostream>
#include "../include/Dapp_string.hpp"
#include "../include/Dapp_for_each.hpp"
#include "../include/Dapp_vector.hpp"

int main(int argc, char* argv[])
{
    String str  = "Hello ther";    
    String str2 = str.substr(0, 5);
    
    std::cout << str2;

    return 0;
}
