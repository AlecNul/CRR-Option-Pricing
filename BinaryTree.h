#include <iostream>
#include <vector>

template <typename T> 
class BinaryTree {
// Attributes
private:
    int depth;
    std::vector<std::vector<T>> tree;

// Setter
public:
    void setDepth(int N) const
    {
        tree.resize(N+1);
        for (int i=0;i<N+1;i++) { tree[i].resize(i+1,T()); }
    }

// Constructor
BinaryTree(int N) : depth(N)
{
    tree.resize(N+1);
    for (int i=0;i<N+1;i++) { tree[i].resize(i+1,T()); }
}
};