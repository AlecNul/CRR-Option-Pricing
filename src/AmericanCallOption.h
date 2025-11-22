#pragma once
#include "Options.h"

class AmericanCallOption : public AmericanOption
{
// Constructor
public:
    AmericanCallOption(double s, double exp) : AmericanOption(s,exp) {}

// Methods
    double payoff(double z) const override
    {
        double res = 0.0;
        if (z > K) { res = z-K; }
        return res;
    }
};