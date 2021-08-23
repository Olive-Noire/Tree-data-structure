#ifndef DEF_TREE_HPP
#define DEF_TREE_HPP

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

    void Push(Type t) {

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

        assert(!"Not implement");

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

        std::vector<Type> result{};

        assert(!"Not implement");

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

    friend std::ostream& operator<<(const Tree &l, const Tree &r) {

        assert(!"Not implement");

    }

    private:

    bool unique{false};

    Type value;

    std::vector<Tree> childrens;
    std::shared_ptr<Tree> parent;

};

#endif // DEF_TREE_HPP