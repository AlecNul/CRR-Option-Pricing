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
    virtual OptionType getOptionType() const = 0;

// Constructor
public:
    EuropeanVanillaOption(double exp, double K) : Option(exp),strike(K)
    {
        if (K<0) 
        {
            // throw OptionInitError("strike must be nonnegative");
            while (K<0)
            {
                cout << "The chosen strike is negative, ("<< K <<") please type a new strike" << endl;
                cin >> K;
            }
        }
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

/////////////////////////////////////////////////////////////

class EuropeanDigitalOption : public EuropeanVanillaOption
// Probably not the best way to do it, but I have to keep the closest to the instructions
{
// Attributes
protected:
    double strike;

// Getter
public:
    virtual OptionType getOptionType() const = 0;

// Constructor
public:
    EuropeanDigitalOption(double exp, double K) : EuropeanVanillaOption(exp,K)
    {
        // if (K<0) {throw OptionInitError("strike must be nonnegative");}
    }

// Methods

// Destructor
    virtual ~EuropeanDigitalOption() override {}

// Friend with BlackScholes
    friend class BlackScholesPricer;
};

//////////////////////////////////////////////////////////

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

///////////////////////////////////////////////////////////

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
 