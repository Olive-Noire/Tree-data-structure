#include <iostream>

#include "./Tree.hpp"

int main() {

    Tree<int> test{13};
    test.Push(-14);

    std::cout << test[0].Get() << ' ' << test[0].Size();
    std::cin.get();

    return 0;

}