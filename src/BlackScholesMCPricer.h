#pragma once
#include "MT.h"
#include <vector>
#include <cmath>
#include "Options.h"

class BlackScholesMCPricer
{
//Attributes 
private:
Option* opt;
double S0; // initial price
double R; // interest_rate
double v; // volatility
int paths; // Paths generated since beginning of program
double payoffs_sum; // Sum of the payoffs
double squared_payoffs_sum; // Sum of the squared payoffs

// Getters
public:
    int getNbPaths() { return paths; }

// Constructor
    BlackScholesMCPricer(Option* option, double initial_price, double interest_rate, double volatility) :
    opt(option),S0(initial_price),R(interest_rate),v(volatility),paths(0),payoffs_sum(0),squared_payoffs_sum(0) {}

// Methods
    void generate(int nb_paths)
    {
        std::vector<double> time_steps;

        // Handle Asian / Other option case
        if (opt->isAsianOption()) 
        { 
            AsianOption* asian_opt = dynamic_cast<AsianOption*>(opt);
            time_steps = asian_opt->getTimeSteps();
        }
        else { time_steps.push_back(opt->getExpiry());}

        // Init
        double T = opt->getExpiry();
        std::vector<double> path;

        for (int k=0;k<nb_paths;k++)
        {
            path.clear();
            double S = S0;
            double previous = 0.0;

            // Generate 1 path
            for (int i=0;i<time_steps.size();i++)
            {
                double dt = time_steps[i]-previous;
                previous = time_steps[i];

                double drift = (R-0.5*v*v)*dt;
                double diffusion = v*std::sqrt(dt);

                double Z = MT::rand_norm();

                S *= std::exp(drift + diffusion*Z);
                
                path.push_back(S);
            }

            double payoff = opt->payoffPath(path);
            double discounted_payoff = payoff*std::exp(-R*T);

            payoffs_sum += discounted_payoff;
            squared_payoffs_sum += discounted_payoff*discounted_payoff;
        }
        paths += nb_paths;
    }

    double operator()() const
    {
        if (paths == 0) { throw PricerError("Undefined."); }
        double res = payoffs_sum / paths;
        return res;
    }

    std::vector<double> confidenceInterval() const
    {
        if (paths == 0) return {0.0, 0.0};

        double mean = payoffs_sum / paths;
        // Keonig-Huygens
        double variance = (squared_payoffs_sum / paths) - (mean * mean);

        double std_error = std::sqrt(variance) / std::sqrt(paths);

        // https://www.ztable.net/
        double lower = mean - 1.96 * std_error;
        double upper = mean + 1.96 * std_error;

        return {lower, upper};
    }
};