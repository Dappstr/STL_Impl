#include <iostream>
#include "../include/Dapp_vector.hpp"

int main(int argc, char* argv[])
{
    dapp::Vector<int> v{};
    v.append(1);
    v.append(2);
    v.append(3);
    std::cout << "CAP: " << v.capacity() << '\n';
    std::cout << v;
    return 0;
}
