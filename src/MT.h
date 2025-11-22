#pragma once
#include <random>

class MT // Singleton Class
{
// Attribute
private:
    std::mt19937 gen;
// Constructor (private)
    MT() : gen(std::random_device{}()) {}

    static MT& getInstance()
    {
        static MT instance;
        return instance;
    }

public:
// Delete default copy methods
    MT(const MT&) = delete;
    MT& operator=(const MT&) = delete;

// Methods
static double rand_unif()
{
    static std::uniform_real_distribution<> dis1(0.0, 1.0);
    return dis1(getInstance().gen);
}

static double rand_norm()
    {
        static std::normal_distribution<double> dist(0.0, 1.0);
        return dist(getInstance().gen);
    }
};