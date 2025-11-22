#pragma once
#include <iostream>
#include <vector>

template <typename T> 
class BinaryTree {
// Attributes
private:
    int depth;
    std::vector<std::vector<T>> tree;

// Setters and Getter
public:
    void setDepth(int N)
    {
        // When testing in the main, I remarked that you resized with smaller values, so the ones you put before were erased.
        if (N<depth) { cout << "Warning you may be erasing some values, the current depth ("<<depth<<") is higher." << endl; }
        tree.resize(N+1);
        depth = N;
        for (int i=0;i<N+1;i++) { tree[i].resize(i+1,T()); }
    }

    void setNode(int n, int i, T value) { tree.at(n).at(i)=value; }

    T getNode(int n, int i) const { return tree.at(n).at(i); }

// Constructors
    BinaryTree() : depth(0) {}
    BinaryTree(int N, std::vector<std::vector<T>> t) : depth(N), tree(t) {}

// Methods
    void display()
    {
        for(int n=0;n<depth+1;n++)
        {
            for (int i=0;i<n+1;i++)
            {
                cout << tree[n][i] << " ";
            }
            cout << endl;
        }
        cout << endl;
    }
    // better display() To Do maybe

};