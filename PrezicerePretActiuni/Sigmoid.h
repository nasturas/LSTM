#pragma once
#include "IActivationFunction.h"
class Sigmoid :
    public IActivationFunction
{
public:
    double Output(double x) const override;
    double Derivate(double x) const override;
};

