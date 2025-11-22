#pragma once
#include "Options.h"

class CallOption : public EuropeanVanillaOption
{
public: 
// Getter
    OptionType getOptionType() const override {return OptionType::Call;}

// Constructor
    CallOption(double exp, double K) : EuropeanVanillaOption(exp, K) {}

// Methods
    double payoff(double z) const override
    {
        double res = 0;
        if (z > strike) {res = z-strike;}
        return res;
    }

// Destructor
    virtual ~CallOption() override {}
};