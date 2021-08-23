#include <iostream>

#include "./Tree.hpp"

int main() {

    Tree<int> test{1};
    test.Push(2);
    test.Push(3);
    test.Push(4);
    test[0].Push(5);
    test[0].Push(6);

    std::cout << test[0].Get() << ' ' << test.Size() << ' ' << test.Height();

    std::cout << "\nVoici l'arbre en vecteur : \n";
    for (int i : MakeVector(test)) {

        std::cout << i << ' ';

    }

    std::cout << "\nVoici l'arbre : \n";
    test.ForEach([](int &i) -> void { std::cout << i << ' '; });
    std::cout << std::endl;

    std::cout << test.Generation() << ' ' << test.GetMost([](int l, int r) -> bool { return l > r; });

    std::cout << "\nVoici l'arbre en largueur : \n";
    test.ForEachWidth([](int i) -> void { std::cout << i << ' '; });
    std::cout << std::endl;

    std::cin.get();

    return 0;

}