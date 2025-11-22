#pragma once
#include "Options.h"

class AsianCallOption : public AsianOption 
{
// Attributes
protected:
    double K;

// Constructor
public:
    AsianCallOption(std::vector<double> time, double strike) : AsianOption(time),K(strike) {}
    
// Methods
    double payoff(double mean) const override
    {

        double res = 0;
        if (K<mean) { res = mean-K; }

        return res;
    }
};