#pragma once
#include <iostream>
#include <vector>
#include "Exceptions.h"

// Variables
enum class OptionType {Call, Put};
class BlackScholesPricer;
class CRRPricer;

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

    virtual double payoffPath(std::vector<double> prices) { return payoff(prices.back()); }

    virtual bool isAsianOption() const { return false; }

    virtual bool isAmericanOption() const { return false; }

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

/////////////////////////////////////////////////////////////

class EuropeanDigitalOption : public EuropeanVanillaOption
// Probably not the best way to do it, but I have to keep the closest to the instructions
{
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

/////////////////////////////////////////////////////////////////

class AsianOption : public Option
{
// Attributes
protected:
    std::vector<double> t; // Time steps

// Getter
public:
    std::vector<double> getTimeSteps() const { return t; }

// Constructor
    AsianOption(std::vector<double> time) : t(time),Option(time.back()) {}

// Methods 
    bool isAsianOption() const override { return true; }

    double payoffPath(std::vector<double> S)
    {
        double sum = 0;
        for (int k=0;k<S.size()+1;k++)
        {
            sum += S[k];
        }
        sum /= S.size();
        return payoff(sum);
    }

};

///////////////////////////////////////////////////////////////////

class AmericanOption : public Option
{
// Attributes
protected:
    double K; // strike

// Constructor
public:
    AmericanOption(double s, double exp) : K(s),Option(exp) {}

// Method
    bool isAmericanOption() const override { return true; }

// Friend with CRR
    friend class CRRPricer;

};