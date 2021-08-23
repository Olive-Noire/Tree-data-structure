#ifndef DEF_TREE_HPP
#define DEF_TREE_HPP

#include <cassert>
#include <memory>
#include <vector>

template <typename Type> class Tree {

    public:

    Tree() = default;

    Tree(const Tree&) = default;
    Tree(Tree&&) noexcept = default;

    Tree(Type t) : value{t}, unique{true} {}

    ~Tree() = default;

    void Clear() { *this = Tree{}; }

    void Push(Type t) {

        if (Empty()) {
            
            value = t;
            unique = true;

        } else if (unique) {
            
            childrens.push_back(Tree{t});
            unique = false;

        } else {

            childrens.push_back(Tree{t});

        }

    }

    void Pop() {

        assert(!Empty() && "Can't pop empty tree");

        if (Size() == 1) {

            Clear();

        } else if (Size() == 2) {

            childrens.pop_back();
            unique = true;

        } else {

            childrens.pop_back();

        }

    }

    bool Empty() const { return !unique && childrens.empty() && parent == nullptr; }
    bool Leaf() const { return childrens.empty() && parent != nullptr; }
    bool Root() const { return !childrens.empty() && parent == nullptr; }
    bool Branche() const { return !childrens.empty() && parent != nullptr; } 

    std::size_t Size() const {

        if (Empty()) {

            return 0;

        } else if (unique) {

            return 1;

        } else {

            std::size_t count{childrens.size()};
            for (Tree t : childrens) count += t.Size();

            return count;

        }

    }

    Type Get() const { return value; }
    Type& Get() { return value; }

    const Tree& operator[](std::size_t index) const { return childrens[index]; }
    Tree& operator[](std::size_t index) { return childrens[index]; }

    Tree& operator=(const Tree&) = default;
    Tree& operator=(Tree&&) noexcept = default;

    private:

    bool unique{false};

    Type value;

    std::vector<Tree> childrens;
    std::shared_ptr<Tree> parent;

};

#endif // DEF_TREE_HPP