#ifndef DEF_TREE_HPP
#define DEF_TREE_HPP

#include <iostream>
#include <cassert>
#include <memory>
#include <queue>

template <typename Type> class Tree {

    public:

    Tree() = default;

    Tree(const Tree&) = default;
    Tree(Tree&&) noexcept = default;

    Tree(Type t) : value{t}, unique{true} {}

    ~Tree() = default;

    void Clear() { *this = Tree{}; }

    void Push(Type t, std::size_t repeat = 1) {

        for (std::size_t i{0}; i < repeat; i++) {

            if (Empty()) {
            
                value = t;
                unique = true;

            } else if (unique) {
            
                childrens.push_back(Tree{t});
                childrens.back().parent = std::unique_ptr<Tree>{this};

                unique = false;

            } else {

                childrens.push_back(Tree{t});
                childrens.back().parent = std::unique_ptr<Tree>{this};

            }

        }

    }

    void Pop(std::size_t repeat = 1) {

        assert(!Empty() && "Can't pop empty tree");

        for (std::size_t i{0}; i < repeat; i++) {

            if (Size() == 1) {

                Clear();

            } else if (Size() == 2) {

                childrens.pop_back();
                unique = true;

            } else {

                childrens.pop_back();

            }

        }

    }

    void Add(Tree t) {

        if (Empty()) {

            *this = t;

        } else if (unique) {

            childrens.push_back(t);
            unique = false;

        } else {

            childrens.push_back(t);

        }

    }

    template <typename Lambda> void ForEach(Lambda function) {

        function(value);
        for (Tree &t : childrens) t.ForEach(function);

    }

    template <typename Lambda> void ForEach(Lambda function) const {

        function(value);
        for (Tree t : childrens) t.ForEach(function);

    }

    template <typename Lambda> void ForEachBFS(Lambda function) const {

        std::queue<Tree> order;
        order.push(*this);

        while (!order.empty()) {

            function(order.front().value);
            for (Tree t : order.front().childrens) order.push(t);

            order.pop();

        }

    }

    template <typename Lambda> void ForEachGeneration(Lambda function, std::size_t generation) const {

        assert(generation < Height() && "Generation index over range");

        std::queue<Tree> order;
        order.push(*this);

        std::size_t distance{0}, count{1};

        while (!order.empty()) {

            if (distance == generation) function(order.front().value);
            for (Tree t : order.front().childrens) order.push(t);

            std::size_t temp{order.front().childrens.size()};

            order.pop();
            count--;

            if (count == 0) {

                distance++;
                count = temp;

            }

        }

    }

    bool Empty() const { return !unique && childrens.empty() && parent == nullptr; }
    bool Leaf() const { return childrens.empty() && parent != nullptr; }
    bool Root() const { return !childrens.empty() && parent == nullptr; }
    bool Branche() const { return !childrens.empty() && parent != nullptr; }

    template <typename Lambda> bool AllHasProperty(Lambda property) const {

        bool temp{true};

        ForEach([property, &temp](Type t) -> void {

            if (!property(t)) {
                
                temp = false;
                return;

            }

        });

        return temp;

    }

    template <typename Lambda> bool OnceHasProperty(Lambda property) const {

        bool temp{false};

        ForEach([property, &temp](Type t) -> void {

            if (!temp && property(t)) temp = true;

        });

        return temp;

    }

    std::size_t Size() const {

        if (Empty()) {

            return 0;

        } else {

            std::size_t count{1};
            for (Tree t : childrens) count += t.Size();

            return count;

        }

    }

    std::size_t Height() const {

        if (Empty()) {

            return 0;

        } else {

            std::size_t best{0};
            for (Tree t : childrens) {

                if (t.Height() > best) best = t.Height();

            }

            return 1+best;

        }

    }

    std::size_t Generation() const {

        if (Empty()) {

            return 0;

        } else {

            std::size_t count{1};
            Tree t{*this};

            while (t.parent != nullptr) {

                count++;
                t = *t.parent;

            }

            return count;

        }


    }

    Type Get() const { return value; }
    Type& Get() { return value; }

    template<typename Lambda> Type GetMost(Lambda comparison) const {

        Type m{value};
        ForEach([comparison, &m](Type t) -> void {

            if (comparison(t, m)) m = t;

        });

        return m;

    }

    template<typename Lambda> Type FirstHasProperty(Lambda property) const {

        assert(OnceHasProperty(property) && "Tree not have this property");

        Type first;
        bool end{false};

        ForEachBFS([property, &first, &end](Type t) -> void {

            if (!end && property(t)) {
                
                first = t;
                end = true;

            }

        });

        return first;

    }

    template<typename Lambda> Type LastHasProperty(Lambda property) const {

        assert(OnceHasProperty(property) && "Tree not have this property");

        Type last;
        ForEachBFS([property, &last](Type t) -> void {

            if (property(t)) last = t;

        });

        return last;

    }

    friend std::vector<Type> MakeVector(const Tree<Type> &t) {

        std::vector<Type> result;

        for (Tree c : t.childrens) {

            for (Type v : MakeVector(c)) {

                result.push_back(v);

            }

        }
        
        result.insert(result.begin(), t.value);
        return result;

    }

    friend std::vector<Type> MakeVectorBFS(const Tree<Type> &t) {

        std::vector<Type> result;

        t.ForEachBFS([&result](Type t) -> void {

            result.push_back(t);

        });

        return result;

    }

    friend std::vector<Type> MakeVectorGeneration(const Tree<Type> &t, std::size_t generation) {

        std::vector<Type> result;

        t.ForEachGeneration([&result](Type t) -> void {

            result.push_back(t);

        }, generation);

        return result;

    }

    friend std::string MakeString(const Tree<Type> &t) {

        std::string result;

        if (!t.Empty()) {

            result += std::to_string(t.value);

            if (!t.unique) {

                result += " : [";

                for (Tree c : t.childrens) {
                    
                    if (!c.unique) {
                        
                        result += MakeString(c) + ", ";

                    } else {

                        result += std::to_string(c.value) + ", ";

                    }

                }

                result.pop_back();
                result.pop_back();

                result.push_back(']');

            }

        }

        return result;

    }

    friend bool operator==(const Tree &l, const Tree &r) {

        return l.value == r.value && l.childrens == r.childrens && l.parent == r.parent;

    }

    friend bool operator!=(const Tree &l, const Tree &r) { return !(l == r); }

    const Tree& operator[](std::size_t index) const { return childrens[index]; }
    Tree& operator[](std::size_t index) { return childrens[index]; }

    Tree& operator=(const Tree&) = default;
    Tree& operator=(Tree&&) noexcept = default;

    friend std::istream& operator>>(const std::string &s, const Tree &t) {

        assert(!"Not implement");
        /*
        
        [] = Empty
        [1] = unique

        [1 : [2, 3, 4]] =
            1
           /|\
          2 3 4

        [1 : [2 : [5, -6], 3, 4]] =
             1
           / | \
          2  3  4
         / \
        5  -6
        
        */

    }

    friend std::ostream& operator<<(std::ostream &flux, const Tree &t) {

        if (!t.Empty()) {

            if (t.unique) {

                flux << t.value;

            } else {

                const std::size_t totalHeight{t.Height()};

                std::size_t sum{0};
                for (std::size_t i{0}; i < t.Height(); i++) sum += i;
                for (std::size_t i{0}; i < sum; i++) flux << ' ';

                sum--;

                flux << t.value << std::endl;

                for (std::size_t i{0}; i < sum; i++) flux << ' ';

                for (std::size_t i{0}; i < t.childrens.size(); i++) {

                    for (std::size_t j{0}; j < t.Height()-totalHeight; j++) flux << ' ';

                    if (i < t.childrens.size()/2) {

                        flux << '/';

                    } else if (i == t.childrens.size()/2) {

                        flux << '|';

                    } else {

                        flux << '\\';

                    }

                }

                for (Tree c : t.childrens) {



                }

                // Work in progress

            }

        }

        return flux;

    }

    private:

    bool unique{false};

    Type value;

    std::vector<Tree> childrens;
    std::shared_ptr<Tree> parent;

};

#endif // DEF_TREE_HPP