#pragma once

#include <exception>
#include <string>

using namespace std;

class OptionInitError : public exception
{
// Attributes
private:
    string msg;

// Constructor
public:
    OptionInitError(const string& text) : msg(text) {}
    
    virtual const char* what() const noexcept {return msg.c_str();}
};

class PricerError : public exception
{
private:
    string msg;

public:
    PricerError(const string& text) : msg(text) {}

    virtual const char* what() const noexcept {return msg.c_str();}
};