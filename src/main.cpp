#include <iostream>
#include "../include/Dapp_string.hpp"
#include "../include/Dapp_for_each.hpp"
#include "../include/Dapp_vector.hpp"

int main(int argc, char* argv[])
{
    
    Vector<int, 5> vec = {1, 2, 3, 4, 5};
    vec.pop();
    const int lval = 6;
    vec.append(lval);

    std::cout << vec;
    return 0;
}
