#pragma once
#include "Options.h"
#include  "AsianCallOption.h"
#include "AsianPutOption.h"
#include "BinaryTree.h"
#include <cmath>


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
    BinaryTree<bool> exercise;

// Getters  
public:
    bool getExercise(int n, int i) { return exercise.getNode(n,i); }

// Constructor
    CRRPricer(Option* option, int depth, double asset_price, double up, double down, double interest_rate) : 
    opt(option),N(depth),S(asset_price),U(up),D(down),R(interest_rate)
    {
        if (option->isAsianOption()) { throw PricerError("Error : You are trying to use CRR for an Asian Option.");}
        if (!(D<R and R<U)) 
        {
            // throw PricerError("There is arbitrage."); 
            cout << "Warning, there's arbitrage, consider creating a new pricer with different Up("<<up<<"), Down(" <<down<<") and Interest rate("<<interest_rate<<") you should have D<R<U" << endl;
        }
        asset_tree.setDepth(depth);
        pricing_tree.setDepth(depth);
        exercise.setDepth(depth-1); // Not necessary for N
        for (int n=0;n<=N;n++)
        {
            for(int i=0;i<=n;i++)
            {
                asset_tree.setNode(n,i,S*std::pow(1+U,i)*std::pow(1+D,n-i));
            }
        }
    }

    CRRPricer(Option* option, int depth, double asset_price, double r, double volatility) : 
    opt(option),N(depth),S(asset_price)
    {
        double h = opt->getExpiry()/depth;
        U = std::exp((r + 0.5 * volatility * volatility) * h + volatility * std::sqrt(h)) - 1.0;
        D = std::exp((r + 0.5 * volatility * volatility) * h - volatility * std::sqrt(h)) - 1.0;
        R = std::exp(r * h) - 1.0;

        if (option->isAsianOption()) { throw PricerError("Error : You are trying to use CRR for an Asian Option.");}
        if (!(D<R and R<U)) 
        {
            // throw PricerError("There is arbitrage."); 
            cout << "Warning, there's arbitrage, consider creating a new pricer with different Up("<<U<<"), Down(" <<D<<") and Interest rate("<<R<<") you should have D<R<U" << endl;
        }
        asset_tree.setDepth(depth);
        pricing_tree.setDepth(depth);
        exercise.setDepth(depth-1); // Not necessary for N
        for (int n=0;n<=N;n++)
        {
            for(int i=0;i<=n;i++)
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
            for(int i=0;i<=n;i++)
            {
                if(n==N) 
                {
                    pricing_tree.setNode(n,i,opt->payoff(asset_tree.getNode(n,i)));
                }
                else
                {
                    next_H_i_1 = pricing_tree.getNode(n+1,i+1);
                    next_H_i = pricing_tree.getNode(n+1,i);
                    H_i = (q*next_H_i_1 + (1-q)*next_H_i) / (1+R); // Continuation value

                    if (opt->isAmericanOption())
                    {
                        double intrinsic_value = opt->payoff(asset_tree.getNode(n,i));
                        if (intrinsic_value > H_i) 
                        { 
                            H_i = intrinsic_value;
                            exercise.setNode(n,i,true);
                        }
                        else { exercise.setNode(n,i,false); }
                    }

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
            }
            res /= std::pow(1+R,N);
        }
        else { compute(); res = pricing_tree.getNode(0,0); }

        return res;
    }
};

// Going to use gamma to compute factorials as G(n+1) = n! for integers. (it crashes)
//res += ((std::tgamma(N+1)/(std::tgamma(i+1)*std::tgamma(N-i+1)))*std::pow(q,i)*std::pow(1-q,N-i)*opt->payoff(asset_tree.getNode(N,i)));