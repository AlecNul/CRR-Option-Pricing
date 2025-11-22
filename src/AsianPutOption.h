#pragma once
#include "Options.h"

class AsianPutOption : public AsianOption 
{
// Attributes
protected:
    double K;

// Constructor
public:
    AsianPutOption(std::vector<double> time, double strike) : AsianOption(time),K(strike) {}
    
// Methods
    double payoff(double mean) const override
    {
        double res = 0;
        if (mean<K) { res = K-mean; }
        
        return res;
    }
};