#include <iostream>

#include "./Tree.hpp"

int main() {

    Tree<int> test{1};
    test.Push(2);
    test.Push(3);
    test.Push(4);
    test[0].Push(5);
    test[0].Push(-6);

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
    test.ForEachBFS([](int i) -> void { std::cout << i << ' '; });
    std::cout << std::endl;

    std::cout << "Tout les nombres sont positifs : " << test.AllHasProperty([](int i) -> bool { return i > 0; });
    std::cout << "\nAu moins un nombre est negatif : " << test.OnceHasProperty([](int i) -> bool { return i < 0; });
    std::cout << "\nLe premier nombre positif est : " << test.FirstHasProperty([](int i) -> bool { return i > 0; });
    std::cout << "\nLe dernier nombre positif est : " << test.LastHasProperty([](int i) -> bool { return i > 0; });

    std::cin.get();

    return 0;

}