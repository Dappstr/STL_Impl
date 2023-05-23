#include <iostream>
#include "../include/Dapp_string.hpp"
#include "../include/Dapp_for_each.hpp"
#include "../include/Dapp_vector.hpp"

int main(int argc, char* argv[])
{
    
    Vector<int, 5> vec1 = {1, 2, 3, 4, 5};
    Vector<int, 5> vec2 = {6, 7, 8, 9, 10};

    Vector<int, 5> vec3 = vec1 + vec2;

    return 0;
}
