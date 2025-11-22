#pragma once
#include "Options.h"

class EuropeanDigitalCallOption : public EuropeanDigitalOption
{
public: 
// Getter
    OptionType getOptionType() const override {return OptionType::Call;}

// Constructor
    EuropeanDigitalCallOption(double exp, double K) : EuropeanDigitalOption(exp, K) {}

// Methods
    double payoff(double z) const override
    {
        double res = 0;
        if (z > strike) {res = 1;}
        return res;
    }

// Destructor
    virtual ~EuropeanDigitalCallOption() override {}
};