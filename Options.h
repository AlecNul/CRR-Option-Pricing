#pragma once
#include <iostream>
#include "Exceptions.h"

using namespace std;

class Option
{
// Attributes
protected:
    double expiry;


public:
// Getter
    double getExpiry() const {return expiry;}

// Constructor
    Option(double exp) : expiry(exp) 
    {
        if (exp<0) {throw OptionInitError("expiry must be nonnegative");}
    }


// Methods
    virtual double payoff(double z) const = 0;
    virtual double payoff() = 0;

// Destructor
    virtual ~Option() {}
};

/////////////////////////////////////////////////////////

class EuropeanVanillaOption : public Option
{
// Attributes
protected:
    double strike;
    enum class optionType {call, put};
    optionType type;

// Getter
    double getStrike() const {return strike;}
    virtual optionType getOptionType() const = 0;

// Constructor
public:
    EuropeanVanillaOption(double exp, double K) : Option(exp),strike(K)
    {
        if (K<0) {throw OptionInitError("strike must be nonnegative");}
    }

// Methods

// Destructor
    virtual ~EuropeanVanillaOption() {}
};

//////////////////////////////////////////////////////////

class CallOption : public EuropeanVanillaOption
{

// Getter
    optionType getOptionType() const override {optionType::call;}

// Constructor
    CallOption(double exp, double K) : EuropeanVanillaOption(exp, K) {}

// Methods
    double payoff(double z) 
    {
        double res = 0;
        if (z > strike) {res = z-strike;}
        return res;
    }

};

class PutOption : EuropeanVanillaOption
{

// Getter
    optionType getOptionType() const override {optionType::call;}

// Constructor
    PutOption(double exp, double K) : EuropeanVanillaOption(exp, K) {}

// Methods
    double payoff(double z) 
    {
        double res = 0;
        if (z > strike) {res = strike-z;}
        return res;
    }

};