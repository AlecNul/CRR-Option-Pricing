#pragma once
#include "Options.h"

class PutOption : public EuropeanVanillaOption
{
public:
// Getter
    OptionType getOptionType() const override {return OptionType::Put;}

// Constructor
    PutOption(double exp, double K) : EuropeanVanillaOption(exp, K) {}

// Methods
    double payoff(double z) const override
    {
        double res = 0;
        if (strike > z) {res = strike-z;}
        return res;
    }

// Destructor
virtual ~PutOption() override {}
};