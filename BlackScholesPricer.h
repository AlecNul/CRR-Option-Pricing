#pragma once
#include <iostream>
#include "Options.h"

class BlackScholesPricer {

// Attributes
private:
    EuropeanVanillaOption* option;
    double asset_price;
    double interest_rate;
    double volatility;

// Constructor
public:
    BlackScholesPricer(EuropeanVanillaOption* opt, double price, double rate, double vola) : 
    option(opt),asset_price(price),interest_rate(rate),volatility(vola) {}

// Methods
    double operator()() const 
    {
        double K = option->getStrike();
        double T = option->getExpiry();
        double S = asset_price;
        double r = interest_rate;
        double s = volatility;
        double q = 0.0; // Dividend yield 

        optionType type = option->getOptionType();

        if (type == OptionType::Call)
        {
            // à faire aaaaa
        }
        return 0;
    }
// Formula : https://www.macroption.com/black-scholes-formula/#call-put-price
};