#pragma once
#include <iostream>
#include "Options.h"
#include <cmath>

class BlackScholesPricer {

// Attributes
private:
    EuropeanVanillaOption* option;
    double S; // asset_price
    double r; // interest_rate
    double s; // volatility

// Constructor
public:
    BlackScholesPricer(EuropeanVanillaOption* opt, double price, double rate, double vola) : 
    option(opt),S(price),r(rate),s(vola) {}

// Methods
private: // Function for Normal CDF computation
    double N(double x) const { return std::erfc(-x/std::sqrt(2))/2; }

public:
    double operator()() const 
    {
        double K = option->getStrike();
        double t = option->getExpiry(); // Having no more information I'll assume it's the time to expiry 
        double q = 0.0; // Dividend yield 

        double d1 = (std::log(S/K) + (t*(r-q+s*s/2))) / (s*std::sqrt(t));
        double d2 = d1 - s*std::sqrt(t);

        OptionType type = option->getOptionType();
        double res;
    
        if (type == OptionType::Call) { res = S*std::exp(-q*t)*N(d1) - K*std::exp(-r*t)*N(d2); }
        else { res = K*std::exp(-r*t)*N(-d2) - S*std::exp(-q*t)*N(-d1); }

        return res;
    }

    double delta() 
    {
        double q = 0.0;
        double K = option->getStrike();
        double t = option->getExpiry();
        double d1 = (std::log(S/K) + (t*(r-q+s*s/2))) / (s*std::sqrt(t));

        OptionType type = option->getOptionType();
        double res;

        if (type == OptionType::Call) { res = std::exp(-q*t)*N(d1); }
        else { res = std::exp(-q*t)*(N(d1)-1); }

        return res;
    }
};

// Formulas : https://www.macroption.com/black-scholes-formula/#call-put-price