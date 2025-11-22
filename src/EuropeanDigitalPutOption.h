#pragma once
#include "Options.h"

class EuropeanDigitalPutOption : public EuropeanDigitalOption
{
public:
// Getter
    OptionType getOptionType() const override {return OptionType::Put;}

// Constructor
    EuropeanDigitalPutOption(double exp, double K) : EuropeanDigitalOption(exp, K) {}

// Methods
    double payoff(double z) const override
    {
        double res = 0;
        if (strike > z) {res = 1;}
        return res;
    }

// Destructor
virtual ~EuropeanDigitalPutOption() override {}
};