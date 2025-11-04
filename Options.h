#pragma once
#include <iostream>
#include "Exceptions.h"

// Variables
enum class OptionType {Call, Put};
class BlackScholesPricer;

/////////////////////////////////////////////////////////

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
    // virtual double payoff() = 0; Not sure to understand if we have to do this ?

// Destructor
    virtual ~Option() {}
};

/////////////////////////////////////////////////////////

class EuropeanVanillaOption : public Option
{
// Attributes
protected:
    double strike;

// Getter
public:
    double getStrike() const {return strike;}
    virtual OptionType getOptionType() const = 0;

// Constructor
public:
    EuropeanVanillaOption(double exp, double K) : Option(exp),strike(K)
    {
        if (K<0) {throw OptionInitError("strike must be nonnegative");}
    }

// Methods

// Destructor
    virtual ~EuropeanVanillaOption() override {}

// Friend with BlackScholes
    friend class BlackScholesPricer;
};

//////////////////////////////////////////////////////////

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

///////////////////////////////////////////////////////////

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