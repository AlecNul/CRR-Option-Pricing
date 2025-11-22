#pragma once

#include <iostream>

#include "Options.h"
#include "Pricers.h"

#include "BinaryTree.h"
#include <vector>

void test_blackscholes() 
{

    char type;
    double t, K;

    std::cout << "Type c if you want to create a call, p for a put." << endl;
    std::cin >> type;
    std::cout << "Type time to expiry" << endl;
    std::cin >> t;
    std::cout << "Type strike" << endl;
    std::cin >> K;

    EuropeanVanillaOption* opt;

    try 
    {
        if (type=='c') { opt = new CallOption(t,K); }
        else if (type=='p') { opt = new PutOption(t,K); }
        else 
        { 
            cout << "Syntax Error : please type \'c\' or \'p\' " << endl; 
            return;
        }

        double S,r,s;
        std::cout << "Type spot" << endl;
        std::cin >> S;
        std::cout << "Type interest rate" << endl;
        std::cin >> r;
        std::cout << "Type volatility" << endl;
        std::cin >> s;

        BlackScholesPricer pricer(opt,S,r,s);

        cout << "Price : " << pricer() << endl;
        cout << "Delta : " << pricer.delta() << endl;

    }
    catch (const OptionInitError& e) { cout << e.what() << endl; }

    delete opt;
}

void test_binarytree()
{
    int depth;
    cout << "type depth" << endl;
    cin >> depth;
    
    double root;
    cout << "type root value (double)" << endl;
    cin >> root;
    cout << endl;

    std::vector<std::vector<double>> tree;
    tree.resize(depth+1);

    double U = 0.2;
    double D = -0.3;

    for(int n=0;n<depth+1;n++)
    {
        tree[n].resize(n+1);
        for(int i=0;i<n+1;i++)
        {
            tree[n][i] = root*std::pow(1+U,i)*std::pow(1+D,n-i);
        }
    }

    BinaryTree test_tree(depth,tree);

    test_tree.display();

}