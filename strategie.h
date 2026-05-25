#ifndef STRATEGIE_H
#define STRATEGIE_H

#include <string>
#include <iostream>

class IStrategieDiscount
{
public:
    virtual double aplicaDiscount(double pretOriginal) const = 0;
    virtual std::string descriere() const = 0;
    virtual ~IStrategieDiscount() = default;
};

// Fara reducere
class FaraDiscount : public IStrategieDiscount
{
public:
    double aplicaDiscount(double p) const override { return p; }
    std::string descriere() const override { return "Fara discount"; }
};

class DiscountGrup : public IStrategieDiscount
{
private:
    double procentReducere;

public:
    explicit DiscountGrup(double procent = 10.0) : procentReducere(procent) {}
    double aplicaDiscount(double p) const override
    {
        return p * (1.0 - procentReducere / 100.0);
    }
    std::string descriere() const override
    {
        return "Discount grup " + std::to_string((int)procentReducere) + "%";
    }
};

class HappyHour : public IStrategieDiscount
{
public:
    double aplicaDiscount(double p) const override { return p * 0.80; }
    std::string descriere() const override { return "Happy Hour (-20%)"; }
};

#endif