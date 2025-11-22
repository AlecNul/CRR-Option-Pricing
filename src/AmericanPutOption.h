#pragma once
#include "Options.h"

class AmericanPutOption : public AmericanOption
{
// Constructor
public:
    AmericanPutOption(double s, double exp) : AmericanOption(s,exp) {}

// Methods
    double payoff(double z) const override
    {
        double res = 0.0;
        if (K > z) { res = K-z; }
        return res;
    }
};