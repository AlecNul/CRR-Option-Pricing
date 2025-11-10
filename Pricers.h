#pragma once
#include <iostream>
#include "Options.h"
#include "BinaryTree.h"
#include <cmath>

class BlackScholesPricer 
{
// Attributes
private:
    EuropeanVanillaOption* option;
    double S; // asset_price
    double r; // interest_rate
    double s; // volatility

    bool isDigital;

// Constructor
public:
    BlackScholesPricer(EuropeanVanillaOption* opt, double price, double rate, double vola) : 
    option(opt),S(price),r(rate),s(vola) { isDigital = false; }

    BlackScholesPricer(EuropeanDigitalOption* opt, double price, double rate, double vola) :
    option(opt),S(price),r(rate),s(vola) { isDigital = true; }

// Methods
private: // Function for Normal CDF computation
    double N(double x) const { return std::erfc(-x/std::sqrt(2))/2; }

    // Function for Normal PDF computation
    double n(double x) const { return (std::exp((-x*x) / 2) / std::sqrt(2*2*std::acos(0.0))); }

public:
    double operator()() const 
    {
        double K = option->strike;
        double t = option->getExpiry(); // Having no more information I'll assume it's the time to expiry 
        double q = 0.0; // Dividend yield 

        double d1 = (std::log(S/K) + (t*(r-q+s*s/2))) / (s*std::sqrt(t));
        double d2 = d1 - s*std::sqrt(t);

        OptionType type = option->getOptionType();
        double res;
    
        if (!isDigital)
        {
            if (type == OptionType::Call) { res = S*std::exp(-q*t)*N(d1) - K*std::exp(-r*t)*N(d2); }
            else { res = K*std::exp(-r*t)*N(-d2) - S*std::exp(-q*t)*N(-d1); }
        }
        else 
        {
            // Closed form formulas for digital (didn't find other methods) 
            if (type == OptionType::Call) { res = std::exp(-r*t)*N(d2); }
            else { res = std::exp(-r*t)*N(-d2); }
        }
        
        return res;
    }

    double delta() 
    {
        double q = 0.0;
        double K = option->strike;
        double t = option->getExpiry();
        double d1 = (std::log(S/K) + (t*(r-q+s*s/2))) / (s*std::sqrt(t));
        double d2 = d1 - s*std::sqrt(t);

        OptionType type = option->getOptionType();
        double res;

        if (!isDigital)
        {
            if (type == OptionType::Call) { res = std::exp(-q*t)*N(d1); }
            else { res = std::exp(-q*t)*(N(d1)-1); }
        }
        else
        {
            if (type == OptionType::Call) { res = (std::exp(-r*t)*n(d2)) / S*s*std::sqrt(t); }
            else { res = -(std::exp(-r*t)*n(d2)) / S*s*std::sqrt(t); }
        }
        return res;
    }
};

// Formulas : https://www.macroption.com/black-scholes-formula/#call-put-price

class CRRPricer
{

// Attributes
private:
    Option* opt;
    int N; // depth
    double S; // asset_price
    double U; // up
    double D; // down
    double R; // interest_rate
    BinaryTree<double> asset_tree;
    BinaryTree<double> pricing_tree;

// Constructor
public:
    CRRPricer(Option* option, int depth, double asset_price, double up, double down, double interest_rate) : 
    opt(option),N(depth),S(asset_price),U(up),D(down),R(interest_rate)
    {
        if (!(U<R and R<U)) 
        { 
            // throw PricerError("There is arbitrage."); 
            cout << "Warning, there's arbitrage, consider creating a new pricer with different Up("<<up<<"), Down(" <<down<<") and Interest rate("<<interest_rate<<")" << endl;
        }
        asset_tree.setDepth(depth);
        pricing_tree.setDepth(depth);
        for (int n=0;n<N;n++)
        {
            for(int i=0;i<n;i++)
            {
                asset_tree.setNode(n,i,S*std::pow(1+U,i)*std::pow(1+D,n-i));
            }
        }
    }

    void compute()
    {
        double q = (R-D)/(U-D);
        double next_H_i_1;
        double next_H_i;
        double H_i;

        for(int n=N;n>=0;n--)
        {
            for(int i=0;i<n;i++)
            {
                if(n==N) 
                {
                    pricing_tree.setNode(n,i,opt->payoff(asset_tree.getNode(n,i)));
                }
                else
                {
                    next_H_i_1 = pricing_tree.getNode(n+1,i+1);
                    next_H_i = pricing_tree.getNode(n+1,i);
                    H_i = (q*next_H_i_1 + (1-q)*next_H_i) / (1+R);

                    pricing_tree.setNode(n,i,H_i);
                }
            }
        }
    }

    double get(int n, int i) const { return pricing_tree.getNode(n,i); }

    double operator()(bool closed_form=false)
    {
        double res;
        if (closed_form)
        {
            res=0;
            double q = (R-D)/(U-D);

            // We initialize
            double left_term = std::pow(1.-q,N);
            res = left_term * opt->payoff(asset_tree.getNode(N,0));

            for (int i = 1; i <= N; ++i) 
            {
                // We update the left term and add it to the sum (Ck_n in function of Ck+1_n)
                left_term *= (((double)N - i + 1.) / (double)i) * (q/(1.-q));
                res += left_term * opt->payoff(asset_tree.getNode(N, i));
                // Going to use gamma to compute factorials as G(n+1) = n! for integers. (it crashes)
                //res += ((std::tgamma(N+1)/(std::tgamma(i+1)*std::tgamma(N-i+1)))*std::pow(q,i)*std::pow(1-q,N-i)*opt->payoff(asset_tree.getNode(N,i)));
            }
            res /= std::pow(1+R,N);
        }
        else if (pricing_tree.getNode(0,0)!=0.0) { compute(); res = pricing_tree.getNode(0,0); }
        else { res = pricing_tree.getNode(0,0); }

        return res;
    }
};