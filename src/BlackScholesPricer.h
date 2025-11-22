#pragma once
#include <iostream>
#include "Options.h"
#include <cmath>

class BlackScholesPricer 
{
// Attributes
private:
    EuropeanVanillaOption* option;
    double S; // asset_price
    double r; // interest_rate
    double s; // volatility

    bool isDigital;

// Constructor
public:
    BlackScholesPricer(EuropeanVanillaOption* opt, double price, double rate, double vola) : 
    option(opt),S(price),r(rate),s(vola) { isDigital = false; }

    BlackScholesPricer(EuropeanDigitalOption* opt, double price, double rate, double vola) :
    option(opt),S(price),r(rate),s(vola) { isDigital = true; }

// Methods
private: // Function for Normal CDF computation
    double N(double x) const { return std::erfc(-x/std::sqrt(2))/2; }

    // Function for Normal PDF computation
    double n(double x) const { return (std::exp((-x*x) / 2) / std::sqrt(2*2*std::acos(0.0))); }

public:
    double operator()() const 
    {
        double K = option->strike;
        double t = option->getExpiry(); // Having no more information I'll assume it's the time to expiry 
        double q = 0.0; // Dividend yield 

        double d1 = (std::log(S/K) + (t*(r-q+s*s/2))) / (s*std::sqrt(t));
        double d2 = d1 - s*std::sqrt(t);

        OptionType type = option->getOptionType();
        double res;
    
        if (!isDigital)
        {
            if (type == OptionType::Call) { res = S*std::exp(-q*t)*N(d1) - K*std::exp(-r*t)*N(d2); }
            else { res = K*std::exp(-r*t)*N(-d2) - S*std::exp(-q*t)*N(-d1); }
        }
        else 
        {
            // Closed form formulas for digital (didn't find other methods) 
            if (type == OptionType::Call) { res = std::exp(-r*t)*N(d2); }
            else { res = std::exp(-r*t)*N(-d2); }
        }
        
        return res;
    }

    double delta() 
    {
        double q = 0.0;
        double K = option->strike;
        double t = option->getExpiry();
        double d1 = (std::log(S/K) + (t*(r-q+s*s/2))) / (s*std::sqrt(t));
        double d2 = d1 - s*std::sqrt(t);

        OptionType type = option->getOptionType();
        double res;

        if (!isDigital)
        {
            if (type == OptionType::Call) { res = std::exp(-q*t)*N(d1); }
            else { res = std::exp(-q*t)*(N(d1)-1); }
        }
        else
        {
            if (type == OptionType::Call) { res = (std::exp(-r*t)*n(d2)) / S*s*std::sqrt(t); }
            else { res = -(std::exp(-r*t)*n(d2)) / S*s*std::sqrt(t); }
        }
        return res;
    }
};

// Formulas : https://www.macroption.com/black-scholes-formula/#call-put-price