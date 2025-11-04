#pragma once

#include <iostream>
#include "Options.h"
#include "BlackScholesPricer.h"

void test_blackscholes() {

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